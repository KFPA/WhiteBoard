#include <FileIO>
#include <QDir>
#include <QImage>
#include "util.h"

FileIO::FileIO(QObject *parent) : QObject(parent) {}

bool FileIO::writeFile(const QString &filePath, const QString &data) {
    QFile file(filePath);
    if (file.open(QFile::WriteOnly)) {
        file.write(data.toLocal8Bit());
        file.close();
        mError.clear();
        return true;
    } else {
        mError = "FileName: " + file.fileName() + " Error: " + file.errorString();
        return false;
    }
}

QString FileIO::readFile(const QString &filePath) {
    QString ret;
    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {
        auto bytes = file.readAll();
        ret = QString(bytes);

        file.close();
        mError.clear();
    } else {
        ret.clear();
        mError = "FileName: " + file.fileName() + " Error: " + file.errorString();
    }
    return ret;
}

bool FileIO::createDir(const QString &fileDir)
{
    // 检查目录是否存在，若不存在则新建
    QDir dir;
    if (!dir.exists(fileDir))
    {
        return dir.mkpath(fileDir);
    }

    return true;
}

QString FileIO::imageBase64(const QString &imagePath)
{
    QString ret;
    QFile file(imagePath);
    if (file.open(QFile::ReadOnly)) {
        ret = QString(file.readAll().toBase64());
        file.close();
    } else {
        ret.clear();
    }

    return ret;
}

const QString &FileIO::errorString() {
    return mError;
}

void FileIO::shapeImage(const QString &imagePath, const QString &savePath, int x, int y, int w, int h)
{
    QImage image(imagePath);
    QImage newImage = image.copy(int(x), int(y), w, h);
    newImage.save(savePath);
}

void FileIO::removeFolderContent(const QString &filePath)
{
    FileUtil::removeFolderContent(filePath);
}
