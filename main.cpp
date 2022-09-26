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
            cout << "File found!" << endl;
        }
    }
    closedir(dp);
    return 0;

}

int main(int argc, char** argv){

    // usage ./main.cpp [absolute path] [filename] --> path must start at /home/.. for now, will be fixed later. 
    // usage should also be  ./myfind [absolute path] [filename] .... in later revisions



    string dir = string("/home/ubuntu/dev/myfind2/test/");
    vector<string> files = vector<string>();


    string filename(argv[2]);
    string directory(argv[1]);
    
    getDirectory(directory, files, argv[2]);


    return 0;
}