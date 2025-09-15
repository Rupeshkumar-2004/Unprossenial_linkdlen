#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>

struct Job {
    std::string title;
    std::string company;
    std::string location;
    std::vector<std::string> skills;
    std::string description; // Added for more detailed info
    double salary;
    std::vector<std::string> tags;
};

#endif // JOB_H