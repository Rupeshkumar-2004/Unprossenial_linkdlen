#include "job.h"
#include <iostream>
#include <algorithm>
using namespace std;

void printJob(const Job& job) {
    cout << "\n-------------------------";
    cout << "\nTitle: " << job.title;
    cout << "\nCompany: " << job.company;
    cout << "\nLocation: " << job.location;
    cout << "\nSkills: " << job.skills;
    cout << "\nDescription: " << job.description;
    cout << "\nSalary: " << job.salary;
    cout << "\nTags: ";
    for (auto &tag : job.tags) cout << tag << " ";
    cout << "\n-------------------------\n";
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
