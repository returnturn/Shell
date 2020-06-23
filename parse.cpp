#include "headers.h"

//parse a string by spaces, put into a vector
vector<string> parse(string str) { 
    vector<string> re;
    string word = ""; 
    for (int i=0;i<str.length();++i) { 
        if (str[i] == ' ') { 
            if((i+1)<str.length()){
                if(str[i+1]!=' '){
                    re.push_back(word);
                    word = ""; 
                }
            }
        } 
        else { 
            word = word + str[i]; 
        } 
    }  
    re.push_back(word);
    return re;
}