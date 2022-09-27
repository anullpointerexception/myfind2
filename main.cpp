#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unistd.h>




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
            // cout << "File found!" << endl;
        }
    }
    closedir(dp);
    return 0;

}

int main(int argc, char* argv[]){

    int opt;

    int options;

    while((opt = getopt(argc, argv, ":if:lrx")) != -1){
        switch(opt){
            case 'r':
                cout << "switched to recursive mode" << endl;
                options++;
                break;
            case 'i':
                cout << "using case insensitive mode" << endl;
                options++;
                break;
        }
    }

    cout << "Filename " << argv[options+2] << endl;




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
    
    getDirectory(finaldir, files, argv[options+2] );

    for(int i = 0; i < files.size(); i++){
        cout << files[i] << endl;
    }

    return 0;
}