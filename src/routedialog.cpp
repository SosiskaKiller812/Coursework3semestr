#include "routedialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QDateTimeEdit>
#include "route.h"

RouteDialog::RouteDialog(QWidget *parent)
    : QDialog(parent){
    leFrom = new QLineEdit(this);
    leTo = new QLineEdit(this);
    sbDuration = new QSpinBox(this);
    sbDuration->setRange(1, 40000);
    sbDuration->setSuffix(" мин");

    dtDeparture = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dtDeparture->setDisplayFormat("yyyy-MM-dd HH:mm");
    dtDeparture->setCalendarPopup(true);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &RouteDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &RouteDialog::reject);

    auto *layout = new QFormLayout(this);
    layout->addRow("Откуда:", leFrom);
    layout->addRow("Куда:", leTo);
    layout->addRow("Время в пути:", sbDuration);
    layout->addRow("Отправление:", dtDeparture);
    layout->addWidget(buttons);

    setWindowTitle("Добавить рейс");
}

QString RouteDialog::fromCity() const{ return leFrom->text(); }
QString RouteDialog::toCity()   const{ return leTo->text(); }
int RouteDialog::durationMinutes() const{ return sbDuration->value(); }
QDateTime RouteDialog::departure() const{ return dtDeparture->dateTime(); }
