#include "headers.h"
//set for saving "suspend" processes
set<int> st;

//handle exit
void sigHandler(int signum){
    if(signum==15){
        _exit(0);
    }
    return;
}

void catcher(){
    struct sigaction sa;
    sa.sa_handler = sigHandler;                                                                                                                                                                          
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGTSTP,&sa,NULL);
}

//execute command using execvp
void execute(vector<string> v, bool bg, int inInx, int outInx) {
	//inFile and outFile
    string inF,outF;
    int in, out;
    if(inInx){
        inF=v[inInx].substr(1,v[inInx].length()-1);
        v.erase(v.begin()+inInx);
    }
    if(outInx){
        outF=v[outInx].substr(1,v[outInx].length()-1);
        v.erase(v.begin()+outInx);
    }
    //prepare arguments
    char *envp[]={NULL};
    vector<string> paths;
    paths.push_back("/bin/");
    paths.push_back("/usr/bin/");
    int status;
    struct stat buffer;
    char *argv1[v.size()+1];
    for(int i=0;i<v.size();++i){
        char* arg = &v[i][0];
        argv1[i]=arg;
    }
    argv1[v.size()]=NULL;
    const char*argv0;
    for(unsigned int i=0;i<paths.size();i++){
        string path=paths[i];
        string argv0_ = path+v[0];
        argv0 = argv0_.c_str();
        status = stat(argv0,&buffer);
        if(status==0){
            break;
        }
    }
    if(status==0){
        pid_t pid = fork();
        if(pid==0){
        	//have inFile
            if(inInx){
                in = open(inF.c_str(), O_RDONLY);
                dup2(in, 0);
                close(in);
            }
            //have outFile
            if(outInx){
                out = open(outF.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                dup2(out, 1);
                close(out);
            }
            if(!bg) execve(argv0,argv1,envp);
            else {
            	//run in background
                if(!outInx){
                    int fn = open("/dev/null",O_WRONLY);
                    dup2(fn,1);
                    close(fn);
                }
                execve(argv0,argv1,envp);
            }
        }
        else if(pid<0){
            perror("fork error");
        }
        else{
            int status;
            waitpid(pid,&status,0);
        }
    }
    return;
}

int main(int argc, char **argv) {
    catcher();
    while(1){
        cout<<"SHELL379: ";
        string input="";
        getline(cin,input);
        if(input.length()!=0){
        	//basic commands
            if(input=="jobs") printJobs();
            else{
                vector<string> v=parse(input);
                if(v[0]=="kill"){
                    kill(stoi(v[1]),SIGKILL);
                }
                else if(v[0]=="suspend"){
                    st.insert(stoi(v[1]));
                    kill(stoi(v[1]),SIGSTOP);
                }
                else if(v[0]=="resume"){
                    st.erase(stoi(v[1]));
                    kill(stoi(v[1]),SIGCONT);
                }
                else if(v[0]=="sleep"){
                    sleep(stoi(v[1]));
                }
                else if(v[0]=="wait"){
                    waitpid(stoi(v[1]),0,0);
                }
                else if(v[0]=="exit"){
                    cout<<"Resources used"<<endl;
                    printTime();
                    kill(0,SIGTERM);
                }
                //otherwise
                else{
                    int outInx=0, inInx=0;
                    //have input/output?
                    for(int i=1;i<v.size();++i){
                        if(v[i][0] =='>'){
                            outInx=i;
                        }
                        if(v[i][0] =='<'){
                            inInx=i;
                        }
                    }
                    //run in background?
                    if(v[v.size()-1]!="&") execute(v,false,inInx,outInx);
                    else{
                        v.pop_back();
                        execute(v,true,inInx,outInx);
                    }
                }
            }
        }
        
    }
    return 0;
}