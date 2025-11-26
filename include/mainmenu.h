#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTimer>
#include <QLineEdit>
#include <QComboBox>
#include "filedatabase.h"
#include "company.h"
#include "route.h"
#include "trip.h"

class MainMenu : public QMainWindow {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

private slots:
    void refreshTrips();
    void onTripDoubleClicked(int row, int /*column*/);
    void onManageRoutes() const;
    void onSearchTextChanged();
    void onCompanyFilterChanged(int index);

private:
    void setupUI();
    void loadData();
    void updateFilters();
    QDateTime getTripDeparture(const std::shared_ptr<Trip>& trip, const std::shared_ptr<Route>& route) const;
    bool tripMatchesFilter(const std::shared_ptr<Trip>& trip, const std::shared_ptr<Route>& route, const QString& companyName) const;

    FileDatabase *db;
    QVector<Company> companies;

    QTableWidget *tableTrips;
    QPushButton *btnManageRoutes;
    QLineEdit *searchEdit;
    QComboBox *companyFilter;
    QTimer *autoRefreshTimer;
};
