#pragma once
#include "ticket.h"
#include "route.h"
#include "trip.h"
#include <QString>
#include <QDateTime>
#include <memory>

class ValidationException;

class TicketValidator {
public:
    TicketValidator();
    ~TicketValidator() = default;
    
    // Валидация билета
    bool validateTicket(std::shared_ptr<Ticket> ticket) const;
    
    // Проверка действительности билета
    bool isTicketValid(std::shared_ptr<Ticket> ticket, const QDateTime& currentTime = QDateTime::currentDateTime()) const;
    
    // Проверка соответствия билета маршруту
    bool validateTicketRoute(std::shared_ptr<Ticket> ticket, std::shared_ptr<Route> route) const;
    
    // Проверка соответствия билета рейсу
    bool validateTicketTrip(std::shared_ptr<Ticket> ticket, std::shared_ptr<Trip> trip) const;
    
    // Проверка статуса билета
    bool validateTicketStatus(std::shared_ptr<Ticket> ticket) const;
    
    // Полная валидация с исключениями
    void validateTicketThrow(std::shared_ptr<Ticket> ticket) const;
    
    // Валидация пассажира
    bool validatePassenger(std::shared_ptr<Passenger> passenger) const;
    
    // Валидация цены
    bool validatePrice(double price) const;
    
    // Получить список ошибок валидации
    QVector<QString> getValidationErrors(std::shared_ptr<Ticket> ticket) const;
    
private:
    static constexpr double MIN_PRICE = 0.01;
    static constexpr double MAX_PRICE = 100000.0;
    
    bool isValidPassengerName(const QString& name) const;
    bool isValidPassport(const QString& passport) const;
};


