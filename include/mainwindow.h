#pragma once
#include <QMainWindow>
#include "filedatabase.h"
#include "company.h"
#include "route.h"
#include "trip.h"

class QComboBox;
class QTableWidget;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCompanyChanged(int index);

    void onAddCompany();
    void onRemoveCompany();
    void onAddRoute();

    void onEditRoute(int row);
    void onCopyRoute(int row);
    void onRemoveRoute(int row);
    void onShowRouteDetails(int row);

    void onDataChanged();

private:
    void loadCompanies();
    void refreshCompanySelector();
    void refreshRoutesTable();

    FileDatabase *db;
    QVector<Company> companies;

    QComboBox *cbCompany;
    QTableWidget *tableRoutes;
    QPushButton *btnAddRoute;
    QPushButton *btnAddCompany;
    QPushButton *btnRemoveCompany;
};
