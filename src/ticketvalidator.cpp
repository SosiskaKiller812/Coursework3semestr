#include "ticketvalidator.h"
#include "ticket.h"
#include "passenger.h"
#include "ValidationException.h"

TicketValidator::TicketValidator() = default;

bool TicketValidator::validateTicket(std::shared_ptr<Ticket> ticket) const {
    if (!ticket) {
        return false;
    }
    
    return validateTicketStatus(ticket) &&
           validatePrice(ticket->price()) &&
           validatePassenger(ticket->passenger());
}

bool TicketValidator::isTicketValid(std::shared_ptr<Ticket> ticket, const QDateTime& currentTime) const {
    if (!validateTicket(ticket)) {
        return false;
    }
    
    if (ticket->status() != "paid" && ticket->status() != "reserved") {
        return false;
    }
    
    if (ticket->trip() && ticket->trip()->departure() < currentTime) {
        return false;
    }
    
    return true;
}

bool TicketValidator::validateTicketRoute(std::shared_ptr<Ticket> ticket, std::shared_ptr<Route> route) const {
    return ticket && route && ticket->route() == route;
}

bool TicketValidator::validateTicketTrip(std::shared_ptr<Ticket> ticket, std::shared_ptr<Trip> trip) const {
    return ticket && trip && ticket->trip() == trip;
}

bool TicketValidator::validateTicketStatus(std::shared_ptr<Ticket> ticket) const {
    if (!ticket) {
        return false;
    }
    
    QString status = ticket->status();
    return status == "reserved" || status == "paid" || status == "cancelled";
}

void TicketValidator::validateTicketThrow(std::shared_ptr<Ticket> ticket) const {
    if (!ticket) {
        throw ValidationException("Ticket is null");
    }
    
    if (!validatePassenger(ticket->passenger())) {
        throw ValidationException("Invalid passenger data");
    }
    
    if (!validatePrice(ticket->price())) {
        throw ValidationException(QString("Invalid price: %1").arg(ticket->price()));
    }
    
    if (!validateTicketStatus(ticket)) {
        throw ValidationException(QString("Invalid ticket status: %1").arg(ticket->status()));
    }
}

bool TicketValidator::validatePassenger(std::shared_ptr<Passenger> passenger) const {
    if (!passenger) {
        return false;
    }
    
    return isValidPassengerName(passenger->name()) &&
           isValidPassport(passenger->passport());
}

bool TicketValidator::validatePrice(double price) const {
    return price >= MIN_PRICE && price <= MAX_PRICE;
}

QVector<QString> TicketValidator::getValidationErrors(std::shared_ptr<Ticket> ticket) const {
    QVector<QString> errors;
    
    if (!ticket) {
        errors.append("Ticket is null");
        return errors;
    }
    
    if (!validatePassenger(ticket->passenger())) {
        errors.append("Invalid passenger data");
    }
    
    if (!validatePrice(ticket->price())) {
        errors.append(QString("Invalid price: %1").arg(ticket->price()));
    }
    
    if (!validateTicketStatus(ticket)) {
        errors.append(QString("Invalid ticket status: %1").arg(ticket->status()));
    }
    
    return errors;
}

bool TicketValidator::isValidPassengerName(const QString& name) const {
    return !name.isEmpty() && name.length() >= 2 && name.length() <= 100;
}

bool TicketValidator::isValidPassport(const QString& passport) const {
    return !passport.isEmpty() && passport.length() >= 6;
}


