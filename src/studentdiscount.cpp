#include "studentdiscount.h"
#include "passenger.h"

StudentDiscount::StudentDiscount(double percentage)
    : m_percentage(percentage) {
}

double StudentDiscount::calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger) const {
    if (isApplicable(passenger)) {
        return basePrice * (m_percentage / 100.0);
    }
    return 0.0;
}

QString StudentDiscount::name() const {
    return "Student Discount";
}

double StudentDiscount::percentage() const {
    return m_percentage;
}

bool StudentDiscount::isApplicable(std::shared_ptr<Passenger> passenger) const {
    if (!passenger) {
        return false;
    }
    // Предполагаем, что студенты в возрасте 18-25 лет
    int age = passenger->age();
    return age >= 18 && age <= 25;
}

void StudentDiscount::setPercentage(double percentage) {
    m_percentage = percentage;
}

void StudentDiscount::setStudentIdRequired(bool required) {
    m_studentIdRequired = required;
}

bool StudentDiscount::studentIdRequired() const {
    return m_studentIdRequired;
}


