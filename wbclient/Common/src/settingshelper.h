#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QObject>

class SettingsHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host NOTIFY hostChanged)
public:
    explicit SettingsHelper(QObject *parent = nullptr);

    // 加载配置
    Q_INVOKABLE void loadSettings(QString appPath);

    // 获取配置数据
    QString host() const;
signals:
    void hostChanged();

private:
    QString mHost;
};

#endif // SETTINGSHELPER_H
