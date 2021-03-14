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

char *mylist;

int main(int argc, char* argv[]){
    int shmid;
    int count = 5;
    int sizeMem = 1024;
    key_t key = 800813;
    
    string input;
    int fileLength = 0;
    ifstream file("input.txt");
    
    while(getline(file, input)){
        fileLength++;
    }

    //Get ID
    shmid = shmget(key, count*sizeof(string), IPC_EXCL);
    if (shmid == -1) {
        perror("Shared memory");
        return 1;
    }
    

    //Attach each part of array
    mylist = (char*)shmat(shmid,NULL,0);
        if(mylist == (void *) -1){
            perror("Shared memory attach");
        return 1;
        }

    //Output
    for(int col = 0; col < fileLength; col++){
        for(int row = 0; row < 20; row++){
            cout << mylist[20*col+row];
        }
        cout << endl;
    }


    
    shmdt((void *) mylist);

    cout << "Ending child process" <<endl;
    return 0;


}