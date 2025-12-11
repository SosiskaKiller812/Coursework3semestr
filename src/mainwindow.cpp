#include <QDateTimeEdit>
#include "mainwindow.h"
#include "DatabaseException.h"
#include "ValidationException.h"
#include "RouteException.h"
#include "routedetailsdialog.h"
#include "editroutedialog.h"
#include "addstopdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    db = new FileDatabase("data", this);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    cbCompany = new QComboBox(this);
    connect(cbCompany, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCompanyChanged);

    // Создаем таблицу для маршрутов
    tableRoutes = new QTableWidget(this);
    tableRoutes->setColumnCount(5);
    tableRoutes->setHorizontalHeaderLabels({"Маршрут", "Остановки", "Рейсы", "Время (мин)", "Действия"});
    tableRoutes->horizontalHeader()->setStretchLastSection(false);
    tableRoutes->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableRoutes->setSelectionMode(QAbstractItemView::SingleSelection);
    tableRoutes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Кнопка добавления маршрута над таблицей маршрутов
    btnAddRoute = new QPushButton("➕ Добавить маршрут", this);
    connect(btnAddRoute, &QPushButton::clicked, this, &MainWindow::onAddRoute);

    // Кнопка добавления компании
    btnAddCompany = new QPushButton("➕ Добавить компанию", this);
    connect(btnAddCompany, &QPushButton::clicked, this, &MainWindow::onAddCompany);

    // Кнопка удаления компании
    btnRemoveCompany = new QPushButton("❌ Удалить компанию", this);
    connect(btnRemoveCompany, &QPushButton::clicked, this, &MainWindow::onRemoveCompany);

    auto *topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel("Компания:"));
    topLayout->addWidget(cbCompany, 1);
    topLayout->addWidget(btnAddCompany);
    topLayout->addWidget(btnRemoveCompany);
    topLayout->addStretch();

    // Основной layout только с маршрутами
    auto *mainContentLayout = new QVBoxLayout;

    // Группа маршрутов
    auto *routesGroup = new QGroupBox("Маршруты компании", this);
    auto *routesLayout = new QVBoxLayout(routesGroup);
    routesLayout->addWidget(btnAddRoute);
    routesLayout->addWidget(tableRoutes);

    mainContentLayout->addWidget(routesGroup);

    auto *layout = new QVBoxLayout(central);
    layout->addLayout(topLayout);
    layout->addLayout(mainContentLayout);

    loadCompanies();
    refreshCompanySelector();
    refreshRoutesTable();

    setWindowTitle("Управление маршрутами");
    resize(1000, 700);
}

void MainWindow::onEditRoute(int row) {
    try {
        int idxC = cbCompany->currentIndex();
        if (idxC < 0 || idxC >= companies.size()) return;
        if (row < 0 || row >= companies[idxC].routes().size()) return;

        auto route = companies[idxC].routes()[row];
        EditRouteDialog dlg(*route, this);

        if (dlg.exec() == QDialog::Accepted) {
            onDataChanged();
        }
    } catch (const RouteException& e) {
        QMessageBox::critical(this, "Ошибка маршрута", e.what());
    } catch (const DatabaseException& e) {
        QMessageBox::critical(this, "Ошибка базы данных", e.what());
    } catch (const ValidationException& e) {
        QMessageBox::critical(this, "Ошибка валидации", e.what());
    }
}

void MainWindow::onCopyRoute(int row) {
    int idxC = cbCompany->currentIndex();
    if (idxC < 0 || idxC >= companies.size()) return;
    if (row < 0 || row >= companies[idxC].routes().size()) return;

    auto originalRoute = companies[idxC].routes()[row];
    auto copiedRoute = std::make_shared<Route>(*originalRoute);

    companies[idxC].addRoute(copiedRoute);
    onDataChanged();
}

