#include "models.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

extern double THETA;

// additional functions

list<Job>::iterator removeJobFromListAndMoveToNext(list<Job> &jobs, list<Job>::iterator i) {
  list<Job>::iterator next = i; 
  next++;
  jobs.erase(i);
  return next;
}

void printJobs(list<Job> jobs) {
  cout << "Jobs: [";
  for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j) {
    if (j != jobs.begin()) cout << ", ";
    cout << j->toString();
  }
  cout << "]" << endl;
}

void printWorkers(list<Worker> workers) {
  cout << "Workers:" << endl;
  for (list<Worker>::iterator w = workers.begin(); w != workers.end(); ++w) {
    cout << w->toString() << endl;
  }
}

// Job

Job::Job(int id, int cpuUsage, int remainingTime) {
  this->id = id;
  this->cpu_usage = cpuUsage;
  this->remaining_time = remainingTime;
}

int Job::getId() {
  return this->id;
}

int Job::getCpuUsage() {
  return this->cpu_usage;
}

int Job::getRemainingTime() {
  return this->remaining_time;
}

bool Job::isCompleted() {
  return this->remaining_time == 0;
}

bool Job::runOneSecond() {
  assert(this->isCompleted() == false);
  --(this->remaining_time);
  return true;
}

string Job::toString() {
  stringstream ss;
  ss << "{id=" << this->id << ", cpu=" << this->cpu_usage << ", t=" << this->remaining_time << "}";
  return ss.str();
}

// Worker

Worker::Worker(int id) {
  this->id = id;
  this->cpu_usage = 100;
  jobs.clear();
}

int Worker::getId() {
  return this->id;
}

void Worker::runOneSecond() {
  list<Job>::iterator i = this->jobs.begin();
  while (i != jobs.end()) {
    bool result = i->runOneSecond();
    assert(result == true);
    if (i->isCompleted()) {
      this->cpu_usage += i->getCpuUsage();
      i = removeJobFromListAndMoveToNext(this->jobs, i);
    }
    else {
      i++;
    }
  }
}

bool Worker::canAssignJob(Job job) {
  return this->cpu_usage >= job.getCpuUsage();
}

void Worker::assignJob(Job job) {
  assert(this->canAssignJob(job) == true);
  this->jobs.push_back(job);
  this->cpu_usage -= job.getCpuUsage();
}

bool Worker::isFree() {
  return this->cpu_usage == 100;
}

string Worker::toString() {
  stringstream ss;
  ss << "  id=" << this->id << ", cpu=" << this->cpu_usage << ", jobs=[";
  for (list<Job>::iterator j = this->jobs.begin(); j != this->jobs.end(); ++j) {
    if (j != this->jobs.begin()) ss << ", ";
    ss << j->toString();
  }
  ss << "]";
  return ss.str();
}

// Scheduler

Scheduler::Scheduler(list<Job> jobs, list<Worker> workers) {
  this->jobs = jobs;
  this->workers = workers;
  this->time = 0;
  this->log_enabled = false;

  this->total_jobs_cpu_usage = 0;
  for (list<Job>::iterator j = jobs.begin(); j != jobs.end(); ++j) {
    this->total_jobs_cpu_usage += j->getCpuUsage() * j->getRemainingTime();
  }
}

void Scheduler::enableLog() {
  this->log_enabled = true;
}

void Scheduler::runOneSecond() {
  for (list<Worker>::iterator w = this->workers.begin(); w != this->workers.end(); ++w) {
    w->runOneSecond();
  }
}

bool Scheduler::allJobsCompleted() {
  if (this->jobs.size() > 0) {
    return false;
  }
  for (list<Worker>::iterator w = this->workers.begin(); w != this->workers.end(); ++w) {
    if (w->isFree() == false) {
      return false;
    }
  }
  return true;
}

void Scheduler::approach1() {
  while (this->allJobsCompleted() == false) {
    list<Job>::iterator j = this->jobs.begin();
    while (j != this->jobs.end()) {
      bool assigned = false;
      for (list<Worker>::iterator w = this->workers.begin(); w != this->workers.end(); ++w) {
        if (w->canAssignJob(*j)) {
          w->assignJob(*j);
          j = removeJobFromListAndMoveToNext(this->jobs, j);
          assigned = true;
          break;
        }
      }
      if (assigned == false) ++j;
    }
    
    this->log();

    this->runOneSecond();
    ++this->time;
  }
}

void Scheduler::approach2() {
  this->jobs.sort(this->jobCompare1);
  this->approach1();
}

void Scheduler::approach3() {
  this->jobs.sort(this->jobCompare2);
  this->approach1();
}

void Scheduler::approach4() {
  this->jobs.sort(this->jobCompare3);
  this->approach1();
}

void Scheduler::approach5() {
  this->jobs.sort(this->jobCompare4);
  this->approach1();
}

void Scheduler::approach6() {
  this->jobs.sort(this->jobCompare5);
  this->approach1();
}

void Scheduler::selectApproach(int approach) {
  this->approach = approach;
}

void Scheduler::schedule() {
  if (this->approach == 1) this->approach1();
  else if (this->approach == 2) this->approach2();
  else if (this->approach == 3) this->approach3();
  else if (this->approach == 4) this->approach4();
  else if (this->approach == 5) this->approach5();
  else if (this->approach == 6) this->approach6();
  else this->approach3();
}

void Scheduler::log() {
  if (this->log_enabled) {
    cout << "Time: " << this->time << endl;
    printJobs(this->jobs);
    printWorkers(this->workers);
    cout << endl;
  }
}

double Scheduler::getAverageUtilization() {
  assert(this->allJobsCompleted() == true);
  return this->total_jobs_cpu_usage / (this->time * this->workers.size() * 100.0); 
}

bool Scheduler::jobCompare1(Job &a, Job &b) {
  if (a.getCpuUsage() > b.getCpuUsage()) return true;
  if (a.getCpuUsage() < b.getCpuUsage()) return false;
  return a.getId() < b.getId();
}

bool Scheduler::jobCompare2(Job &a, Job &b) {
  if (a.getRemainingTime() > b.getRemainingTime()) return true;
  if (a.getRemainingTime() > b.getRemainingTime()) return false;
  return a.getId() < b.getId();
}

bool Scheduler::jobCompare3(Job &a, Job &b) {
  if (a.getCpuUsage() > b.getCpuUsage()) return true;
  if (a.getCpuUsage() < b.getCpuUsage()) return false;
  if (a.getRemainingTime() > b.getRemainingTime()) return true;
  if (a.getRemainingTime() > b.getRemainingTime()) return false;
  return a.getId() < b.getId();
}

bool Scheduler::jobCompare4(Job &a, Job &b) {
  if (a.getRemainingTime() > b.getRemainingTime()) return true;
  if (a.getRemainingTime() < b.getRemainingTime()) return false;
  if (a.getCpuUsage() > b.getCpuUsage()) return true;
  if (a.getCpuUsage() < b.getCpuUsage()) return false;
  return a.getId() < b.getId();
}

bool Scheduler::jobCompare5(Job &a, Job &b) {
  double wa = a.getCpuUsage() + THETA * a.getRemainingTime();
  double wb = b.getCpuUsage() + THETA * b.getRemainingTime();
  if (wa > wb) return true;
  if (wa < wb) return false;
  return a.getId() < b.getId();
}


