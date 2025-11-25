#include "editroutedialog.h"
#include "route.h"
#include "trip.h"
#include "addstopdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTimeEdit>

EditRouteDialog::EditRouteDialog(Route &route, QWidget *parent)
    : QDialog(parent), m_route(route){

    leRouteName = new QLineEdit(route.name(), this);

    // Создаем таблицу для остановок вместо списка
    tableStops = new QTableWidget(this);
    tableStops->setColumnCount(4);
    tableStops->setHorizontalHeaderLabels({"Остановка", "Длительность", "Цена", "Действия"});
    tableStops->horizontalHeader()->setStretchLastSection(false); // Отключаем растягивание последней колонки
    tableStops->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableStops->setSelectionMode(QAbstractItemView::SingleSelection);
    tableStops->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Создаем таблицу для рейсов
    tableTrips = new QTableWidget(this);
    tableTrips->setColumnCount(3);
    tableTrips->setHorizontalHeaderLabels({"Отправление", "Прибытие", "Действия"});
    tableTrips->horizontalHeader()->setStretchLastSection(false); // Отключаем растягивание последней колонки
    tableTrips->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTrips->setSelectionMode(QAbstractItemView::SingleSelection);
    tableTrips->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Кнопка добавления остановки над таблицей
    btnAddStop = new QPushButton("➕ Добавить остановку", this);

    // Кнопка добавления рейса над таблицей
    btnAddTrip = new QPushButton("➕ Добавить рейс", this);

    connect(btnAddStop, &QPushButton::clicked, this, &EditRouteDialog::onAddStop);
    connect(btnAddTrip, &QPushButton::clicked, this, &EditRouteDialog::onAddTrip);

    auto *mainLayout = new QVBoxLayout(this);

    auto *nameLayout = new QFormLayout;
    nameLayout->addRow("Название маршрута:", leRouteName);
    mainLayout->addLayout(nameLayout);

    auto *contentLayout = new QHBoxLayout;

    auto *stopsLayout = new QVBoxLayout;
    stopsLayout->addWidget(new QLabel("Остановки:"));
    stopsLayout->addWidget(btnAddStop);
    stopsLayout->addWidget(tableStops);

    auto *tripsLayout = new QVBoxLayout;
    tripsLayout->addWidget(new QLabel("Рейсы:"));
    tripsLayout->addWidget(btnAddTrip);
    tripsLayout->addWidget(tableTrips);

    contentLayout->addLayout(stopsLayout, 1);
    contentLayout->addLayout(tripsLayout, 1);
    mainLayout->addLayout(contentLayout);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    updateStopsTable();
    updateTripsTable();

    setWindowTitle("Редактирование маршрута");
    setMinimumSize(1000, 600);
}

void EditRouteDialog::updateStopsTable(){
    tableStops->setRowCount(0);
    auto stops = m_route.getAllStops();
    tableStops->setRowCount(stops.size());

    for(int i = 0; i < stops.size(); ++i){
        const auto &stop = stops[i];

        tableStops->setItem(i, 0, new QTableWidgetItem(stop->city));
        tableStops->setItem(i, 1, new QTableWidgetItem(QString::number(stop->durationMinutes) + " мин"));
        tableStops->setItem(i, 2, new QTableWidgetItem(QString::number(stop->price, 'f', 2) + " руб"));

        // Создаем контейнер для кнопок остановки
        QWidget *buttonsWidget = new QWidget(this);
        QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
        buttonsLayout->setContentsMargins(0, 0, 0, 0); // Убираем все отступы
        buttonsLayout->setSpacing(0); // Убираем расстояние между кнопками

        // Кнопка редактировать остановку
        QPushButton *btnEdit = new QPushButton("✏️", this);
        btnEdit->setFixedSize(25, 25);
        btnEdit->setToolTip("Редактировать остановку");
        connect(btnEdit, &QPushButton::clicked, this, [this, i]() {
            onEditStop(i);
        });

        // Кнопка удалить остановку
        QPushButton *btnRemove = new QPushButton("❌", this);
        btnRemove->setFixedSize(25, 25);
        btnRemove->setToolTip("Удалить остановку");
        connect(btnRemove, &QPushButton::clicked, this, [this, i]() {
            onRemoveStop(i);
        });

        // Кнопка вверх
        QPushButton *btnUp = new QPushButton("⬆️", this);
        btnUp->setFixedSize(25, 25);
        btnUp->setToolTip("Переместить вверх");
        connect(btnUp, &QPushButton::clicked, this, [this, i]() {
            onMoveStopUp(i);
        });

        // Кнопка вниз
        QPushButton *btnDown = new QPushButton("⬇️", this);
        btnDown->setFixedSize(25, 25);
        btnDown->setToolTip("Переместить вниз");
        connect(btnDown, &QPushButton::clicked, this, [this, i]() {
            onMoveStopDown(i);
        });

        buttonsLayout->addWidget(btnEdit);
        buttonsLayout->addWidget(btnRemove);
        buttonsLayout->addWidget(btnUp);
        buttonsLayout->addWidget(btnDown);

        tableStops->setCellWidget(i, 3, buttonsWidget);
    }

    tableStops->resizeColumnsToContents();
    // Устанавливаем фиксированную ширину для колонки действий
    tableStops->setColumnWidth(3, 4 * 25); // 4 кнопки по 25 пикселей
}

