#include "util.h"
#include <QStringList>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QMessageAuthenticationCode>
#include <QDebug>
#include <QDir>

QByteArray StringUtil::hexstringToBytearray(QString hexstring)
{
    bool ok;
    QByteArray ret;
    hexstring = hexstring.trimmed();
    hexstring = hexstring.simplified();
    QStringList sl = hexstring.split(" ");

    foreach (QString s, sl)
    {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok, 16) & 0xFF;
            if(ok)
            {
                ret.append(c);
            }
        }
    }

    return ret;
}

QString StringUtil::bytearrayToHexstring(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length() / 2;
    for(int i = 1; i < len; i++)
    {
        ret.insert(2 * i + i - 1," ");
    }

    return ret;
}

QString StringUtil::toBase64Md5(QString input)
{
   QCryptographicHash md(QCryptographicHash::Md5);
   QByteArray result, data;
   data.append(input);
   md.addData(data);
   result = md.result();
   return result.toBase64();
}

QString StringUtil::toQstyle(const QString& str, int args)
{
    QString ret = str;    
    QString before = "{%1}";
    QString after = "%%1";
    for(int i = 0; i < args; ++i)
    {
        ret = ret.replace(before.arg(i), after.arg(i + 1));
    }

    return ret;
}

QByteArray StringUtil::reverse(const QByteArray& data)
{
    QByteArray ret;
    int len = data.length();
    ret.resize(len);
    for(int i = 0; i < len; i++)
    {
        ret[i] = data[len - i - 1];
    }

    return ret;
}

QJsonObject JsonUtil::stringToJson(const QString& str)
{
    QJsonObject obj;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(str.toLocal8Bit().data(), &jsonError);  // 转化为 JSON 文档
    if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))  // 解析未发生错误
    {
        if (document.isObject()) // JSON 文档为对象
        {
            obj = document.object();
        }
    }

    return obj;
}

QString JsonUtil::jsonToString(const QJsonObject& obj)
{
    QByteArray byte_array = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    return QString(byte_array);
}

QString DateUtil::formatIso8601Date(QDateTime date)
{
    return date.toString(Qt::ISODate);
}

QString DateUtil::convertDatetimeInt(QDateTime time)
{
    return QString::number(time.toTime_t(), 10).append("000");
}

QString SignerUtil::getHexString(QString secret_key, QString text)
{
    int block = 64;
    QByteArray key = secret_key.toLatin1();
    if (key.length() > block)
    {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray inner_padding(block, char(0x36));
    QByteArray outer_padding(block, char(0x5c));
    for (int i = 0; i < key.length(); i++)
    {
        inner_padding[i] = inner_padding[i] ^ key.at(i);
        outer_padding[i] = outer_padding[i] ^ key.at(i);
    }

    QByteArray total = outer_padding;
    QByteArray part = inner_padding;
    part.append(text);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    return hashed.toHex();
}

QString SignerUtil::getBase64String(QString secret_key, QString text)
{
    int block = 64;
    QByteArray key = secret_key.toLatin1();
    if (key.length() > block)
    {
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray inner_padding(block, char(0x36));
    QByteArray outer_padding(block, char(0x5c));
    for (int i = 0; i < key.length(); i++)
    {
        inner_padding[i] = inner_padding[i] ^ key.at(i);
        outer_padding[i] = outer_padding[i] ^ key.at(i);
    }

    QByteArray total = inner_padding;
    QByteArray part = outer_padding;
    part.append(text);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    return hashed.toBase64();
}

QFileInfoList FileUtil::getFileList(const QString& path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = getFileList(name);
        file_list.append(child_file_list);
    }

    return file_list;
}

void FileUtil::removeFolderContent(const QString &filePath)
{
    QDir dir(filePath);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists()) {
        return;
    }
    fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable |
                                 QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);
    while(fileList.size() > 0)
    {
        int infoNum = fileList.size();
        for(int i = infoNum - 1;i >= 0; i--)
        {
            curFile = fileList[i];
            if(curFile.isFile())
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
        }
    }
}

QString NetUtil::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&ipAddressesList.at(i).toIPv4Address())
        {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果没有找到，则以本地IP地址为IP
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return strIpAddress;
}

QString NetUtil::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}
