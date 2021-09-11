#ifndef SSD1351_HPP
#define SSD1351_HPP

#include <stdint.h>
#include "hwlib.hpp"

/**
 * @brief SSD1351 Library
 * 
 */
class ssd1351{
    protected:  
        // hwlib spi bus 
        hwlib::spi_bus &spi;
        
        // reset pin for the oled screen
        hwlib::pin_out &reset;
        
        // data/command pin for the screen
        hwlib::pin_out &dc; 
        
        // chip select pin of the screen
        hwlib::pin_out &cs;

        void write_command(const uint8_t *data, const uint32_t size);

        void write_data(const uint8_t *data, const uint32_t size);   

        void write_command(const uint8_t data);

        void write_data(const uint8_t data);   

    public:
        /**
         * @brief Construct a new ssd1351 object
         * 
         * @param spi the spi bus
         * @param reset the reset pin
         * @param dc the data/command pin
         * @param cs the chip select pin
         */
        ssd1351(hwlib::spi_bus &spi, hwlib::pin_out &reset, 
                hwlib::pin_out &dc, hwlib::pin_out &cs);

        /**
         * @brief Write screen data to the screen
         * 
         * @param data data to write
         * @param size amount of data to write
         */
        void write_screen_data(uint8_t *data, uint32_t size);

        /**
         * @brief set clock divider
         * 
         * @param divider clock divider and oscillator frequency
         */
        void set_clock_divider(const uint8_t divider, const uint8_t frequency);

        /**
         * @brief Set the gpio output
         * 
         * @param gpio gpio0 and gpio1 output state
         */
        void set_gpio(const uint8_t gpio);

        /**
         * @brief Set the oled phase lengths
         * 
         * @param phase1 phase 1 clock
         * @param phase2 phase 2 clock
         */
        void set_phase_lenght(const uint8_t phase1, const uint8_t phase2);

        /**
         * @brief Set the display offset
         * 
         * @details For this command you need to enable the poweroptions
         * 
         * @param offset display offset
         */
        void set_display_offset(const uint8_t offset);
        
        /**
         * @brief Set the display startline offset
         * 
         * @param startline display start line
         */
        void set_display_startline(const uint8_t startline);

        /**
         * @brief Set the interface registers
         * 
         * @param regulator enable or disable the internal regulator during sleep
         * 0b0 = Disable internal regulator during sleep\n
         * 0b1 = Enable internal regulator 
         * @param interface select the interface
         * 0b00 = Select 8-bit parallel interface
         * 0b01 = Select 16-bit parallel interface
         * 0b11 = Select 18-bit parallel interface
         */
        void set_interface_registers(const bool regulator, const uint8_t interface);
        
        /**
         * @brief Set the command lock
         * 
         * @param lock lock or unlock the command lock
         */
        void set_command_lock(const bool lock);
        
        /**
         * @brief change the power options
         * 
         * @details Allow access to commands: 0xA2, 0xB1, 0xB3, 0xBB, 0xBE, 0xC1
         * 
         * @warning use with caution. 
         * 
         * @param allow_commands enable or disable commands
         */
        void enable_power_options(const bool allow_commands);
        
        /**
         * @brief Set the color contrast
         * 
         * @param a contrast for color A
         * @param b contrast for color B
         * @param c contrast for color C
         */
        void set_color_contrast(const uint8_t a, const uint8_t b, const uint8_t c);
        
        /**
         * @brief Set the master contrast
         * 
         * @param contrast master contrast
         */
        void set_master_contrast(const uint8_t contrast);
        
        /**
         * @brief Set the mux ratio
         * 
         * @param ratio mux ratio
         */
        void set_mux_ratio(const uint8_t ratio);
        
        /**
         * @brief Set the colum address
         * 
         * @param start_address start address
         * @param end_address end address
         */
        void set_column_address(const uint8_t start_address, const uint8_t end_address);
        
        /**
         * @brief Set the row address
         * 
         * @param start_address start address
         * @param end_address end address
         */
        void set_row_address(const uint8_t start_address, const uint8_t end_address);
        
        /**
         * @brief Set the display mode
         * 
         * @param mode screen mode
         * 0b00 = All off
         * 0b01 = All on
         * 0b10 = Reset to normal display
         * 0b11 = Inverse display
         */
        void set_display_mode(const uint8_t mode);
        
        /**
         * @brief set the sleep mode of the screen
         * 
         * @param sleep enable/disable sleep mode
         */
        void set_sleep_mode(const bool sleep);
        
        /**
         * @brief Set the ReCo object
         * 
         * @param increment increment settings
         * 0b0 = Horizontal address increment
         * 0b1 = Vertical address increment
         * @param map display map settings
         * 0b0 = Column address 0 is mapped to SEG0
         * 0b1 = Column address 127 is mapped to SEG0
         * @param sequence color sequence
         * 0b0 = Color sequence A -> B -> C
         * 0b1 = Color sequence C -> B -> A   
         * @param scan scan direction
         * 0b0 = Scan from COM0 to COM[n-1]
         * 0b1 = Scan from COM[n-1] to COM0
         * @param split display split
         * 0b0 = Disable COM split odd even
         * 0b1 = Enable COM split odd even  
         * @param depth display color depth
         * 0b00 = 65K color depth
         * 0b01 = 65K color depth
         * 0b10 = 262K color depth
         * 0b11 = 262K color depth, 16 bit format 2
         */
        void set_re_co(const bool increment, const bool map, const bool sequence, const bool scan, 
                      const bool split, const uint8_t depth);

        /**
         * @brief Set external vsl
         * 
         * @param vsl vsl mode
         * 0b00 = External VSL [reset]
         * 0b01, 0b10, 0b11 = invalid
         */
        void set_external_vsl(const uint8_t vsl);

        /**
         * @brief Set COM deselect voltage level
         * 
         * @param voltage com deselect voltage
         * 0b000 = 0x00 = 0.72 x VCC
         * 0b101 = 0x05 = 0.82 x VCC [reset]
         * 0b111 = 0x07 = 0.86 x VCC
         */
        void set_com_deselect_voltage(const uint8_t voltage);

        /**
         * @brief Set second precharge period
         * 
         * @param precharge second precharge period
         * 0b0000 = invalid
         * 0b0001 = 1 DCLKS
         * 0b0010 = 2 DCLKS
         * 0b1111 = 15 DCLKS
         */
        void set_second_precharge(const uint8_t precharge);
};

#endif