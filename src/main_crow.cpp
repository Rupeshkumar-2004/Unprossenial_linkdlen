#define CROW_MAIN
#define CROW_JSON_USE_OPTIONAL_ERROR_CHECKING
#include "crow.h"
#include "include/job_portal.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <string>

using json = nlohmann::json;

// Global data structures
std::vector<Job> jobs;
std::unordered_map<std::string, Candidate> candidates; // sessionId -> Candidate
InvertedIndex skillIndex;
InvertedIndex locationIndex;
Trie jobTitleTrie;

// Helper function to convert Job to JSON
json jobToJson(const Job& job, int index = -1) {
    json j;
    if (index >= 0) j["id"] = index;
    j["title"] = job.title;
    j["company"] = job.company;
    j["location"] = job.location;
    j["salary"] = job.salary;
    j["skills"] = job.skills;
    j["description"] = job.description;
    return j;
}

int main() {
    crow::SimpleApp app;

    // Serve static HTML page
    CROW_ROUTE(app, "/")
    ([]() -> crow::response {
        std::ifstream file("web/index.html");
        if (file) {
            std::string content((std::istreambuf_iterator<char>(file)), 
                               std::istreambuf_iterator<char>());
            auto resp = crow::response(content);
            resp.set_header("Content-Type", "text/html");
            return resp;
        }
        return crow::response(404, "web/index.html not found");
    });

    // API: Post a new job
    CROW_ROUTE(app, "/api/jobs")
    .methods("POST"_method)([](const crow::request& req) -> crow::response {
        try {
            auto body = json::parse(req.body);
            Job newJob;
            newJob.title = body["title"].get<std::string>();
            newJob.company = body["company"].get<std::string>();
            newJob.location = body["location"].get<std::string>();
            newJob.salary = body["salary"].get<double>();
            newJob.description = body.value("description", "");
            
            for (const auto& skill : body["skills"]) {
                newJob.skills.push_back(skill.get<std::string>());
            }

            jobs.push_back(newJob);
            int newJobIndex = jobs.size() - 1;

            // Update indexes
            for (const auto& skill : newJob.skills) {
                skillIndex[toLower(skill)].push_back(newJobIndex);
            }
            locationIndex[toLower(newJob.location)].push_back(newJobIndex);
            jobTitleTrie.insert(newJob.title);

            json response;
            response["success"] = true;
            response["message"] = "Job posted successfully!";
            response["job"] = jobToJson(newJob, newJobIndex);
            
            return crow::response(response.dump());
        } catch (const std::exception& e) {
            json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(400, error.dump());
        }
    });

    // API: Get all jobs
    CROW_ROUTE(app, "/api/jobs")
    .methods("GET"_method)([]() -> crow::response {
        json response;
        response["jobs"] = json::array();
        for (size_t i = 0; i < jobs.size(); ++i) {
            response["jobs"].push_back(jobToJson(jobs[i], i));
        }
        return crow::response(response.dump());
    });

    // API: Search jobs by keyword
    CROW_ROUTE(app, "/api/jobs/search")( [](const crow::request& req) -> crow::response {
        auto keyword = req.url_params.get("q");
        if (!keyword) {
            json error;
            error["success"] = false;
            error["message"] = "Missing search query";
            return crow::response(400, error.dump());
        }

        const int K = 10;
        using ScorePair = std::pair<int, int>;
        std::priority_queue<ScorePair, std::vector<ScorePair>, std::greater<ScorePair>> topKJobs;

        for (size_t i = 0; i < jobs.size(); ++i) {
            int score = relevanceScore(jobs[i], keyword);
            if (score > 0) {
                if (topKJobs.size() < K) {
                    topKJobs.push({score, (int)i});
                } else if (score > topKJobs.top().first) {
                    topKJobs.pop();
                    topKJobs.push({score, (int)i});
                }
            }
        }

        std::vector<ScorePair> results;
        while (!topKJobs.empty()) {
            results.push_back(topKJobs.top());
            topKJobs.pop();
        }
        std::reverse(results.begin(), results.end());

        json response;
        response["results"] = json::array();
        for (const auto& p : results) {
            auto jobJson = jobToJson(jobs[p.second], p.second);
            jobJson["score"] = p.first;
            response["results"].push_back(jobJson);
        }

        return crow::response(response.dump());
    });

    // API: Update candidate profile
    CROW_ROUTE(app, "/api/profile")
    .methods("POST"_method)([](const crow::request& req) -> crow::response {
        try {
            auto body = json::parse(req.body);
            std::string sessionId = body.value("sessionId", "default");
            
            Candidate& candidate = candidates[sessionId];
            candidate.name = body["name"].get<std::string>();
            candidate.preferredLocation = body["location"].get<std::string>();
            candidate.expectedSalary = body["salary"].get<double>();
            candidate.skills.clear();
            for (const auto& skill : body["skills"]) {
                candidate.skills.push_back(skill.get<std::string>());
            }
            candidate.isProfileSet = true;

            json response;
            response["success"] = true;
            response["message"] = "Profile updated successfully!";
            return crow::response(response.dump());
        } catch (const std::exception& e) {
            json error;
            error["success"] = false;
            error["message"] = e.what();
            return crow::response(400, error.dump());
        }
    });

    // API: Get job recommendations
    CROW_ROUTE(app, "/api/recommendations")( [](const crow::request& req) -> crow::response {
        const char* sessionParam = req.url_params.get("sessionId");
        std::string sessionId = sessionParam ? std::string(sessionParam) : std::string("default");

        if (candidates.find(sessionId) == candidates.end() || !candidates[sessionId].isProfileSet) {
            json error;
            error["success"] = false;
            error["message"] = "Profile not set. Please create your profile first.";
            return crow::response(400, error.dump());
        }

        const Candidate& candidate = candidates[sessionId];
        std::unordered_map<int, int> jobMatchScores;
        
        for (const auto& skill : candidate.skills) {
            auto it = skillIndex.find(toLower(skill));
            if (it != skillIndex.end()) {
                for (int jobIndex : it->second) {
                    jobMatchScores[jobIndex]++;
                }
            }
        }

        json response;
        response["recommendations"] = json::array();
        
        for (const auto& [jobIndex, matchCount] : jobMatchScores) {
            const Job& job = jobs[jobIndex];
            if (job.salary >= candidate.expectedSalary) {
                if (candidate.preferredLocation.empty() || 
                    toLower(job.location) == toLower(candidate.preferredLocation)) {
                    auto jobJson = jobToJson(job, jobIndex);
                    jobJson["matchedSkills"] = matchCount;
                    response["recommendations"].push_back(jobJson);
                }
            }
        }

        return crow::response(response.dump());
    });

    app.loglevel(crow::LogLevel::Warning);
    std::cout << "🚀 Job Portal Server starting on http://localhost:8080\n";
    app.port(8080).multithreaded().run();
    
    return 0;
}
