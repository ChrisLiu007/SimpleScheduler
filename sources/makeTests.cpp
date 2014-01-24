#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

using namespace std;

string serialize(int a, int b, int c, int d) {
  stringstream ss;
  
  ss << a << "_";
  
  if (b < 10) ss << "00";
  else if (b < 100) ss << "0";
  ss << b << "_";

  if (c < 10) ss << "0";
  ss << c << "_";
  
  if (d < 10) ss << "0";
  ss << d << "_in.txt";
  
  return ss.str();
}

void makeTest(string folder, int n_workers, int nJobs, int percentHighCpu, int percentLongDuration) {

  string file = folder + serialize(n_workers, nJobs, percentHighCpu, percentLongDuration);
  ofstream fo(file.c_str());
  
  fo << n_workers << endl;
  fo << nJobs << endl;
  
  for (int i = 0; i < nJobs; ++i) {
    if (percentHighCpu > 0) {
      if (rand() % 100 < percentHighCpu) {
        fo << 75 + (rand() % 6) * 5 << " ";
      }
      else {
        fo << (rand() % 5 + 1) * 5 << " ";
      }
    } else {
      fo << (rand() % 5 + 1) * 20 << " ";
    }
    
    if (percentLongDuration > 0) {
      if (rand() % 100 < percentLongDuration) {
        fo << 75 + (rand() % 6) * 5 << endl;
      }
      else {
        fo << (rand() % 5 + 1) * 5 << endl;
      }
    }
    else {
      fo << (rand() % 5 + 1) * 20 << endl;
    }
  }

  fo.close();
}

int get(int i) {
  switch (i) {
    case 0: return 0;
    case 1: return 10;
    case 2: return 90;
  }
}

int get1(int i) {
  switch (i) {
    case 0: return 0;
    case 1: return 20;
    case 2: return 80;
  }
}

int get2(int i) {
  switch (i) {
    case 0: return 0;
    case 1: return 30;
    case 2: return 70;
  }
}

int main() {
  srand(0);

  int n_workers = 20;

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1);
        makeTest("tests/1/", n_workers, n_jobs, get(u), get(v));
      }
    }
  }

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1);
        makeTest("tests/2/", n_workers, n_jobs, get1(u), get1(v));
      }
    }
  }

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1);
        makeTest("tests/3/", n_workers, n_jobs, get2(u), get2(v));
      }
    }
  }

  n_workers = 2;

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1) * (j + 1);
        makeTest("tests/4/", n_workers, n_jobs, get(u), get(v));
      }
    }
  }

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1) * (j + 1);
        makeTest("tests/5/", n_workers, n_jobs, get1(u), get1(v));
      }
    }
  }

  for (int u = 0; u < 3; ++u) {
    for (int v = 0; v < 3; ++v) {
      for (int j = 0; j < 10; ++j) {
        int n_jobs = n_workers * (j + 1) * (j + 1);
        makeTest("tests/6/", n_workers, n_jobs, get2(u), get2(v));
      }
    }
  }

  return 0;
}

