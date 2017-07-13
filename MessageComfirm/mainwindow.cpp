#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rsa.h"
#include <QString>
#include <iostream>
#include <QTextEdit>
#include <QTextCodec>
#include <QMessageBox>
#include <cmath>
#include <iostream>
#include<iterator>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    SHA_1 sha1;
    rsa.init();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_encode_clicked()
{
    QString qstr;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    qstr=ui->plain->toPlainText();

    if(qstr.length()==0)
    {
       QMessageBox::warning(this,tr("warning"),tr("原文不能为空！"),QMessageBox::Yes);
    }
    else
       {

        QByteArray ba=qstr.toLocal8Bit();
        char* buff=ba.data();
        string text="";
        for(int i=0;i<strlen(buff);i++)
            text=text+buff[i];
        //生成消息摘要
       //只取前面2^64/2^3个字符
       long long unsigned maxLen=pow(2.0,61);
       long long unsigned textLen=text.length();
       if(textLen>maxLen)
       {
            QMessageBox::warning(this,tr("warning"),tr("消息太长！"),QMessageBox::Yes);
       }
      /* for(int i=0;i<pow(2,61)/8;i++)
       {
           s+=buff[i];
       }*/
       //消息加密
      string absm=sha1.sha1(text);
      ui->absM->setText(QString(absm.c_str()));

        char* bits = new char[text.length() * 8];
        rsa.CharToBit(text, bits);
        vector<long long int> dec_code = rsa.BitsToDec(bits,text.length()*8,rsa.len,rsa.Rlen);
        char* cipherBin = new char[dec_code.size()*rsa.Rlen];

        rsa.encode(dec_code, rsa.e, rsa.n,rsa.len,rsa.Rlen, cipherBin);
        char* temp=new char[dec_code.size()*rsa.Rlen+1];
        temp[dec_code.size()*rsa.Rlen]='\0';
        for(int i=0;i<dec_code.size()*rsa.Rlen;i++)
        {
           *(temp+i)='0'+*(cipherBin+i);
         }
        qstr=QString(QLatin1String(temp));
       ui->cipher->setPlainText(qstr);

     }

}

void MainWindow::on_decode_clicked()
{
    QString qstr;
    string str;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QByteArray ba;

    qstr=ui->cipher->toPlainText();
    if(qstr.length()==0)
    {
        QMessageBox::warning(this,tr("warning"),tr("密文无效！"),QMessageBox::Yes);
        return;
    }
    else
    {

        qstr=ui->cipher->toPlainText();
        ba=qstr.toLocal8Bit();
        char* cipherBin=ba.data();
        int len=strlen(cipherBin);

        for(int i=0;i<len;i++)
            cipherBin[i]=cipherBin[i]-'0';

       // cout << "解密：\n";
        int textLen=len/rsa.Rlen*rsa.len/8;
        char* textbits = new char[len/rsa.Rlen*rsa.len];
        rsa.decode(cipherBin, len, rsa.d, rsa.n, rsa.len, rsa.Rlen, textbits);
        char* text=new char[textLen+1];
        text[textLen]='\0';
        string s="";
        s=rsa.toPlain(textbits, len/rsa.Rlen*rsa.len,text);
        cout << s << endl;

        string::reverse_iterator it=s.rbegin();
         long long unsigned sLen=s.length();
      /*  for(it;it!=s.rend();it++)
        {
            if(*it==0)
            {
                s.erase(sLen-1,sLen-1);
                sLen=sLen-1;
            }
            else
                break;
        }*/


        //string absm="";
       long long unsigned maxLen=pow(2.0,61);
      if(sLen>maxLen)
      {
           QMessageBox::warning(this,tr("warning"),tr("密文太长！"),QMessageBox::Yes);
           return;
      }
      string check=sha1.sha1(s);

      QString fm=ui->absM->toPlainText();
      if(QString(check.c_str())==fm)
      {
          QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
          qstr=QString::fromLocal8Bit(text);
          ui->deco_plain->setPlainText(qstr);
      }
      else
      {
           QMessageBox::warning(this,tr("warning"),tr("消息摘要不正确！"),QMessageBox::Yes);
      }

    }
}
