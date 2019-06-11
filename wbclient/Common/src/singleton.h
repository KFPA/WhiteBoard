/**
* @brief    单例模板
* @author   Ivan_XZ
* @date     2018_8_12
*/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

/************************************Singleton 声明********************************/
template<typename T>
class Singleton
{
public:
    /**
     * @brief 对外接口
     * @return  实例对象
     */
    static T& get_instance();

    /**
     * @brief 禁止拷贝
     * @param other
     */
    Singleton(const Singleton& other);

    /**
     * @brief 禁止赋值
     * @param other
     * @return
     */
    Singleton<T>& operator = (const Singleton& other);

private:
    static QMutex mutex;        // 同步使用锁
    static QScopedPointer<T> instance;      // 垃圾回收
};
/*********************************************************************************/

/************************************Singleton 实现********************************/
template<typename T> QMutex Singleton<T>::mutex;
template<typename T> QScopedPointer<T> Singleton<T>::instance;

template<typename T>
T& Singleton<T>::get_instance()
{
    if(instance.isNull())
    {
        mutex.lock();
        if(instance.isNull())
        {
            instance.reset(new T());
        }
        mutex.unlock();
    }

    return *instance.data();
}
/*********************************************************************************/

/************************************Singleton 宏*********************************/
#define SINGLETON(Class)                        \
private:                                        \
    Class();                                    \
    ~Class();                                   \
    Class(const Class &other);                  \
    Class& operator=(const Class &other);       \
    friend class Singleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;
/*********************************************************************************/

#endif // SINGLETON_H
