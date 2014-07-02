#ifndef CHANNEL_H_FC6YZN5J
#define CHANNEL_H_FC6YZN5J

#include <boost/thread.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <future>
#include <memory>
#include <utility>
#include "../kit.h"

template<class T>
class Channel:
    public kit::mutexed<std::mutex>
    //public std::enable_shared_from_this<Channel<T>>
{
    public:

        virtual ~Channel() {}

        // Put into stream
        Channel& operator<<(T val) {
            do{
                boost::this_thread::interruption_point();
                auto l = lock(std::defer_lock);
                if(l.try_lock())
                {
                    if(!m_Buffered || m_Vals.size() < m_Buffered) {
                        m_Vals.push(std::move(val));
                        break;
                    }
                }
                boost::this_thread::yield();
            }while(true);
            return *this;
        }
        
        // Get from stream
        bool operator>>(T& val) {
            auto l = lock();
            if(!m_Vals.empty()) {
                val = std::move(m_Vals.front());
                m_Vals.pop();
                return true;
            }
            return false;
        }

        //operator bool() const {
        //    return m_bClosed;
        //}
        bool empty() const {
            auto l = lock();
            return m_Vals.empty();
        }
        size_t size() const {
            auto l = lock();
            return m_Vals.size();
        }
        size_t buffered() const {
            auto l = lock();
            return m_Buffered;
        }
        void unbuffer() {
            auto l = lock();
            m_Buffered = 0;
        }
        void buffer(size_t sz) {
            auto l = lock();
            m_Buffered = sz;
        }
        void close() {
            // atomic
            m_bClosed = true;
        }
        bool closed() const {
            // atomic
            return m_bClosed;
        }

    private:
        
        size_t m_Buffered = 0;
        std::queue<T> m_Vals;
        std::atomic<bool> m_bClosed = ATOMIC_VAR_INIT(false);
};

#endif

