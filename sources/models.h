#ifndef _MODELS_H
#define _MODELS_H

#include <list>
#include <string>

using namespace std;

class Job {
private:
  int id;
  int cpu_usage;
  int remaining_time;
public:
  Job(int, int, int);
  int getId();
  int getCpuUsage();
  int getRemainingTime();
  bool runOneSecond();
  bool isCompleted();
  string toString();
};

class Worker {
private:
  int id;
  int cpu_usage;
  list<Job> jobs;
public:
  Worker(int);
  int getId();
  bool canAssignJob(Job);
  void assignJob(Job);
  void runOneSecond();
  bool isFree();
  string toString();
};

class Scheduler {
private:
  int time;
  int total_jobs_cpu_usage;
  int approach;
  bool log_enabled;
 list<Job> jobs;
  list<Worker> workers;
  void assignJobToWorker(list<Job>::iterator, list<Worker>::iterator);
  void runOneSecond();
  bool allJobsCompleted();
  void log();
  void approach1();
  void approach2();
  void approach3();
  void approach4();
  void approach5();
  void approach6();
  static bool jobCompare1(Job&, Job&);
  static bool jobCompare2(Job&, Job&);
  static bool jobCompare3(Job&, Job&);
  static bool jobCompare4(Job&, Job&);
  static bool jobCompare5(Job&, Job&);
public:
  Scheduler(list<Job>, list<Worker>);
  void selectApproach(int);
  void enableLog();
  void schedule();
  double getAverageUtilization();
};

#endif
