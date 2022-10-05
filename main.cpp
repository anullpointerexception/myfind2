#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <filesystem>
#include <string>
#include <vector>
#include <unistd.h>
#include <thread>
#include <sys/wait.h>
#include <filesystem>
#include <string.h>

using namespace std;
namespace fs = filesystem;

bool recursive = false;
bool case_insensitive = false;

void listFiles(string path, string filename)
{
    if (recursive)
    {
        for (auto const &dir_entry : fs::recursive_directory_iterator(path))
        {
            string name = fs::path(dir_entry).filename();
            // cout << "Filename: " << name << " Relative Path of File: " << fs::path(dir_entry).relative_path() << "\n";
            // cout << "Filename dir: " << name << endl;
            // cout << "Filename given " << filename << endl;

            if(case_insensitive){
                if(strcasecmp(name.c_str(), filename.c_str()) == 0){
                    cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << " using recursion " << endl;
                    return;
                }
            }else{
                if(strcmp(name.c_str(), filename.c_str()) == 0){
                    cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << " using recursion " << endl;
                    return;
                }
            }
        }
        cout << "File " << filename << " not found" << endl;
    }
    else
    {
        for (auto const &dir_entry : fs::directory_iterator(path))
        {
            string name = fs::path(dir_entry).filename();
            if(case_insensitive){
                if(strcasecmp(name.c_str(), filename.c_str())==0){
                    cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << endl;
                    return;
                }
            }else{
                if(strcmp(name.c_str(), filename.c_str())==0){
                    cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << endl;
                    return;
                }
            }
        }
        cout << "File " << filename << " not found" << endl;
    }
    //return -2;
}

void test(string filename){
    cout << getppid() << endl;
    cout << getpid() << " File: " << filename << endl;
}

int main(int argc, char *argv[])
{

    string search_path;
    vector<string> files;

    pid_t parent_pid = getpid();

    if (argc < 3)
    {
        cerr << "ERROR: missing parameters\nRequired parameters: searchpath, filename\nOptional parameters: additional filenames, \"-i\": insensitive search, \"-R\": search in subdirectories" << endl;
        return -1;
    }

    int opt;

    //Flag handling
    while ((opt = getopt(argc, argv, ":iR")) != -1)
    {
        switch (opt)
        {
        case 'R':
            if (recursive == true)
            {
                cerr << "ERROR: Invalid flag!" << endl;
                return -1;
            }
            cout << "switched to recursive mode" << endl;
            recursive = true;

            break;
        case 'i':
            if (case_insensitive == true)
            {
                cerr << "ERROR: Invalid flag!" << endl;
                return -1;
            }
            //cout << "using case insensitive mode" << endl;
            case_insensitive = true;
            break;
        case '?':
            cerr << "ERROR: Invalid flag!" << endl;
            return -1;
        }
    }

    search_path = argv[optind++];
    //TODO: check if valid searchpath
    for (; optind < argc; ++optind)
    {
        files.push_back(argv[optind]);
    }
    string pathVar = fs::current_path();
    if (recursive == false)
    {
        cout << "Search path: " << search_path << endl;
        pathVar = pathVar + search_path;
    }
    cout << "pathVar " << pathVar << endl;

    cout << "Parent PID: " << parent_pid << endl;

    for (unsigned int i = 0; i < files.size(); ++i)
    {
        //start child process with a file each
        if(fork()==0){
            listFiles(pathVar, files[i]);
            exit(0);
        }    
    }

    for(unsigned int i=0;i<files.size();++i){
        wait(nullptr);
    }

    return 0;
}