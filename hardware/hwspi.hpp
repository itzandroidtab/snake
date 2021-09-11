#ifndef DUE_HWSPI_HPP
#define DUE_HWSPI_HPP

#include <hwlib.hpp>
#include <atmel\sam3xa\include\sam3xa.h>

#include "variant.h"

/**
 * @brief Due hardware spi library
 * 
 * @details Library for writing directly to the sam3x8e hardware spi
 * 
 */
class hwspi : public hwlib::spi_bus {
	protected:
        /**
         * @brief Configure a gpio pin for spi usage
         * 
         * @param pPio 
         * @param dwType 
         * @param dwMask 
         */
        void configure_pin(const uint32_t dwMask) {
            // get the mulitplexed peripherals
            uint32_t dwSR = PIOA->PIO_ABSR;

            // clear the mask from the peripheral
            PIOA->PIO_ABSR &= (~dwMask & dwSR);
            
            // enable the interrupts
            PIOA->PIO_PDR = dwMask;

            // disable the interrupt on the gpio
            PIOA->PIO_IDR = dwMask;

            // disable the pullup on the gpio
            PIOA->PIO_PUDR = dwMask;
        }

        /**
         * @brief Init the spi hardware
         * 
         * @param divider 
         * @param loopback 
         */
        void init(const uint8_t divider, const bool loopback) {
            // disable the hardware spi write protection
	        REG_SPI0_WPMR=0x53504900;

            // disable the spi hardware
            enable(false);

            // configure the gpio pins for spi
            configure_pin(PIO_PA28A_SPI0_NPCS0);
            configure_pin(PIO_PA25A_SPI0_MISO);
            configure_pin(PIO_PA26A_SPI0_MOSI);
            configure_pin(PIO_PA27A_SPI0_SPCK); 

            // check if the peripheral clock is already enabled
            if ((PMC->PMC_PCSR0 & (1u << ID_SPI0)) != (1u << ID_SPI0)) {
                // enable peripheral Clock
                PMC->PMC_PCER0 = 1 << ID_SPI0;
            }

            // execute a software reset twice
            reset();
            reset();

            // set spi configuration parameters.
            SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PS | SPI_MR_MODFDIS | loopback << 7;
            SPI0->SPI_CSR[0] = SPI_MODE0 | SPI_CSR_SCBR(divider) | SPI_CSR_DLYBCT(1);

            // enable the spi hardware
            enable(true);
        }

        /**
         * @brief Enables or disables the spi hardware
         * 
         * @param enable 
         */
        void enable(const bool enable = true) {
            // check if we need to enable the spi hardware
            if (enable) {
                // enable the spi hardware by writing to the control register
                SPI0->SPI_CR = SPI_CR_SPIEN;
            }
            else {
                // disable the hardware by writing to the control register 
            	SPI0->SPI_CR = SPI_CR_SPIDIS;
            }
        }

        /**
         * @brief Do a software reset on the spi hardware
         * 
         */
        void reset() {
            SPI0->SPI_CR = SPI_CR_SWRST;
        }

        /**
         * @brief Write one byte to the spi hardware
         * 
         * @details might wait until enough clock cycles have been send
         */
        void write(const uint8_t data) {
            // wait until the tx buffer is empty
            while ((SPI0->SPI_SR & SPI_SR_TXEMPTY) == 0) {
                // wait until we can write data
            }

            // write the data
            SPI0->SPI_TDR = data | SPI_PCS(0);

            // wait until the data is written
            while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0) {
                // wait until the write is done
            }
        }

        /**
         * @brief Read one byte from the spi hardware
         * 
         * @details might wait until enough clock cycles have been send
         */
        uint8_t read() {
            while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0) {
                // wait until data is ready
            }

            // return the last byte from the register
            return SPI0->SPI_RDR & 0xFF;
        }

        /**
         * @brief Write and read one byte of data
         * 
         * @param data 
         * @return uint8_t 
         */
        uint8_t read_write(const uint8_t data) {
            // write the data first
            write(data);

            // return the read data
            return read();
        }

	public:
		/**
		 * @brief Hardware spi constructor
		 * 
		 * @param divider the spi divider to calculate the spi speed (84 mhz / 21 = 4mhz\n)
		 * @param loopback to enable or disable the loopback in hardware. This connects the 
         * mosi to the miso if enabled
		 */
        hwspi(const uint8_t divider = 21, const bool loopback = 0) {
            // init the spi hardware
			init(divider, loopback);
		}

        /**
         * @brief write and read implementation for the hardware spi
         * 
         * @warning cs pin not used as hardware spi chip select. D10 is used instead 
         * 
         * @param cs 
         * @param amount
         * @param data_out 
         * @param data_in 
         */
		void write_and_read(hwlib::pin_out & cs, const size_t amount, const uint8_t *data_out, 
                            uint8_t *data_in) override {
            // loop until we wrote/read enough data
			for(size_t i = 0; i < amount; i++){
                // check if we have valid data in and out
                if (data_in && data_out) {
                    // write and read data to the spi bus
				    data_in[i] = read_write(data_out[i]);
                }
                else if (data_in) {
                    // read data into the data in array
                    data_in[i] = read();
                }
                else {
                    // write the data to the spi output
                    write(data_out[i]);
                }
			}
		}
};

#endif