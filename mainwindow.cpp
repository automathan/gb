#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    emu = new emulator();
    connect(emu, SIGNAL(debug(QString)), SLOT(consolePrint(QString)));
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
    connect(ui->stepButton,SIGNAL(clicked(bool)),SLOT(stepCPU()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::consolePrint(QString str){
    ui->consoleEdit->append(str);
}

void MainWindow::stepCPU(){
    emu->step();
    auto regs = emu->getRegs();

    ui->regview_f->setText("0x" + QString("%1").arg(regs[F], 2, 16, QChar('0')));
    ui->regview_c->setText("0x" + QString("%1").arg(regs[C], 2, 16, QChar('0')));
    ui->regview_e->setText("0x" + QString("%1").arg(regs[E], 2, 16, QChar('0')));
    ui->regview_l->setText("0x" + QString("%1").arg(regs[L], 2, 16, QChar('0')));

    ui->regview_a->setText("0x" + QString("%1").arg(regs[A], 2, 16, QChar('0')));
    ui->regview_b->setText("0x" + QString("%1").arg(regs[B], 2, 16, QChar('0')));
    ui->regview_d->setText("0x" + QString("%1").arg(regs[D], 2, 16, QChar('0')));
    ui->regview_h->setText("0x" + QString("%1").arg(regs[H], 2, 16, QChar('0')));
}

void MainWindow::updateLCD(){
    uint colors[4] = {0xff000000,0xff343434,0xff777777,0xffaaaaaa};

    //ui->consoleEdit->append("updating LCD...");
    auto frame = emu->getFrame(ui->spinBox->value());

    QRgb* imgdata = (QRgb*)lcdImage->bits();
    for(int i = 0; i < 256 * 256; ++i)
        imgdata[i] = colors[frame[i]];
    ui->lcdLabel->setPixmap(QPixmap::fromImage(*lcdImage));
}
