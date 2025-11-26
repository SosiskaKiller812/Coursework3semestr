#include "configmanager.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <fstream>

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() = default;

ConfigManager::~ConfigManager() = default;

bool ConfigManager::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    m_config.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        
        int eqPos = line.indexOf("=");
        if (eqPos > 0) {
            QString key = line.left(eqPos).trimmed();
            QString value = line.mid(eqPos + 1).trimmed();
            m_config[key] = unescapeValue(value);
        }
    }
    
    return true;
}

bool ConfigManager::saveToFile(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    
    for (auto it = m_config.begin(); it != m_config.end(); ++it) {
        out << it.key() << "=" << escapeValue(it.value().toString()) << "\n";
    }
    
    return true;
}

bool ConfigManager::loadFromStream(std::ifstream& stream) {
    if (!stream.is_open()) {
        return false;
    }
    
    m_config.clear();
    
    std::string line;
    while (std::getline(stream, line)) {
        QString qline = QString::fromStdString(line).trimmed();
        if (qline.isEmpty() || qline.startsWith("#")) {
            continue;
        }
        
        int eqPos = qline.indexOf("=");
        if (eqPos > 0) {
            QString key = qline.left(eqPos).trimmed();
            QString value = qline.mid(eqPos + 1).trimmed();
            m_config[key] = unescapeValue(value);
        }
    }
    
    return true;
}

bool ConfigManager::saveToStream(std::ofstream& stream) const {
    if (!stream.is_open()) {
        return false;
    }
    
    for (auto it = m_config.begin(); it != m_config.end(); ++it) {
        stream << it.key().toStdString() << "=" 
               << escapeValue(it.value().toString()).toStdString() << "\n";
    }
    
    return true;
}

void ConfigManager::setValue(const QString& key, const QVariant& value) {
    m_config[key] = value;
}

QVariant ConfigManager::getValue(const QString& key, const QVariant& defaultValue) const {
    return m_config.value(key, defaultValue);
}

QString ConfigManager::getString(const QString& key, const QString& defaultValue) const {
    return m_config.value(key, defaultValue).toString();
}

int ConfigManager::getInt(const QString& key, int defaultValue) const {
    return m_config.value(key, defaultValue).toInt();
}

double ConfigManager::getDouble(const QString& key, double defaultValue) const {
    return m_config.value(key, defaultValue).toDouble();
}

bool ConfigManager::getBool(const QString& key, bool defaultValue) const {
    return m_config.value(key, defaultValue).toBool();
}

void ConfigManager::removeValue(const QString& key) {
    m_config.remove(key);
}

void ConfigManager::clear() {
    m_config.clear();
}

QStringList ConfigManager::getAllKeys() const {
    return m_config.keys();
}

bool ConfigManager::contains(const QString& key) const {
    return m_config.contains(key);
}

QVariant ConfigManager::operator[](const QString& key) const {
    return getValue(key);
}

ConfigManager& ConfigManager::operator+=(const std::pair<QString, QVariant>& entry) {
    setValue(entry.first, entry.second);
    return *this;
}

QString ConfigManager::escapeValue(const QString& value) const {
    QString escaped = value;
    escaped.replace(R"(\)", R"(\\)");
    escaped.replace("=", R"(\=)");
    escaped.replace("\n", R"(\n)");
    return escaped;
}

QString ConfigManager::unescapeValue(const QString& value) const {
    QString unescaped = value;
    unescaped.replace(R"(\n)", "\n");
    unescaped.replace(R"(\=)", "=");
    unescaped.replace(R"(\\)", R"(\)");
    return unescaped;
}

