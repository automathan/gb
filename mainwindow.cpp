#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    emu = new emulator();
    connect(emu, SIGNAL(debug(const char*)), SLOT(consolePrint(const char*)));
    emu->play("/home/jonathaj/Downloads/Tetris (World)/Tetris (World).gb");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::consolePrint(const char* str){
    ui->consoleEdit->append(str);
}
