#include "rsa.h"
#include <iostream>
using namespace std;
RSA::RSA()
{
    p=0;
    q=0;
    n=0;
    yn=0;
    e=0;
    d=0;
    Rlen=0;
    len=0;
}
long long int RSA::expmod(long long int m, long long int E,long long int N)
{
    if (E == 1)return m;
    long long int cipher;
    long long int mid = E / 2;
    long long int tmp = expmod(m, mid, N);
    if (E % 2 == 0)
    {
        cipher = (tmp*tmp) % N;
    }
    else
    {
        long long int tmp2 = m * tmp % N;
        cipher = tmp * tmp2 % N;
    }
    return cipher;
}
void RSA::init()
{
    findPQ();
    n = p*q;
    yn = (p - 1)*(q - 1);
    e = 65537;
    d = calD(e, yn);
    Rlen = log(n*1.0) / log(2.0) + 1;
    len = (Rlen / 8 - 1) * 8;
}

void RSA::findPQ()
{
    int a[10000];
    int len = 1;
    a[0] = 2;
    for (int i = 3; i < 65536; i++)
    {
        bool isP = true;
        int sqr = sqrt((double)i);
        for (int j = 0; a[j] <= sqr; j++)
        {
            if (i % a[j] == 0)
            {
                isP = false;
                break;
            }

        }
        if (isP) a[len++] = i;
    }
    srand(time(0));
    int a1 = rand() % len;
    int a2 = rand() % len;
    p = a[a1]; q = a[a2];
}

int RSA::ByteToBit(char ch, char bit[8]){
    int cnt;
    for (cnt = 0; cnt < 8; cnt++)
    {
        *(bit + cnt) = (ch >> (7 - cnt)) & 1;
    }
    return 0;
}

//转为2进制,string变成数组
void  RSA::CharToBit(string s, char* bits){
    char* chs = new char[s.length()];
    strcpy(chs, s.c_str());
    //数组的截止符
    for (int i = 0; i < s.length(); i++)
    {
        ByteToBit(*(chs + i), bits + i * 8);
    }
}

//十进制转换为2进制,定长,最后补齐
void RSA::toBin(long long int dec,int len,char* bin)
{
    long long int tmp = dec;
    int cnt;
    for (cnt = 0; cnt < len; cnt++)
    {
        bin[cnt] = (tmp>>(len-cnt-1))&1;
    }

}

//转为10进制，定长转换
long long int RSA::toDec(char* bit, int bitsLen,int len)
{
    long long int dec = 0;
    int cnt = 0;
    for (int i = 0; i < len; i++)
    {
        if (i < bitsLen)
        {
            dec = (dec << 1) + bit[i];
        }
        else
            dec = (dec << 1);
    }
    return dec;
}

    //整个原文进行转换
vector<long long int>  RSA::BitsToDec(char* bits, int bitsLen, int len, int Rlen)
    {
        vector<long long int> text;
        //len应该为n最大值
        bool isEnd = 1;
        int times = bitsLen / len;
        if (times*len < bitsLen)
        {
            isEnd = 0;
        }
        for (int i = 0; i < times; i++)
        {
            text.push_back(toDec(bits + i*len, len,len));
        }
        if (!isEnd)
        {
            text.push_back(toDec(bits + times*len, bitsLen - times*len, len));
        }
        return text;
}

//算出N，e,d;e固定，N可以直接计算，无需写函数
long long int RSA::calD(long long int e, long long int yn)
{
    long long int a[59640];
    long long int b[59640];
    int len = 0;
    long long int m = yn, n = e, r = 1;
    while (r != 0)
    {
        a[len] = m / n;
        len++;
        r = m % n;
        m = n;
        n = r;
    }
    b[0] = 1;
    b[1] = a[len - 2];
    for (int i = 2; i < len; i++)
    {
        b[i] = a[len - i - 1] * b[i - 1] + b[i - 2];
    }
    if (len % 2 == 1) return b[len - 1];
    else return yn - b[len - 1];
}

char* RSA::encode(vector<long long int> dec_code, long long int E, long long int N, int len,int Rlen, char* cipherBin)
{
    long long int sub_cipher_dec = 0;
    for (int i = 0; i < dec_code.size(); i++)
    {
        sub_cipher_dec = 0;
        sub_cipher_dec = expmod(dec_code[i], E, N);
        toBin(sub_cipher_dec, Rlen, cipherBin + i*Rlen);
    }
    return cipherBin;
}

char* RSA::decode(char* cipherBin, int cipherLen, long long int d, long long n, int len, int Rlen, char* bits)
{
    long long int dec;
    long long int sub_text_dec = 0;
    int times = cipherLen / Rlen;
    for (int i = 0; i < times; i++)
    {
        sub_text_dec = toDec(cipherBin + i*Rlen, Rlen,Rlen);
        dec = expmod(sub_text_dec, d, n);
        toBin(dec, len, bits + i*len);

    }
    return bits;
}

char RSA::BitToChar(char ch, char bit[8])
{
    //char ch = 0;
    int dec = 0;
    for (int i = 0; i < 8; i++)
    {
        dec = dec * 2 + bit[i];
    }
    ch = char(dec);
    return ch;
}

//变成原文，每8位作为一个char
string RSA::toPlain(char* bin,int bitslen,char* text)
{
    string s = "";
    char ch=0;
    int times=bitslen/8;
    for (int i = 0; i < times;i++)
    {
        ch = BitToChar(ch, bin + i*8);
        text[i]=ch;
        s = s + ch;
    }
    return s;
}
