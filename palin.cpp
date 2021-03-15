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
#include <sys/msg.h>
#include <ctime>

using namespace std;

char *mylist;

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    int mesg_index;
    int mesg_pid;
    char mesg_string[100];
}message;

int main(int argc, char* argv[]){

    //Basic Variables
    int shmid;
    int count = 5;
    int sizeMem = 1024;
    key_t key = 800813;
    double RANDMAX = 30000.0;
    double RANDMIN = 00000.0;
    srand(time(0) ^ getpid());
    double sleepTime = (rand()%3000)/1000.0;
    sleep(sleepTime);
    string input;
    int fileLength = 0;
    ifstream file("input.txt");
    string arg = "";
    int index;

    //Parse args from parent;
    for (int i = 0; i < argc; i++){
        arg = argv[i];
        stringstream degree(arg);
        degree >> index;
    }

    //Get filelength
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

    char word[20];
    //Output Specific Index of Palindrome Given
    int len = strlen(mylist);
    for(int row = 0; row < 20; row++){
        word[row] = mylist[20*index+row];
    }

    
    //Basic Palindrome Check Functionality
    int l = 0;
    int h = strlen(word)-2;
    int palindrome = 1;
    while(h > l){
        if(word[l++] != word[h--]){
            palindrome = 0;
            h = l;
        }
    }

    


    //Create Message Queue
    int msgid;
    key_t messageKey;
    int MAX = 10;

    messageKey = ftok("poggies", 65);

    //Change values of message queue to send 
    msgid = msgget(messageKey, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    message.mesg_index = index;
    message.mesg_pid = getpid();
    strcpy(message.mesg_string, word);

    if(palindrome == 1)
        strcpy(message.mesg_text, "Palindrome");
    else if(palindrome == 0)
        strcpy(message.mesg_text, "Not Palindrome");
    else 
        return 0;


    msgsnd(msgid, &message, sizeof(message), 0);
    shmdt((void *) mylist);

    return 0;


}