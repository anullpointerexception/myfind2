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

using namespace std;
namespace fs = filesystem;

bool recursive = false;
bool case_insensitive = false;

void listFiles(string path, string filename)
{
    if (recursive)
    {
        if (case_insensitive)
        {
            // i dunno what here? -> all to lower case?
        }
        else
        {
        }
        for (auto const &dir_entry : fs::recursive_directory_iterator(path))
        {
            string name = fs::path(dir_entry).filename();
            string extension = fs::path(dir_entry).extension();
            // cout << "Filename: " << name << " Relative Path of File: " << fs::path(dir_entry).relative_path() << "\n";
            // cout << "Filename dir: " << name << endl;
            // cout << "Filename given " << filename << endl;
            if (name == filename)
            {
                cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << " using recursion " << endl;
                //return 1;
            }
            else
            {
                //return -1;
            }
        }
    }
    else
    {
        for (auto const &dir_entry : fs::directory_iterator(path))
        {
            string name = fs::path(dir_entry).filename();
            cout << name << endl;
            if (name == filename)
            {
                cout << "File " << filename << " found @ location " << fs::path(dir_entry).relative_path() << endl;
                break;
                //return 1;
            }
            else
            {

                cout << "File " << filename << " not found" << endl;
                //return -1;
            }
        }
    }
    //return -2;
}

int main(int argc, char *argv[])
{

    string search_path;
    vector<string> files;

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
    //check if valid searchpath
    for (; optind < argc; ++optind)
    {
        files.push_back(argv[optind]);
    }
    string pathVar = fs::current_path();
    if (recursive == true)
    {
        string pathVar = fs::current_path();
    }
    else
    {
        cout << "Search path: " << search_path << endl;
        pathVar = pathVar + search_path;
    }
    cout << "pathVar " << pathVar << endl;

    for (unsigned int i = 0; i < files.size(); ++i)
    {
        cout << "File " << i + 1 << ": " << files[i] << endl;
        //start child process with a file each
        listFiles(pathVar, files[i]);
    }

    return 0;
}