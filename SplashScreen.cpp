#include "SplashScreen.h"
#include "ui_SplashScreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::setText(QString msg)
{
    ui->label_3->setText(msg);
    QApplication::processEvents();
}

void SplashScreen::setTitle(QString msg)
{
    ui->label_1->setText(msg);
    QApplication::processEvents();
}

void SplashScreen::setSubTitle(QString msg)
{
    ui->label_2->setText(msg);
    QApplication::processEvents();
}

