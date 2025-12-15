#include "filedatabase.h"
#include "DatabaseException.h"
#include "ValidationException.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringConverter>
#include <QDir>
#include <QSet>
#include <stdexcept>
#include "route.h"
#include "trip.h"

FileDatabase::FileDatabase(const QString &folderPath, QObject *parent)
    : QObject(parent), m_folderPath(folderPath)
{
    m_autoSaveTimer = new QTimer(this);
    m_autoSaveTimer->setSingleShot(true);
    connect(m_autoSaveTimer, &QTimer::timeout, this, &FileDatabase::performAutoSave);

    try {
        m_companies = loadCompanies();
        if (m_companies.isEmpty()) {
            m_companies.append(Company("Default Bus Co."));
        }
    } catch (const DatabaseException& e) {
        qWarning() << "Failed to load companies, creating default:" << e.what();
        m_companies.append(Company("Default Bus Co."));
    }
}

QString FileDatabase::companiesFilePath() const {
    return m_folderPath + "/companies.txt";
}

QVector<Company> FileDatabase::loadCompanies() const {
    QVector<Company> list;
    QFile file(companiesFilePath());

    if (!file.exists()) {
        qWarning() << "Companies file does not exist, will be created on save";
        return list;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw DatabaseException(QString("Could not open companies file for reading: %1. Error: %2")
                                    .arg(file.fileName()).arg(file.errorString()));
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    Company currentCompany;
    std::shared_ptr<Route> currentRoute;
    int lineNumber = 0;

    try {
        while (!in.atEnd()) {
            lineNumber++;
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            if (line == "---") {
                processCompanySeparator(currentCompany, currentRoute, list);
                continue;
            }

            if (line.startsWith("Company: ")) {
                processCompanyLine(line, lineNumber, currentCompany, currentRoute, list);
                continue;
            }

            if (line.startsWith("Route: ")) {
                processRouteLine(line, lineNumber, currentCompany, currentRoute);
                continue;
            }

            if (line.startsWith("Stop: ")) {
                processStopLine(line, lineNumber, currentRoute);
                continue;
            }

            if (line.startsWith("Trip: ")) {
                processTripLine(line, lineNumber, currentRoute);
                continue;
            }

            // Неизвестный формат строки
            throw ValidationException(QString("Unknown line format at line %1: %2").arg(lineNumber).arg(line));
        }

        if (currentRoute)
            currentCompany.addRoute(currentRoute);
        if (!currentCompany.name().isEmpty())
            list.append(currentCompany);

    } catch (const std::exception& e) {
        file.close();
        // Перебрасываем исключение с дополнительной информацией
        throw DatabaseException(QString("Error parsing companies file at line %1: %2").arg(lineNumber).arg(e.what()));
    }

    file.close();
    qDebug() << "Successfully loaded" << list.size() << "companies";
    return list;
}

void FileDatabase::saveCompanies() {
    QFile file(companiesFilePath());

    // Создаем директорию если не существует
    if (QDir dir(m_folderPath); !dir.exists() && !dir.mkpath(".")) {
        throw DatabaseException(QString("Could not create data directory: %1").arg(m_folderPath));
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw DatabaseException(QString("Could not open companies file for writing: %1. Error: %2")
                                    .arg(file.fileName()).arg(file.errorString()));
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    try {
        // Валидация данных перед сохранением
        validateCompanies(m_companies);

        for (int i = 0; i < m_companies.size(); ++i) {
            const bool isLast = (i == m_companies.size() - 1);
            saveCompany(m_companies[i], out, isLast);
        }

        // Проверяем статус записи вместо flush()
        out.flush();
        if (out.status() != QTextStream::Ok) {
            throw DatabaseException("Failed to write data to file");
        }

    } catch (const std::exception& e) {
        file.close();
        // Удаляем частично записанный файл
        file.remove();
        throw DatabaseException(QString("Error writing companies file: %1").arg(e.what()));
    }

    file.close();
    qDebug() << "Data saved successfully to" << file.fileName();
}

void FileDatabase::validateCompany(const Company& company, QSet<QString>& companyNames) const {
    if (company.name().isEmpty()) {
        throw ValidationException("Found company with empty name");
    }

    if (companyNames.contains(company.name())) {
        throw ValidationException(QString("Duplicate company name: %1").arg(company.name()));
    }
    companyNames.insert(company.name());

    const auto &routes = company.routes();
    if (routes.isEmpty()) {
        qWarning() << "Company" << company.name() << "has no routes";
    }

    QList<QString> routeNames;
    for (const auto &route : routes) {
        validateRoute(route, company.name(), routeNames);
    }
}

void FileDatabase::validateCompanies(const QVector<Company> &companies) const {
    if (companies.isEmpty()) {
        throw ValidationException("No companies to save");
    }

    QSet<QString> companyNames;
    for (const auto &company : companies) {
        validateCompany(company, companyNames);
    }
}


void FileDatabase::validateRoute(const std::shared_ptr<Route>& route, const QString& companyName, QList<QString>& routeNames) const {
    if (route->name().isEmpty()) {
        throw ValidationException(QString("Empty route name in company: %1").arg(companyName));
    }

    routeNames.append(route->name());

    if (route->totalStops() == 0) {
        throw ValidationException(QString("Route '%1' has no stops").arg(route->name()));
    }

    for (const auto &trip : route->trips()) {
        validateTrip(trip, route->name());
    }
}

void FileDatabase::setCompanies(const QVector<Company> &companies) {
    try {
        validateCompanies(companies);
        m_companies = companies;
        scheduleAutoSave();
    } catch (const ValidationException& e) {
        throw ValidationException(QString("Invalid companies data: %1").arg(e.what()));
    }
}

void FileDatabase::scheduleAutoSave() {
    m_autoSaveTimer->start(2000);
}

void FileDatabase::setAutoSaveEnabled(bool enabled) {
    m_autoSaveEnabled = enabled;
    if (!enabled) {
        m_autoSaveTimer->stop();
    }
}

void FileDatabase::performAutoSave() {
    try {
        saveCompanies();
    } catch (const DatabaseException& e) {
        qCritical() << "Auto-save failed:" << e.what();

    } catch (const ValidationException& e) {
        qCritical() << "Auto-save validation failed:" << e.what();
    }
}

void FileDatabase::validateStop(const std::shared_ptr<Stop>& stop) const {
    if (!stop) {
        throw ValidationException("Stop is null");
    }
    if (stop->city.isEmpty()) {
        throw ValidationException("Stop city cannot be empty");
    }

    if (stop->durationMinutes < 0) {
        throw ValidationException(QString("Invalid duration for stop '%1': %2").arg(stop->city).arg(stop->durationMinutes));
    }
    if (stop->price < 0) {
        throw ValidationException(QString("Invalid price for stop '%1': %2").arg(stop->city).arg(stop->price));
    }
}

void FileDatabase::validateTrip(const std::shared_ptr<Trip>& trip, const QString& routeName) const {
    if (!trip) {
        throw ValidationException("Trip is null");
    }
    if (!trip->departure().isValid()) {
        if (routeName.isEmpty()) {
            throw ValidationException("Trip has invalid departure datetime");
        } else {
            throw ValidationException(QString("Invalid trip datetime in route: %1").arg(routeName));
        }
    }
}

void FileDatabase::processCompanySeparator(Company& currentCompany, std::shared_ptr<Route>& currentRoute, QVector<Company>& list) const {
    if (currentRoute) {
        currentCompany.addRoute(currentRoute);
    }
    if (!currentCompany.name().isEmpty()) {
        list.append(currentCompany);
    }
    currentCompany = Company();
    currentRoute = nullptr;
}

void FileDatabase::processCompanyLine(const QString& line, int lineNumber, Company& currentCompany, std::shared_ptr<Route>& currentRoute, QVector<Company>& list) const {
    if (currentRoute) {
        currentCompany.addRoute(currentRoute);
    }
    if (!currentCompany.name().isEmpty()) {
        list.append(currentCompany);
    }

    QString companyName = line.mid(QString("Company: ").length());
    if (companyName.isEmpty()) {
        throw ValidationException(QString("Empty company name at line %1").arg(lineNumber));
    }

    currentCompany = Company(companyName);
    currentRoute = nullptr;
}

void FileDatabase::processRouteLine(const QString& line, int lineNumber, Company& currentCompany, std::shared_ptr<Route>& currentRoute) const {
    if (currentRoute) {
        currentCompany.addRoute(currentRoute);
    }

    QString routeName = line.mid(QString("Route: ").length());
    if (routeName.isEmpty()) {
        throw ValidationException(QString("Empty route name at line %1").arg(lineNumber));
    }

    currentRoute = std::make_shared<Route>(routeName);
}

void FileDatabase::processStopLine(const QString& line, int lineNumber, const std::shared_ptr<Route>& currentRoute) const {
    if (!currentRoute) {
        throw ValidationException(QString("Stop without route at line %1").arg(lineNumber));
    }

    QStringList parts = line.mid(6).split(';');
    if (parts.size() < 3) {
        throw ValidationException(QString("Invalid stop format at line %1. Expected: Stop:city;duration;price").arg(lineNumber));
    }

    QString city = parts[0].trimmed();
    if (city.isEmpty()) {
        throw ValidationException(QString("Empty city name at line %1").arg(lineNumber));
    }

    bool durationOk;
    bool priceOk;
    int duration = parts[1].toInt(&durationOk);
    double price = parts[2].toDouble(&priceOk);


    if (!durationOk || duration < 0) {
        throw ValidationException(QString("Invalid duration at line %1: %2").arg(lineNumber).arg(parts[1]));
    }

    if (!priceOk || price < 0) {
        throw ValidationException(QString("Invalid price at line %1: %2").arg(lineNumber).arg(parts[2]));
    }

    currentRoute->addStop(city, duration, price);
}

void FileDatabase::processTripLine(const QString& line, int lineNumber, const std::shared_ptr<Route>& currentRoute) const {
    if (!currentRoute) {
        throw ValidationException(QString("Trip without route at line %1").arg(lineNumber));
    }

    QDateTime dep = QDateTime::fromString(line.mid(6), Qt::ISODate);
    if (!dep.isValid()) {
        throw ValidationException(QString("Invalid trip datetime at line %1: %2").arg(lineNumber).arg(line.mid(6)));
    }

    currentRoute->addTrip(dep);
}

void FileDatabase::saveCompany(const Company& company, QTextStream& out, bool isLast) const {
    if (company.name().isEmpty()) {
        throw ValidationException("Company name cannot be empty");
    }

    out << "Company: " << company.name() << "\r\n";

    for (const auto &route : company.routes()) {
        saveRoute(route, out);
    }

    if (!isLast) {
        out << "---\r\n";
    }
}

void FileDatabase::saveRoute(const std::shared_ptr<Route>& route, QTextStream& out) const {
    if (route->name().isEmpty()) {
        throw ValidationException("Route name cannot be empty");
    }

    if (route->totalStops() == 0) {
        throw ValidationException(QString("Route '%1' has no stops").arg(route->name()));
    }

    out << "Route: " << route->name() << "\r\n";


    for (auto stop = route->firstStop(); stop; stop = stop->next) {
        validateStop(stop);
        out << "Stop: " << stop->city << ";" << stop->durationMinutes << ";" << stop->price << "\r\n";
    }

    // Сохраняем рейсы
    for (const auto &trip : route->trips()) {
        validateTrip(trip, route->name());
        out << "Trip: " << trip->departure().toString(Qt::ISODate) << "\r\n";
    }
}
