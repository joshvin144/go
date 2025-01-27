#include <curses.h>
#include <unistd.h>

#include "keyboard_monitor.h"


#define SCAN_PERIOD (10ll)


void KeyboardMonitor::register_key_queue(std::queue<int>* p_key_queue)
{
    this->p_key_queue = p_key_queue;
}


void KeyboardMonitor::register_key_mutex(std::mutex* p_key_mutex)
{
    this->p_key_mutex = p_key_mutex;
}


void KeyboardMonitor::run(void)
{
    (void)initscr();
    (void)cbreak();
    (void)nodelay(stdscr, TRUE);

    for(;;)
    {
        int ascii_char = getch(); // Currently blocks
        if(ERR != ascii_char)
        {
            this->p_key_mutex->lock();
            this->p_key_queue->push(ascii_char);
            this->p_key_mutex->unlock();
        }
        else
        {
            (void)usleep(SCAN_PERIOD);
        }
    }
}

