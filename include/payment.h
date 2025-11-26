#pragma once
#include <QString>
#include <QDateTime>
#include <memory>
#include <compare>

class Ticket;

class Payment {
public:
    enum class PaymentMethod {
        Cash,
        Card,
        Online
    };
    
    enum class PaymentStatus {
        Pending,
        Completed,
        Failed,
        Refunded
    };
    
    explicit Payment(int id = 0, std::shared_ptr<Ticket> ticket = nullptr,
                     double amount = 0.0, PaymentMethod method = PaymentMethod::Cash,
                     const QDateTime& paymentTime = QDateTime());
    ~Payment() = default;
    
    int id() const;
    void setId(int id);
    
    std::shared_ptr<Ticket> ticket() const;
    void setTicket(std::shared_ptr<Ticket> ticket);
    
    double amount() const;
    void setAmount(double amount);
    
    PaymentMethod method() const;
    void setMethod(PaymentMethod method);
    
    QDateTime paymentTime() const;
    void setPaymentTime(const QDateTime& time);
    
    PaymentStatus status() const;
    void setStatus(PaymentStatus status);
    
    void process();
    void refund();
    
    QString transactionId() const;
    void setTransactionId(const QString& id);
    
    // Перегрузка операций
    bool operator==(const Payment& other) const;
    auto operator<=>(const Payment& other) const; // по времени платежа
    
    // Дружественная функция для вывода
    friend QString getPaymentInfo(const Payment& payment);
    
private:
    int m_id;
    std::shared_ptr<Ticket> m_ticket;
    double m_amount;
    PaymentMethod m_method;
    QDateTime m_paymentTime;
    PaymentStatus m_status;
    QString m_transactionId;
};

// Дружественная функция
QString getPaymentInfo(const Payment& payment);


