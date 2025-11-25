#pragma once
#include <QDialog>
#include <QDateTime>

class QLineEdit;
class QSpinBox;
class QDateTimeEdit;

class RouteDialog : public QDialog{
    Q_OBJECT
public:
    explicit RouteDialog(QWidget *parent = nullptr);
    QString fromCity() const;
    QString toCity() const;
    int durationMinutes() const;
    QDateTime departure() const;

private:
    QLineEdit *leFrom;
    QLineEdit *leTo;
    QSpinBox *sbDuration;
    QDateTimeEdit *dtDeparture;
};
