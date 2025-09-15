#ifndef JOB_PORTAL_H
#define JOB_PORTAL_H

#include "job.h"
#include "candidate.h"
#include <vector>
using namespace std;

// Core functions
void postJob(vector<Job>& jobs);
void searchJobs(const vector<Job>& jobs);
void recommendJobs(const vector<Job>& jobs, Candidate& candidate);

#endif
