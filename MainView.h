#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include "XmlSyntaxHighlighter.h"
#include "dbs/database.h"
#include "SupplierView.h"
#include "models/receipetablemodel.h"
#include "ReceipeForm.h"


namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    explicit MainView(Database *dbs, const QMap<QString, SPositionConfig>& positionsMap, QWidget *parent = nullptr);
    ~MainView();

    void setSupplierList(const QList<SSupplier>& suppliers);
    void setStorageList(const QList<SStorage>& storageList);
    void showXml(const QString& xml);

protected:
    void changeEvent(QEvent* event);

signals:
    void prijemkaCreate(const SReceipe& receipe);
    void vydejkaCreate(const SReceipe& receipe);

public slots:
    void toggleViewAction_page1(bool state);
    void toggleViewAction_page2(bool state);
    void toggleViewAction_page3(bool state);

private slots:
    void prijemkaForm_receipeCreate(const SReceipe& receipe);
    void vydejkaForm_receipeCreate(const SReceipe& receipe);


private:
    Ui::MainView            *ui;
    ReceipeForm             *receipeInForm;
    ReceipeForm             *receipeOutForm;
    XMLSyntaxHighlighter    *highlighter;          
};

#endif // MAINVIEW_H
