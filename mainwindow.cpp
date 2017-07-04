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

    lcdImage = new QImage(256, 256, QImage::Format_RGB32);

    /*
    QRgb* imgdata = (QRgb*)lcdImage->bits();
    for(int i = 0; i < 256 * 256; ++i)
        imgdata[i] = 0xff000000 | ((i / 256) << 8) | (i & 0xff);
    ui->lcdLabel->setPixmap(QPixmap::fromImage(*lcdImage));
    ui->lcdLabel->setFixedWidth(256);
    ui->lcdLabel->setFixedHeight(256);
    */

    connect(ui->refreshButton,SIGNAL(clicked(bool)),SLOT(updateLCD()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::consolePrint(const char* str){
    ui->consoleEdit->append(str);
}

void MainWindow::updateLCD(){
    uint colors[3] = {0xff343434,0xff777777,0xffaaaaaa};

    ui->consoleEdit->append("updating LCD...");
    auto frame = emu->getFrame();

    QRgb* imgdata = (QRgb*)lcdImage->bits();
    for(int i = 0; i < 256 * 256; ++i)
        imgdata[i] = colors[frame[i]];
    ui->lcdLabel->setPixmap(QPixmap::fromImage(*lcdImage));
}
