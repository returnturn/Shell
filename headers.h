#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <algorithm>
#include <sys/time.h>
#include <sys/resource.h>
#include <set>

using namespace std;

vector<string> parse(string str);
int numSec(string time);
void printTime();
void printJobs();

extern set<int> st;