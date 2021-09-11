#ifndef HWLIB_SSD1351_HPP
#define HWLIB_SSD1351_HPP

#include <hwlib.hpp>
#include "ssd1351.hpp"

class hwlib_ssd1351: public hwlib::window {
    protected:
        // display driver
        ssd1351 display;

        // height and width of the screen
        const uint8_t height = 128;
        const uint8_t width = 128;

        // last cursor position
    	uint8_t x;
    	uint8_t y;

        /**
         * @brief Set the cursor position
         * 
         * @param x 
         * @param y 
         */
        void set_cursor(const uint8_t x, const uint8_t y) {
            // check if we need to update the cursor
            if (this->x != x) {
                // update the column address
                display.set_column_address(x, width - 1);
            }
            if (this->y != y) {
                // update the column address
                display.set_row_address(y, height - 1);
            }

            // update the cursor position
            this->x = x;
            this->y = y;
        }

        /**
         * @brief Convert a color to a uint16_t
         * 
         * @param color 
         * @return uint16_t 
         */
        uint16_t color_to_data(const hwlib::color &color) {
            return (((uint16_t(color.red) * 0x1F) / 0xFF) << 11) | 
                   (((uint16_t(color.green) * 0x3F) / 0xFF) << 5) |
                   ((uint16_t(color.blue) * 0x1F) / 0xFF);
        }

    public:
        hwlib_ssd1351(hwlib::spi_bus & spi, hwlib::pin_out & reset, hwlib::pin_out & dc, hwlib::pin_out & cs):
            hwlib::window(hwlib::location(height, width), hwlib::black, hwlib::white),
            display(spi, reset, dc, cs), x(0), y(0)
        { 
            // disable command lock
            display.set_command_lock(0);
            // enable access to commands
            display.enable_power_options(1);
            // turn sleep on(screen off)	
            display.set_sleep_mode(1);
            // set clock
            display.set_clock_divider(0xF, 0X1);
            // set mux ratio    
            display.set_mux_ratio(0x7F);
            // set multiple screen settings
            display.set_re_co(0, 0, 1, 1, 1, 0x01);
            // set start line(height)
            display.set_display_startline(0);
            // set offset(width)
            display.set_display_offset(0);
            // set gpio
            display.set_gpio(0);
            // set power options and interface registers
            display.set_interface_registers(1, 0x00);
            // set phases
            display.set_phase_lenght(0x2, 0x3);
            // set vsl
            display.set_external_vsl(0);
            // set com deselect voltage
            display.set_com_deselect_voltage(0x05);
            // set master contrast
            display.set_master_contrast(0x0F);
            // set second precharge period
            display.set_second_precharge(0x01);
            // reset display commands
            display.set_display_mode(0x2);
            // set color contrast
            display.set_color_contrast(0xFF, 0xFF, 0xFF);
            // set column to 0
            display.set_column_address(0, width - 1);
            // set row to 0
            display.set_row_address(0, height - 1);
            // turn sleep off(screen on) 
            display.set_sleep_mode(0);
        }

        /**
         * @brief the write implementation for the oled screen
         * 
         * @param pos the location of the pixel
         * @param col the color of the pixel
         * @param buf un/buffered 
         */
        void write_implementation(hwlib::location pos, hwlib::color col, hwlib::buffering buf = hwlib::buffering::unbuffered) override {
            // get binary data from a color
            uint16_t color = color_to_data(col);

            // convert the uint16_t to a array
            uint8_t buffer[2] = {uint8_t(color >> 8), uint8_t(color & 0xFF)};

            // update the screen cursor
            set_cursor(pos.x, pos.y);

            // write the screen data
            display.write_screen_data(buffer, sizeof(buffer));

            // update the cursor
            if (x < (width - 1)) {
                // increment the row to the next
                x++;
            }
            else if (y < (height - 1)) {
                // reset the x cursor position
                x = 0;

                // change cursor to the beginning 
                display.set_column_address(0, width - 1);

                // increment the column to the next
                y++;
            }
            else {
                // we reached the end of the screen reset the row and column
                x = 0;
                y = 0;

                // set the cursor to the start
                display.set_column_address(0, width - 1);
                display.set_row_address(0, height - 1);
            }
        }

        /**
         * @brief clears the screen
         * 
         * @param buf 
         */
        void clear(hwlib::buffering buf = hwlib::buffering::unbuffered) override {
            // get binary data from a color
            uint16_t color = color_to_data(window::background);

            // convert the uint16_t to a array
            uint8_t buffer[2] = {uint8_t(color >> 8), uint8_t(color & 0xFF)};

            // update the screen cursor
            set_cursor(0, 0);

            // loop over all the pixels in the screen
            for (uint16_t i = 0; i < (height * width); i++) {
                // write the color to the screen
                display.write_screen_data(buffer, sizeof(buffer));
            }
        }

        /**
         * @brief flushes the screen.
         * 
         * @warning Not implemented as a buffer is needed for this.
         * 
         */
        void flush() override {
            // do nothjing
        }      
};

#endif