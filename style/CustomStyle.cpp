#include "CustomStyle.h"

CustomStyle::CustomStyle(ECustomStyle type) :
    CustomStyle(styleBase(type))
{
    m_type = type;
}

CustomStyle::CustomStyle(QStyle *style) : QProxyStyle(style)
{

}

QStyle *CustomStyle::styleBase(ECustomStyle type, QStyle *style) const
{    
    static QStyle *base = nullptr;
    if(!style)
    {
        switch(type)
        {
        case ECustomStyle_Light:
        case ECustomStyle_Dark:
            base = QStyleFactory::create(QStringLiteral("Fusion"));
            break;
        default:
            base = QStyleFactory::create(QStringLiteral("WindowsVista"));
            break;
        }
    }
    else
        base = style;

    return base;
}

QStyle *CustomStyle::baseStyle() const
{
    return styleBase();
}

void CustomStyle::polish(QPalette &palette)
{
    switch(m_type)
    {
    case ECustomStyle_Dark:
        // modify palette to dark
        palette.setColor(QPalette::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        palette.setColor(QPalette::Base, QColor(42, 42, 42));
        palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        palette.setColor(QPalette::Dark, QColor(35, 35, 35));
        palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
        palette.setColor(QPalette::HighlightedText, Qt::white);
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
        break;
    default:
        break;
    }
}

void CustomStyle::polish(QApplication *app)
{
    if (!app)
        return;

    // increase font size for better reading,
    // setPointSize was reduced from +2 because when applied this way in Qt5, the
    // font is larger than intended for some reason
    //QFont defaultFont = QApplication::font();
    //defaultFont.setPointSize(defaultFont.pointSize() + 1);
    //app->setFont(defaultFont);

    // loadstylesheet
    switch(m_type)
    {
    case ECustomStyle_Light:
        {
            QFile fstyle(QStringLiteral(":/style/light/style.qss"));
            if (fstyle.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                // set stylesheet
                QString qsStylesheet = QString::fromLatin1(fstyle.readAll());
                app->setStyleSheet(qsStylesheet);
                fstyle.close();
            }
        }
        break;
    case ECustomStyle_Dark:
        {
            QFile fstyle(QStringLiteral(":/style/dark/style.qss"));
            if (fstyle.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                // set stylesheet
                QString qsStylesheet = QString::fromLatin1(fstyle.readAll());
                app->setStyleSheet(qsStylesheet);
                fstyle.close();
            }
        }
        break;
    default:
        break;
    }

}
