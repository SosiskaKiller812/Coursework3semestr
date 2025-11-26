#include "reportgenerator.h"
#include "ticketoffice.h"
#include "company.h"
#include "ticket.h"
#include "payment.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <fstream>

ReportGenerator::ReportGenerator() = default;

QString ReportGenerator::generateDailyReport(const QDate& date, const TicketOffice& office, 
                                              const QVector<Company>&) const {
    QString report;
    report += QString("═══════════════════════════════════════\n");
    report += QString("DAILY REPORT - %1\n").arg(formatDate(date));
    report += QString("═══════════════════════════════════════\n\n");
    
    auto tickets = office.getAllTickets();
    int ticketsCount = 0;
    double revenue = 0.0;
    
    for (const auto& ticket : tickets) {
        if (ticket && ticket->trip() && ticket->trip()->departure().date() == date) {
            ticketsCount++;
            if (ticket->status() == "paid") {
                revenue += ticket->price();
            }
        }
    }
    
    report += QString("Tickets sold: %1\n").arg(ticketsCount);
    report += QString("Revenue: %1\n").arg(formatCurrency(revenue));
    report += QString("\n═══════════════════════════════════════\n");
    
    return report;
}

QString ReportGenerator::generateRouteReport(std::shared_ptr<Route> route, 
                                              const TicketOffice& office) const {
    if (!route) {
        return "Invalid route";
    }
    
    QString report;
    report += QString("═══════════════════════════════════════\n");
    report += QString("ROUTE REPORT: %1\n").arg(route->name());
    report += QString("═══════════════════════════════════════\n\n");
    
    auto tickets = office.getTicketsByRoute(route);
    report += QString("Total tickets: %1\n").arg(tickets.size());
    report += QString("Route duration: %1 minutes\n").arg(route->totalDuration());
    report += QString("Route price: %1\n").arg(formatCurrency(route->totalPrice()));
    
    report += QString("\n═══════════════════════════════════════\n");
    return report;
}

QString ReportGenerator::generateCompanyReport(const Company& company, 
                                                  const TicketOffice& office) const {
    QString report;
    report += QString("═══════════════════════════════════════\n");
    report += QString("COMPANY REPORT: %1\n").arg(company.name());
    report += QString("═══════════════════════════════════════\n\n");
    
    int routeCount = company.routes().size();
    report += QString("Routes: %1\n").arg(routeCount);
    
    double totalRevenue = 0.0;
    for (const auto& route : company.routes()) {
        auto tickets = office.getTicketsByRoute(route);
        for (const auto& ticket : tickets) {
            if (ticket && ticket->status() == "paid") {
                totalRevenue += ticket->price();
            }
        }
    }
    
    report += QString("Total revenue: %1\n").arg(formatCurrency(totalRevenue));
    report += QString("\n═══════════════════════════════════════\n");
    
    return report;
}

QString ReportGenerator::generateFinancialReport(const QDate& startDate, const QDate& endDate,
                                                  const TicketOffice& office) const {
    QString report;
    report += QString("═══════════════════════════════════════\n");
    report += QString("FINANCIAL REPORT\n");
    report += QString("Period: %1 - %2\n").arg(formatDate(startDate), formatDate(endDate));
    report += QString("═══════════════════════════════════════\n\n");
    
    double totalRevenue = office.totalRevenue();
    int ticketsSold = office.totalTicketsSold();
    
    report += QString("Total revenue: %1\n").arg(formatCurrency(totalRevenue));
    report += QString("Tickets sold: %1\n").arg(ticketsSold);
    report += QString("Average ticket price: %1\n")
        .arg(formatCurrency(ticketsSold > 0 ? totalRevenue / ticketsSold : 0.0));
    
    report += QString("\n═══════════════════════════════════════\n");
    return report;
}

QString ReportGenerator::generatePassengerReport(std::shared_ptr<Passenger> passenger,
                                                  const TicketOffice& office) const {
    if (!passenger) {
        return "Invalid passenger";
    }
    
    QString report;
    report += QString("═══════════════════════════════════════\n");
    report += QString("PASSENGER REPORT: %1\n").arg(passenger->name());
    report += QString("═══════════════════════════════════════\n\n");
    
    auto tickets = office.getTicketsByPassenger(passenger);
    report += QString("Total tickets: %1\n").arg(tickets.size());
    
    double totalSpent = 0.0;
    for (const auto& ticket : tickets) {
        if (ticket && ticket->status() == "paid") {
            totalSpent += ticket->price();
        }
    }
    
    report += QString("Total spent: %1\n").arg(formatCurrency(totalSpent));
    report += QString("\n═══════════════════════════════════════\n");
    
    return report;
}

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

