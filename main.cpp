#include "MainWindow.h"
#include <QApplication>

#include "style/CustomStyle.h"

#include <windows.h>
#include <stdio.h>

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"
#include "log4cxx/xml/domconfigurator.h"

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

//LoggerPtr logger(Logger::getLogger("Warehouse"));

int main(int argc, char *argv[])
{   
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Warehouse");
    QCoreApplication::setApplicationVersion(APP_VER);

    setlocale(LC_CTYPE, "UTF-8");

    QCommandLineParser parser;
    parser.setApplicationDescription("Application for warehouse management");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption styleOption(QStringList() << "s" << "style",
                                   QCoreApplication::translate("main", "Set style"),
                                   QCoreApplication::translate("main", "dark, light"));
    parser.addOption(styleOption);

    QCommandLineOption consoleOption(QStringList() << "c" << "console", QCoreApplication::translate("main", "Show console"));
    parser.addOption(consoleOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    bool isStyle = parser.isSet(styleOption);
    QString styleStr = parser.value(styleOption);
    bool isConsole = parser.isSet(consoleOption);

    //console output
    if(isConsole)
    {
        // detach from the current console window
        // if launched from a console window, that will still run waiting for the new console (below) to close
        // it is useful to detach from Qt Creator's <Application output> panel
#ifdef _DEBUG
        FreeConsole();
#endif
        // create a separate new console window
        bool res = AllocConsole();
        if(res)
        {
            FILE* pCout1 = NULL;
            FILE* pCout2 = NULL;
            FILE* pCout3 = NULL;

            // attach the new console to this application's process
            AttachConsole(GetCurrentProcessId());

            freopen_s(&pCout1, "conin$", "r", stdin);
            freopen_s(&pCout2, "conout$", "w", stdout);
            freopen_s(&pCout3, "conout$", "w", stderr);
        }
    }

    //LOG4CXX
    QString logFilePath = QFileInfo(QCoreApplication::applicationFilePath()).filePath();
    logFilePath.replace(".exe", ".log4cxx.xml");
    QFile logFile(logFilePath);
    if(logFile.exists())
    {
        //DOMConfigurator::configure(logFilePath.toUtf8().constData());
    }
    else
    {
        //BasicConfigurator::configure();
    }

    LOG4QT(INFO, QString("Start application" + QString(isStyle? " -s":"") + QString(isConsole? " -c":"")) );

    // style our application with custom style
    if(isStyle)
    {
        if(styleStr.contains("light", Qt::CaseInsensitive))
        {
            app.setStyle(new CustomStyle(ECustomStyle_Light));
        }
        else if(styleStr.contains("dark", Qt::CaseInsensitive))
        {
            app.setStyle(new CustomStyle(ECustomStyle_Dark));
        }
    }
    else
        app.setStyle(new CustomStyle(ECustomStyle_Light));

    MainWindow w;
    w.show();

    int ret = app.exec();

    LOG4QT(INFO, QString("Exit application"));

    return ret;
}
