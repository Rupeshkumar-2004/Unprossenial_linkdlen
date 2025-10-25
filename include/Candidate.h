#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
#include <vector>

struct Candidate {
    std::string name;
    std::vector<std::string> skills;
    std::string preferredLocation;
    double expectedSalary;
    bool isProfileSet = false;
};

#endif // CANDIDATE_H
