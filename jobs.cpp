#include "headers.h"
int numSec(string time){
    int m=stoi(time.substr(0,1));
    int s=stoi(time.substr(2,2));
    int ms=stoi(time.substr(5,2));
    if(ms>50) ms=1;
    else ms=0;
    int re=m*60+s+ms;
    return re;
}

//print out time
void printTime() {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    string utime=to_string(ru.ru_utime.tv_sec) + '.' + to_string(ru.ru_utime.tv_usec);
    string stime=to_string(ru.ru_stime.tv_sec) + '.' + to_string(ru.ru_stime.tv_usec);
    cout<<"User time ="<<setw(7)<<utime<<" seconds"<<endl;
    cout<<"Sys  time ="<<setw(7)<<stime<<" seconds"<<endl;
}

//print the job table
void printJobs() {
    cout<<endl;
    //pipe to ps
    FILE *fp;
    fp = popen("ps", "r");
    int numLine=0;
    cout<<"Running processes:"<<endl;
    if(fp!=NULL){
        char line[100];
        cout<<" #    PID S SEC COMMAND"<<endl;
        bool first=true;
        //read line by line
        while (fgets(line, sizeof(line), fp) != NULL) {
            if(!first){
                vector<string> processData = parse(line);
                string status = " R ";
                if(st.find(stoi(processData[0])) != st.end()){
                    status = " S ";
                }
                cout<<' '<<numLine<<':'<<setw(6)<<processData[0]<<status<<setw(3)<<numSec(processData[2])<<' '<<processData[3];
                numLine++;
            }
            else first=false;
        }
    }
    cout<<"Processes ="<<setw(7)<<max(0,numLine)<<" active"<<endl;
    cout<<"Completed processes:"<<endl;
    printTime();
    cout<<endl;
}