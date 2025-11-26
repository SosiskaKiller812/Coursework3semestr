#pragma once
#include <QString>
#include <QHash>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <fstream>

class ConfigManager {
public:
    inline static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }
    
    // Загрузка и сохранение конфигурации
    bool loadFromFile(const QString& filename);
    bool saveToFile(const QString& filename) const;
    
    // Работа с файлами через потоки
    bool loadFromStream(std::ifstream& stream);
    bool saveToStream(std::ofstream& stream) const;
    
    // Установка и получение значений
    void setValue(const QString& key, const QVariant& value);
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    
    // Типизированные геттеры
    QString getString(const QString& key, const QString& defaultValue = "") const;
    int getInt(const QString& key, int defaultValue = 0) const;
    double getDouble(const QString& key, double defaultValue = 0.0) const;
    bool getBool(const QString& key, bool defaultValue = false) const;
    
    // Удаление значения
    void removeValue(const QString& key);
    
    // Очистка конфигурации
    void clear();
    
    // Получение всех ключей
    QStringList getAllKeys() const;
    
    // Проверка наличия ключа
    bool contains(const QString& key) const;
    
    // Перегрузка операций
    QVariant operator[](const QString& key) const;
    ConfigManager& operator+=(const std::pair<QString, QVariant>& entry);
    
private:
    ConfigManager();
    ~ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    QHash<QString, QVariant> m_config;
    
    QString escapeValue(const QString& value) const;
    QString unescapeValue(const QString& value) const;
};


