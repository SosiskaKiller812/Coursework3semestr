#include "discount.h"

bool Discount::operator==(const Discount& other) const {
    return name() == other.name() && percentage() == other.percentage();
}

bool Discount::operator!=(const Discount& other) const {
    return !(*this == other);
}


