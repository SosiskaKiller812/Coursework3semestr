#pragma once
#include "company.h"
#include "route.h"
#include "trip.h"
#include "stop.h"
#include <QVector>
#include <QString>
#include <QTimer>
#include <QObject>
#include <memory>

class FileDatabase : public QObject {
    Q_OBJECT

public:
    explicit FileDatabase(const QString &folderPath, QObject *parent = nullptr);

    QVector<Company> loadCompanies() const;
    void saveCompanies();
    void setCompanies(const QVector<Company> &companies);

    void scheduleAutoSave();
    void setAutoSaveEnabled(bool enabled);

private slots:
    void performAutoSave();

private:
    QString companiesFilePath() const;
    void validateCompany(const Company& company, QSet<QString>& companyNames) const;
    void validateCompanies(const QVector<Company> &companies) const;
    void validateStop(const std::shared_ptr<Stop>& stop) const;
    void validateTrip(const std::shared_ptr<Trip>& trip, const QString& routeName) const;
    void validateRoute(const std::shared_ptr<Route>& route, const QString& companyName, QSet<QString>& routeNames) const;

    // Вспомогательные функции для loadCompanies
    void processCompanySeparator(Company& currentCompany, std::shared_ptr<Route>& currentRoute, QVector<Company>& list) const;
    void processCompanyLine(const QString& line, int lineNumber, Company& currentCompany, std::shared_ptr<Route>& currentRoute, QVector<Company>& list) const;
    void processRouteLine(const QString& line, int lineNumber, Company& currentCompany, std::shared_ptr<Route>& currentRoute) const;
    void processStopLine(const QString& line, int lineNumber, const std::shared_ptr<Route>& currentRoute) const;
    void processTripLine(const QString& line, int lineNumber, const std::shared_ptr<Route>& currentRoute) const;
    
    // Вспомогательные функции для saveCompanies
    void saveCompany(const Company& company, QTextStream& out, bool isLast) const;
    void saveRoute(const std::shared_ptr<Route>& route, QTextStream& out) const;

    QString m_folderPath;
    QVector<Company> m_companies;
    QTimer *m_autoSaveTimer;
    bool m_autoSaveEnabled = true;
};
