#include "ssd1351.hpp"

ssd1351::ssd1351(hwlib::spi_bus & spi, hwlib::pin_out & reset, hwlib::pin_out & dc, hwlib::pin_out & cs): spi(spi), reset(reset), dc(dc), cs(cs) {
    // wake the oled screen for the reset
    cs.set(false); 

    // reset the oled screen
    reset.set(true);

    // wait a bit for the oled screen to register the cs update
    hwlib::wait_ms(10);

    // reset the oled screen (resets when reset is low)
    reset.set(false); 

    // wait a bit until the oled screen has fully resetted
    hwlib::wait_ms(10);

    // clear the reset
    reset.set(true);

    // wait a bit for the oled screen to turn back on
    hwlib::wait_ms(10);
};


void ssd1351::write_command(const uint8_t *data, const uint32_t size) {
    // clear the dc pin for a command
    dc.set(false);

    //Write commands to the spi interface
    spi.write_and_read(cs , size, data, nullptr);
}

void ssd1351::write_data(const uint8_t *data, const uint32_t size) {
    // set the dc pin for data
    dc.set(true);

    //Write data to the spi interface
    spi.write_and_read(cs , size, data, nullptr);
}

void ssd1351::write_command(const uint8_t data) {
    write_command(&data, 1);
}

void ssd1351::write_data(const uint8_t data) {
    write_data(&data, 1);
}

void ssd1351::write_screen_data(uint8_t *data, uint32_t size) {
    // 0x5C = command for screen data
    write_command(0x5C);

    // write the display data
    write_data(data, size);
}

void ssd1351::set_clock_divider(const uint8_t divider, const uint8_t frequency) {
    // 0xB3 = command for changing the front clock divider
    // d0:d3 = clock divider
    // d4:d7 = oscillator frequency    
    write_command(0xB3);

    // write the clock divider
    write_data((frequency << 4) | (divider & 0xF));
}

void ssd1351::set_gpio(const uint8_t gpio) {
    // 0xB5 = command for setting gpio
    write_command(0xB5);

    // write the states
    write_data(gpio & 0x0F);
}

void ssd1351::set_phase_lenght(const uint8_t phase1, const uint8_t phase2) {
    // 0xB1 = command for changing phase periods
    // A0:A3 = Phase 1 clocks
    // A4:A7 = Phase 2 clocks    
    write_command(0xB1);
    // write the phase lengths
    write_data(uint8_t((phase2 << 4) | (phase1 & 0x0F)));
}

void ssd1351::set_display_offset(const uint8_t offset) {
    // 0xA2 = command for display offset
    write_command(0xA2);

    // write the offset
    write_data(offset & 0x7F);
}

void ssd1351::set_display_startline(const uint8_t startline) {
    // 0xA1 = command for setting the starting line
    write_command(0xA1);
    
    // write the start line
    write_data(startline & 0x7F);
}

void ssd1351::set_interface_registers(const bool regulator, const uint8_t interface) {
    // 0xAB = command to set the interface type and to enable and disable the internal regulator during sleep
    // A0 = 0b0 = Disable internal regulator during sleep
    // A0 = 0b1 = Enable internal regulator
    // A6:7 = 0b00 = Select 8-bit parallel interface
    // A6:7 = 0b01 = Select 16-bit parallel interface
    // A6:7 = 0b11 = Select 18-bit parallel interface
    write_command(0xAB);
    
    // write the interface and regulator
    write_data(uint8_t((interface & 0x03) << 6 | regulator));
}

void ssd1351::set_command_lock(const bool lock) {
    // 0xFD = command for setting command lock without this unlocked the SSD1351 wont respond to commands and doesn't give memory access
    // 0x12 = disable command lock
    // 0x16 = enable command lock
    write_command(0xFD);
    
    write_data(lock ? 0x16 : 0x12);
}

void ssd1351::enable_power_options(const bool allow_commands) {
    // Normaly you dont need these commands use with caution
    // enable to use commands 0xA2, 0xB1, 0xB3, 0xBB, 0xBE, 0xC1 these commands are necessary certain operations look at the datasheet for more info
    // 0xFD = command for setting command lock without this unlocked the SSD1351 wont respond to commands and doesn't give memory access
    // 0xB0 = dont allow access to the commands
    // 0xB1 = allow access to the commands
    write_command(0xFD);
    
    write_data(0xB0 | uint8_t(allow_commands));
}

