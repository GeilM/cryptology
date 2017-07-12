#ifndef _SHA1_H
#define _SHA1_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;
class SHA_1{
public:
    string words;
    unsigned circleShift(const unsigned& word, const int& bits);
    unsigned sha1Fun(const unsigned& B, const unsigned& C, const unsigned& D, const unsigned& t);
    string sha1(const string& strRaw);
};

#endif // _SHA1_H
