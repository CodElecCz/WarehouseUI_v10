#ifndef LOADVIEW_H
#define LOADVIEW_H

#include <QWidget>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

    void setText(QString msg);
    void setTitle(QString msg);
    void setSubTitle(QString msg);

    QString text();
    QString title();
    QString subTitle();

private:
    Ui::SplashScreen         *ui;
};

#endif // LOADVIEW_H
