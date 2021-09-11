#include "snake.hpp"
#include "hwlib-font-color-16x16.hpp"

#include "image_snake.cpp"

namespace game {
void snake::setup_game() {
    // init the random function with the start time
    srand(hwlib::now_ticks());

    // set the start position in the map
    map[head.x + (head.y * width)] = length + 2;

    // update screen where the head is
    write_screen_block(head.x + head.y * width, hwlib::green);

    // draw walls in the map and on the screen
    for (uint8_t i = 0; i < height; i++) {
        // set the edge to wall in the map
        map[i] = 1;

        // show a grey color on the wall position
        write_screen_block(i, hwlib::gray);

        // set the edge to wall in the map
        map[height * (width - 1) + i] = 1;

        // show a grey color on the wall position
        write_screen_block(height * (width - 1) + i, hwlib::gray);        
    }

    for (uint8_t i = 1; i < (height - 1); i++) {
        // set the edge to wall in the map
        map[i * width] = 1;

        // show a grey color on the wall position
        write_screen_block(i * width, hwlib::gray);

        // set the edge to wall in the map
        map[(width -1) + i * width] = 1;

        // show a grey color on the wall position
        write_screen_block((width -1) + i * width, hwlib::gray);
    }
    
    // spawn food in the game
    spawn_food();
}

uint8_t snake::move_direction(const uint8_t direction) {
    uint8_t hit = 0;

    // move the snake and check if we hit something
    switch (direction) {
        case 0:
            hit = move(-1, 0);
            break;
        case 1:
            hit = move(0, 1);
            break;
        case 2:
            hit = move(1, 0);
            break;
        case 3:
            hit = move(0, -1);
            break;     
    }

    // return if we hit something
    return hit;
}

void snake::screen_snake_update() {
    // loop over all the items in the map to check if we need to make the tail smaller
    for (int i = 0; i < height * width; i++) {
        // check if the item is part of the snake
        if (map[i] > 2) {
            // check if we need to remove the tail from the map/screen
            if (map[i] == 3) {
                // remove the tail from the screen
                map[i] = 0;

                // clear the image from the window
                write_screen_block(i, window.background);
            }
            else{
                // decrement the length on the map
                map[i]--;
            }
        }
    }
}

void snake::spawn_food() {
    // loop until we have a spot for food
    while (true) {
        // get a random x and y position for the food
        uint8_t x = rand() % (width - 2) + 1;
        uint8_t y = rand() % (height - 2) + 1;

        // check if the position is available
        if (map[x + (y * width)] == 0) {
            // set the map location to food
            map[x + (y * width)] = 2;

            // show the food to the screen
            write_screen_block(x + y * width, hwlib::red);

            // break the loop
            break;
        }
    }
}

uint8_t snake::move(const int8_t x, const int8_t y) {
    // calculate the new head position
    head.x += x;
    head.y += y;

    // update the snake on the screen
    write_screen_block(head.x + head.y * width, hwlib::green);

    // get the map data on the new location
    uint16_t map_data = map[head.x + head.y * width];

    // update the position on the map
    map[head.x + head.y * width] = length + 2;

    // check if we hit ourselfs or a wall
    if (map_data > 0 && map_data != 2) {
        // we hit ourselfs or a wall return a hit
        return 2;
    }

    // check if we got food
    if (map_data == 2) {
        return 1;
    }

    // return if we 
    return 0;
}

void snake::write_screen_block(const uint16_t block, const hwlib::color color) {
    // divide the screen size and the amount of blocks we want
    const uint8_t ty = 128 / height;
    const uint8_t tx = 128 / width;

    // calculate the remainder and modulo
    const uint16_t modulo = block / width;
    const uint8_t remainder = block % width;

    // write a scaled block of color to the screen
    for (uint8_t y = 0; y < ty; y++) {
        for(uint8_t x = 0; x < tx; x++) {
            // write to the window
            window.write(hwlib::location(remainder * tx + x, modulo * ty + y), color);
        }        
    }
}

void snake::change_direction(const uint8_t button_input) {
    // check if button 1 is pressed
    if (button_input & 0x1) {
        if (direction <= last_direction) {
            direction++;
        }
    }
    // check if button 2 is pressed
    if (button_input & 0x2) {
        if (direction >= last_direction) {
            direction--;
        }
    }
}

void snake::run() {
    //load start screen
    start_screen(); 

    // wait on any keypress
    while (buttons.get() == 0) {
        hwlib::wait_ms(25);
    }

    // wait until the keys are released
    while (buttons.get() != 0) {
        hwlib::wait_ms(25);
    }

    // clear the screen
    window.clear();

    // init the game
    setup_game();

    // flush the window to update the screen
    window.flush();

    // create a variable for the button state
    uint8_t button_state = 0;

    // get the current time for getting the target fps
    uint_fast64_t current_time = now_ms();

    // set the old time to the current time
    uint_fast64_t old_time = current_time;

    // loop until the snake dies
    while (true) {
        // check for user input
        uint8_t state = buttons.get(); 

        // check if user input is not the same as the old input
        if (state != button_state && state != 0x03) { 
            // update the button state
            button_state = state;

            // update the direction using the button input
            change_direction(button_state);
        }

        // check if we need to update the screen to reach the target fps
        if (current_time - old_time >= (1'000 / target_fps)) {
            last_direction = direction;

            // move the snake to the next position and get if we hit something
            uint8_t hit = move_direction(direction);

            // update the snake on the screen
            screen_snake_update();

            // check if we hit food
            if (hit == 1) {
                // increment the score
                score++;

                // increment the length
                length++;

                // spawn new food
                spawn_food();
            }
            // check if we hit something we should not
            else if (hit == 2) {
                // show the death screen
                death_screen();

                // clear the window
                window.clear();

                // break the loop to exit the function
                break;
            }

            // flush the screen
            window.flush();

            // update the old time to only update on the target fps
            old_time = current_time;
        }

        // wait 10 ms to not trigger to many button presses
        hwlib::wait_ms(10);

        // update the current time
        current_time = now_ms();
    }
}

void snake::start_screen() {
    // loop over all the pixels in the image to show the start screen
    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 128; x++) {
            // get the position of the data in the image array
            uint32_t j = (y * 128 * 3) + x * 3;

            // create a pixel out of the pixel data
            hwlib::color t{image_data_snake[j], image_data_snake[j+1], image_data_snake[j+2]}; 
            // hwlib::color t = hwlib::green;

            // write the pixel to the display
            window.write(hwlib::location(x, y), t);
        }        
    }
}

