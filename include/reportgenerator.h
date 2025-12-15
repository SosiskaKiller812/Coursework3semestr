#pragma once
#include "route.h"
#include "company.h"
#include <QString>
#include <QDate>
#include <QVector>
#include <memory>
#include <fstream>


class ReportGenerator {
public:
    ReportGenerator();
    ~ReportGenerator() = default;
    
    
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


