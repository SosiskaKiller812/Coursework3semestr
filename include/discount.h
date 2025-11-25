#pragma once
#include <QString>
#include <memory>

class Passenger;

// Абстрактный класс для скидок
class Discount {
public:
    virtual ~Discount() = default;
    
    // Виртуальная функция для расчета скидки
    virtual double calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger = nullptr) const = 0;
    
    // Виртуальная функция для получения названия скидки
    virtual QString name() const = 0;
    
    // Виртуальная функция для получения процента скидки
    virtual double percentage() const = 0;
    
    // Виртуальная функция для проверки применимости скидки
    virtual bool isApplicable(std::shared_ptr<Passenger> passenger = nullptr) const = 0;
    
    // Перегрузка операций
    bool operator==(const Discount& other) const;
    bool operator!=(const Discount& other) const;
    
protected:
    Discount() = default;
};


