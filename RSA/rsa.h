#ifndef _RSA
#define _RSA
#include<cmath>
#include<cstring>
#include<string>
#include<random>
#include<ctime>
using namespace std;

class RSA{
public:
    long long int p, q;
    long long int n ;
    long long int yn;
    long long int e;
    long long int d;
    int Rlen;
    int len;

    RSA();
    void init();
    long long int expmod(long long int m, long long int E,long long int N);
    void findPQ();
    int ByteToBit(char ch, char bit[8]);
    void  CharToBit(string s, char* bits);
    void toBin(long long int dec,int len,char* bin);
    long long int toDec(char* bit, int bitsLen,int len);
    vector<long long int>  BitsToDec(char* bits, int bitsLen, int len, int Rlen);
    long long int calD(long long int E, long long int VN);
    char* encode(vector<long long int> dec_code, long long int E, long long int N, int len,int Rlen, char* cipherBin);
    char* decode(char* cipherBin, int cipherLen, long long int d, long long n, int len, int Rlen, char* bits);
    char BitToChar(char ch, char bit[8]);
    string toPlain(char* bin,int bitslen,char* text);
};


#endif // RSA

