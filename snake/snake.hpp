#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <hwlib.hpp>
#include <stdio.h>
#include <stdlib.h>  

#include "hwlib-font-color-16x16.hpp"

namespace game {
/**
 * @brief Snake game
 * 
 */
class snake {
    private:   
        /**
         * @brief XY struct for positioning
         * 
         */
        struct node{
            // x coordinate
            uint32_t x;
            
            // y coordinate
            uint32_t y;
        };  

        // height of the game
        constexpr static uint8_t height = 32;
        
        // width of the game
        constexpr static uint8_t width = 32;
    
        // map for all the positions of the game
        uint16_t map[height * width] = {};

        // window to show the game on
        hwlib::window & window;

        // game buttons
        hwlib::port_in_from_pins & buttons;

        // position of the head
        node head = {width / 2, height / 2};

        // length of the snake (start lenght is 3)
        uint16_t length = 3;

        // score of the game
        uint16_t score = 0;        

        // direction
        uint8_t direction:2;
        uint8_t last_direction:2;

        // target fps of the game
        const uint8_t target_fps = 5;

        /**
         * @brief Setup the game for playing
         * 
         */
        void setup_game();

        /**
         * @brief Get the current run time in ms
         * 
         * @return uint_fast64_t 
         */
        uint_fast64_t now_ms();

        /**
         * @brief Try to move the snake in a direction and return if we hit something
         * 
         * @param direction 
         * @return uint8_t 
         * 0 = no hit
         * 1 = got food
         * 2 = hit wall/ourselfs
         */
        uint8_t move_direction(const uint8_t direction);

        /**
         * @brief Move the snake to a relative position of the current position and return 
         * if we hit something.
         * 
         * @param x 
         * @param y 
         * @return uint8_t 
         * 0 = no hit
         * 1 = got food
         * 2 = hit wall/ourselfs
         */
        uint8_t move(const int8_t x, const int8_t y);

        /**
         * @brief Change the direction usint the button input
         * 
         * @param buttons 
         */
        void change_direction(const uint8_t buttons);        

        /**
         * @brief Write a whole screen block to the window
         * 
         * @param block 
         * @param c 
         */
        void write_screen_block(const uint16_t block, const hwlib::color color);

        /**
         * @brief Spawn food on the game map and on the screen
         * 
         */
        void spawn_food();        

        /**
         * @brief Update the snake by making it smaller and updating the screen
         * 
         */
        void screen_snake_update();

        /**
         * @brief Show the death screen of the game
         * 
         */
        void death_screen();

        /**
         * @brief Show the start screen of the game
         * 
         */
        void start_screen();

    public:
        /**
         * @brief The constructor for snake game
         * 
         * @param display the window the game is running on
         * @param buttons two buttons that control the snake
         */
        snake(hwlib::window & display, hwlib::port_in_from_pins & buttons):
            window(display), buttons(buttons),
            direction(0), last_direction(0)
        {}

        /**
         * @brief Run the game in an endless loop until the snake dies
         * 
         */
    	void run();
};
}

#endif