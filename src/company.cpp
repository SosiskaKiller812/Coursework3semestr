#include "company.h"
#include "route.h"

Company::Company(const QString &name) : m_name(name) {}


Company::Company(const Company& other) : m_name(other.m_name) {
    for (const auto& route : other.m_routes) {
        m_routes.append(std::make_shared<Route>(*route));
    }
}

Company& Company::operator=(const Company& other) {
    if (this != &other) {
        m_name = other.m_name;
        m_routes.clear();
        for (const auto& route : other.m_routes) {
            m_routes.append(std::make_shared<Route>(*route));
        }
    }
    return *this;
}

QString Company::name() const { return m_name; }
void Company::setName(const QString &name) { m_name = name; }

void Company::addRoute(std::shared_ptr<Route> route) {
    m_routes.append(route);
}

QVector<std::shared_ptr<Route>>& Company::routes() { return m_routes; }
const QVector<std::shared_ptr<Route>>& Company::routes() const { return m_routes; }
