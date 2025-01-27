#ifndef GAME_BOARD_H
#define GAME_BOARD_H


#include <iostream>
#include <stdint.h>


typedef struct coordinate
{
    uint8_t u8_x_coordinate;
    uint8_t u8_y_coordinate;
}coordinate_t;


class GameBoard
{
    // A game board is created once before the game starts
    // The same game board is destroyed once when the game ends
    public:
        // The following functions will be called once before the game starts
        // The properties that they set shall be immutable
        void set_grid_dimension(uint8_t u8_grid_dimension);
        void create_game_grid();
        void update_game_grid(coordinate_t* p_coordinate, uint32_t u32_value);
        // The following functions will be called once after the game ends
        void display_game_grid();
        void destroy_game_grid();
    private:
        uint8_t u8_grid_dimension;
        uint32_t* pu32_game_grid;
};


#endif

