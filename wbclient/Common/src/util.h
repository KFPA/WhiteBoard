/**
* @brief    常用方法集合
* @author   Ivan_XZ
* @date     2017_11_26
*/

#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QDateTime>
#include <QtCore/QFileInfoList>

#include <QNetworkInterface>
#include <QList>

/**
 * @brief 字符串
 */
class StringUtil
{
public:
    /**
     * @brief 将16进制字符串转换为16进制字节序列
     * @param 16进制字符串
     * @return 16进制字节序列
     */
    static QByteArray hexstringToBytearray(QString hexstring);

    /**
     * @brief 将16进制字节序列转换为16进制字符串
     * @param 16进制字节序列
     * @return 16进制字符串
     */
    static QString bytearrayToHexstring(QByteArray data);

    /**
     * @brief MD5和Base64加密
     * @param 字符串
     * @return 加密后的字符串
     */
    static QString toBase64Md5(QString input);

    /**
     * @brief 将{0}风格的占位符转化为qt风格的%1
     * @param 待转换字符串
     * @param 占位符的个数
     * @return 转换后的字符串
     */
    static QString toQstyle(const QString& str, int args = 1);

    /**
     * @brief 反转
     * @param 待转换字符串
     * @return 转换后字符串
     */
    static QByteArray reverse(const QByteArray& data);
};

/**
 * @brief json转换
 */
class JsonUtil
{
public:
    /**
     * @brief 将字符串转换为json对象
     * @param 字符串
     * @return json对象
     */
    static QJsonObject stringToJson(const QString& str);

    /**
     * @brief 将json对象转换为字符串
     * @param json对象
     * @return 字符串
     */
    static QString jsonToString(const QJsonObject& obj);
};

/**
 * @brief 时间转换
 */
class DateUtil
{
public:
    /**
     * @brief 格式化ISO8601日期格式
     * @param 时间
     * @return 格式化后的时间
     */
    static QString formatIso8601Date(QDateTime date);

    /**
     * @brief 转换为时间戳
     * @param 时间
     * @return 时间戳
     */
    static QString convertDatetimeInt(QDateTime time);
};

class SignerUtil
{
public:
    /**
     * @brief 加密后返回Hex
     * @param secretKey
     * @param text
     * @return
     */
    static QString getHexString(QString secret_key, QString text);

    /**
     * @brief 加密后返回Base64
     * @param secretKey
     * @param text
     * @return
     */
    static QString getBase64String(QString secret_key, QString text);
};

class FileUtil
{
public:
    /**
     * @brief 获取文件夹下所有文件
     * @param path
     * @return
     */
    static QFileInfoList getFileList(const QString& path);

    /**
     * @brief 移除文件夹下的所有文件
     * @param filePath
     */
    static void removeFolderContent(const QString &filePath);
};

class NetUtil
{
public:

    /**
     * @brief 获取IP地址
     * @return
     */
    static QString getHostIpAddress();

    /**
     * @brief 获取Mac地址
     * @return
     */
    static QString getHostMacAddress();
};


#endif // UTIL_H
