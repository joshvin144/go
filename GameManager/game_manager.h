#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H


#include <iostream>
#include <stdint.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>

#include "game_board.h"
#include "player.h"
#include "display_manager.h"


typedef enum GameState
{
    GAME_STATE_STARTUP,
    GAME_STATE_IN_PROGRESS,
    GAME_STATE_PAUSED,
    GAME_STATE_END,
    NUM_GAME_STATES
}game_state_e;


class GameManager
{
    public:
        // Handle Players
        void add_player(player_type_e player_type);
        void remove_player(void);

        // Handle Game Board setup
        void setup_board_game(GameBoard* p_game_board, uint8_t u8_grid_dimension);
        void teardown_board_game(GameBoard* p_game_board);

        // Communication with Keyboard Monitor
        void register_key_queue(std::queue<int>* p_key_queue);
        void register_key_mutex(std::mutex* p_key_mutex);
        void process_input(int i32_input);

        // Communication with Display Manager
        void register_display_queue(std::queue<display_update_event_t>* p_display_update_queue);
        void register_display_mutex(std::mutex* p_display_mutex);
        
        // Threadable
        void run(void);
    private:
        bool game_update;
        uint32_t game_turn = 0;
        uint32_t game_state = GAME_STATE_STARTUP;

        std::queue<int>* p_key_queue;
        std::mutex* p_key_mutex;

        std::queue<display_update_event_t>* p_display_queue;
        std::mutex* p_display_mutex;

        // A Game consists of Players and a Game Board
        GameBoard* p_game_board;
        std::vector<std::unique_ptr<GenericPlayer>> v_players;
};


#endif

