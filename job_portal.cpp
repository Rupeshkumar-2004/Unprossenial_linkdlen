#include "job_portal.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <queue>

using namespace std;

// --- Utility Function Implementations ---

void printJob(const Job& job) {
    cout << "\n----------------------------------------\n";
    cout << "Title: " << job.title <<endl;
    cout << "Company: " << job.company <<endl;
    cout << "Location: " << job.location <<endl;
    cout << "Salary: $" << job.salary <<endl;
    cout << "Skills: ";
    for (size_t i = 0; i < job.skills.size(); ++i) {
        cout << job.skills[i] << (i == job.skills.size() - 1 ? "" : ", ");
    }
    cout << "\n----------------------------------------\n";
}

string toLower(string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return ::tolower(c); });
    return s;
}

vector<std::string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
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
    cout << "\nEnter Job Title: ";
    getline(cin, newJob.title);
    cout << "Enter Company: ";
    getline(cin, newJob.company);
    cout << "Enter Location: ";
    getline(cin, newJob.location);
    cout << "Enter Required Skills (comma-separated): ";
    string skills_line;
    getline(cin, skills_line);
    newJob.skills = split(skills_line, ',');
    
    cout << "Enter Salary: ";
    cin >> newJob.salary;
    while (cin.fail()) {
        cout << "Invalid input. Please enter a number for salary: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> newJob.salary;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
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
    cout << "\n--- Create/Update Your Profile ---\n";
    cout << "Enter Your Name: ";
    getline(cin, candidate.name);
    cout << "Enter Your Skills (comma-separated): ";
    string skills_line;
    getline(cin, skills_line);
    candidate.skills = split(skills_line, ',');
    cout << "Enter Preferred Location: ";
    getline(cin, candidate.preferredLocation);
    cout << "Enter Expected Minimum Salary: ";
    cin >> candidate.expectedSalary;
    while (cin.fail()) {
        cout << "Invalid input. Please enter a number for salary: ";
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        cin >> candidate.expectedSalary;
    }
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    candidate.isProfileSet = true;
    cout << "\n✅ Profile updated successfully!\n";
}

int relevanceScore(const Job& job, const string& keyword) {
    int score = 0;
    string lowerKeyword = toLower(keyword);
    if (toLower(job.title).find(lowerKeyword) != string::npos) score += 10;
    for (const auto& skill : job.skills) {
        if (toLower(skill).find(lowerKeyword) != string::npos) {
            score += 5;
            break;
        }
    }
    return score;
}

void searchJobs(const vector<Job>& jobs) {
    cout << "Enter keyword to search: ";
    string keyword;
    getline(cin, keyword);

    const int K = 5; // We want the Top 5 results
    using ScorePair = pair<int, int>; // <score, jobIndex>
    priority_queue<ScorePair, vector<ScorePair>,greater<ScorePair>> topKJobs;

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
        cout << "\n❌ No matching jobs found.\n";
        return;
    }
    
    vector<ScorePair> results;
    while(!topKJobs.empty()){
        results.push_back(topKJobs.top());
        topKJobs.pop();
    }
    reverse(results.begin(), results.end()); // To display from highest score to lowest

    cout << "\n🔥 Top " << results.size() << " Jobs Matching Your Search:\n";
    for(const auto& p : results) {
        printJob(jobs[p.second]);
    }
}

void recommendJobs(const vector<Job>& jobs, const InvertedIndex& skillIndex, const Candidate& candidate) {
    cout << "\n🎯 Recommended Jobs for " << candidate.name << ":\n";
    unordered_map<int, int> jobMatchScores; // Key: jobIndex, Value: count of matched skills
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
        cout << "❌ No recommendations match your profile at this time.\n";
    }
}

void autocompleteSearch(const Trie& jobTitleTrie) {
    cout << "Enter a prefix to search job titles: ";
    string prefix;
    getline(cin, prefix);
    vector<string> suggestions = jobTitleTrie.searchPrefix(prefix);
    
    if (suggestions.empty()) {
        cout << "No suggestions found for \"" << prefix << "\"\n";
    } else {
        cout << "Suggestions:\n";
        for(const auto& title : suggestions) {
            cout << "- " << title << "\n";
        }
    }
}