#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "models.h"

using namespace std;

double THETA;
double maxTheta = -1;
double maxAverageUtilization = -1;

bool readParameter(int argc, char *argv[], int &approach, bool &log) {
  string message = "Usage: " + string(argv[0]) + " --approach=<1/2/3/4/5/6> --log=<true/1/false/0>";
  
  for (int i = 1; i < argc; ++i) {
    string tmp = string(argv[i]);

    if (tmp.find("--approach=") == 0) {
      approach = atoi(tmp.substr(string("--approach=").length()).c_str());
      if (approach < 1 || approach > 6) {
        cout << "Error: Invalid approach." << endl;
        return false;
      }
    }
    else if (tmp.find("--log=") == 0) {
      string strlog = tmp.substr(string("--log=").length());
      if (strlog == "true" || strlog == "1") log = true;
      else if (strlog == "false" || strlog == "0") log = false;
      else {
        cout << "Error: Invalid log value." << endl;
        return false;
      }
    }
  }
  
  if (approach == 0) {
    cout << message << endl;
    return false;
  }
  return true;
}

double averageUtilizationAtTheta(list<Job> jobs, list<Worker> workers, double theta) {
  THETA = theta;
  Scheduler scheduler(jobs, workers);
  scheduler.selectApproach(6);
  scheduler.schedule();
  double u = scheduler.getAverageUtilization();
  if (u > maxAverageUtilization) {
    maxAverageUtilization = u;
    maxTheta = THETA;
  }
  return u;
}

void moveTHETA(list<Job> jobs, list<Worker> workers, double left, double right, double delta) {
  double current_average_utilization = averageUtilizationAtTheta(jobs, workers, left);
  double current_theta = left;
  while (current_theta + delta <= right) {
    double next_theta = current_theta + delta;
    double next_average_utilization = averageUtilizationAtTheta(jobs, workers, next_theta);
    if (current_average_utilization != next_average_utilization && delta > 0.001) {
      moveTHETA(jobs, workers, current_theta, next_theta, delta / 10.0);
    }
    current_theta = next_theta;
    current_average_utilization = next_average_utilization; 
  }
}

int main(int argc, char *argv[]) {
  // read arguments
  int approach = 0;
  bool log_enabled = false;
  if (readParameter(argc, argv, approach, log_enabled) == false) {
    return 0;
  }

  // read input
  int n;

  list<Worker> workers;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    workers.push_back(Worker(i));
  }

  list<Job> jobs;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    int cpu_usage, time;
    cin >> cpu_usage >> time;
    jobs.push_back(Job(i, cpu_usage, time));
  }
  
  // run scheduler
  if (approach != 6) {
    Scheduler scheduler(jobs, workers);
    scheduler.selectApproach(approach);
    if (log_enabled) scheduler.enableLog();
    scheduler.schedule();
    cout << "Average utilization: " << scheduler.getAverageUtilization() << endl;
  }
  else {
    moveTHETA(jobs, workers, 0, 1, 0.1);
    moveTHETA(jobs, workers, 1, 10, 1);
    moveTHETA(jobs, workers, 10, 110, 10);
    cout << "Average utilization: " << maxAverageUtilization << " " << maxTheta << endl;
  }
   
  return 0;
}

