#pragma once
#include <QString>
#include <memory>

struct Stop{
    QString city;
    int durationMinutes;
    double price;
    std::shared_ptr<Stop> next;

    Stop(const QString &c, int dur, double p)
        : city(c), durationMinutes(dur), price(p), next(nullptr){
    }
};