uint_fast64_t snake::now_ms() {
    // divide the us by 1'000 to get ms
    return hwlib::now_us() / 1'000;
}

uint8_t countdigits(uint32_t digit) {
    // return value
    uint8_t ret = 0;

    // loop until we are below 0
    for (; digit > 0; ret++) {
        // divide to get the data to 0
        digit /= 10;
    }

    // return the amount of times we had to divi
    return ret;
}

void snake::death_screen() {
    // loop until the length of the snake is 0
    for (uint16_t i = length; i > 0; i--) {
        // update the snake and make it 1 smaller
        screen_snake_update();

        // flush the window
        window.flush();

        // wait 1 frame time
        hwlib::wait_ms(500);
    }

    // wait 250 ms
    hwlib::wait_ms(250);

    // clear the screen
    window.clear();

    // wait 250 ms
    hwlib::wait_ms(250);

    // flush the clear to the window
    window.flush();

    // create a font for the text
    hwlib::font_color_16x16 font(window.foreground, window.background);

    // create a ostream object of the window to write text to the screen
    hwlib::window_ostream t_display(window, font);

    // print text on the display
    t_display << "\t0001" << "You died" << "\t0103" << "Score:" << "\t0204" ;
    
    // add enough zero's to fill 4 characters including the score
    for (int i = 0; i < 4 - countdigits(score); i++) {
        // print zero's to the screen
        t_display << "0";
    }
    
    // print the score and flush the display
    t_display << score << hwlib::flush;

    // wait on any keypress
    while (buttons.get() == 0) {
        hwlib::wait_ms(25);
    }

    // wait until the keys are released
    while (buttons.get() != 0) {
        hwlib::wait_ms(25);
    }
}
}