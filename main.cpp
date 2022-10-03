#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unistd.h>
#include <thread>

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


void testForThreads(string param){
    //temp function
    cout << "Thread searches: " << param << endl;
}

int main(int argc, char* argv[]){

    bool recursive = false;
    bool caseinsensitive = false;
    
    if(argc<3){
        cerr << "ERROR: missing parameters\nRequired parameters: searchpath, filename\nOptional parameters: additional filenames, \"-i\": insensitive search, \"-R\": search in subdirectories" << endl;
        return -1;
    }

    int opt;

    int options = 0;
    vector<thread> threadpool;

    while((opt = getopt(argc, argv, ":iR")) != -1){
        switch(opt){
            case 'R':
                cout << "switched to recursive mode" << endl;
                options++;
                recursive = true;
                break;
            case 'i':
                cout << "using case insensitive mode" << endl;
                options++;
                caseinsensitive = true;
                break;
        }
    }

    cout << "Searchpath: " << argv[optind++] << endl;

    if(recursive){
        for(int i=0;i<argc-options-2;++i){
            thread thread_obj(testForThreads, argv[optind++]); //search also in subdirectories recursively
            threadpool.push_back(move(thread_obj));
        }
    }else{
        for(int i=0;i<argc-options-2;++i){
            thread thread_obj(testForThreads, argv[optind++]); //search only in given directory (function, params)
            threadpool.push_back(move(thread_obj));
        }
    }
    
    
    for(int i=0;i<threadpool.size(); ++i){
        threadpool[i].join();
    }

    char currentLocation[256];

    getcwd(currentLocation, 256);

    // usage ./main.cpp [absolute path] [filename] --> path must start at /home/.. for now, will be fixed later. 
    // usage should also be  ./myfind [absolute path] [filename] .... in later revisions

    // string dir = string("/home/ubuntu/dev/myfind2/test/");
    vector<string> files = vector<string>();

    // cout << "Current Location: " << currentLocation << endl;

    string currentLocationString(currentLocation);
    string currentFileString(argv[options+1]);

    string finaldir = currentLocationString + currentFileString;

    string filename(argv[options+2]);
    
    // getDirectory(finaldir, files, argv[options+2]);

    for(int i = 0; i < files.size(); i++){
        // cout << files[i] << endl;
    }
    // cout << "finaldir: " << finaldir << endl;
    listDirectoryRecursive(finaldir);


    return 0;
}