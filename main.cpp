#include <iostream>
#include <vector>
#include "job_portal.h"
using namespace std;

int main() {
    vector<Job> jobs;
    Candidate candidate;
    int choice;

    while (true) {
        cout << "\n==== Job Searching Portal ====\n";
        cout << "1. Post Job\n";
        cout << "2. Search Jobs\n";
        cout << "3. Recommend Jobs\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: postJob(jobs); break;
            case 2: searchJobs(jobs); break;
            case 3: recommendJobs(jobs, candidate); break;
            case 4: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice!\n";
        }
    }
}
