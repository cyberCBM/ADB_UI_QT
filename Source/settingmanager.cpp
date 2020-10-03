#include "settingmanager.hpp"


SettingManager *SettingManager::s_instance = nullptr;

SettingManager::SettingManager()
{
    Q_ASSERT(s_instance == nullptr);
    s_instance = this;
}

SettingManager::~SettingManager()
{
    Q_ASSERT(s_instance != nullptr);
    s_instance = nullptr;
}

bool SettingManager::contains(const QString &key)
{
    return s_instance->m_settings.contains(key);
}

QVariant SettingManager::value(const QString &key, const QVariant &defaultValue)
{
    return s_instance->m_settings.value(key, defaultValue);
}

void SettingManager::setValue(const QString &key, const QVariant &value)
{
    s_instance->m_settings.setValue(key, value);
}
