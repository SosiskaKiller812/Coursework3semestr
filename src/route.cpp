#include "route.h"
#include "RouteException.h"
#include "trip.h"

Route::Route(const QString &name) : m_name(name) {}

Route::~Route() = default;


Route::Route(const Route& other)
    : m_name(other.m_name) {

    for (auto stop = other.m_head; stop; stop = stop->next) {
        addStop(stop->city, stop->durationMinutes, stop->price);
    }

    for (const auto& trip : other.m_trips) {
        addTrip(trip->departure());
    }
}

Route& Route::operator=(const Route& other) {
    if (this != &other) {
        m_name = other.m_name;
        m_head = nullptr;
        m_tail = nullptr;
        m_trips.clear();

        for (auto stop = other.m_head; stop; stop = stop->next) {
            addStop(stop->city, stop->durationMinutes, stop->price);
        }

        for (const auto& trip : other.m_trips) {
            addTrip(trip->departure());
        }
    }
    return *this;
}

QString Route::name() const { return m_name; }

void Route::addStop(const QString &city, int durationMinutes, double price) {
    auto stop = std::make_shared<Stop>(city, durationMinutes, price);
    if (!m_head) {
        m_head = stop;
        m_tail = stop;
    } else {
        m_tail->next = stop;
        m_tail = stop;
    }
}

QString Route::info() const {
    QString text = "Маршрут: " + m_name + "\n";
    text += "Общая длительность: " + QString::number(totalDuration()) + " мин\n";
    text += "Общая цена: " + QString::number(totalPrice()) + " руб\n";

    text += "\nОстановки:\n";
    int stopNumber = 1;
    for (auto s = m_head; s != nullptr;) {
        text += QString("%1. %2 - %3 мин, %4 руб\n")
                    .arg(stopNumber)
                    .arg(s->city)
                    .arg(s->durationMinutes)
                    .arg(s->price);
        ++stopNumber;
        s = s->next;
    }

    return text;
}

std::shared_ptr<Stop> Route::firstStop() const {
    return m_head;
}

int Route::totalDuration() const {
    int sum = 0;
    for (auto s = m_head; s; s = s->next) sum += s->durationMinutes;
    return sum;
}

double Route::totalPrice() const {
    double sum = 0;
    for (auto s = m_head; s; s = s->next) sum += s->price;
    return sum;
}

int Route::totalStops() const {
    int count = 0;
    for (auto s = m_head; s; s = s->next) count++;
    return count;
}

QString Route::detailedInfo() const {
    QString text;

    text += "═══════════════════════════════════════\n";
    text += "МАРШРУТ: " + m_name.toUpper() + "\n";
    text += "═══════════════════════════════════════\n\n";

    // Информация о маршруте
    text += "ОБЩАЯ ИНФОРМАЦИЯ:\n";
    text += "   • Общее время в пути: " + QString::number(totalDuration()) + " мин\n";
    text += "   • Общая стоимость: " + QString::number(totalPrice()) + " руб\n";
    text += "   • Количество остановок: " + QString::number(totalStops()) + "\n\n";

    text += "МАРШРУТ СЛЕДОВАНИЯ:\n";
    int stopNumber = 1;
    int accumulatedTime = 0;
    double accumulatedPrice = 0;

    for (auto s = m_head; s != nullptr; s = s->next) {
        text += QString("   %1. %2\n")
                    .arg(stopNumber, 2)
                    .arg(s->city);
        text += QString("      +%1 мин (%2 мин)\n")
                    .arg(s->durationMinutes)
                    .arg(accumulatedTime + s->durationMinutes);
        text += QString("      +%1 руб (%2 руб)\n")
                    .arg(s->price)
                    .arg(accumulatedPrice + s->price);
        ++stopNumber;

        if (s->next) {
            text += "      ↓\n";
        }

        accumulatedTime += s->durationMinutes;
        accumulatedPrice += s->price;
    }

    text += "\nЗАПЛАНИРОВАННЫЕ РЕЙСЫ:\n";
    if (m_trips.empty()) {
        text += "   • Рейсов нет\n";
    } else {
        for (int i = 0; i < m_trips.size(); ++i) {
            const auto& trip = m_trips[i];
            text += QString("   %1. %2 → %3\n")
                        .arg(i + 1)
                        .arg(trip->departure().toString("dd.MM.yyyy HH:mm"))
                        .arg(trip->arrival(*this).toString("dd.MM.yyyy HH:mm"));
        }
    }

    text += "\n═══════════════════════════════════════\n";
    return text;
}

void Route::addTrip(const QDateTime &departure) {
    m_trips.append(std::make_shared<Trip>(departure));
}

QVector<std::shared_ptr<Trip>>& Route::trips() {
    return m_trips;
}

const QVector<std::shared_ptr<Trip>>& Route::trips() const {
    return m_trips;
}

void Route::insertStop(int position, const QString &city, int durationMinutes, double price) {
    if (position < 0 || position > totalStops()) {
        throw RouteException(QString("Invalid position for insert: %1. Total stops: %2")
                                 .arg(position).arg(totalStops()));
    }

    auto newStop = std::make_shared<Stop>(city, durationMinutes, price);

    if (position == 0) {
        newStop->next = m_head;
        m_head = newStop;
        if (!m_tail) m_tail = newStop;
    } else if (position == totalStops()) {
        m_tail->next = newStop;
        m_tail = newStop;
    } else {
        auto current = m_head;
        for (int i = 0; i < position - 1; ++i) {
            if (!current) throw RouteException("Invalid linked list state during insert");
            current = current->next;
        }
        newStop->next = current->next;
        current->next = newStop;
    }
}

void Route::removeStop(int position) {
    if (position < 0 || position >= totalStops()) {
        throw RouteException(QString("Invalid position for remove: %1. Total stops: %2")
                                 .arg(position).arg(totalStops()));
    }

    if (position == 0) {
        m_head = m_head->next;
        if (!m_head) m_tail = nullptr;
    } else {
        auto current = m_head;
        for (int i = 0; i < position - 1; ++i) {
            if (!current) throw RouteException("Invalid linked list state during remove");
            current = current->next;
        }
        if (!current->next) throw RouteException("Invalid linked list state during remove");
        current->next = current->next->next;
        if (!current->next) m_tail = current;
    }
}

std::shared_ptr<Stop> Route::getStop(int position) const {
    if (position < 0 || position >= totalStops()) {
        return nullptr;
    }

    auto current = m_head;
    int i = 0;
    while (i < position && current) {
        current = current->next;
        ++i;
    }
    return current;
}

QVector<std::shared_ptr<Stop>> Route::getAllStops() const {
    QVector<std::shared_ptr<Stop>> stops;
    for (auto current = m_head; current; current = current->next) {
        stops.append(current);
    }
    return stops;
}
