#include "mainmenu.h"
#include "mainwindow.h"
#include "routedetailsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QGroupBox>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
{
    db = new FileDatabase("data", this);
    setupUI();
    loadData();
    updateFilters();
    refreshTrips();

    // Автообновление всегда включено
    autoRefreshTimer = new QTimer(this);
    connect(autoRefreshTimer, &QTimer::timeout, this, &MainMenu::refreshTrips);
    autoRefreshTimer->start(30000); // Обновление каждые 30 секунд

    setWindowTitle("Автовокзал - Главное меню");
    resize(1400, 700);
}

void MainMenu::setupUI()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    // Создаем кнопку управления
    btnManageRoutes = new QPushButton("Управление маршрутами", this);
    connect(btnManageRoutes, &QPushButton::clicked, this, &MainMenu::onManageRoutes);

    // Создаем элементы поиска и фильтрации
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(""); // Убираем текст
    searchEdit->setClearButtonEnabled(true);

    companyFilter = new QComboBox(this);
    companyFilter->addItem("Все компании", "");

    connect(searchEdit, &QLineEdit::textChanged, this, &MainMenu::onSearchTextChanged);
    connect(companyFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainMenu::onCompanyFilterChanged);

    // Создаем таблицу для отображения рейсов
    tableTrips = new QTableWidget(this);
    tableTrips->setColumnCount(6);
    tableTrips->setHorizontalHeaderLabels({
        "Отправление", "Прибытие", "Маршрут", "Компания",
        "Время в пути", "Стоимость"
    });
    tableTrips->horizontalHeader()->setStretchLastSection(true);
    tableTrips->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableTrips->setSelectionMode(QAbstractItemView::SingleSelection);
    tableTrips->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableTrips->setSortingEnabled(true);

    connect(tableTrips, &QTableWidget::cellDoubleClicked,
            this, &MainMenu::onTripDoubleClicked);

    // Создаем layout для кнопки (самый верх)
    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(btnManageRoutes);
    buttonsLayout->addStretch();

    // Создаем layout для фильтров (под кнопками)
    auto *filtersLayout = new QHBoxLayout;
    filtersLayout->addWidget(new QLabel("Поиск:"));
    filtersLayout->addWidget(searchEdit, 2);
    filtersLayout->addWidget(new QLabel("Компания:"));
    filtersLayout->addWidget(companyFilter, 1);

    // Основной layout
    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(buttonsLayout);  // Кнопка в самом верху
    mainLayout->addLayout(filtersLayout);  // Поиск под кнопкой
    mainLayout->addWidget(tableTrips);     // Таблица внизу
}

void MainMenu::loadData()
{
    companies = db->loadCompanies();
    if (companies.isEmpty()) {
        companies.append(Company("Default Bus Co."));
    }
}

void MainMenu::updateFilters()
{
    // Обновляем фильтр компаний
    QString currentCompany = companyFilter->currentData().toString();
    companyFilter->clear();
    companyFilter->addItem("Все компании", "");

    QSet<QString> uniqueCompanies;
    for (const auto &company : companies) {
        uniqueCompanies.insert(company.name());
    }

    for (const QString &companyName : uniqueCompanies) {
        companyFilter->addItem(companyName, companyName);
    }

    // Устанавливаем "Все компании" по умолчанию
    companyFilter->setCurrentIndex(0);
}

