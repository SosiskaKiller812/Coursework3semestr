#pragma once
#include "route.h"
#include <QVector>
#include <QString>
#include <memory>
#include <algorithm>
#include <functional>

// Шаблонный класс для управления коллекциями маршрутов
template<typename T>
class RouteManager {
public:
    RouteManager() = default;
    ~RouteManager() = default;
    
    void addRoute(std::shared_ptr<T> route);
    void removeRoute(std::shared_ptr<T> route);
    void removeRoute(const QString& name);
    
    std::shared_ptr<T> findRoute(const QString& name) const;
    QVector<std::shared_ptr<T>> findRoutes(const std::function<bool(std::shared_ptr<T>)>& predicate) const;
    
    QVector<std::shared_ptr<T>> getAllRoutes() const;
    void clear();
    
    int count() const;
    bool isEmpty() const;
    
    // Итераторы
    using Iterator = typename QVector<std::shared_ptr<T>>::iterator;
    using ConstIterator = typename QVector<std::shared_ptr<T>>::const_iterator;
    
    Iterator begin() { return m_routes.begin(); }
    Iterator end() { return m_routes.end(); }
    ConstIterator begin() const { return m_routes.begin(); }
    ConstIterator end() const { return m_routes.end(); }
    ConstIterator cbegin() const { return m_routes.cbegin(); }
    ConstIterator cend() const { return m_routes.cend(); }
    
    // Перегрузка операций
    std::shared_ptr<T> operator[](int index) const;
    std::shared_ptr<T> operator[](const QString& name) const;
    
private:
    QVector<std::shared_ptr<T>> m_routes;
};

// Реализация шаблонного класса
template<typename T>
void RouteManager<T>::addRoute(std::shared_ptr<T> route) {
    if (route) {
        m_routes.append(route);
    }
}

template<typename T>
void RouteManager<T>::removeRoute(std::shared_ptr<T> route) {
    m_routes.removeAll(route);
}

template<typename T>
void RouteManager<T>::removeRoute(const QString& name) {
    m_routes.erase(std::remove_if(m_routes.begin(), m_routes.end(),
        [&name](std::shared_ptr<T> route) {
            return route && route->name() == name;
        }), m_routes.end());
}

template<typename T>
std::shared_ptr<T> RouteManager<T>::findRoute(const QString& name) const {
    auto it = std::find_if(m_routes.begin(), m_routes.end(),
        [&name](std::shared_ptr<T> route) {
            return route && route->name() == name;
        });
    return (it != m_routes.end()) ? *it : nullptr;
}

template<typename T>
QVector<std::shared_ptr<T>> RouteManager<T>::findRoutes(const std::function<bool(std::shared_ptr<T>)>& predicate) const {
    QVector<std::shared_ptr<T>> result;
    std::copy_if(m_routes.begin(), m_routes.end(), std::back_inserter(result), predicate);
    return result;
}

template<typename T>
QVector<std::shared_ptr<T>> RouteManager<T>::getAllRoutes() const {
    return m_routes;
}

template<typename T>
void RouteManager<T>::clear() {
    m_routes.clear();
}

template<typename T>
int RouteManager<T>::count() const {
    return m_routes.size();
}

template<typename T>
bool RouteManager<T>::isEmpty() const {
    return m_routes.isEmpty();
}

template<typename T>
std::shared_ptr<T> RouteManager<T>::operator[](int index) const {
    if (index >= 0 && index < m_routes.size()) {
        return m_routes[index];
    }
    return nullptr;
}

template<typename T>
std::shared_ptr<T> RouteManager<T>::operator[](const QString& name) const {
    return findRoute(name);
}


