#include <hwlib.hpp>

#include "snake.hpp"
#include "hwspi.hpp"
#include "hwlib_ssd1351.hpp"

int main() {
    // kill the watchdog (ATSAM3X8E specific)
    WDT->WDT_MR = WDT_MR_WDDIS;

    // Update the cpu frequency by sleeping (hwlib thing)
    hwlib::wait_ms(1);

    // create the reset output
    auto reset = hwlib::target::pin_out(hwlib::target::pins::d8);
    
    // create the data/command output
    auto dc = hwlib::target::pin_out(hwlib::target::pins::d9);
    
    // create the chipselect
    auto cs = hwlib::target::pin_out(hwlib::target::pins::d10);

    // create the hardware spi bus at 84/5 = 16.8mhz
    auto bus = hwspi(5);

    // create the display object from the pins and spi bus
    auto display = hwlib_ssd1351(bus, reset, dc, cs);

    // set the fore/background
    display.foreground = hwlib::white;
    display.background = hwlib::black;

    // clear the display using the background
    display.clear();

    // create the buttons for the user input
    auto left_button = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto right_button = hwlib::target::pin_in(hwlib::target::pins::d3);    
    
    // create a port from multiple buttons
    auto buttons = hwlib::port_in_from_pins(left_button, right_button);

    // create the game 
    auto snake = game::snake(display, buttons);

    // run the snake game
    snake.run();

    // only happens if the snake game ends
    while (true) {
        // loop until we die
    }
}