void MainMenu::refreshTrips()
{
    // Собираем все рейсы из всех компаний
    QVector<QPair<std::shared_ptr<Trip>, std::shared_ptr<Route>>> allTrips;

    for (const auto &company : companies) {
        for (const auto &route : company.routes()) {
            for (const auto &trip : route->trips()) {
                allTrips.append(qMakePair(trip, route));
            }
        }
    }

    // Применяем фильтры
    QString searchText = searchEdit->text().toLower();
    QString selectedCompany = companyFilter->currentData().toString();

    QVector<QPair<std::shared_ptr<Trip>, std::shared_ptr<Route>>> filteredTrips;

    for (const auto &tripPair : allTrips) {
        const auto &trip = tripPair.first;
        const auto &route = tripPair.second;

        // Находим компанию для этого маршрута
        QString companyName;
        for (const auto &company : companies) {
            if (company.routes().contains(route)) {
                companyName = company.name();
                break;
            }
        }

        if (tripMatchesFilter(trip, route, companyName)) {
            filteredTrips.append(tripPair);
        }
    }

    // Сортируем по времени отправления (ближайшие сначала)
    std::ranges::sort(filteredTrips,
              [this](const auto &a, const auto &b) {
                  return getTripDeparture(a.first, a.second) <
                         getTripDeparture(b.first, b.second);
              });

    // Обновляем таблицу
    tableTrips->setSortingEnabled(false);
    tableTrips->setRowCount(filteredTrips.size());

    for (qsizetype i = 0; i < filteredTrips.size(); ++i) {
        const auto &trip = filteredTrips[i].first;
        const auto &route = filteredTrips[i].second;

        // Находим компанию для этого маршрута
        QString companyName;
        for (const auto &company : companies) {
            if (company.routes().contains(route)) {
                companyName = company.name();
                break;
            }
        }

        QDateTime departure = getTripDeparture(trip, route);
        QDateTime arrival = trip->arrival(*route);

        tableTrips->setItem(i, 0, new QTableWidgetItem(departure.toString("dd.MM.yyyy HH:mm")));
        tableTrips->setItem(i, 1, new QTableWidgetItem(arrival.toString("dd.MM.yyyy HH:mm")));
        tableTrips->setItem(i, 2, new QTableWidgetItem(route->name()));
        tableTrips->setItem(i, 3, new QTableWidgetItem(companyName));
        tableTrips->setItem(i, 4, new QTableWidgetItem(QString::number(route->totalDuration()) + " мин"));
        tableTrips->setItem(i, 5, new QTableWidgetItem(QString::number(route->totalPrice(), 'f', 2) + " руб"));
    }

    tableTrips->setSortingEnabled(true);
    tableTrips->resizeColumnsToContents();
}

bool MainMenu::tripMatchesFilter(const std::shared_ptr<Trip>&, const std::shared_ptr<Route>& route, const QString& companyName) const
{
    QString searchText = searchEdit->text().toLower();
    QString selectedCompany = companyFilter->currentData().toString();

    // Фильтр по компании
    if (!selectedCompany.isEmpty() && companyName != selectedCompany) {
        return false;
    }

    // Поиск по тексту
    if (!searchText.isEmpty()) {
        bool textFound = false;

        // Поиск в названии маршрута
        if (route->name().toLower().contains(searchText)) {
            textFound = true;
        }

        // Поиск в названии компании
        if (companyName.toLower().contains(searchText)) {
            textFound = true;
        }

        // Поиск в городах остановок
        for (auto stop = route->firstStop(); stop; stop = stop->next) {
            if (stop->city.toLower().contains(searchText)) {
                textFound = true;
                break;
            }
        }

        if (!textFound) {
            return false;
        }
    }

    return true;
}

QDateTime MainMenu::getTripDeparture(const std::shared_ptr<Trip>& trip, const std::shared_ptr<Route>&) const
{
    return trip->departure();
}

void MainMenu::onTripDoubleClicked(int row, int /*column*/)
{

    // Находим маршрут для выбранного рейса
    QString routeName = tableTrips->item(row, 2)->text();
    QString companyName = tableTrips->item(row, 3)->text();

    for (const auto &company : companies) {
        if (company.name() == companyName) {
            for (const auto &route : company.routes()) {
                if (route->name() == routeName) {
                    RouteDetailsDialog dlg(*route, this);
                    dlg.exec();
                    return;
                }
            }
            break;
        }
    }
}

void MainMenu::onManageRoutes()
{
    auto *manageWindow = new MainWindow();
    manageWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(manageWindow, &MainWindow::destroyed, this, &MainMenu::refreshTrips);
    manageWindow->show();
}

void MainMenu::onSearchTextChanged()
{
    refreshTrips();
}

void MainMenu::onCompanyFilterChanged(int)
{
    refreshTrips();
}
