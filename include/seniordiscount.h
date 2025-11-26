#pragma once
#include "discount.h"
#include <QString>
#include <memory>

class Passenger;

class SeniorDiscount : public Discount {
public:
    explicit SeniorDiscount(double percentage = 15.0, int minAge = 60);
    ~SeniorDiscount() override = default;
    
    double calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger = nullptr) const override;
    QString name() const override;
    double percentage() const override;
    bool isApplicable(std::shared_ptr<Passenger> passenger = nullptr) const override;
    
    void setPercentage(double percentage);
    void setMinAge(int minAge);
    int minAge() const;
    
private:
    double m_percentage;
    int m_minAge;
};


