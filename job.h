#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>
using namespace std;

struct Job {
    string title;
    string company;
    string location;
    string skills;
    string description;
    double salary;
    vector<string> tags;
};

// Utility functions
void printJob(const Job& job);
string toLower(string s);

#endif
