#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <csignal>
#include <bits/stdc++.h>
#include <sys/msg.h>
#include <string>

using namespace std;

char *mylist;

struct mesg_buffer{
    long mesg_type;
    char mesg_text[100];
} message;

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
    key_t key = 800813;

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

    
    mylist = (char*)shmat(shmid,NULL,0);
    if(mylist == (void *) -1){
        perror("Shared memory attach");
        return 1;
    }

    file.clear();
    file.seekg(0,ios::beg);
    //Parse input
    

    //Using 1D array as a 2D array
    for(int col = 0; col < fileLength; col++){
        getline(file,input);
        for(int row = 0; row < input.length(); row++){
            mylist[20*col+row] = input[row];
        }
    }

    //Testing output for 1D array as 2D Array
    //for(int col = 0; col < fileLength; col++){
        //for(int row = 0; row < 20; row++){
            //cout << mylist[20*col+row];
        //}
        //cout << endl;
    //}

    char buffer[50];
    
    for(int i = 0; i < fileLength; i++){
        cout << "Forking Child" <<endl;
        sprintf(buffer, "%d", i);
        if(fork() == 0)
            execl("./palin", buffer);
    }

    

    //Receive message queue
    key_t messageKey = ftok("pog", 65);
    int msgid;

    msgid = msgget(messageKey, 0666|IPC_CREAT);

    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Data Received is : %s \n", message.mesg_text);

    msgctl(msgid, IPC_RMID, NULL);
    wait(NULL);




    shmdt((void *) mylist);
    cout << "Complete" <<endl;

   




}