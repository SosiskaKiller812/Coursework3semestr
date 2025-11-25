#include "seniordiscount.h"
#include "passenger.h"

SeniorDiscount::SeniorDiscount(double percentage, int minAge)
    : m_percentage(percentage), m_minAge(minAge) {
}

double SeniorDiscount::calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger) const {
    if (isApplicable(passenger)) {
        return basePrice * (m_percentage / 100.0);
    }
    return 0.0;
}

QString SeniorDiscount::name() const {
    return "Senior Discount";
}

double SeniorDiscount::percentage() const {
    return m_percentage;
}

bool SeniorDiscount::isApplicable(std::shared_ptr<Passenger> passenger) const {
    if (!passenger) {
        return false;
    }
    return passenger->age() >= m_minAge;
}

void SeniorDiscount::setPercentage(double percentage) {
    m_percentage = percentage;
}

void SeniorDiscount::setMinAge(int minAge) {
    m_minAge = minAge;
}

int SeniorDiscount::minAge() const {
    return m_minAge;
}


