#pragma once
#include <QDialog>
#include <QTextEdit>

class Route;

class RouteDetailsDialog : public QDialog{
    Q_OBJECT
public:
    explicit RouteDetailsDialog(const Route &route, QWidget *parent = nullptr);

private:
    QTextEdit *textInfo;
};
