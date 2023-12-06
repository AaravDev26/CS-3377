/*
Name of Module: Find the Bison
Date: March 1, 2023
Author: Aarav Dev
Program Purpose: Find the number of patterns made with parentheses in a given text file and keep track of the time taken to complete the task
*/

// imports
#include <iostream> // common import, used for input and output
#include <fstream> // common import, used for input and output files
#include <string> // used for filenames
#include <iomanip> // used to format output
#include <chrono> // used to keep track of time
#include <limits>
#include <locale>
#include "LineInfo.h" // imported for exception handling

using namespace std; // used to avoid putting "std::" in front of every statement which requries the std namespace
using namespace chrono; // used to avoid putting "chrono::" in front of every statement which requries the chrono namespace

const string INPUT_FILE_NAME = "bisonsearchin.txt"; // used to hold input filename
const string OUTPUT_FILE_NAME = "bisonfoundin.txt"; // used to hold output filename
const uint8_t MAX_FILE_NO = 10; // holds max number of files

int main() // main method
{
    // condition cout set to local digit separation commas (USA)
    cout.imbue(locale(""));
    
    // instantiating variables
    string searchBisonInGrassStr;
    string inputFileNameStr, outputFileNameStr;
    uint16_t fileCount = 0;
    
    try {
        do {
            fileCount++;
            
            inputFileNameStr = outputFileNameStr = to_string(fileCount);
            
            // prepends 0 to filenames if size is 1
            if (inputFileNameStr.size() == 1) {
                inputFileNameStr = "0" + inputFileNameStr;
                outputFileNameStr = "0" + outputFileNameStr;
            }
            
            // adds filenames to end of string
            inputFileNameStr = inputFileNameStr + INPUT_FILE_NAME;
            outputFileNameStr = outputFileNameStr + OUTPUT_FILE_NAME;
            
            // new ifstream and throws error if failure occurs
            ifstream inputParensStreamObj(inputFileNameStr);
            if (inputParensStreamObj.fail()) {
                throw domain_error(LineInfo("open FAILURE File " + inputFileNameStr, __FILE__, __LINE__));
            }
            
            // new ofstream
            ofstream outputParensStreamObj(outputFileNameStr);
            
            inputParensStreamObj >> searchBisonInGrassStr;
            
            // instantiating new variables to hold parentheses counts
            unsigned answerFoundBisonPatternCount = 0,
                backParenCount = 0;
            
            auto timeStart = steady_clock::now(); // starts clock
            
            size_t size = searchBisonInGrassStr.size(); // holds size of string
            // loop to find patterns
            for (unsigned i = 1; i < size; i++) {
                // finding parentheses patterns
                if (searchBisonInGrassStr[i - 1] == ')' && searchBisonInGrassStr[i] == ')') {
                    answerFoundBisonPatternCount += backParenCount;
                }
                
                else if (searchBisonInGrassStr[i - 1] == '(' && searchBisonInGrassStr[i] == '(') {
                    backParenCount++;
                }
            }
            
            auto timeElapsed = duration_cast<nanoseconds> (steady_clock::now() - timeStart);
            
            // writing to file with output stream
            outputParensStreamObj << "Time Elapsed (nano) : " << setw(15) << timeElapsed.count() << endl << endl;
            outputParensStreamObj << "Found Pattern Count : " << answerFoundBisonPatternCount << endl << endl;
            outputParensStreamObj << "Searched Pattern    : " << endl << endl;
            outputParensStreamObj << searchBisonInGrassStr << endl;
            
            outputParensStreamObj.close(); // closes stream
            
        } while (fileCount != MAX_FILE_NO); // repeat until done 10 times
    } //try
    // catches exceptions
    catch (exception& e) {
        cout << e.what() << endl;
        cout << endl << "Press the enter key once or twice to leave..." << endl;
        cin.ignore(); cin.get();
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
} // main{}
