/**
* @brief    文件处理类
* @author   Ivan_XZ
* @date     2018_8_12
*/

#ifndef FILEINFO_H
#define FILEINFO_H

#include <QUrl>
#include <QDir>
#include <QObject>
#include <QString>
#include <QFileInfo>

//为qml提供文件路径处理、文件名处理相关的功能
class FileInfo : public QObject {
    Q_OBJECT
public:
    explicit FileInfo(QObject *parent = nullptr);

    //这些API的功能，请参考QFileInfo的文档。
    Q_INVOKABLE QString baseName(const QString &filePath);
    Q_INVOKABLE QString fileName(const QString &filePath);
    Q_INVOKABLE QString suffix(const QString &filePath);
    Q_INVOKABLE QString absoluteDir(const QString &filePath);
    //转成QUrl
    Q_INVOKABLE QUrl toUrl(const QString &filePath);
    //转成local
    Q_INVOKABLE QString toLocal(const QUrl &url);
};

#endif // FILEINFO_H
