#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <filesystem>
#include <string>
#include <vector>


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
        if(fname.find(filename) != string::npos){
            files.push_back(fname);
        }
    }
    closedir(dp);
    return 0;

}

int main(int argc, char** argv){


    string dir = string("/home/ubuntu/dev/myfind2/test/");
    vector<string> files = vector<string>();


    string filename(argv[2]);
    string directory(argv[1]);
    
    getDirectory(directory, files, argv[2]);
    for(unsigned int i = 0; i < files.size(); i++){
        cout << files[i] << endl;
    }


    return 0;
}