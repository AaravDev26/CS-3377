#ifndef LINE_INFO_H
#define LINE_INFO_H

// imports
#include <string>
#include <iostream> // common import, used for input and output
#include <string.h>
#include <cstring>
#include <stdexcept>
#include <sstream>

using namespace std; // used to avoid putting "std::" in front of every statement which requries the std namespace

string LineInfo(string const& errorstr, char const* file, long line) 
{
    // prints output for exceptions
    stringstream s;
    s << endl << "EXCEPTION: " << endl << "Oh my Goodness... " << endl << errorstr
        << " in " << file << "\",line:" << line << endl;
    return s.str();
}

#endif
