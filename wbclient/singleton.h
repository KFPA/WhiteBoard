#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <pthread.h>
namespace wb{
    template <class T>
    class singleton
    {
    protected:
        singleton(){};
    private:
        singleton(const singleton&){};
        singleton& operator=(const singleton&){};
        static T* m_instance;
        static pthread_mutex_t mutex;
    public:
        static T* GetInstance();
    };


    template <class T>
    T* singleton<T>::GetInstance()
    {
        if( m_instance == nullptr){
            pthread_mutex_lock(&mutex);
            if( m_instance == nullptr)
            {
                m_instance = new T();
            }
            pthread_mutex_unlock(&mutex);
        }
        return m_instance;
    }


    template <class T>
    pthread_mutex_t singleton<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

    template <class T>
    T* singleton<T>::m_instance = nullptr;
}


#endif // __SINGLETON_H__
