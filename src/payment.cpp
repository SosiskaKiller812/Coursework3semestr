#include "payment.h"
#include "ticket.h"

Payment::Payment(int id, std::shared_ptr<Ticket> ticket,
                 double amount, PaymentMethod method,
                 const QDateTime& paymentTime)
    : m_id(id), m_ticket(ticket), m_amount(amount), m_method(method),
      m_paymentTime(paymentTime), m_status(PaymentStatus::Pending) {
}

int Payment::id() const {
    return m_id;
}

void Payment::setId(int id) {
    m_id = id;
}

std::shared_ptr<Ticket> Payment::ticket() const {
    return m_ticket;
}

void Payment::setTicket(std::shared_ptr<Ticket> ticket) {
    m_ticket = ticket;
}

double Payment::amount() const {
    return m_amount;
}

void Payment::setAmount(double amount) {
    m_amount = amount;
}

Payment::PaymentMethod Payment::method() const {
    return m_method;
}

void Payment::setMethod(PaymentMethod method) {
    m_method = method;
}

QDateTime Payment::paymentTime() const {
    return m_paymentTime;
}

void Payment::setPaymentTime(const QDateTime& time) {
    m_paymentTime = time;
}

Payment::PaymentStatus Payment::status() const {
    return m_status;
}

void Payment::setStatus(PaymentStatus status) {
    m_status = status;
}

void Payment::process() {
    m_status = PaymentStatus::Completed;
    m_paymentTime = QDateTime::currentDateTime();
    if (m_ticket) {
        m_ticket->setStatus("paid");
    }
}

void Payment::refund() {
    m_status = PaymentStatus::Refunded;
    if (m_ticket) {
        m_ticket->setStatus("cancelled");
    }
}

QString Payment::transactionId() const {
    return m_transactionId;
}

void Payment::setTransactionId(const QString& id) {
    m_transactionId = id;
}

bool Payment::operator==(const Payment& other) const {
    return m_id == other.m_id;
}

bool Payment::operator!=(const Payment& other) const {
    return !(*this == other);
}

bool Payment::operator<(const Payment& other) const {
    return m_paymentTime < other.m_paymentTime;
}

QString getPaymentInfo(const Payment& payment) {
    QString methodStr = (payment.m_method == Payment::PaymentMethod::Cash) ? "Cash" :
                        (payment.m_method == Payment::PaymentMethod::Card) ? "Card" : "Online";
    QString statusStr = (payment.m_status == Payment::PaymentStatus::Pending) ? "Pending" :
                        (payment.m_status == Payment::PaymentStatus::Completed) ? "Completed" :
                        (payment.m_status == Payment::PaymentStatus::Failed) ? "Failed" : "Refunded";
    
    QString info = QString("Payment #%1\n").arg(payment.m_id);
    info += QString("Amount: %1\n").arg(payment.m_amount);
    info += QString("Method: %1\n").arg(methodStr);
    info += QString("Status: %1\n").arg(statusStr);
    info += QString("Time: %1").arg(payment.m_paymentTime.toString("dd.MM.yyyy HH:mm"));
    return info;
}


