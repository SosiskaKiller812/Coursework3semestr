#include "childdiscount.h"
#include "passenger.h"

ChildDiscount::ChildDiscount(double percentage, int maxAge)
    : m_percentage(percentage), m_maxAge(maxAge) {
}

double ChildDiscount::calculateDiscount(double basePrice, std::shared_ptr<Passenger> passenger) const {
    if (isApplicable(passenger)) {
        return basePrice * (m_percentage / 100.0);
    }
    return 0.0;
}

QString ChildDiscount::name() const {
    return "Child Discount";
}

double ChildDiscount::percentage() const {
    return m_percentage;
}

bool ChildDiscount::isApplicable(std::shared_ptr<Passenger> passenger) const {
    if (!passenger) {
        return false;
    }
    return passenger->age() <= m_maxAge;
}

void ChildDiscount::setPercentage(double percentage) {
    m_percentage = percentage;
}

void ChildDiscount::setMaxAge(int maxAge) {
    m_maxAge = maxAge;
}

int ChildDiscount::maxAge() const {
    return m_maxAge;
}


