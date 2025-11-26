#include "ticketoffice.h"
#include "passenger.h"
#include "route.h"
#include "trip.h"
#include "seat.h"
#include "discount.h"

TicketOffice::TicketOffice() = default;

int TicketOffice::createTicket(std::shared_ptr<Passenger> passenger,
                                std::shared_ptr<Route> route,
                                std::shared_ptr<Trip> trip,
                                int seatNumber,
                                double price) {
    const int ticketId = m_nextTicketId;
    ++m_nextTicketId;
    auto ticket = std::make_shared<Ticket>(ticketId, passenger, route, trip, seatNumber, price);
    m_tickets.append(ticket);
    return ticketId;
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

bool TicketOffice::cancelTicket(int ticketId) const {
    if (auto ticket = getTicket(ticketId); ticket) {
        ticket->setStatus("cancelled");
        return true;
    }
    return false;
}

bool TicketOffice::refundTicket(int ticketId) const {
    if (auto ticket = getTicket(ticketId); ticket && ticket->status() == "paid") {
        ticket->setStatus("refunded");
        // Логика возврата средств
        return true;
    }
    return false;
}

int TicketOffice::createBooking(std::shared_ptr<Passenger> passenger,
                                 std::shared_ptr<Route> route,
                                 std::shared_ptr<Trip> trip,
                                 std::shared_ptr<Seat> seat) {
    const int bookingId = m_nextBookingId;
    ++m_nextBookingId;
    auto booking = std::make_shared<Booking>(bookingId, passenger, route, trip, seat);
    if (seat) {
        seat->reserve(passenger);
    }
    m_bookings.append(booking);
    return bookingId;
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

void TicketOffice::processExpiredBookings() const {
    for (const auto& booking : m_bookings) {
        if (booking && booking->isExpired()) {
            booking->setStatus(Booking::BookingStatus::Expired);
            if (auto seat = booking->seat(); seat) {
                seat->release();
            }
        }
    }
}

int TicketOffice::createPayment(std::shared_ptr<Ticket> ticket,
                                 double amount,
                                 Payment::PaymentMethod method) {
    const int paymentId = m_nextPaymentId;
    ++m_nextPaymentId;
    auto payment = std::make_shared<Payment>(paymentId, ticket, amount, method);
    payment->process();
    m_payments.append(payment);
    return paymentId;
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

void TicketOffice::applyDiscountToTicket(int ticketId, std::shared_ptr<Discount> discount) const {
    if (auto ticket = getTicket(ticketId); ticket && discount) {
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


