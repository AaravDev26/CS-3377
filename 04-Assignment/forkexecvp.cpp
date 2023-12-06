/*
Program Name: Fork Execvp
Date: 2023-04-03
Author: Aarav Dev
Module Purpose
Design forks to create several child processes
*/

// imports
#include <string>
#include <fstream>
#include <iostream> // import used for input and output
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include <stdexcept>
#include <sstream>

#include "LineInfo.h" // exception handling

using namespace std; // used to avoid putting "std::" in front of every statement which requries the std namespace

// constant variables
int const READ   = 0;
int const WRITE  = 1;
int const PIPE_ERROR = -1;
int const CHILD_PID  =  0;
int const MAX_PIPE_MESSAGE_SIZE = 1000;
int const EXECVP_FAILED = -1;

void CreateArg1FileWithArg2RandomNumbersArg3RandomRange(string randomFileNameStr, unsigned noRandomNumbersUns, unsigned randomRange) {
    
    ofstream outfilestream (randomFileNameStr);
    
    if (outfilestream.fail()) {
        stringstream s;
        s << "Error opening file Random Numbers File " << randomFileNameStr << endl;
        throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
    }
    
    //seed the random number generator
    time_t t; srand((unsigned)time(&t));
    
    for (unsigned i = 0; i < noRandomNumbersUns; ++i)
        outfilestream << ( ( rand() % randomRange) + 1 ) << endl;
        
    outfilestream.close();
}
    
int main(int argc, char* argv[]) { // main method
    
    try {
        
        if (argc != 4) { // if number of arguments is not equal to 4
            stringstream s;
            s << endl << endl
              << "Wrong arguments" << endl
              << "Usage: " << endl
              << "forkexecvp <file to create> <amount of random numbers> <random number range" << endl
              << "Example: " << endl
              << "forkexecvp randomnumber 10000 100 " << endl << endl;
            throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
        }
        
        string   randomFileNameStr  = argv[1];
        unsigned noRandomNumbersUns = atoi(argv[2]),
                 randomRange        = atoi(argv[3]);
        
        cout << endl << argv[0] << " " << randomFileNameStr << " " << noRandomNumbersUns << " " << randomRange << endl;
        
        CreateArg1FileWithArg2RandomNumbersArg3RandomRange
        (randomFileNameStr, noRandomNumbersUns, randomRange);
        
        pid_t pid;
        int pipeParentWriteChildReadfds[2];
        
        string messages[] = {"sum", "average", "greatest", "least"}; // initlialize array
        int noOfMessages = sizeof(messages) / sizeof(messages[0]);
        
        for (int childProcessNo = 0; childProcessNo < noOfMessages; ++childProcessNo) {
            
            if(pipe(pipeParentWriteChildReadfds) == PIPE_ERROR) {
                stringstream s;
                s << "Unable to create pipe pipeParentWriteChildReadfds";
                throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
            }
            
            pid_t forkpid = fork();
            
            if (forkpid < 0) {
                stringstream s;
                s << "fork failed\n";
                throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
            }
            
            if (forkpid != CHILD_PID)
            {
                //parent
                close(pipeParentWriteChildReadfds[READ]);
                
                cout << "Parent pid : " << getpid() << " to Child Process No : "
                     << childProcessNo << endl <<
                     "Send Message : " << messages[childProcessNo] << endl;
                
                if (write(pipeParentWriteChildReadfds[WRITE],
                          messages[childProcessNo].c_str(),
                          sizeof(messages[childProcessNo].c_str())) == PIPE_ERROR) {
                    stringstream s;
                    s << "pipe write failed";
                    throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
                }
            }
            else
            {
                //child code
                close(pipeParentWriteChildReadfds[WRITE]);
                
                char pipeReadMessage[MAX_PIPE_MESSAGE_SIZE] = {0};
                if (read(pipeParentWriteChildReadfds[READ],
                         pipeReadMessage,
                         sizeof(pipeReadMessage)) == PIPE_ERROR) {
                    stringstream s;
                    s << "pipe read failed";
                    throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
                }
                
                cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo << 
                    " Receieved Message : " << pipeReadMessage << endl;
                    
                // execvp
                char * arglist[] = { (char *)"./calculate", pipeReadMessage,
                                     (char*)randomFileNameStr.c_str(), NULL };
                                     
                cout << "Child pid : " << getpid() << " Child Process No : " << childProcessNo << endl << 
                    "execvp(" << arglist[0] << ", ./calculate, " << randomFileNameStr.c_str() << ", NULL)" << endl;
                
                //replace stdout to the answer file named with pipeReadMessage
                string answerFileName = "answer"; answerFileName += pipeReadMessage; answerFileName += ".txt";
                freopen(answerFileName.c_str(), "w", stdout);
                
                close(pipeParentWriteChildReadfds[READ]);
                
                if (execvp(arglist[0], arglist) == EXECVP_FAILED) {
                    stringstream s;
                    s << "execvp failed";
                    throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
                }
            }
        }// for
        
        cout << "Parent pid: " << getpid() << " Start - Wait for all calculate children to finish" << endl;
        while (wait(NULL) > 0);
        cout << "Parent pid: " << getpid() << " Done  - Wait for all calculate children to finish" << endl;
        
        close(pipeParentWriteChildReadfds[WRITE]);
        
        cout << "Parent pid : " << getpid() << " Use execvp() cat to display answer files: " << endl;
        
        for (int childProcessNo = 0; childProcessNo < noOfMessages; ++childProcessNo) {
            
            pid_t forkpid = fork();
            
            if (forkpid < 0)
            {
                stringstream s;
                s << "fork failed";
                throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
            }
            
            if (forkpid == CHILD_PID) {
                //child code
                
                string answerFileName = "answer"; answerFileName += messages[childProcessNo]; answerFileName += ".txt";
                
                char* arglist[] = { (char*)"cat", (char*)answerFileName.c_str(), NULL };
                
                cout << "Child pid : " << getpid() << endl <<
                     "cat " << answerFileName << endl;
                     
                if (execvp("cat", arglist) == EXECVP_FAILED) {
                    stringstream s;
                    s << "execvp failed";
                    throw domain_error(LineInfo(s.str(), __FILE__, __LINE__));
                }
            }
        }// for
        
        cout << "Parent pid: " << getpid() << " Start - Wait for all display answer file children to finish" << endl;
        while (wait(NULL) > 0);
        cout << "Parent pid: " << getpid() << " Done  - Wait for all display answer file children to finish" << endl;
    }// try
    catch (exception& e) {
        cout << e.what() << endl;
        cout << endl << "Press the enter key once or twice to leave..." << endl;
        cin.ignore(); cin.get();
        exit(EXIT_FAILURE);
    }//catch
    
    exit(EXIT_SUCCESS);
}


 