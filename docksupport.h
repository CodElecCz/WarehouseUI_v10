#ifndef DOCKSUPPORT_H
#define DOCKSUPPORT_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QDockWidget>

class DockSupport : public QMainWindow
{
    Q_OBJECT

public:
    explicit DockSupport(QWidget *parent = nullptr);
    ~DockSupport();

    void tabifyDockWidget(QDockWidget *dockwidget, const QString& title, const Qt::DockWidgetArea& area);
    void retranslate();

private:
    void tabifyTopDockWidget(QDockWidget *dockwidget, const QString& title);
    void tabifyBottomDockWidget(QDockWidget *dockwidget, const QString& title);
    void tabifyLeftDockWidget(QDockWidget *dockwidget, const QString& title);
    void tabifyRightDockWidget(QDockWidget *dockwidget, const QString& title);

private:
    QList<QDockWidget*> m_topDockTabbing;
    QList<QDockWidget*> m_bottomDockTabbing;
    QList<QDockWidget*> m_leftDockTabbing;
    QList<QDockWidget*> m_rightDockTabbing;

    QStringList         m_topDockTitles;
    QStringList         m_bottomDockTitles;
    QStringList         m_leftDockTitles;
    QStringList         m_rightDockTitles;


};

#endif // DOCKSUPPORT_H
