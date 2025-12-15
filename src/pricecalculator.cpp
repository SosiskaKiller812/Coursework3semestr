#include "pricecalculator.h"
#include "route.h"
#include <algorithm>

PriceCalculator::PriceCalculator() = default;

double PriceCalculator::calculateBasePrice(std::shared_ptr<Route> route) {
    if (!route) {
        return 0.0;
    }
    return route->totalPrice();
}

double PriceCalculator::calculateSegmentPrice(std::shared_ptr<Route> route, 
                                               int fromStop, int toStop) {
    if (!route || fromStop < 0 || toStop < 0 || fromStop >= toStop) {
        return 0.0;
    }
    
    double price = 0.0;
    auto stops = route->getAllStops();
    
    if (toStop > stops.size()) {
        return 0.0;
    }
    
    const int endIndex = std::min(toStop, static_cast<int>(stops.size()));
    for (int i = fromStop; i < endIndex; ++i) {
        if (stops[i]) {
            price += stops[i]->price;
        }
    }
    
    return price;
}


double PriceCalculator::operator()(std::shared_ptr<Route> route) const {
    return calculateBasePrice(route);
}