void MainWindow::onRemoveRoute(int row) {
    int idxC = cbCompany->currentIndex();
    if (idxC < 0 || idxC >= companies.size()) return;
    if (row < 0 || row >= companies[idxC].routes().size()) return;

    if (QMessageBox::question(this, "Подтверждение",
                              "Удалить выбранный маршрут?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        companies[idxC].routes().remove(row);
        onDataChanged();
    }
}

void MainWindow::onShowRouteDetails(int row) {
    int idxC = cbCompany->currentIndex();
    if (idxC < 0 || idxC >= companies.size()) return;
    if (row < 0 || row >= companies[idxC].routes().size()) return;

    const auto &route = companies[idxC].routes()[row];
    RouteDetailsDialog dlg(*route, this);
    dlg.exec();
}

void MainWindow::loadCompanies() {
    try {
        companies = db->loadCompanies();
        if (companies.isEmpty()) {
            companies.append(Company("Default Bus Co."));
        }
    } catch (const DatabaseException& e) {
        QMessageBox::warning(this, "Ошибка загрузки",
                             QString("Не удалось загрузить данные: %1\nСоздана пустая компания.").arg(e.what()));
        companies.append(Company("Default Bus Co."));
    }
}

void MainWindow::onDataChanged() {
    try {
        db->setCompanies(companies);
        refreshRoutesTable();
    } catch (const DatabaseException& e) {
        QMessageBox::critical(this, "Ошибка сохранения", e.what());
    } catch (const ValidationException& e) {
        QMessageBox::warning(this, "Ошибка валидации", e.what());
    }
}

void MainWindow::refreshCompanySelector() {
    cbCompany->clear();
    for (const auto &c : companies) cbCompany->addItem(c.name());
}

void MainWindow::refreshRoutesTable() {
    tableRoutes->setRowCount(0);
    int idx = cbCompany->currentIndex();
    if (idx < 0 || idx >= companies.size()) return;

    const auto &routes = companies[idx].routes();
    tableRoutes->setRowCount(routes.size());

    for (int i = 0; i < routes.size(); ++i) {
        const auto &r = routes[i];

        tableRoutes->setItem(i, 0, new QTableWidgetItem(r->name()));
        tableRoutes->setItem(i, 1, new QTableWidgetItem(QString::number(r->totalStops())));
        tableRoutes->setItem(i, 2, new QTableWidgetItem(QString::number(r->trips().size())));
        tableRoutes->setItem(i, 3, new QTableWidgetItem(QString::number(r->totalDuration())));

        // Создаем контейнер для кнопок
        auto *buttonsWidget = new QWidget(this);
        auto *buttonsLayout = new QHBoxLayout(buttonsWidget);
        buttonsLayout->setContentsMargins(0, 0, 0, 0);
        buttonsLayout->setSpacing(0);

        // Кнопка редактировать
        auto *btnEdit = new QPushButton("✏️", this);
        btnEdit->setFixedSize(25, 25);
        btnEdit->setToolTip("Редактировать маршрут");
        connect(btnEdit, &QPushButton::clicked, this, [this, i]() {
            onEditRoute(i);
        });

        // Кнопка копировать
        auto *btnCopy = new QPushButton("📋", this);
        btnCopy->setFixedSize(25, 25);
        btnCopy->setToolTip("Копировать маршрут");
        connect(btnCopy, &QPushButton::clicked, this, [this, i]() {
            onCopyRoute(i);
        });

        // Кнопка удалить
        auto *btnRemove = new QPushButton("❌", this);
        btnRemove->setFixedSize(25, 25);
        btnRemove->setToolTip("Удалить маршрут");
        connect(btnRemove, &QPushButton::clicked, this, [this, i]() {
            onRemoveRoute(i);
        });

        // Кнопка детали
        auto *btnDetails = new QPushButton("👁️", this);
        btnDetails->setFixedSize(25, 25);
        btnDetails->setToolTip("Показать детали маршрута");
        connect(btnDetails, &QPushButton::clicked, this, [this, i]() {
            onShowRouteDetails(i);
        });

        buttonsLayout->addWidget(btnEdit);
        buttonsLayout->addWidget(btnCopy);
        buttonsLayout->addWidget(btnRemove);
        buttonsLayout->addWidget(btnDetails);

        tableRoutes->setCellWidget(i, 4, buttonsWidget);
    }

    tableRoutes->resizeColumnsToContents();
    tableRoutes->setColumnWidth(4, 4 * 25);
}

void MainWindow::onCompanyChanged(int) {
    refreshRoutesTable();
}

void MainWindow::onAddCompany() {
    bool ok = false;
    QString name = QInputDialog::getText(this, "Новая компания", "Название:", QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) return;
    companies.append(Company(name.trimmed()));
    onDataChanged();
    refreshCompanySelector();
    cbCompany->setCurrentIndex(companies.size() - 1);
    refreshRoutesTable();
}

void MainWindow::onRemoveCompany() {
    int idx = cbCompany->currentIndex();
    if (idx < 0 || idx >= companies.size()) return;

    if (QMessageBox::question(this, "Подтверждение",
                              "Удалить выбранную компанию?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        companies.remove(idx);
        onDataChanged();
        refreshCompanySelector();
    }
}

void MainWindow::onAddRoute() {
    int idxC = cbCompany->currentIndex();
    if (idxC < 0 || idxC >= companies.size()) return;

    bool ok = false;
    QString name = QInputDialog::getText(this, "Новый маршрут", "Название маршрута:", QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) return;

    auto route = std::make_shared<Route>(name.trimmed());
    route->addStop("Город А", 60, 100.0);
    route->addStop("Город Б", 45, 150.0);

    companies[idxC].addRoute(route);
    onDataChanged();
    refreshRoutesTable();
}
