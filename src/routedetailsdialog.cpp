#include "routedetailsdialog.h"
#include "route.h"
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QFont>

RouteDetailsDialog::RouteDetailsDialog(const Route &route, QWidget *parent)
    : QDialog(parent)
    , textInfo(nullptr)
{
    textInfo = new QTextEdit(this);
    textInfo->setReadOnly(true);
    textInfo->setPlainText(route.detailedInfo());



    auto *btnClose = new QPushButton("Закрыть", this);
    connect(btnClose, &QPushButton::clicked, this, &RouteDetailsDialog::accept);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(textInfo);
    layout->addWidget(btnClose);

    setWindowTitle("Детали маршрута");
    setMinimumSize(600, 700);

    textInfo->verticalScrollBar()->setValue(0);
}
