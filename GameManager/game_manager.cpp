#include <assert.h>
#include <vector>

#include "game_manager.h"
#include "ascii_user.h"


// Escape
#define ESCAPE_KEY (10)

// Turn order
#define PLAYING_FIRST (1)
#define PLAYING_SECOND (2)

// Input that may be taken at any time
typedef enum AnyTimeInput
{
    ANY_TIME_INPUT_PROXY
}any_time_input_t;


void GameManager::add_player(player_type_e player_type)
{
    switch(player_type)
    {
        case HUMAN_PLAYER:
            this->v_players.push_back(std::make_unique<HumanPlayer>());
            break;
        case COMPUTER_PLAYER:
            this->v_players.push_back(std::make_unique<ComputerPlayer>());
            break;
        default:
            break;
    }
}


// Remove the last player added
void GameManager::remove_player(void)
{
    this->v_players.pop_back();
}


void GameManager::setup_board_game(GameBoard* p_game_board, uint8_t u8_grid_dimension)
{
    p_game_board->set_grid_dimension(u8_grid_dimension);
    p_game_board->create_game_grid();
    this->p_game_board = p_game_board;
    this->game_state = GAME_STATE_STARTUP;
}


void GameManager::register_key_queue(std::queue<int>* p_key_queue)
{
    this->p_key_queue = p_key_queue;
}


void GameManager::register_key_mutex(std::mutex* p_key_mutex)
{
    this->p_key_mutex = p_key_mutex;
}


void GameManager::register_display_queue(std::queue<display_update_event_t>* p_display_queue)
{
    this->p_display_queue = p_display_queue;
}


void GameManager::register_display_mutex(std::mutex* p_display_mutex)
{
    this->p_display_mutex = p_display_mutex;
}


void GameManager::process_input(int i32_input)
{
    switch(i32_input)
    {
        case ANY_TIME_INPUT_PROXY:
        default:
            break;
    }
}


void GameManager::run(void)
{
    bool process_ascii_char_set = false;
    std::vector<int> ascii_char_set;

    for(;;)
    {
        // Check for characters sent from the Keyboard Monitor
        if(!this->p_key_queue->empty())
        {
            if(this->p_key_mutex->try_lock())
            {
                int ascii_char = this->p_key_queue->front();
                // Do not add the escape character to the character set
                if(ESCAPE_KEY == ascii_char)
                {
                    process_ascii_char_set = true;
                }
                else
                {
                    ascii_char_set.push_back(ascii_char);
                }
                this->p_key_queue->pop();
                this->p_key_mutex->unlock();
            }
        }

        // Process user input after fully entered and escaped
        coordinate_t t_coordinate;
        int i32_input = -1;
        if(process_ascii_char_set)
        {
            process_ascii_char_set = false;

            if(1 <= ascii_char_set.size())
            {
                // Check for commas, as in the case of coordinates
                // If comma_idx is -1, a comma has not been found
                // If comma_idx is nonzero, comma_idx is the index at which the comma is found
                int comma_idx = find_comma(&ascii_char_set);
                if(-1 != comma_idx)
                {
                    i32_input = PLAYER_ACTION_PLACE_STONE;
                    // If a comma is found, split into two vectors and convert to coordiantes
                    std::vector<int> x_ascii_char_set = std::vector<int>(ascii_char_set.begin(), ascii_char_set.begin() + comma_idx);
                    std::vector<int> y_ascii_char_set = std::vector<int>(ascii_char_set.begin() + comma_idx + 1, ascii_char_set.end());
                    t_coordinate.u8_x_coordinate = (uint8_t)ascii_char_set_to_decimal(&x_ascii_char_set);
                    t_coordinate.u8_y_coordinate = (uint8_t)ascii_char_set_to_decimal(&y_ascii_char_set);
                }
                else
                {
                    // Convert to decimal encoding
                    i32_input = ascii_char_set_to_decimal(&ascii_char_set);

                    // For input at any time, such as pausing the game, quick quitting, etc.
                    this->process_input(i32_input);
                }
            }
            
            // Flush the character set
            ascii_char_set.erase(ascii_char_set.begin(), ascii_char_set.end());
        }

        // The next action is determined by the game state
        int player_idx = 0;
        bool sub_actions_complete = false;
        display_update_event_t display_update_event;
        switch(this->game_state)
        {
            case GAME_STATE_STARTUP:
                // The setup step prompts the user to choose the turn order
                // For now, the human player always goes first
                this->game_state=GAME_STATE_IN_PROGRESS;
                break;
            case GAME_STATE_IN_PROGRESS:
                // All player's actions are handled here
                // An action may consist of sub-actions
                // We want to wait until these sub-actions are complete incrementing the game turn
                player_idx = this->game_turn%this->v_players.size();
                // Each type of player has a different implementation of "process_action"
                // I.E. a "HumanPlayer" has a different process_action than a "ComputerPlayer"
                // For the sake of consistency and conciseness, each "process_action" method takes the same parameters
                // However, the ComputerPlayer only needs this->p_game_board and this->p_game_turn
                // "game_turn" is initialized at zero and is used to index through the players
                // However, the first turn at zero may be unintuitive to the Human Player
                sub_actions_complete = this->v_players[player_idx]->process_action(this->p_game_board, this->game_turn + 1u, &t_coordinate, i32_input);
                if(sub_actions_complete)
                {
                    // For testing purposes only
                    sub_actions_complete = false;
                    if(this->p_display_mutex->try_lock())
                    {
                        // Testing message type event and board type event in sequence
                        display_update_event.type = DISPLAY_UPDATE_TYPE_MESSAGE;
                        char c_message[] = "Updating Display\n";
                        memcpy(display_update_event.message, c_message, sizeof(c_message));
                        this->p_display_queue->push(display_update_event);

                        display_update_event.type = DISPLAY_UPDATE_TYPE_BOARD;
                        this->p_display_queue->push(display_update_event);

                        this->p_display_mutex->unlock();
                    }
                    this->game_turn++;
                }
                break;
            case GAME_STATE_PAUSED:
                break;
            case GAME_STATE_END:
                break;
            default:
                break;
        }
    }
}


void GameManager::teardown_board_game(GameBoard* p_game_board)
{
    p_game_board->destroy_game_grid();
}

