#pragma once
#include "company.h"
#include "route.h"
#include "trip.h"
#include <QVector>
#include <QString>
#include <QTimer>
#include <QObject>

class FileDatabase : public QObject {
    Q_OBJECT

public:
    explicit FileDatabase(const QString &folderPath, QObject *parent = nullptr);

    QVector<Company> loadCompanies();
    void saveCompanies();
    void setCompanies(const QVector<Company> &companies);

    void scheduleAutoSave();
    void setAutoSaveEnabled(bool enabled);

private slots:
    void performAutoSave();

private:
    QString companiesFilePath() const;
    void validateCompanies(const QVector<Company> &companies); // Добавляем объявление

    QString m_folderPath;
    QVector<Company> m_companies;
    QTimer *m_autoSaveTimer;
    bool m_autoSaveEnabled = true;
};
