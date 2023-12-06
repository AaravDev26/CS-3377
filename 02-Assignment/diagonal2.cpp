/*
Name of Module: Diagonal - Binary Files
Date: March 15, 2023
Author: Aarav Dev
Program Purpose: Enhance the old file to produce a pattern with any number of words as arguments
*/

// imports
#include <iostream> // common import, used for input and output
#include <iomanip> // used to format output
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "LineInfo.h" // imported for exception handling

using namespace std; // used to avoid putting "std::" in front of every statement which requries the std namespace

const int    LINESIZE = 16; // constant int initialized

//use one command line argument
int main(int argc, char* argv[]) {
    try {
        if (argc != 1) { // checking for error
            throw domain_error(LineInfo("Usage: diagonal < textstring>", __FILE__, __LINE__));
        }

        //create a file so that 16 rows of empty will appear with od -Ax -c command
        int fdbinaryout = open("diagonal2.bin", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
        
        if (fdbinaryout == 0) // checking for error
            throw domain_error(LineInfo("Open Failed File: diagonal.bin", __FILE__, __LINE__));

        // initializing variables
        char space = '.';
        
        int length = 0;
        int blockcount = 0;
        
        for (int argNo = 1; argNo < argc; argNo++) {
            cout << endl << endl << endl;
            // printing variable names and values
            cout << "argNo " << argNo << endl;
            cout << "argv[" << argNo << "]" << argv[argNo] << endl;
            length = strlen(argv[argNo]);
            cout << "length " << length << endl;
            cout << endl;
            
            int position;
            cout << "blockCount " << blockcount << endl;
            
            if (lseek(fdbinaryout, blockcount, SEEK_SET) == -1) // checking for error
                throw domain_error(LineInfo("lseek() failed", __FILE__, __LINE__));
            
            cout << "line size " << LINESIZE << endl << endl;
            
            for (int line = 0; line < LINESIZE; line++) {
                cout << setw(2) << line << " : ";
                
                for (int column = 0; column < LINESIZE; column++) {
                    
                    if ((position = lseek(fdbinaryout, 0, SEEK_CUR)) == -1) // checking for error
                        throw domain_error(LineInfo("lseek() failed", __FILE__, __LINE__));
                    cout << setw(04) << position << space << ' ';
                    
                    if ((write(fdbinaryout, &space, 1)) == -1) // checking for error
                        throw domain_error(LineInfo("write() failed", __FILE__, __LINE__));
                        
                }
                
                cout << endl;
            }
            cout << endl << endl;
            
            //Each line of od outputs 16 characters 
            //So, to make the output diagonal, we will use 0, 17, 34, ....

            int argvlength = strlen(argv[argNo]);
            cout << "argNo       " << argNo << endl;
            cout << "argv[argNo] " << argv[argNo] << endl;
            cout << "argvlength  " << argvlength << endl;
            cout << "blockCount  " << blockcount << endl;
            
            // odd left to right
            if (argNo % 2 != 0) {
                
                for (int i = 0; i < argvlength; i++) {
                    
                    cout << "lseek " << setw(06) << (blockcount + (LINESIZE + 1) * i) << " argv[" << argNo << "][" << i << "]" << argv[argNo][i] << endl;
                    
                    if (lseek(fdbinaryout, (blockcount + (LINESIZE + 1) * i), SEEK_SET) == -1) // checking for error
                        throw domain_error(LineInfo("lseek() failed", __FILE__, __LINE__));
                    
                    if (write(fdbinaryout, &argv[argNo][i], 1) == -1) // checking for error
                        throw domain_error(LineInfo("write() failed", __FILE__, __LINE__));
                }
                
            }
            // even right to left
            else {
                const int adjust = 15;
                cout << "adjust " << adjust << endl;
                
                for (int i = 1; i < argvlength; i++) {
                    cout << "lseek " << setw(06) << (blockcount + (adjust * i)) << " argv[" << argNo << "][" << (i - 1) << "] " << argv[argNo][i - 1] << endl;
                    
                     // checking for error
                    if (lseek(fdbinaryout, (blockcount + (adjust * i)), SEEK_SET) == -1) // checking for error
                        throw domain_error(LineInfo("lseek() failed", __FILE__, __LINE__));
                    
                    if ((write(fdbinaryout, &argv[argNo][i - 1], 1)) == -1) // checking for error
                        throw domain_error(LineInfo("write() failed", __FILE__, __LINE__));
                }
            }
            // incrementing blockcount by 256 and printing it out
            blockcount += 256;
            cout << "blockCount " << blockcount << endl;
        }

        close(fdbinaryout); // closing the file

        cout << "diagonal2.bin" << " has been created." << endl
            << "Use od -c diagonal.bin to see the contents." << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
        cout << endl << "Press the enter key once or twice to leave..." << endl;
        cin.ignore(); cin.get();
        exit(EXIT_FAILURE);
    }//catch

    exit(EXIT_SUCCESS); // exit
}

