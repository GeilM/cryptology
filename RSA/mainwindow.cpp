#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rsa.h"
#include <QString>
#include <iostream>
#include <QTextEdit>
#include <QTextCodec>
#include <QMessageBox>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
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
    }
    else
    {

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
       // cout << s << endl;

        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
        qstr=QString::fromLocal8Bit(text);
        ui->deco_plain->setPlainText(qstr);
    }
}
