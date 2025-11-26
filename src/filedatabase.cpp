#include "filedatabase.h"
#include "DatabaseException.h"
#include "ValidationException.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringConverter>
#include <QDir>
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
                if (currentRoute)
                    currentCompany.addRoute(currentRoute);
                if (!currentCompany.name().isEmpty())
                    list.append(currentCompany);

                currentCompany = Company();
                currentRoute = nullptr;
                continue;
            }

            if (line.startsWith("Company: ")) {
                if (currentRoute)
                    currentCompany.addRoute(currentRoute);
                if (!currentCompany.name().isEmpty())
                    list.append(currentCompany);

                QString companyName = line.mid(QString("Company: ").length());
                if (companyName.isEmpty()) {
                    throw ValidationException(QString("Empty company name at line %1").arg(lineNumber));
                }

                currentCompany = Company(companyName);
                currentRoute = nullptr;
                continue;
            }

            if (line.startsWith("Route: ")) {
                if (currentRoute)
                    currentCompany.addRoute(currentRoute);

                QString routeName = line.mid(QString("Route: ").length());
                if (routeName.isEmpty()) {
                    throw ValidationException(QString("Empty route name at line %1").arg(lineNumber));
                }

                currentRoute = std::make_shared<Route>(routeName);
                continue;
            }

            if (line.startsWith("Stop: ")) {
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

                // Разрешаем 0 минут для остановок (например, конечные остановки)
                if (!durationOk || duration < 0) {
                    throw ValidationException(QString("Invalid duration at line %1: %2").arg(lineNumber).arg(parts[1]));
                }

                if (!priceOk || price < 0) {
                    throw ValidationException(QString("Invalid price at line %1: %2").arg(lineNumber).arg(parts[2]));
                }

                currentRoute->addStop(city, duration, price);
                continue;
            }

            if (line.startsWith("Trip: ")) {
                if (!currentRoute) {
                    throw ValidationException(QString("Trip without route at line %1").arg(lineNumber));
                }

                QDateTime dep = QDateTime::fromString(line.mid(6), Qt::ISODate);
                if (!dep.isValid()) {
                    throw ValidationException(QString("Invalid trip datetime at line %1: %2").arg(lineNumber).arg(line.mid(6)));
                }

                currentRoute->addTrip(dep);
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
            const Company &c = m_companies[i];

            if (c.name().isEmpty()) {
                throw ValidationException("Company name cannot be empty");
            }

            out << "Company: " << c.name() << "\r\n";

            for (const auto &r : c.routes()) {
                if (r->name().isEmpty()) {
                    throw ValidationException("Route name cannot be empty");
                }

                if (r->totalStops() == 0) {
                    throw ValidationException(QString("Route '%1' has no stops").arg(r->name()));
                }

                out << "Route: " << r->name() << "\r\n";

                // Сохраняем остановки
                for (auto stop = r->firstStop(); stop; stop = stop->next) {
                    if (stop->city.isEmpty()) {
                        throw ValidationException("Stop city cannot be empty");
                    }
                    // Разрешаем 0 минут для остановок
                    if (stop->durationMinutes < 0) {
                        throw ValidationException(QString("Invalid duration for stop '%1': %2").arg(stop->city).arg(stop->durationMinutes));
                    }
                    if (stop->price < 0) {
                        throw ValidationException(QString("Invalid price for stop '%1': %2").arg(stop->city).arg(stop->price));
                    }

                    out << "Stop: " << stop->city << ";" << stop->durationMinutes << ";" << stop->price << "\r\n";
                }

                // Сохраняем рейсы
                for (const auto &t : r->trips()) {
                    if (!t->departure().isValid()) {
                        throw ValidationException("Trip has invalid departure datetime");
                    }

                    out << "Trip: " << t->departure().toString(Qt::ISODate) << "\r\n";
                }
            }

            if (i != m_companies.size() - 1)
                out << "---\r\n";
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

void FileDatabase::validateCompanies(const QVector<Company> &companies) const {
    if (companies.isEmpty()) {
        throw ValidationException("No companies to save");
    }

    QSet<QString> companyNames;
    for (const auto &company : companies) {
        if (company.name().isEmpty()) {
            throw ValidationException("Found company with empty name");
        }

        if (companyNames.contains(company.name())) {
            throw ValidationException(QString("Duplicate company name: %1").arg(company.name()));
        }
        companyNames.insert(company.name());

        // Проверяем маршруты компании
        const auto &routes = company.routes();
        if (routes.isEmpty()) {
            qWarning() << "Company" << company.name() << "has no routes";
        }

        QSet<QString> routeNames;
        for (const auto &route : routes) {
            if (route->name().isEmpty()) {
                throw ValidationException(QString("Empty route name in company: %1").arg(company.name()));
            }

            if (routeNames.contains(route->name())) {
                throw ValidationException(QString("Duplicate route name '%1' in company: %2").arg(route->name()).arg(company.name()));
            }
            routeNames.insert(route->name());

            // Проверяем остановки маршрута
            if (route->totalStops() == 0) {
                throw ValidationException(QString("Route '%1' has no stops").arg(route->name()));
            }

            // Проверяем рейсы
            for (const auto &trip : route->trips()) {
                if (!trip->departure().isValid()) {
                    throw ValidationException(QString("Invalid trip datetime in route: %1").arg(route->name()));
                }
            }
        }
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
    m_autoSaveTimer->start(2000); // Сохраняем через 2 секунды после изменения
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
        // Можно показать сообщение пользователю или записать в лог
    } catch (const ValidationException& e) {
        qCritical() << "Auto-save validation failed:" << e.what();
    } catch (const std::runtime_error& e) {
        qCritical() << "Unexpected runtime error during auto-save:" << e.what();
    }
}
