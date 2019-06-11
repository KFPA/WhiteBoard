#include "loghelper.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <QCoreApplication>

/************************************LogHelperPrivate 声明************************/
struct LogHelperPrivate
{
    LogHelperPrivate();
    ~LogHelperPrivate();

    // 打开日志文件，如果不是当天创建的，则用创建日期将其重新命名yyyy-MM-dd.log，并创建一个新的log.txt
    void openAndBackupLogFile();

    // 如果日志目录不存在，则创建
    void makeSureLogDirectory() const;

    // 消息处理函数
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QDir logDir;                        // 日志文件夹
    QTimer renameLogFileTimer;          // 重命名日志文件使用的定时器
    QTimer flushLogFileTimer;           // 刷新输出日志文件使用的定时器
    QDate logFileCreatedDate;           // 日志文件创建的时间

    static QFile *logFile;              // 日志文件
    static QTextStream *logOut;         // 输出日志文件流
    static QMutex logMutex;             // 同步使用锁
};

/*********************************************************************************/

/************************************LogHelperPrivate 实现*************************/
QFile *LogHelperPrivate::logFile = NULL;
QTextStream *LogHelperPrivate::logOut = NULL;
QMutex LogHelperPrivate::logMutex;

LogHelperPrivate::LogHelperPrivate()
{
    QString path = "";
#if defined(Q_OS_MACOS)
    path = QCoreApplication::applicationDirPath() + "/../../../log/";
#else
    path = QCoreApplication::applicationDirPath() + "/log/";
#endif

    logDir.setPath(path);  // TODO: 日志文件夹的路径，为程序所在目录下的 log 文件夹，可从配置文件读取
    QString logPath = logDir.absoluteFilePath("log.txt");   // 日志文件路径
    logFileCreatedDate = QFileInfo(logPath).birthTime().date();   // 创建日志文件时间
    openAndBackupLogFile();     // 打开日志文件

    // 十分钟检查一次日志文件创建时间
    renameLogFileTimer.setInterval(1000 * 60 * 10); // TODO: 可从配置文件中读取
    renameLogFileTimer.start();
    QObject::connect(&renameLogFileTimer, &QTimer::timeout, [this]{
        QMutexLocker locker(&LogHelperPrivate::logMutex);
        openAndBackupLogFile();
    });

    // 定时刷新日志输出到文件，尽快的能在日志文件里看到最新的日志
    flushLogFileTimer.setInterval(1000); // TODO: 可从配置文件读取
    flushLogFileTimer.start();
    QObject::connect(&flushLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHelperPrivate::logMutex);
        if (NULL != logOut)
        {
            logOut->flush();
        }
    });
}

LogHelperPrivate::~LogHelperPrivate()
{
    if (NULL != logFile)
    {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
        logOut  = NULL;
        logFile = NULL;
    }
}

void LogHelperPrivate::openAndBackupLogFile()
{
    makeSureLogDirectory(); // 如果日志所在目录不存在，则创建
    QString logPath = logDir.absoluteFilePath("log.txt"); // 日志的路径
    if (NULL == logFile)
    {
        logFile = new QFile(logPath);
        logOut  = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ?  new QTextStream(logFile) : NULL;
        if (NULL != logOut)
        {
            logOut->setCodec("UTF-8");
        }

        // 如果文件是第一次创建，则创建日期是无效的，把其设置为当前日期
        if (logFileCreatedDate.isNull()) {
            logFileCreatedDate = QDate::currentDate();
        }
        // TODO: 可以检查日志文件超过 30 个，删除 30 天前的日志文件
    }

    // 程序运行时如果创建日期不是当前日期，则使用创建日期重命名，并生成一个新的 log.txt
    if (logFileCreatedDate != QDate::currentDate())
    {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
        QString newLogPath = logDir.absoluteFilePath(logFileCreatedDate.toString("yyyy-MM-dd.log"));;
        QFile::copy(logPath, newLogPath);
        QFile::remove(logPath); // 删除重新创建，改变创建时间
        logFile = new QFile(logPath);
        logOut  = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) ?  new QTextStream(logFile) : NULL;
        logFileCreatedDate = QDate::currentDate();
        if (NULL != logOut)
        {
            logOut->setCodec("UTF-8");
        }
    }
}

void LogHelperPrivate::makeSureLogDirectory() const
{
    if (!logDir.exists())
    {
        logDir.mkpath("."); // 可以递归的创建文件夹
    }
}

void LogHelperPrivate::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&LogHelperPrivate::logMutex);
    QString level;
    switch (type)
    {
    case QtDebugMsg:
        level = "Debug";
        break;
    case QtInfoMsg:
        level = "Info";
        break;
    case QtWarningMsg:
        level = "Warn";
        break;
    case QtCriticalMsg:
        level = "Error";
        break;
    case QtFatalMsg:
        level = "Fatal";
        break;
    default:;
    }

    // 输出到标准输出
    QByteArray localMsg = msg.toLocal8Bit();
    std::cout << std::string(localMsg) << std::endl;
    if (NULL == LogHelperPrivate::logOut)
    {
        return;
    }

    if(level == "Warn")
    {
        return;
    }

    // 输出到日志文件, 格式: 时间 - [Level] (文件名:行数, 函数): 消息
    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);
//    (*LogHelperPrivate::logOut) << QString("%1 - [%2] (%3:%4, %5): %6\n")
//                                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(level)
//                                    .arg(fileName).arg(context.line).arg(context.function).arg(msg);
    (*LogHelperPrivate::logOut) << QString("%1 - [%2]\n %3\n\n")
                                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(level).arg(msg);
}

/*********************************************************************************/

/************************************LogHelper 实现********************************/
LogHelper::LogHelper():d(NULL)
{
}

LogHelper::~LogHelper()
{
}

void LogHelper::install_message_handler()
{
    QMutexLocker locker(&LogHelperPrivate::logMutex);

    if(NULL == d)
    {
        d = new LogHelperPrivate();
        qInstallMessageHandler(LogHelperPrivate::messageHandler);
    }
}

void LogHelper::release()
{
    QMutexLocker locker(&LogHelperPrivate::logMutex);
    qInstallMessageHandler(0);
    delete d;
    d = NULL;
}
/*********************************************************************************/

/************************************Log4Qml 实现********************************/
Log4Qml::Log4Qml(QObject *parent) : QObject(parent) {}

void Log4Qml::logger(const QString& log, int type)
{
    switch (type) {
    case 0:
        qInfo().noquote() << log;
        break;
    case 1:
        qDebug().noquote() << log;
        break;
    case 2:
        qWarning().noquote() << log;
        break;
    case 3:
        qCritical().noquote() << log;
        break;
    default:
        break;
    }
}
/*********************************************************************************/
