/**
 * Game Manager and Keyboard Monitor
 * - Players
 * -- Game Board
 * -- Stones
 */

// /usr/bin/g++ -std=c++14 -lcurses -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/single_include/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameBoard/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/Players/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameManager/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameManager/ASCII/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/KeyboardMonitor/ -I /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/DisplayManager/ -fdiagnostics-color=always -g /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameBoard/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/Players/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameManager/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/GameManager/ASCII/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/KeyboardMonitor/*.cpp /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/DisplayManager/*.cpp -o /Users/jvincent/Documents/ProfessionalDevelopment/ContinuousLearning/Projects/Repositories/go/main

#include <iostream>
#include <stdint.h>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>

#include "game_board.h"
#include "player.h"
#include "game_manager.h"
#include "keyboard_monitor.h"
#include "display_manager.h"
#include "toml.hpp"


#define CONFIG_PATH ("./config.toml")


int main(int argc, char* argv[])
{
    // Load config
    auto config = toml::parse(CONFIG_PATH);

    // Instantiate the necessary managers
    GameManager game_manager;
    KeyboardMonitor keyboard_monitor;
    DisplayManager display_manager;

    // Connect the GameManager and the KeyboardMonitor by a queue
    std::queue<int> key_queue;
    std::mutex key_mutex;
    game_manager.register_key_queue(&key_queue);
    game_manager.register_key_mutex(&key_mutex);
    keyboard_monitor.register_key_queue(&key_queue);
    keyboard_monitor.register_key_mutex(&key_mutex);

    // Connect the GameManager and the Display Manager
    std::queue<display_update_event_t> display_update_queue;
    std::mutex display_update_mutex;
    game_manager.register_display_queue(&display_update_queue);
    game_manager.register_display_mutex(&display_update_mutex);
    display_manager.register_display_queue(&display_update_queue);
    display_manager.register_display_mutex(&display_update_mutex);

    GameBoard game_board;
    game_manager.add_player(HUMAN_PLAYER);
    game_manager.add_player(COMPUTER_PLAYER);

    // Set up a game of Go
    game_manager.setup_board_game(&game_board, (uint8_t)config.at("grid_dimension").as_integer());
    // The Game Manager will update the game board and the Display Manager will display it
    // Therefore, the Display Manager must also be aware of the Game Board
    display_manager.register_game_board(&game_board);

    // Prompt the user to choose the turn order during setup
    // std::cout << "Choose to play first (1) or second (2):\n\r";

    // Start thread execution
    std::thread game_manager_thread(&GameManager::run, &game_manager);
    std::thread keyboard_monitor_thread(&KeyboardMonitor::run, &keyboard_monitor);
    // Currently runs indefinitely
    display_manager.run();

    // Cleanup
    game_manager.teardown_board_game(&game_board);
    std::cout << "Teardown complete.\n";
    return 0;
}

