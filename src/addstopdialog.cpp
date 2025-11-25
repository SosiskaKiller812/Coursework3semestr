#include "addstopdialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

AddStopDialog::AddStopDialog(QWidget *parent)
    : QDialog(parent){
    leCity = new QLineEdit(this);
    sbDuration = new QSpinBox(this);
    sbDuration->setRange(1, 1440);
    sbDuration->setSuffix(" мин");

    dsbPrice = new QDoubleSpinBox(this);
    dsbPrice->setRange(0, 10000);
    dsbPrice->setSuffix("руб");
    dsbPrice->setDecimals(2);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &AddStopDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &AddStopDialog::reject);

    auto *layout = new QFormLayout(this);
    layout->addRow("Остановка:", leCity);
    layout->addRow("Длительность:", sbDuration);
    layout->addRow("Цена:", dsbPrice);
    layout->addWidget(buttons);

    setWindowTitle("Добавить остановку");
}

QString AddStopDialog::cityName() const{ return leCity->text(); }
int AddStopDialog::duration() const{ return sbDuration->value(); }
double AddStopDialog::price() const{ return dsbPrice->value(); }
