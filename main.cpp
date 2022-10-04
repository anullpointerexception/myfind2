#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unistd.h>
#include <thread>
#include <sys/wait.h>

using namespace std;

int getDirectory(string dir, vector<string> &files, char* filename){

    DIR *dp;

    struct dirent *dirEntry;

    if((dp = opendir(dir.c_str())) == NULL){
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    while((dirEntry = readdir(dp)) != NULL){
        string fname = dirEntry->d_name;
        // cout << dirEntry->d_name;
        if(fname.find(filename) != string::npos){
            files.push_back(fname);
            // cout << "File found!" << endl;
        }
    }
    closedir(dp);
    return 0;
}

void listDirectoryRecursive(const string& location){
    DIR *dp;
    struct dirent *entry;

    if((dp = opendir(location.c_str())) != NULL){
        while((entry = readdir(dp)) != NULL){
           if(string(entry->d_name).compare(".") != 0){
                cout<< entry->d_name << endl;
            } else {
                string nextDir = string(entry -> d_name);
                nextDir += "\\";

                cout <<  nextDir << endl;

                listDirectoryRecursive(nextDir);
            }
        }
    }
    closedir(dp);
}


int main(int argc, char* argv[]){

    bool recursive = false;
    bool case_insensitive = false;
    string search_path;
    vector<string> files;
    
    if(argc<3){
        cerr << "ERROR: missing parameters\nRequired parameters: searchpath, filename\nOptional parameters: additional filenames, \"-i\": insensitive search, \"-R\": search in subdirectories" << endl;
        return -1;
    }

    int opt;

    vector<thread> threadpool;

    while((opt = getopt(argc, argv, ":iR")) != -1){
        switch(opt){
            case 'R':
                if(recursive == true){
                    cerr << "ERROR: Invalid flag!" << endl;
                    return -1;
                }
                cout << "switched to recursive mode" << endl;
                recursive = true;
                break;
            case 'i':
                if(case_insensitive == true){
                    cerr << "ERROR: Invalid flag!" << endl;
                    return -1;
                }
                cout << "using case insensitive mode" << endl;
                case_insensitive = true;
                break;
            case '?':
                cerr << "ERROR: Invalid flag!" << endl;
                return -1;
        }
    }

    search_path = argv[optind++];

    cout << "Searchpath:  " << search_path << endl;

    for(;optind<argc;++optind){
        files.push_back(argv[optind]);
    }

    for(unsigned int i=0;i<files.size();++i){
        cout << "File " << i+1 << ": " << files[i] << endl;
    }

    return 0;
}