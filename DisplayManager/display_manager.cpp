#include "display_manager.h"


void DisplayManager::register_display_queue(std::queue<display_update_event_t>* p_display_queue)
{
    this->p_display_queue = p_display_queue;
}


void DisplayManager::register_display_mutex(std::mutex* p_display_mutex)
{
    this->p_display_mutex = p_display_mutex;
}


void DisplayManager::register_game_board(GameBoard* p_game_board)
{
    this->p_game_board = p_game_board;
}


// Display Manager currently runs in the main thread
// To be implemented in a separate thread later
void DisplayManager::run(void)
{
    while(true)
    {
        // Monitor the display update event queue
        if(0 != this->p_display_queue->size())
        {
            if(this->p_display_mutex->try_lock())
            {
                display_update_event_t display_update_event = this->p_display_queue->front();
                switch(display_update_event.type)
                {
                    case DISPLAY_UPDATE_TYPE_MESSAGE:
                        for(uint8_t i = 0; (i <= (uint8_t)(MESSAGE_BUFFER_SIZE - 1u)) and (MESSAGE_ESCAPE_CHARACTER != display_update_event.message[i]); i++)
                        {
                            std::cout << display_update_event.message[i];
                        }
                        std::cout << "\n\r";
                        break;
                    case DISPLAY_UPDATE_TYPE_BOARD:
                        p_game_board->display_game_grid();
                        break;
                    default:
                        break;
                }
                this->p_display_queue->pop();
                this->p_display_mutex->unlock();
            }
        }
    }
}
