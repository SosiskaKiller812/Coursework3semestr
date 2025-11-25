#pragma once
#include "ticket.h"
#include "route.h"
#include "company.h"
#include <QString>
#include <QDate>
#include <QVector>
#include <memory>
#include <fstream>

class TicketOffice;

class ReportGenerator {
public:
    ReportGenerator();
    ~ReportGenerator() = default;
    
    // Генерация отчетов
    QString generateDailyReport(const QDate& date, const TicketOffice& office, 
                                const QVector<Company>& companies) const;
    
    QString generateRouteReport(std::shared_ptr<Route> route, 
                                const TicketOffice& office) const;
    
    QString generateCompanyReport(const Company& company, 
                                  const TicketOffice& office) const;
    
    QString generateFinancialReport(const QDate& startDate, const QDate& endDate,
                                    const TicketOffice& office) const;
    
    QString generatePassengerReport(std::shared_ptr<Passenger> passenger,
                                    const TicketOffice& office) const;
    
    // Экспорт отчетов в файл
    bool exportToFile(const QString& filename, const QString& content) const;
    bool exportToCSV(const QString& filename, const QVector<QStringList>& data) const;
    
    // Работа с файлами через потоки
    bool saveReportToStream(std::ofstream& stream, const QString& content) const;
    QString loadReportFromStream(std::ifstream& stream) const;
    
private:
    QString formatCurrency(double amount) const;
    QString formatDate(const QDate& date) const;
    QString formatDateTime(const QDateTime& dateTime) const;
};


