#ifndef BASE64UPLOAD_H
#define BASE64UPLOAD_H

#include <QObject>
#include <HttpRequest>
#include <QNetworkAccessManager>

class FileUpload : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString responseText READ getResponseText NOTIFY responseTextChanged)
public:
    explicit FileUpload(QObject *parent = nullptr);

    Q_INVOKABLE void uploadImage(QString filePath, QString url);

    QString getResponseText() const;
signals:
    void responseTextChanged();
    void timeoutError();

private slots:
    void finished();
    void timeout();

private:
    QString m_responseText;
};

#endif // BASE64UPLOAD_H
