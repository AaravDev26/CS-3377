/*
Program Name: Number Guess
Date: 2023-04-19
Author: Aarav Dev
Module Purpose
Design a game where the user guesses a Number
*/

#include <string>
#include <iostream>
#include <sstream>

#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>

#include "LineInfo.h"

using namespace std;

const int    MESSAGE_MAX_SIZE = 1024;
const int    ERROR     = -1;
const int    SOCK_PORT = 8888;
const string LOCAL_HOST_ADDRESS = "127.0.0.1"; // loopback address

const int MAX_GUESS_LIMIT = 100;
const int MAX_NO_TRIES    =  12;

int main(int argc, char* argv[]) {

    int          socketServerId;
    sockaddr_in  server;
    string       messageStr;

  try {

    //Create socketServerId
    if ((socketServerId = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
        throw domain_error(LineInfo("Socket socket create failure ", __FILE__, __LINE__));
    //$$

    server.sin_addr.s_addr = inet_addr(LOCAL_HOST_ADDRESS.c_str());
    server.sin_family      = AF_INET;
    server.sin_port        = htons(SOCK_PORT);

    //Connect to remote server socketServerId
    if ( connect(socketServerId, (struct sockaddr*) &server, sizeof(server)) == ERROR )
        throw domain_error(LineInfo("Socket connect failure ", __FILE__, __LINE__));
    //$$
    
    cout << endl << endl << "Try to guess a number between 1 and " << MAX_GUESS_LIMIT
         << endl << endl;

    // Prompt for a guess number in staed range

    int nooftries = 0;
    int guessno;

    // start game
    do {
        // prompt for guessno
        //$$
        cout << "Current Try Number : " << ++nooftries << endl;
        cout << "Enter number guess : " << endl << endl;
        cin >> guessno;
        
        stringstream guessnoss; 
        guessnoss << guessno;
        string guessNoStr = guessnoss.str();

        // write to socketServerId guessNoStr
        if ( (write(socketServerId, guessNoStr.c_str(), guessNoStr.size() + 1) ) == ERROR)
            throw domain_error(LineInfo("Socket write failure ", __FILE__, __LINE__));
        //$$
        
        // read from socketServerId the receiveMessage
        char receiveMessage[MESSAGE_MAX_SIZE]={0};
        if ( ( read(socketServerId, receiveMessage, MESSAGE_MAX_SIZE) ) == ERROR)
            throw domain_error(LineInfo("Socket read failure ", __FILE__, __LINE__));
        //$$

        string receiveMessageStr = receiveMessage;
        cout << endl << "Server reply :" << endl << endl << receiveMessageStr << endl << endl;

        // Check if a Win Message
        if (receiveMessageStr.find("Win") != string::npos)
            break;

        // Processs if equal to max numbere of tries
        if (nooftries == MAX_NO_TRIES){
            // display done for max no of tries
            cout << endl << "The maximum number of tries " << MAX_NO_TRIES << " is done." << endl;
            if ( (write(socketServerId, "Send Random Number", sizeof("Send Random Number")+1) ) == ERROR)
                throw domain_error(LineInfo("Socket write failure ", __FILE__, __LINE__));
            // write socketServerId Sned Random Number
            
            //$$
            
            // read from socketServerId randomNoMessage
            char randomNoMessage[MESSAGE_MAX_SIZE] = {0};
            if ( (read(socketServerId, randomNoMessage, MESSAGE_MAX_SIZE) ) == ERROR)
                throw domain_error(LineInfo("Socket read failure ", __FILE__, __LINE__));
            //$$

            int randomno = stoi(randomNoMessage);
            
            
            //display the random no and state the game is over 
            cout << "The random number is : " << randomno << endl << endl;
            cout << "Game is over " << endl << endl;
            //$$
            
            //leave the loop
            break;
            //$$
        }

    } while (true);
    

    if (close(socketServerId) == ERROR)
        throw domain_error(LineInfo("Socket close failure ", __FILE__, __LINE__));
    
    close(socketServerId);
    
  }
  //try
  catch (exception& e) {
      cout << e.what() << endl;
      exit(EXIT_FAILURE);
  }//catch

  exit(EXIT_SUCCESS);
        
}