#include "driver.h"
#include "bus.h"

Driver::Driver(const QString& name, const QString& licenseNumber, 
               const QDate& licenseExpiry, int experienceYears)
    : m_name(name), m_licenseNumber(licenseNumber), 
      m_licenseExpiry(licenseExpiry), m_experienceYears(experienceYears) {
}

QString Driver::name() const {
    return m_name;
}

void Driver::setName(const QString& name) {
    m_name = name;
}

QString Driver::licenseNumber() const {
    return m_licenseNumber;
}

void Driver::setLicenseNumber(const QString& number) {
    m_licenseNumber = number;
}

QDate Driver::licenseExpiry() const {
    return m_licenseExpiry;
}

void Driver::setLicenseExpiry(const QDate& date) {
    m_licenseExpiry = date;
}

int Driver::experienceYears() const {
    return m_experienceYears;
}

void Driver::setExperienceYears(int years) {
    m_experienceYears = years;
}

bool Driver::isLicenseValid() const {
    return m_licenseExpiry.isValid() && m_licenseExpiry >= QDate::currentDate();
}

void Driver::assignBus(std::shared_ptr<Bus> bus) {
    if (bus && !m_assignedBuses.contains(bus)) {
        m_assignedBuses.append(bus);
        // Водитель будет установлен через setDriver снаружи
    }
}

QVector<std::shared_ptr<Bus>> Driver::assignedBuses() const {
    return m_assignedBuses;
}

QString Driver::serialize() const {
    return QString("%1|%2|%3|%4")
        .arg(m_name, m_licenseNumber)
        .arg(m_licenseExpiry.toString(Qt::ISODate))
        .arg(m_experienceYears);
}

void Driver::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() >= 4) {
        m_name = parts[0];
        m_licenseNumber = parts[1];
        m_licenseExpiry = QDate::fromString(parts[2], Qt::ISODate);
        m_experienceYears = parts[3].toInt();
    }
}

bool Driver::operator==(const Driver& other) const {
    return m_licenseNumber == other.m_licenseNumber;
}

bool Driver::operator!=(const Driver& other) const {
    return !(*this == other);
}

bool Driver::operator<(const Driver& other) const {
    return m_experienceYears < other.m_experienceYears;
}

QString getDriverInfo(const Driver& driver) {
    QString info = QString("Driver: %1\n").arg(driver.m_name);
    info += QString("License: %1\n").arg(driver.m_licenseNumber);
    info += QString("Experience: %1 years\n").arg(driver.m_experienceYears);
    info += QString("License valid: %1").arg(driver.isLicenseValid() ? "Yes" : "No");
    return info;
}

