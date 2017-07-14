#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/emulator.h"
#include <QImage>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

enum reg_index {A, B, C, D, E, H, L, F};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    emulator* emu;
    QImage* lcdImage;
public slots:
    void consolePrint(QString);
    void updateLCD();
    void stepCPU();
};

#endif // MAINWINDOW_H
