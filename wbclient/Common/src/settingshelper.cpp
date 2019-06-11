#include "settingshelper.h"
#include <QSettings>
#include <QDebug>

SettingsHelper::SettingsHelper(QObject *parent) :
    QObject(parent),
    mHost("")
{

}

QString SettingsHelper::host() const
{
    return mHost;
}

void SettingsHelper::loadSettings(QString appPath)
{
    QString path = appPath + "/config.ini";
    QSettings *config = new QSettings(path, QSettings::IniFormat);
    mHost = config->value("/url/URL_HOST", "").toString();
    emit hostChanged();
    delete config;
}
