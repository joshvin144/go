#include "game_board.h"


void GameBoard::set_grid_dimension(uint8_t u8_grid_dimension)
{
    this->u8_grid_dimension = u8_grid_dimension;
}


void GameBoard::create_game_grid()
{
    this->pu32_game_grid = new uint32_t[this->u8_grid_dimension * this->u8_grid_dimension];
}


void GameBoard::update_game_grid(coordinate_t* p_coordinate, uint32_t u32_value)
{
    this->pu32_game_grid[(p_coordinate->u8_x_coordinate*this->u8_grid_dimension) + p_coordinate->u8_y_coordinate] = u32_value;
}


void GameBoard::display_game_grid()
{
    for(uint8_t i = 0; i <= (this->u8_grid_dimension - (uint8_t)1u); i++)
    {
        for(uint8_t j = 0; j <= (this->u8_grid_dimension - (uint8_t)1u); j++)
        {
            // One dimensional array representing two dimensional data
            std::cout << this->pu32_game_grid[(i*this->u8_grid_dimension) + j] << " ";
        }
        std::cout << "\n\r";
    }
}


void GameBoard::destroy_game_grid()
{
    delete this->pu32_game_grid;
}