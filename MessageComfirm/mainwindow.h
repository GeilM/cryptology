#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "rsa.h"
#include "_sha1.h"
#include <QMainWindow>

using namespace std;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_encode_clicked();

    void on_decode_clicked();

private:
    Ui::MainWindow *ui;
    RSA rsa;
    SHA_1 sha1;
};

#endif // MAINWINDOW_H
