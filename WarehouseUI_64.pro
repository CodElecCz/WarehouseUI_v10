#-------------------------------------------------
#
# Project created by QtCreator 2018-02-03T14:53:21
#
#-------------------------------------------------

QT       += core gui sql xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Warehouse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += OPENCV
DEFINES += _TIMESPEC_DEFINED
DEFINES += _TIMEVAL_DEFINED

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Lib/log4cxxLib/msvc2017/bin/x64/release/ -llog4cxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Lib/log4cxxLib/msvc2017/bin/x64/debug/ -llog4cxx
else:unix: LIBS += -L$$PWD/../../Lib/log4cxxLib/msvc2017/bin/x64/ -llog4cxx

INCLUDEPATH += $$PWD/../../Lib/log4cxxLib/msvc2017/bin/x64
DEPENDPATH += $$PWD/../../Lib/log4cxxLib/msvc2017/bin/x64

INCLUDEPATH += \
    .\..\..\Lib\log4cxxLib\log4cxx\src\main\include

TRANSLATIONS = \
    languages/warehouse_en.ts \
    languages/warehouse_cs.ts

SOURCES += \
    DockSupport.cpp \
    MainView.cpp \
    MainWindowLanguage.cpp \
    MainWindowStock.cpp \
    MainWindowStockPositions.cpp \
    MainWindowStorage.cpp \
    MainWindowSupplier.cpp \
    ReceipeForm.cpp \
    SplashScreen.cpp \
    SqlConnectionDialog.cpp \
    StockPositions.cpp \
    StockView.cpp \
    StorageView.cpp \
    SupplierView.cpp \
    XmlSyntaxHighlighter.cpp \
    dbs/database.cpp \
    gzip.cpp \
    main.cpp \
    MainWindow.cpp \
    models/comboboxdbsdelegate.cpp \
    models/comboboxitemdelegate.cpp \
    models/progressbaritemdelegate.cpp \
    models/receipelisttablemodel.cpp \
    models/receipetablemodel.cpp \
    models/regexpitemdelegate.cpp \
    models/stockpositiontablemodel.cpp \
    models/stocktablemodel.cpp \
    models/storagetablemodel.cpp \
    models/suppliertablemodel.cpp \
    pohoda/pohoda.cpp \
    pohoda/pohodaaccountingrequest.cpp \
    pohoda/pohodareceipeinrequest.cpp \
    pohoda/pohodareceipeoutrequest.cpp \
    pohoda/pohodastockrequest.cpp \
    pohoda/pohodastoragerequest.cpp \
    pohoda/pohodasupplierrequest.cpp \
    style/CustomStyle.cpp

HEADERS += \
    DockSupport.h \
    LightWidget.h \
    MainView.h \
    MainWindow.h \
    ReceipeForm.h \
    SplashScreen.h \
    SqlConnectionDialog.h \
    StockPositions.h \
    StockView.h \
    StorageView.h \
    SupplierView.h \
    XmlSyntaxHighlighter.h \
    dbs/database.h \
    gzip.h \
    models/comboboxdbsdelegate.h \
    models/comboboxitemdelegate.h \
    models/progressbaritemdelegate.h \
    models/receipelisttablemodel.h \
    models/receipetablemodel.h \
    models/regexpitemdelegate.h \
    models/stockpositiontablemodel.h \
    models/stocktablemodel.h \
    models/storagetablemodel.h \
    models/suppliertablemodel.h \
    pohoda/pohoda.h \
    pohoda/pohodaaccountingrequest.h \
    pohoda/pohodareceipeinrequest.h \
    pohoda/pohodareceipeoutrequest.h \
    pohoda/pohodastockrequest.h \
    pohoda/pohodastoragerequest.h \
    pohoda/pohodasupplierrequest.h \
    style/CustomStyle.h

FORMS += \
    AboutDialog.ui \
    MainView.ui \
    MainWindow.ui \
    ReceipeForm.ui \
    SplashScreen.ui \
    SqlConnectionDialog.ui \
    StockPositions.ui \
    StockView.ui \
    StorageView.ui \
    SupplierView.ui

RESOURCES += \
    DarkStyle.qrc \
    LightStyle.qrc \
    Resources.qrc

#RC_FILE += \
#    Resource.rc

DISTFILES += \
    pohoda/xml/CleneniSkladu_01_v2.0 (response).xml \
    pohoda/xml/Pohoda - mServer.png \
    pohoda/xml/error.txt \
    pohoda/xml/faktury_01_v2.0 (response).xml \
    pohoda/xml/new.xml \
    pohoda/xml/post_request.txt \
    pohoda/xml/prijemka_01_v2.0 (response).xml \
    pohoda/xml/sklady_01_v2.0 (response).xml \
    pohoda/xml/vydejka_01_v2.0 (response).xml \
    pohoda/xml/zasoby_02_v2.0 (response).xml
