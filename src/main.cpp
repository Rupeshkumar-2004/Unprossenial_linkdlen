#include <iostream>
#include <vector>
#include <limits>
#include "job_portal.h"

int main() {
    // --- Main Data Storage ---
    std::vector<Job> jobs;
    Candidate candidate;
    
    // --- Advanced Data Structures for Performance ---
    InvertedIndex skillIndex;
    InvertedIndex locationIndex;
    Trie jobTitleTrie;

    int choice;

    while (true) {
        std::cout << "\n==== Job Searching Portal ====\n";
        std::cout << "1. Post a New Job\n";
        std::cout << "2. Update My Candidate Profile\n";
        std::cout << "3. Find Jobs (Keyword Search)\n";
        std::cout << "4. Get Job Recommendations\n";
        std::cout << "5. Autocomplete Job Title\n";
        std::cout << "6. Exit\n";
        std::cout << "==============================\n";
        std::cout << "Enter choice: ";
        
        std::cin >> choice;
        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> choice;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                postJob(jobs, skillIndex, locationIndex, jobTitleTrie);
                break;
            case 2:
                updateCandidateProfile(candidate);
                break;
            case 3:
                searchJobs(jobs);
                break;
            case 4:
                if (!candidate.isProfileSet) {
                    std::cout << "\nPlease create your profile first to get recommendations.\n";
                    updateCandidateProfile(candidate);
                }
                recommendJobs(jobs, skillIndex, candidate);
                break;
            case 5:
                autocompleteSearch(jobTitleTrie);
                break;
            case 6:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
