#include "reportgenerator.h"
#include "company.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <fstream>

ReportGenerator::ReportGenerator() = default;


bool ReportGenerator::exportToFile(const QString& filename, const QString& content) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << content;
    
    return true;
}

bool ReportGenerator::exportToCSV(const QString& filename, const QVector<QStringList>& data) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    for (const auto& row : data) {
        out << row.join(",") << "\n";
    }
    
    return true;
}

bool ReportGenerator::saveReportToStream(std::ofstream& stream, const QString& content) const {
    if (!stream.is_open()) {
        return false;
    }
    
    stream << content.toStdString();
    return true;
}

QString ReportGenerator::loadReportFromStream(std::ifstream& stream) const {
    if (!stream.is_open()) {
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(stream)),
                        std::istreambuf_iterator<char>());
    return QString::fromStdString(content);
}

QString ReportGenerator::formatCurrency(double amount) const {
    return QString("%1 руб.").arg(amount, 0, 'f', 2);
}

QString ReportGenerator::formatDate(const QDate& date) const {
    return date.toString("dd.MM.yyyy");
}

QString ReportGenerator::formatDateTime(const QDateTime& dateTime) const {
    return dateTime.toString("dd.MM.yyyy HH:mm");
}

