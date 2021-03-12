#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <csignal>
#include <bits/stdc++.h>

using namespace std;
const char *mylist[10];


int main(int argc, char* argv[]){
    int shmid;
    string arg = "";
    string argNext = "";
    int count = 5;
    //Parsing command line inputs
    for(int i  = 1; i < argc; i++){
        arg = argv[i];
        if(arg == "-h"){
            cout << "Help:" << endl;
            cout << "master [-h] [-c i] [-m j] datafile" <<endl;
            cout << " -h : Help Options" <<endl;
            cout << " -c : Indicate how many child processes i should be launched in total" <<endl;
            cout << " -m : Indicate the maximum number of children j allowed to exist in the system at the same time. (Default 20)" <<endl;
            cout << " datafile : Input file containing the plaindromes, one per line" <<endl;
            cout << "Example run: ./Coordinator -c 7 -m 20 input.txt" <<endl;
            return 0;
        } else if(arg == "-c"){
            argNext = argv[i+1];
            cout << "-c " << argNext << endl;
        } else if(arg == "-m"){
            argNext = argv[i+1];
            cout << "-m " << argNext << endl;
        }
    }


    //Create shared memory
    int sizeMem = 1024;
    key_t key = 8008;

    //Get SHMID, check for errors
    shmid = shmget(key, count*sizeof(string), 0644|IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }

    //Check file length and open the files
    string input;
    int fileLength = 0;
    string fileName = argv[argc-1];
    fstream file;
    int incr = 0;
    int temp;
    file.open(fileName.c_str());
    while(getline(file,input)){
        fileLength++;
    }
    
    file.clear();
    file.seekg(0,ios::beg);

    //Create a new array of strings, so that I can use c_str later
    string *allInputs;
    allInputs = new string[fileLength];
    for(int i = 0; i < fileLength; i++){
        getline(file,input);
        allInputs[i] = input;
    }

    



    //Attach 
    for(int i = 0 ; i < fileLength; i++){
    mylist[i] = (char*)shmat(shmid,NULL,0);
        if(mylist == (void *) -1){
            perror("Shared memory attach");
         return 1;
        }
    }

    file.clear();
    file.seekg(0,ios::beg);
    //Parse input
    

    //Take inputs and add the c_str version to the array of characters.
    for(int i = 0 ; i < fileLength; i++){
        mylist[i] = allInputs[i].c_str();
    }

    for(int i = 0; i < fileLength; i++){
        cout << mylist[i] <<endl;
    }

   




}