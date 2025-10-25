#ifndef JOB_PORTAL_H
#define JOB_PORTAL_H

#include "job.h"
#include "Candidate.h"
#include "Trie.h"
#include <vector>
#include <string>
#include <unordered_map>

// A hash map for fast lookups (Skill/Location -> List of Job Indices)
using InvertedIndex = std::unordered_map<std::string, std::vector<int>>;

// Search relevance scoring
int relevanceScore(const Job& job, const std::string& keyword);

// --- Utility Functions ---
void printJob(const Job& job);
std::string toLower(std::string s);
std::vector<std::string> split(const std::string& s, char delimiter);

// --- Core Application Logic ---
void postJob(std::vector<Job>& jobs, InvertedIndex& skillIndex, InvertedIndex& locationIndex, Trie& jobTitleTrie);
void updateCandidateProfile(Candidate& candidate);
void searchJobs(const std::vector<Job>& jobs);
void recommendJobs(const std::vector<Job>& jobs, const InvertedIndex& skillIndex, const Candidate& candidate);
void autocompleteSearch(const Trie& jobTitleTrie);

#endif // JOB_PORTAL_H
