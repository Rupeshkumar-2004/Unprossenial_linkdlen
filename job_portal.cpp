#include "job_portal.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <queue>

// --- Utility Function Implementations ---

void printJob(const Job& job) {
    std::cout << "\n----------------------------------------\n";
    std::cout << "Title: " << job.title << std::endl;
    std::cout << "Company: " << job.company << std::endl;
    std::cout << "Location: " << job.location << std::endl;
    std::cout << "Salary: $" << job.salary << std::endl;
    std::cout << "Skills: ";
    for (size_t i = 0; i < job.skills.size(); ++i) {
        std::cout << job.skills[i] << (i == job.skills.size() - 1 ? "" : ", ");
    }
    std::cout << "\n----------------------------------------\n";
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        token.erase(0, token.find_first_not_of(" \t\n\r"));
        token.erase(token.find_last_not_of(" \t\n\r") + 1);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// --- Core Logic Implementations ---

void postJob(std::vector<Job>& jobs, InvertedIndex& skillIndex, InvertedIndex& locationIndex, Trie& jobTitleTrie) {
    Job newJob;
    std::cout << "\nEnter Job Title: ";
    std::getline(std::cin, newJob.title);
    std::cout << "Enter Company: ";
    std::getline(std::cin, newJob.company);
    std::cout << "Enter Location: ";
    std::getline(std::cin, newJob.location);
    std::cout << "Enter Required Skills (comma-separated): ";
    std::string skills_line;
    std::getline(std::cin, skills_line);
    newJob.skills = split(skills_line, ',');
    
    std::cout << "Enter Salary: ";
    std::cin >> newJob.salary;
    while (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number for salary: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> newJob.salary;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    jobs.push_back(newJob);
    int newJobIndex = jobs.size() - 1;

    // Update indexes and Trie
    for (const auto& skill : newJob.skills) {
        skillIndex[toLower(skill)].push_back(newJobIndex);
    }
    locationIndex[toLower(newJob.location)].push_back(newJobIndex);
    jobTitleTrie.insert(newJob.title);

    std::cout << "\n✅ Job Posted Successfully!\n";
}

void updateCandidateProfile(Candidate& candidate) {
    std::cout << "\n--- Create/Update Your Profile ---\n";
    std::cout << "Enter Your Name: ";
    std::getline(std::cin, candidate.name);
    std::cout << "Enter Your Skills (comma-separated): ";
    std::string skills_line;
    std::getline(std::cin, skills_line);
    candidate.skills = split(skills_line, ',');
    std::cout << "Enter Preferred Location: ";
    std::getline(std::cin, candidate.preferredLocation);
    std::cout << "Enter Expected Minimum Salary: ";
    std::cin >> candidate.expectedSalary;
    while (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number for salary: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> candidate.expectedSalary;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    candidate.isProfileSet = true;
    std::cout << "\n✅ Profile updated successfully!\n";
}

int relevanceScore(const Job& job, const std::string& keyword) {
    int score = 0;
    std::string lowerKeyword = toLower(keyword);
    if (toLower(job.title).find(lowerKeyword) != std::string::npos) score += 10;
    for (const auto& skill : job.skills) {
        if (toLower(skill).find(lowerKeyword) != std::string::npos) {
            score += 5;
            break;
        }
    }
    return score;
}

void searchJobs(const std::vector<Job>& jobs) {
    std::cout << "Enter keyword to search: ";
    std::string keyword;
    std::getline(std::cin, keyword);

    const int K = 5; // We want the Top 5 results
    using ScorePair = std::pair<int, int>; // <score, jobIndex>
    std::priority_queue<ScorePair, std::vector<ScorePair>, std::greater<ScorePair>> topKJobs;

    for (int i = 0; i < jobs.size(); ++i) {
        int score = relevanceScore(jobs[i], keyword);
        if (score > 0) {
            if (topKJobs.size() < K) {
                topKJobs.push({score, i});
            } else if (score > topKJobs.top().first) {
                topKJobs.pop();
                topKJobs.push({score, i});
            }
        }
    }

    if (topKJobs.empty()) {
        std::cout << "\n❌ No matching jobs found.\n";
        return;
    }
    
    std::vector<ScorePair> results;
    while(!topKJobs.empty()){
        results.push_back(topKJobs.top());
        topKJobs.pop();
    }
    std::reverse(results.begin(), results.end()); // To display from highest score to lowest

    std::cout << "\n🔥 Top " << results.size() << " Jobs Matching Your Search:\n";
    for(const auto& p : results) {
        printJob(jobs[p.second]);
    }
}

void recommendJobs(const std::vector<Job>& jobs, const InvertedIndex& skillIndex, const Candidate& candidate) {
    std::cout << "\n🎯 Recommended Jobs for " << candidate.name << ":\n";
    std::unordered_map<int, int> jobMatchScores; // Key: jobIndex, Value: count of matched skills
    for (const auto& skill : candidate.skills) {
        auto it = skillIndex.find(toLower(skill));
        if (it != skillIndex.end()) {
            for (int jobIndex : it->second) {
                jobMatchScores[jobIndex]++;
            }
        }
    }

    bool found = false;
    for (const auto& [jobIndex, matchCount] : jobMatchScores) {
        const Job& job = jobs[jobIndex];
        // Recommend if candidate has at least one matching skill and meets other criteria
        if (job.salary >= candidate.expectedSalary) {
             if(candidate.preferredLocation.empty() || toLower(job.location) == toLower(candidate.preferredLocation)) {
                printJob(job);
                found = true;
             }
        }
    }

    if (!found) {
        std::cout << "❌ No recommendations match your profile at this time.\n";
    }
}

void autocompleteSearch(const Trie& jobTitleTrie) {
    std::cout << "Enter a prefix to search job titles: ";
    std::string prefix;
    std::getline(std::cin, prefix);
    std::vector<std::string> suggestions = jobTitleTrie.searchPrefix(prefix);
    
    if (suggestions.empty()) {
        std::cout << "No suggestions found for \"" << prefix << "\"\n";
    } else {
        std::cout << "Suggestions:\n";
        for(const auto& title : suggestions) {
            std::cout << "- " << title << "\n";
        }
    }
}