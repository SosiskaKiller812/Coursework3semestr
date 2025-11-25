#pragma once
#include "discount.h"
#include <QString>
#include <memory>

class Passenger;

class ChildDiscount : public Discount {
public:
    ChildDiscount(double percentage = 50.0, int maxAge = 12);
    ~ChildDiscount() override = default;
    
    double calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger = nullptr) const override;
    QString name() const override;
    double percentage() const override;
    bool isApplicable(std::shared_ptr<Passenger> passenger = nullptr) const override;
    
    void setPercentage(double percentage);
    void setMaxAge(int maxAge);
    int maxAge() const;
    
private:
    double m_percentage;
    int m_maxAge;
};


