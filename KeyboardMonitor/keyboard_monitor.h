#ifndef KEYBOARD_MONITOR_H
#define KEYBOARD_MONITOR_H


#include <iostream>
#include <stdint.h>
#include <thread>
#include <queue>
#include <mutex>


class KeyboardMonitor
{
    public:
        void register_key_queue(std::queue<int>* p_key_queue);
        void register_key_mutex(std::mutex* p_key_mutex);
        void run(void);
    private:
        std::queue<int>* p_key_queue;
        std::mutex* p_key_mutex;
        
};


#endif

