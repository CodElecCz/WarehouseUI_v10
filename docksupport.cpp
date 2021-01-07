#include "DockSupport.h"

DockSupport::DockSupport(QWidget *parent) :
    QMainWindow(parent)
{

}

DockSupport::~DockSupport()
{

}

void DockSupport::retranslate()
{
    //dock widgets titles

    int index = 0;
    foreach(QDockWidget* w, m_topDockTabbing)
    {
        if(m_topDockTitles.size()>index)
            w->setWindowTitle(tr(m_topDockTitles.at(index++).toLatin1()));
    }

    index = 0;
    foreach(QDockWidget* w, m_bottomDockTabbing)
    {
        if(m_bottomDockTitles.size()>index)
            w->setWindowTitle(tr(m_bottomDockTitles.at(index++).toLatin1()));
    }

    index = 0;
    foreach(QDockWidget* w, m_leftDockTabbing)
    {
        if(m_leftDockTitles.size()>index)
            w->setWindowTitle(tr(m_leftDockTitles.at(index++).toLatin1()));
    }

    index = 0;
    foreach(QDockWidget* w, m_rightDockTabbing)
    {
        if(m_rightDockTitles.size()>index)
            w->setWindowTitle(tr(m_rightDockTitles.at(index++).toLatin1()));
    }

}

void DockSupport::tabifyDockWidget(QDockWidget *dockwidget, const QString& title, const Qt::DockWidgetArea& area)
{
    switch(area)
    {
    case Qt::BottomDockWidgetArea:
        tabifyBottomDockWidget(dockwidget, title);
        break;
    case Qt::TopDockWidgetArea:
        tabifyTopDockWidget(dockwidget, title);
        break;
    case Qt::LeftDockWidgetArea:
        tabifyLeftDockWidget(dockwidget, title);
        break;
    case Qt::RightDockWidgetArea:
        tabifyRightDockWidget(dockwidget, title);
        break;
    default:
        break;
    }
}

void DockSupport::tabifyTopDockWidget(QDockWidget *dockwidget, const QString &title)
{
    m_topDockTabbing.insert(0, dockwidget);
    m_topDockTitles.insert(0, title);

    addDockWidget(Qt::TopDockWidgetArea, dockwidget);

    int size = m_topDockTabbing.size();
    if(size>1)
    {
        QDockWidget* first = m_topDockTabbing.at(size-1);
        QDockWidget* second = m_topDockTabbing.at(0);

        QMainWindow::tabifyDockWidget(first, second);
    }
}

void DockSupport::tabifyBottomDockWidget(QDockWidget *dockwidget, const QString &title)
{
    m_bottomDockTabbing.insert(0, dockwidget);
    m_bottomDockTitles.insert(0, title);

    addDockWidget(Qt::BottomDockWidgetArea, dockwidget);

    int size = m_bottomDockTabbing.size();
    if(size>1)
    {
        QDockWidget* first = m_bottomDockTabbing.at(size-1);
        QDockWidget* second = m_bottomDockTabbing.at(0);

        QMainWindow::tabifyDockWidget(first, second);
    }
}

void DockSupport::tabifyLeftDockWidget(QDockWidget *dockwidget, const QString &title)
{
    m_leftDockTabbing.insert(0, dockwidget);
    m_leftDockTitles.insert(0, title);

    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);

    int size = m_leftDockTabbing.size();
    if(size>1)
    {
        QDockWidget* first = m_leftDockTabbing.at(size-1);
        QDockWidget* second = m_leftDockTabbing.at(0);

        QMainWindow::tabifyDockWidget(first, second);
    }
}

void DockSupport::tabifyRightDockWidget(QDockWidget *dockwidget, const QString& title)
{
    m_rightDockTabbing.insert(0, dockwidget);
    m_rightDockTitles.insert(0, title);

    addDockWidget(Qt::RightDockWidgetArea, dockwidget);

    int size = m_rightDockTabbing.size();
    if(size>1)
    {
        QDockWidget* first = m_rightDockTabbing.at(size-1);
        QDockWidget* second = m_rightDockTabbing.at(0);

        QMainWindow::tabifyDockWidget(first, second);
    }
}
