#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "rsa.h"
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
};

#endif // MAINWINDOW_H