void ssd1351::set_color_contrast(const uint8_t a, const uint8_t b, const uint8_t c) {
    // 0xC1 = command for setting color contrast
    // For this command you need to enable the poweroptions
    write_command(0xC1);

    uint8_t tmp[] = {a, b, c};
    write_data(tmp, sizeof(tmp));
}

void ssd1351::set_master_contrast(const uint8_t contrast) {
    // 0xC7 = command for setting master contrast
    // the higher M is the more the output current is limited
    // 0x00 = reduce output currents for all color to 1/16
    //        ...
    // 0x0E = reduce output currents for all color to 15/16
    // 0x0F = no change
    write_command(0xC7);
    
    write_data(uint8_t(contrast & 0x0F));
}

void ssd1351::set_mux_ratio(const uint8_t ratio) {
    // 0xCA = command for mux ratio setting
    write_command(0xCA);
    
    write_data(ratio & 0x7F);
}

void ssd1351::set_column_address(const uint8_t start_address, const uint8_t end_address) {
    // 0x15 = command for setting the column 
    // second byte is start adress
    // third byte is end adress
    write_command(0x15);

    uint8_t tmp[] = {uint8_t(start_address & 0x7F), uint8_t(end_address & 0x7F)};
    write_data(tmp, sizeof(tmp));
}

void ssd1351::set_row_address(const uint8_t start_address, const uint8_t end_address) {
    // 0x75 = command for setting the row
    // second byte is start adress
    // third byte is end adress
    write_command(0x75);

    uint8_t tmp[] = {uint8_t(start_address & 0x7F), uint8_t(end_address & 0x7F)};
    write_data(tmp, sizeof(tmp));
}

void ssd1351::set_display_mode(const uint8_t mode) {
    // 0xA4:0xA7 = commands for the display modes
    // 0xA4 = All off
    // 0xA5 = All on
    // 0xA6 = reset to normal display
    // 0xA7 = Inverse Display
    write_command(uint8_t(0xA4 | (mode & 0x03)));
}

void ssd1351::set_sleep_mode(const bool sleep) {
    // 0xAE:0xAF = command for sleep state
    // 0xAE = Sleep mode on
    // 0xAF = Sleep mode off
    write_command(0xAF - sleep);
}

void ssd1351::set_re_co(const bool increment, const bool map, const bool sequence, const bool scan, 
                        const bool split, const uint8_t depth) {
    // Set Remap/ Color depth
    // A0 = 0b0 = Horizontal address increment
    // A0 = 0b1 = Vertical address increment
    // A1 = 0b0 = Column address 0 is mapped to SEG0
    // A1 = 0b1 = Column address 127 is mapped to SEG0
    // A2 = 0b0 = Color sequence A -> B -> C
    // A2 = 0b1 = Color sequence C -> B -> A
    // A3 = Reserved
    // A4 = 0b0 Scan from COM0 to COM[n-1]
    // A4 = 0b1 Scan from COM[n-1] to COM0
    // A5 = 0b0 Disable COM split odd even
    // A5 = 0b1 Enable COM split odd even
    // A6:7 = 0b00 = 65K color depth
    // A6:7 = 0b01 = 65K color depth
    // A6:7 = 0b10 = 262K color depth
    // A6:7 = 0b11 = 262K color depth, 16 bit format 2
    write_command(0xA0);

    // write the settings
    write_data(((depth & 0x03) << 6) | (split << 5) | (scan << 4) | 
               (sequence << 2) | (map << 1) | increment); 
}


void ssd1351::set_external_vsl(const uint8_t v) {
    // 0xB4 = command for vsl
    // 0b00 = External VSL [reset]
    // 0b01, 0b10, 0b11 = invalid    
    write_command(0xB4);

    uint8_t tmp[] = {uint8_t(0xA0 | (v & 0x03)), 0xB5, 0x55};
    write_data(tmp, sizeof(tmp));
}

void ssd1351::set_com_deselect_voltage(const uint8_t voltage) {
    // 0xBE = com deselect voltage command
    // 0b000 = 0x00 = 0.72 x VCC\n
    // 0b101 = 0x05 = 0.82 x VCC [reset]\n
    // 0b111 = 0x07 = 0.86 x VCC\n
    write_command(0xBE);
    
    // write the voltage
    write_data(voltage & 0x07);
}

void ssd1351::set_second_precharge(const uint8_t precharge) {
    // 0xB6 = second pre-charge period command
    // 0b0000 = invalid\n
    // 0b0001 = 1 DCLKS\n
    // 0b0010 = 2 DCLKS\n
    // 0b1111 = 15 DCLKS\n    
    write_command(0xB6);

    // write precharge
    write_data(precharge & 0x0F);
}