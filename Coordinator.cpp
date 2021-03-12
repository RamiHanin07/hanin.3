#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
    string arg = "";
    string argNext = "";
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
}