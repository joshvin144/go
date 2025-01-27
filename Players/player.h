#ifndef PLAYER_H
#define PLAYER_H


#include <stdint.h>

#include "game_board.h"

typedef enum PlayerType
{
    HUMAN_PLAYER,
    COMPUTER_PLAYER,
    NUM_PLAYER_TYPES
}player_type_e;


typedef enum PlayerAction
{
    PLAYER_ACTION_PLACE_STONE,
    PLAYER_ACTION_PASS,
    PLAYER_ACTION_RESIGN,
    NUM_PLAYER_ACTIONS
}player_action_e;


typedef enum PlayerSubAction
{
    PLAYER_SUB_ACTION_PROMPT_STEP,
    PLAYER_SUB_ACTION_ENTRY_STEP,
    PLAYER_SUB_ACTION_YIELD_STEP,
    NUM_PLAYER_SUB_ACTIONS
}player_sub_action_t;


class GenericPlayer
{
    public:
        virtual ~GenericPlayer() = default;
        virtual bool process_action(GameBoard* p_game_board, uint32_t u32_game_turn, coordinate_t* pt_coordinate, int i32_action) = 0;

        uint8_t u8_stone_color;
        bool is_multistep_action = false;
        int i32_action;
        int i32_sub_action = PLAYER_SUB_ACTION_PROMPT_STEP;
};


class HumanPlayer: public GenericPlayer
{
    public:
        HumanPlayer() { std::cout << "Constructing HumanPlayer \n\r"; }
        ~HumanPlayer() { std::cout << "Deconstructing HumanPlayer \n\r"; }
        bool process_action(GameBoard* p_game_board, uint32_t u32_game_turn, coordinate_t* pt_coordinate, int i32_action) override
        {
            bool sub_actions_complete = false;
            switch(i32_action)
            {
                case PLAYER_ACTION_PLACE_STONE:
                    this->i32_action = PLAYER_ACTION_PLACE_STONE;
                    switch(this->i32_sub_action)
                    {
                        case PLAYER_SUB_ACTION_PROMPT_STEP:
                            this->i32_sub_action=PLAYER_SUB_ACTION_ENTRY_STEP;
                            // std::cout << "Enter coordinates (x,y)\n\r";
                            break;
                        case PLAYER_SUB_ACTION_ENTRY_STEP:
                            // Check that the coordinates are valid
                            coordinate_t t_coordinate;
                            t_coordinate.u8_x_coordinate = pt_coordinate->u8_x_coordinate;
                            t_coordinate.u8_y_coordinate = pt_coordinate->u8_y_coordinate;
                            // Place a stone if they are
                            p_game_board->update_game_grid(&t_coordinate, u32_game_turn);
                            this->i32_sub_action=PLAYER_SUB_ACTION_YIELD_STEP;
                            // Allow fall-through to yield step
                        case PLAYER_SUB_ACTION_YIELD_STEP:
                            this->i32_sub_action = PLAYER_SUB_ACTION_PROMPT_STEP;
                            sub_actions_complete = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case PLAYER_ACTION_PASS:
                    this->i32_action = PLAYER_ACTION_PASS;
                    sub_actions_complete = true;
                    break;
                case PLAYER_ACTION_RESIGN:
                    this->i32_action = PLAYER_ACTION_RESIGN;
                    sub_actions_complete = true;
                    break;
                default:
                    break;
            }

            return sub_actions_complete;
        }
};


class ComputerPlayer: public GenericPlayer
{
    public:
        ComputerPlayer() { std::cout << "Constructing ComputerPlayer \n\r"; }
        ~ComputerPlayer() { std::cout << "Deconstructing HumanPlayer \n\r"; }
        bool process_action(GameBoard* p_game_board, uint32_t u32_game_turn, coordinate_t* pt_coordinate, int i32_action) override
        {
            // Analyze the game board
            // Play: (1) Place a stone, (2) Pass, or (3) Resign
            // Update display
            // Update the turn counter
            bool sub_actions_complete = true;
            return sub_actions_complete;
        }
};


#endif

