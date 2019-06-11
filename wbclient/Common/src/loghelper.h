/**
* @brief    日志类
* @author   Ivan_XZ
* @date     2018_8_12
*/

#ifndef LOGHELPER_H
#define LOGHELPER_H

#include "singleton.h"

// 前置声明
struct LogHelperPrivate;

class LogHelper
{
    SINGLETON(LogHelper)    // 使用单例模式
public:
     /**
      * @brief 释放资源
     */
    void release();

    /**
     * @brief 给QT安装消息处理函数
     */
    void install_message_handler();

private:
    LogHelperPrivate *d;
};

//为qml提供日志读写
#include <QObject>
class Log4Qml : public QObject {
    Q_OBJECT
public:
    explicit Log4Qml(QObject *parent = nullptr);

    Q_INVOKABLE void logger(const QString& log, int type = 0);
};

#endif // LOGHELPER_H
