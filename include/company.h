#pragma once
#include <QVector>
#include <QString>
#include <memory>

class Route;

class Company {
public:
    explicit Company(const QString &name = "");
    ~Company();
    Company(const Company& other);
    Company& operator=(const Company& other);

    QString name() const;
    void setName(const QString &name);

    void addRoute(std::shared_ptr<Route> route);
    QVector<std::shared_ptr<Route>>& routes();
    const QVector<std::shared_ptr<Route>>& routes() const;

private:
    QString m_name;
    QVector<std::shared_ptr<Route>> m_routes;
};
