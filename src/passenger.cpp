#include "passenger.h"
#include <QStringList>

Passenger::Passenger(const QString& name, const QString& passport, const QDate& birthDate)
    : m_name(name), m_passport(passport), m_birthDate(birthDate) {
}

QString Passenger::name() const {
    return m_name;
}

void Passenger::setName(const QString& name) {
    m_name = name;
}

QString Passenger::passport() const {
    return m_passport;
}

void Passenger::setPassport(const QString& passport) {
    m_passport = passport;
}

QDate Passenger::birthDate() const {
    return m_birthDate;
}

void Passenger::setBirthDate(const QDate& date) {
    m_birthDate = date;
}

int Passenger::age() const {
    if (!m_birthDate.isValid()) {
        return 0;
    }
    QDate today = QDate::currentDate();
    int age = today.year() - m_birthDate.year();
    if (today.month() < m_birthDate.month() || 
        (today.month() == m_birthDate.month() && today.day() < m_birthDate.day())) {
        age--;
    }
    return age;
}

QString Passenger::serialize() const {
    return QString("%1|%2|%3").arg(m_name, m_passport, m_birthDate.toString(Qt::ISODate));
}

void Passenger::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() >= 3) {
        m_name = parts[0];
        m_passport = parts[1];
        m_birthDate = QDate::fromString(parts[2], Qt::ISODate);
    }
}

bool Passenger::operator==(const Passenger& other) const {
    return m_passport == other.m_passport;
}


