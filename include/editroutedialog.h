#pragma once
#include <QDialog>
#include <QListWidget>
#include <QTableWidget>

class QLineEdit;
class QTableWidget;
class QPushButton;
class Route;

class EditRouteDialog : public QDialog{
    Q_OBJECT
public:
    explicit EditRouteDialog(Route &route, QWidget *parent = nullptr);

private slots:
    void onAddStop();
    void onEditStop(int row);
    void onRemoveStop(int row);
    void onMoveStopUp(int row);
    void onMoveStopDown(int row);
    void onAddTrip();
    void onEditTrip(int row);
    void onCopyTrip(int row);
    void onRemoveTrip(int row);
    void updateStopsTable();
    void updateTripsTable();

private:
    Route &m_route;
    QLineEdit *leRouteName;
    QTableWidget *tableStops;
    QTableWidget *tableTrips;
    QPushButton *btnAddStop;
    QPushButton *btnAddTrip;
};
