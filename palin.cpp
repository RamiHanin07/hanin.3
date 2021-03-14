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
}message;

int main(int argc, char* argv[]){
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

    //Output all in memory
    //for(int col = 0; col < fileLength; col++){
        //for(int row = 0; row < 20; row++){
            //cout << mylist[20*col+row];
        //}
        //cout << endl;
    //}
    char word[20];
    //Output Specific Index of Palindrome Given
    int len = strlen(mylist);
    for(int row = 0; row < 20; row++){
        word[row] = mylist[20*index+row];
        //cout << mylist[20*index+row];
        cout << word[row];
    }

    cout << endl;
    

    int l = 0;
    int h = strlen(word)-2;


    int palindrome = 1;
    while(h > l){
        if(word[l++] != word[h--]){
            palindrome = 0;
            h = l;
        }
    }

    cout << palindrome <<endl;

    


    //Create Message Queue
    int msgid;
    key_t messageKey;
    int MAX = 10;

    messageKey = ftok("pog", 65);

    msgid = msgget(messageKey, 0666 | IPC_CREAT);
    message.mesg_type =1;

    if(palindrome == 1)
        strcpy(message.mesg_text, "Palindrome");
    else if(palindrome == 0)
        strcpy(message.mesg_text, "Not Palindrome");
    else 
        return 0;


    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Data send is : %s \n", message.mesg_text);

    
    shmdt((void *) mylist);

    //cout << "Ending child process" <<endl;
    return 0;


}