#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    emu = new emulator();
    emu->play("roms/tetris.gb");
}

MainWindow::~MainWindow()
{
    delete ui;
}
