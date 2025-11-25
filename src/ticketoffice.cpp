#include "ticketoffice.h"
#include "passenger.h"
#include "route.h"
#include "trip.h"
#include "seat.h"
#include "discount.h"

TicketOffice::TicketOffice()
    : m_nextTicketId(1), m_nextBookingId(1), m_nextPaymentId(1) {
}

int TicketOffice::createTicket(std::shared_ptr<Passenger> passenger,
                                std::shared_ptr<Route> route,
                                std::shared_ptr<Trip> trip,
                                int seatNumber,
                                double price) {
    auto ticket = std::make_shared<Ticket>(m_nextTicketId++, passenger, route, trip, seatNumber, price);
    m_tickets.append(ticket);
    return ticket->id();
}

std::shared_ptr<Ticket> TicketOffice::getTicket(int id) const {
    for (const auto& ticket : m_tickets) {
        if (ticket && ticket->id() == id) {
            return ticket;
        }
    }
    return nullptr;
}

QVector<std::shared_ptr<Ticket>> TicketOffice::getAllTickets() const {
    return m_tickets;
}

QVector<std::shared_ptr<Ticket>> TicketOffice::getTicketsByPassenger(std::shared_ptr<Passenger> passenger) const {
    QVector<std::shared_ptr<Ticket>> result;
    for (const auto& ticket : m_tickets) {
        if (ticket && ticket->passenger() == passenger) {
            result.append(ticket);
        }
    }
    return result;
}

QVector<std::shared_ptr<Ticket>> TicketOffice::getTicketsByRoute(std::shared_ptr<Route> route) const {
    QVector<std::shared_ptr<Ticket>> result;
    for (const auto& ticket : m_tickets) {
        if (ticket && ticket->route() == route) {
            result.append(ticket);
        }
    }
    return result;
}

bool TicketOffice::cancelTicket(int ticketId) {
    auto ticket = getTicket(ticketId);
    if (ticket) {
        ticket->setStatus("cancelled");
        return true;
    }
    return false;
}

bool TicketOffice::refundTicket(int ticketId) {
    auto ticket = getTicket(ticketId);
    if (ticket) {
        ticket->setStatus("cancelled");
        // Логика возврата средств
        return true;
    }
    return false;
}

int TicketOffice::createBooking(std::shared_ptr<Passenger> passenger,
                                 std::shared_ptr<Route> route,
                                 std::shared_ptr<Trip> trip,
                                 std::shared_ptr<Seat> seat) {
    auto booking = std::make_shared<Booking>(m_nextBookingId++, passenger, route, trip, seat);
    if (seat) {
        seat->reserve(passenger);
    }
    m_bookings.append(booking);
    return booking->id();
}

std::shared_ptr<Booking> TicketOffice::getBooking(int id) const {
    for (const auto& booking : m_bookings) {
        if (booking && booking->id() == id) {
            return booking;
        }
    }
    return nullptr;
}

QVector<std::shared_ptr<Booking>> TicketOffice::getAllBookings() const {
    return m_bookings;
}

void TicketOffice::processExpiredBookings() {
    for (auto& booking : m_bookings) {
        if (booking && booking->isExpired()) {
            booking->setStatus(Booking::BookingStatus::Expired);
            if (booking->seat()) {
                booking->seat()->release();
            }
        }
    }
}

int TicketOffice::createPayment(std::shared_ptr<Ticket> ticket,
                                 double amount,
                                 Payment::PaymentMethod method) {
    auto payment = std::make_shared<Payment>(m_nextPaymentId++, ticket, amount, method);
    payment->process();
    m_payments.append(payment);
    return payment->id();
}

std::shared_ptr<Payment> TicketOffice::getPayment(int id) const {
    for (const auto& payment : m_payments) {
        if (payment && payment->id() == id) {
            return payment;
        }
    }
    return nullptr;
}

QVector<std::shared_ptr<Payment>> TicketOffice::getAllPayments() const {
    return m_payments;
}

void TicketOffice::applyDiscountToTicket(int ticketId, std::shared_ptr<Discount> discount) {
    auto ticket = getTicket(ticketId);
    if (ticket && discount) {
        ticket->applyDiscount(discount);
    }
}

double TicketOffice::totalRevenue() const {
    double total = 0.0;
    for (const auto& payment : m_payments) {
        if (payment && payment->status() == Payment::PaymentStatus::Completed) {
            total += payment->amount();
        }
    }
    return total;
}

int TicketOffice::totalTicketsSold() const {
    int count = 0;
    for (const auto& ticket : m_tickets) {
        if (ticket && ticket->status() == "paid") {
            count++;
        }
    }
    return count;
}

int TicketOffice::totalBookings() const {
    return m_bookings.size();
}

TicketOffice::TicketIterator TicketOffice::ticketsBegin() {
    return m_tickets.begin();
}

TicketOffice::TicketIterator TicketOffice::ticketsEnd() {
    return m_tickets.end();
}

TicketOffice::ConstTicketIterator TicketOffice::ticketsBegin() const {
    return m_tickets.begin();
}

TicketOffice::ConstTicketIterator TicketOffice::ticketsEnd() const {
    return m_tickets.end();
}


