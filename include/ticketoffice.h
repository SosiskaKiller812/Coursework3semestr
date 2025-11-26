#pragma once
#include "ticket.h"
#include "booking.h"
#include "payment.h"
#include <QVector>
#include <QString>
#include <memory>
#include <algorithm>

class Passenger;
class Route;
class Trip;
class Discount;

// Контейнерный класс для управления билетами
class TicketOffice {
public:
    TicketOffice();
    ~TicketOffice() = default;
    
    // Работа с билетами
    int createTicket(std::shared_ptr<Passenger> passenger,
                     std::shared_ptr<Route> route,
                     std::shared_ptr<Trip> trip,
                     int seatNumber,
                     double price);
    
    std::shared_ptr<Ticket> getTicket(int id) const;
    QVector<std::shared_ptr<Ticket>> getAllTickets() const;
    QVector<std::shared_ptr<Ticket>> getTicketsByPassenger(std::shared_ptr<Passenger> passenger) const;
    QVector<std::shared_ptr<Ticket>> getTicketsByRoute(std::shared_ptr<Route> route) const;
    
    bool cancelTicket(int ticketId) const;
    bool refundTicket(int ticketId) const;
    
    // Работа с бронированиями
    int createBooking(std::shared_ptr<Passenger> passenger,
                      std::shared_ptr<Route> route,
                      std::shared_ptr<Trip> trip,
                      std::shared_ptr<Seat> seat);
    
    std::shared_ptr<Booking> getBooking(int id) const;
    QVector<std::shared_ptr<Booking>> getAllBookings() const;
    void processExpiredBookings() const;
    
    // Работа с платежами
    int createPayment(std::shared_ptr<Ticket> ticket,
                      double amount,
                      Payment::PaymentMethod method);
    
    std::shared_ptr<Payment> getPayment(int id) const;
    QVector<std::shared_ptr<Payment>> getAllPayments() const;
    
    // Применение скидок
    void applyDiscountToTicket(int ticketId, std::shared_ptr<Discount> discount) const;
    
    // Статистика
    double totalRevenue() const;
    int totalTicketsSold() const;
    int totalBookings() const;
    
    // Итераторы для STL-совместимости
    using TicketIterator = QVector<std::shared_ptr<Ticket>>::iterator;
    using ConstTicketIterator = QVector<std::shared_ptr<Ticket>>::const_iterator;
    
    TicketIterator ticketsBegin();
    TicketIterator ticketsEnd();
    ConstTicketIterator ticketsBegin() const;
    ConstTicketIterator ticketsEnd() const;
    
private:
    QVector<std::shared_ptr<Ticket>> m_tickets;
    QVector<std::shared_ptr<Booking>> m_bookings;
    QVector<std::shared_ptr<Payment>> m_payments;
    int m_nextTicketId = 1;
    int m_nextBookingId = 1;
    int m_nextPaymentId = 1;
};


