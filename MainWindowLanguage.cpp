#include <QDockWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

/* TRANSLATION
lupdate -verbose WarehouseUI_64.pro
linguist languages/warehouse_cs.ts
lrelease WarehouseUI_64.pro
*/

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "cs_CZ"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "cs"

    m_langPath = QApplication::applicationDirPath();
    m_langPath.append("/languages");
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("warehouse_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "warehouse_cs.qm"
        locale.truncate(locale.lastIndexOf('.')); // "warehouse_cs"
        locale.remove(0, locale.indexOf('_') + 1); // "cs"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(m_langPath, locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action)
    {
        // load the language dependant on the action content
        loadLanguage(action->data().toString());
        //setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    QString path = QApplication::applicationDirPath();
    path.append("/languages/");
    if(translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
        qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage)
    {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("warehouse_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        //ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(0 != event)
    {
        switch(event->type())
        {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            DockSupport::retranslate();
            break;

        // this event is send, if the system, language changes
        case QEvent::LocaleChange:
            {
                QString locale = QLocale::system().name();
                locale.truncate(locale.lastIndexOf('_'));
                loadLanguage(locale);
            }
            break;
        default:
            break;
        }
    }
    QMainWindow::changeEvent(event);
}
