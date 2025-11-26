#pragma once
#include "discount.h"
#include <QString>
#include <memory>

class Passenger;

class StudentDiscount : public Discount {
public:
    explicit StudentDiscount(double percentage = 10.0);
    ~StudentDiscount() override = default;
    
    double calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger = nullptr) const override;
    QString name() const override;
    double percentage() const override;
    bool isApplicable(std::shared_ptr<Passenger> passenger = nullptr) const override;
    
    void setPercentage(double percentage);
    void setStudentIdRequired(bool required);
    bool studentIdRequired() const;
    
private:
    double m_percentage;
    bool m_studentIdRequired;
};


