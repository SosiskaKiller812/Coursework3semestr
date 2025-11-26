#pragma once
#include <QString>
#include <memory>
#include <compare>

class Passenger;

class Seat {
public:
    enum class SeatType {
        Window,
        Aisle,
        ExtraLegroom
    };
    
    explicit Seat(int number = 0, SeatType type = SeatType::Window, bool isAvailable = true);
    ~Seat() = default;
    
    int number() const;
    void setNumber(int number);
    
    SeatType type() const;
    void setType(SeatType type);
    
    bool isAvailable() const;
    void setAvailable(bool available);
    
    void reserve(std::shared_ptr<Passenger> passenger);
    void release();
    
    std::shared_ptr<Passenger> passenger() const;
    
    // Перегрузка операций
    bool operator==(const Seat& other) const;
    auto operator<=>(const Seat& other) const; // по номеру
    
    // Дружественная функция для вывода
    friend QString getSeatInfo(const Seat& seat);
    
private:
    int m_number;
    SeatType m_type;
    bool m_isAvailable;
    std::shared_ptr<Passenger> m_passenger;
};

// Дружественная функция
QString getSeatInfo(const Seat& seat);


