#pragma once
#include <QString>
#include <memory>

struct Stop{
    QString city;
    int durationMinutes;
    double price;
    std::shared_ptr<Stop> next = nullptr;

    Stop(const QString &c, int dur, double p)
        : city(c), durationMinutes(dur), price(p){
    }
};
