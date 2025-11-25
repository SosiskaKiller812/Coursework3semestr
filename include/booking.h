#pragma once
#include <QString>
#include <QDateTime>
#include <QDate>
#include <memory>

class Passenger;
class Route;
class Trip;
class Seat;

class Booking {
public:
    enum class BookingStatus {
        Pending,
        Confirmed,
        Cancelled,
        Expired
    };
    
    Booking(int id = 0, std::shared_ptr<Passenger> passenger = nullptr,
            std::shared_ptr<Route> route = nullptr,
            std::shared_ptr<Trip> trip = nullptr,
            std::shared_ptr<Seat> seat = nullptr,
            const QDateTime& bookingTime = QDateTime());
    ~Booking() = default;
    
    int id() const;
    void setId(int id);
    
    std::shared_ptr<Passenger> passenger() const;
    void setPassenger(std::shared_ptr<Passenger> passenger);
    
    std::shared_ptr<Route> route() const;
    void setRoute(std::shared_ptr<Route> route);
    
    std::shared_ptr<Trip> trip() const;
    void setTrip(std::shared_ptr<Trip> trip);
    
    std::shared_ptr<Seat> seat() const;
    void setSeat(std::shared_ptr<Seat> seat);
    
    QDateTime bookingTime() const;
    void setBookingTime(const QDateTime& time);
    
    BookingStatus status() const;
    void setStatus(BookingStatus status);
    
    bool isExpired(int expirationMinutes = 30) const;
    void confirm();
    void cancel();
    
    // Перегрузка операций
    bool operator==(const Booking& other) const;
    bool operator!=(const Booking& other) const;
    bool operator<(const Booking& other) const; // по времени бронирования
    
    // Дружественная функция для вывода
    friend QString getBookingInfo(const Booking& booking);
    
private:
    int m_id;
    std::shared_ptr<Passenger> m_passenger;
    std::shared_ptr<Route> m_route;
    std::shared_ptr<Trip> m_trip;
    std::shared_ptr<Seat> m_seat;
    QDateTime m_bookingTime;
    BookingStatus m_status;
};

// Дружественная функция
QString getBookingInfo(const Booking& booking);


