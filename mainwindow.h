#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/emulator.h"
#include <QImage>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

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
    void consolePrint(const char* str);
    void updateLCD();
};

#endif // MAINWINDOW_H
