#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pohoda/pohoda.h"
#include "dbs/database.h"

#include "StockView.h"
#include "SupplierView.h"
#include "StorageView.h"
#include "StockPositions.h"
#include "MainView.h"
#include "SplashScreen.h"

#define APP_VER "v1.2"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initializeApp();
    void initializeToolbar();
    void readAppSettings();
    void writeAppSettings();

    void createStockWindow(bool showWindow);
    void closeStockWindow();

    void createSupplierWindow(bool showWindow);
    void closeSupplierWindow();

    void createStorageWindow(bool showWindow);
    void closeStorageWindow();

    void createStockPositionsWindow(bool showWindow, const QMap<QString, SPositionConfig> &positionsMap);
    void closeStockPositionsWindow();

    void changeEvent(QEvent*);

    void loadLanguage(const QString& rLanguage);
    void createLanguageMenu(void);

    void tabifyBottomDockWidget(QDockWidget* dockwidget, QString title = "");
    void tabifyLeftDockWidget(QDockWidget* dockwidget, QString title = "");
    void tabifyRightDockWidget(QDockWidget* dockwidget, QString title = "");

    void closeEvent(QCloseEvent *event);

private slots:    
    void pohoda_postResponse(ERequestType type, QString xml);
    void view_prijemkaCreate(SReceipe receipe);
    void view_vydejkaCreate(SReceipe receipe);

    void stockView_importData();
    void supplierView_importData();
    void storageView_importData();

    //dialogs
    void aboutDialog();
    int closeDialog();

    void slotLanguageChanged(QAction* action);

private:
    Ui::MainWindow  *ui;
    MainView        *view;
    StockView       *stockView;
    SupplierView    *supplierView;
    StorageView     *storageView;
    StockPositions  *stockPositions;
    Pohoda          *pohoda;
    Database        *dbs;

    QList<QDockWidget*> m_bottomDockTabbing;
    QList<QDockWidget*> m_leftDockTabbing;
    QList<QDockWidget*> m_rightDockTabbing;
    QStringList         m_bottomDockTitles;

    QString             m_defAccoutningUnitIco;
    QTranslator         m_translator; // contains the translations for this application
    QTranslator         m_translatorQt; // contains the translations for qt
    QString             m_currLang; // contains the currently loaded language
    QString             m_langPath; // Path of language files. This is always fixed to /languages.
    bool                m_allowShutDown;
    bool                m_pohodaDisable;
};

#endif // MAINWINDOW_H
