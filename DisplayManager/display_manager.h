#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H


#include <iostream>
#include <stdint.h>
#include <queue>
#include <mutex>

#include "game_board.h"

// Self-explanatory
#define MESSAGE_BUFFER_SIZE (32u)
// Escape character for Display Messages
#define MESSAGE_ESCAPE_CHARACTER '\n'


typedef enum DisplayUpdateType
{
    DISPLAY_UPDATE_TYPE_BOARD,
    DISPLAY_UPDATE_TYPE_MESSAGE
}display_update_type_e;


typedef struct display_update_event
{
    display_update_type_e type;
    char message[MESSAGE_BUFFER_SIZE];
}display_update_event_t;


class DisplayManager
{
    public:
        DisplayManager() { std::cout << "Constructing DisplayManager..."; }
        ~DisplayManager() { std::cout << "Deconstructing DisplayManager..."; }

        // Communication with Game Manager
        void register_display_queue(std::queue<display_update_event_t>* p_display_queue);
        void register_display_mutex(std::mutex* p_display_mutex);

        // The Display Manager must be aware of the Game Board
        void register_game_board(GameBoard* p_game_board);

        // Execution
        void run(void);

    private:
        std::queue<display_update_event_t>* p_display_queue;
        std::mutex* p_display_mutex;

        GameBoard* p_game_board;
};


#endif