#include "fileupload.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QHttpMultiPart>

QNetworkAccessManager manager;
HttpRequest httpRequest;

FileUpload::FileUpload(QObject *parent) : QObject(parent){ }

void FileUpload::finished()
{
    m_responseText = httpRequest.getResponseText();
    disconnect(&httpRequest, &HttpRequest::finished, this, &FileUpload::finished);
    disconnect(&httpRequest, &HttpRequest::timeout, this, &FileUpload::timeout);
    emit responseTextChanged();
}

void FileUpload::timeout()
{
    disconnect(&httpRequest, &HttpRequest::finished, this, &FileUpload::finished);
    disconnect(&httpRequest, &HttpRequest::timeout, this, &FileUpload::timeout);
    emit timeoutError();
}

QString FileUpload::getResponseText() const
{
    return m_responseText;
}

void FileUpload::uploadImage(QString filePath, QString url)
{
    httpRequest.setManager(&manager);
    connect(&httpRequest, &HttpRequest::finished, this, &FileUpload::finished);
    connect(&httpRequest, &HttpRequest::timeout, this, &FileUpload::timeout);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    QString formData = "form-data; name=\"file\"; filename=\"%1\"";
    QFileInfo fileInfo(filePath);
    formData = formData.arg(fileInfo.fileName());
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(formData));

    QFile *file = new QFile(filePath);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    QString method = "POST";
    httpRequest.open(method, url);
    httpRequest.send(multiPart);
}