void EditRouteDialog::updateTripsTable(){
    tableTrips->setRowCount(0);
    const auto &trips = m_route.trips();
    tableTrips->setRowCount(trips.size());

    for(int i = 0; i < trips.size(); ++i){
        const auto &trip = trips[i];
        tableTrips->setItem(i, 0, new QTableWidgetItem(trip->departure().toString("dd.MM.yyyy HH:mm")));
        tableTrips->setItem(i, 1, new QTableWidgetItem(trip->arrival(m_route).toString("dd.MM.yyyy HH:mm")));

        // Создаем контейнер для кнопок рейса
        QWidget *buttonsWidget = new QWidget(this);
        QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
        buttonsLayout->setContentsMargins(0, 0, 0, 0); // Убираем все отступы
        buttonsLayout->setSpacing(0); // Убираем расстояние между кнопками

        // Кнопка редактировать рейс
        QPushButton *btnEdit = new QPushButton("✏️", this);
        btnEdit->setFixedSize(25, 25);
        btnEdit->setToolTip("Редактировать рейс");
        connect(btnEdit, &QPushButton::clicked, this, [this, i]() {
            onEditTrip(i);
        });

        // Кнопка копировать рейс
        QPushButton *btnCopy = new QPushButton("📋", this);
        btnCopy->setFixedSize(25, 25);
        btnCopy->setToolTip("Копировать рейс");
        connect(btnCopy, &QPushButton::clicked, this, [this, i]() {
            onCopyTrip(i);
        });

        // Кнопка удалить рейс
        QPushButton *btnRemove = new QPushButton("❌", this);
        btnRemove->setFixedSize(25, 25);
        btnRemove->setToolTip("Удалить рейс");
        connect(btnRemove, &QPushButton::clicked, this, [this, i]() {
            onRemoveTrip(i);
        });

        buttonsLayout->addWidget(btnEdit);
        buttonsLayout->addWidget(btnCopy);
        buttonsLayout->addWidget(btnRemove);

        tableTrips->setCellWidget(i, 2, buttonsWidget);
    }

    tableTrips->resizeColumnsToContents();
    // Устанавливаем фиксированную ширину для колонки действий
    tableTrips->setColumnWidth(2, 3 * 25); // 3 кнопки по 25 пикселей
}

void EditRouteDialog::onAddStop(){
    AddStopDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted){
        m_route.addStop(dlg.cityName(), dlg.duration(), dlg.price());
        updateStopsTable();
    }
}

void EditRouteDialog::onEditStop(int row){
    if(row < 0) return;

    auto stop = m_route.getStop(row);
    if(!stop) return;

    AddStopDialog dlg(this);
    dlg.setWindowTitle("Редактировать остановку");

    if(dlg.exec() == QDialog::Accepted){
        m_route.removeStop(row);
        m_route.insertStop(row, dlg.cityName(), dlg.duration(), dlg.price());
        updateStopsTable();
    }
}

void EditRouteDialog::onRemoveStop(int row){
    if(row < 0) return;

    if(QMessageBox::question(this, "Подтверждение",
                              "Удалить выбранную остановку?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
        m_route.removeStop(row);
        updateStopsTable();
    }
}

void EditRouteDialog::onMoveStopUp(int row){
    if(row <= 0) return;

    auto stop = m_route.getStop(row);
    if(!stop) return;

    m_route.removeStop(row);
    m_route.insertStop(row - 1, stop->city, stop->durationMinutes, stop->price);
    updateStopsTable();
    tableStops->setCurrentCell(row - 1, 0);
}

void EditRouteDialog::onMoveStopDown(int row){
    if(row < 0 || row >= m_route.totalStops() - 1) return;

    auto stop = m_route.getStop(row);
    if(!stop) return;

    m_route.removeStop(row);
    m_route.insertStop(row + 1, stop->city, stop->durationMinutes, stop->price);
    updateStopsTable();
    tableStops->setCurrentCell(row + 1, 0);
}

void EditRouteDialog::onAddTrip(){
    QDateTimeEdit *dtEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dtEdit->setDisplayFormat("dd.MM.yyyy HH:mm");
    dtEdit->setCalendarPopup(true);

    QDialog dlg(this);
    dlg.setWindowTitle("Добавить рейс");
    auto *layout = new QVBoxLayout(&dlg);
    layout->addWidget(new QLabel("Дата и время отправления:"));
    layout->addWidget(dtEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(buttons);

    if(dlg.exec() == QDialog::Accepted){
        m_route.addTrip(dtEdit->dateTime());
        updateTripsTable();
    }
}

void EditRouteDialog::onEditTrip(int row){
    if(row < 0) return;

    auto trip = m_route.trips()[row];

    QDateTimeEdit *dtEdit = new QDateTimeEdit(trip->departure(), this);
    dtEdit->setDisplayFormat("dd.MM.yyyy HH:mm");
    dtEdit->setCalendarPopup(true);

    QDialog dlg(this);
    dlg.setWindowTitle("Редактировать рейс");
    auto *layout = new QVBoxLayout(&dlg);
    layout->addWidget(new QLabel("Дата и время отправления:"));
    layout->addWidget(dtEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(buttons);

    if(dlg.exec() == QDialog::Accepted){
        auto newTrip = std::make_shared<Trip>(dtEdit->dateTime());
        m_route.trips()[row] = newTrip;
        updateTripsTable();
    }
}

void EditRouteDialog::onCopyTrip(int row){
    if(row < 0) return;

    auto originalTrip = m_route.trips()[row];
    m_route.addTrip(originalTrip->departure());
    updateTripsTable();
}

void EditRouteDialog::onRemoveTrip(int row){
    if(row < 0) return;

    if(QMessageBox::question(this, "Подтверждение",
                              "Удалить выбранный рейс?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
        m_route.trips().remove(row);
        updateTripsTable();
    }
}
