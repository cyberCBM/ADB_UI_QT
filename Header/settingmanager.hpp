#pragma once

#include <QString>
#include <QVariant>
#include <QSettings>

/// Singleton! Create only one instance!
class SettingManager
{
public:
    SettingManager();
    ~SettingManager();

    static bool contains(const QString &key);
    static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    static void setValue(const QString &key, const QVariant &value);

private:
    static SettingManager*  s_instance;
    QSettings               m_settings;
};
