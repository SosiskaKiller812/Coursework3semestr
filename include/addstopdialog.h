#pragma once
#include <QDialog>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

class AddStopDialog : public QDialog{
    Q_OBJECT
public:
    explicit AddStopDialog(QWidget *parent = nullptr);

    QString cityName() const;
    int duration() const;
    double price() const;

private:
    QLineEdit *leCity;
    QSpinBox *sbDuration;
    QDoubleSpinBox *dsbPrice;
};