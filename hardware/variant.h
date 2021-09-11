#ifndef DUE_VARIANT_HPP
#define DUE_VARIANT_HPP

#include "atmel\sam3xa\include\sam3xa.h"

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		12000000

/** Master clock frequency */
#define VARIANT_MCK			84000000

// Spi Modes
/** SPI_MODE0 */
#define SPI_MODE0 0x02
/** SPI_MODE1 */
#define SPI_MODE1 0x00
/** SPI_MODE2 */
#define SPI_MODE2 0x03
/** SPI_MODE3 */
#define SPI_MODE3 0x01

// Number of pins defined in PinDescription array
/** MAX_PERIPH_ID */
# define MAX_PERIPH_ID    44
/** PINS_COUNT */
#define PINS_COUNT           (79u)
/** NUM_DIGITAL_PINS */
#define NUM_DIGITAL_PINS     (66u)
/** NUM_ANALOG_INPUTS */
#define NUM_ANALOG_INPUTS    (12u)

// LEDs
/** PIN_LED_13 */
#define PIN_LED_13           (13u)
/** PIN_LED_RXL */
#define PIN_LED_RXL          (72u)
/** PIN_LED_TXL */
#define PIN_LED_TXL          (73u)
/** PIN_LED */
#define PIN_LED              PIN_LED_13
/** PIN_LED2 */
#define PIN_LED2             PIN_LED_RXL
/** PIN_LED3 */
#define PIN_LED3             PIN_LED_TXL
/** LED_BUILTIN */
#define LED_BUILTIN          13

/*
 * SPI Interfaces
 */
/** SPI_INTERFACES_COUNT */
#define SPI_INTERFACES_COUNT 1

/** SPI_INTERFACE */
#define SPI_INTERFACE        SPI0
/** SPI_INTERFACE_ID */
#define SPI_INTERFACE_ID     ID_SPI0
/** SPI_CHANNELS_NUM */
#define SPI_CHANNELS_NUM 4
/** PIN_SPI_SS0 */
#define PIN_SPI_SS0          (77u)
/** PIN_SPI_SS1 */
#define PIN_SPI_SS1          (87u)
/** PIN_SPI_SS2 */
#define PIN_SPI_SS2          (86u)
/** PIN_SPI_SS3 */
#define PIN_SPI_SS3          (78u)
/** PIN_SPI_MOSI */
#define PIN_SPI_MOSI         (75u)
/** PIN_SPI_MISO */
#define PIN_SPI_MISO         (74u)
/** PIN_SPI_SCK */
#define PIN_SPI_SCK          (76u)
/** BOARD_SPI_SS0 */
#define BOARD_SPI_SS0        (10u)
/** BOARD_SPI_SS1 */
#define BOARD_SPI_SS1        (4u)
/** BOARD_SPI_SS2 */
#define BOARD_SPI_SS2        (52u)
/** BOARD_SPI_SS3 */
#define BOARD_SPI_SS3        PIN_SPI_SS3
/** BOARD_SPI_DEFAULT_SS */
#define BOARD_SPI_DEFAULT_SS BOARD_SPI_SS3

static const uint8_t SS   = BOARD_SPI_SS0;
static const uint8_t SS1  = BOARD_SPI_SS1;
static const uint8_t SS2  = BOARD_SPI_SS2;
static const uint8_t SS3  = BOARD_SPI_SS3;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

/// \brief
/// EPioType pin
typedef enum _EPioType
{
  PIO_NOT_A_PIN, /* Not under control of a peripheral. */
  PIO_PERIPH_A, /* The pin is controlled by the associated signal of peripheral A. */
  PIO_PERIPH_B, /* The pin is controlled by the associated signal of peripheral B. */
  PIO_INPUT, /* The pin is an input. */
  PIO_OUTPUT_0, /* The pin is an output and has a default level of 0. */
  PIO_OUTPUT_1 /* The pin is an output and has a default level of 1. */
} EPioType ;

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

/** Calculate the PCS field value given the chip select NPCS value */
#define SPI_PCS(npcs)       ((~(1 << (npcs)) & 0xF) << 16)

/** Calculates the value of the CSR SCBR field given the baudrate and MCK. */
#define SPI_SCBR(baudrate, masterClock) ((uint32_t) ((masterClock) / (baudrate)) << 8)

/** Calculates the value of the CSR DLYBS field given the desired delay (in ns) */
#define SPI_DLYBS(delay, masterClock) ((uint32_t) ((((masterClock) / 1000000) * (delay)) / 1000) << 16)

/** Calculates the value of the CSR DLYBCT field given the desired delay (in ns) */
#define SPI_DLYBCT(delay, masterClock) ((uint32_t) ((((masterClock) / 1000000) * (delay)) / 32000) << 24)

/**  Default pin configuration (no attribute). */
#define PIO_DEFAULT                 (0u << 0)
/**  The internal pin pull-up is active. */
#define PIO_PULLUP                  (1u << 0)
/**  The internal glitch filter is active. */
#define PIO_DEGLITCH                (1u << 1)
/**  The pin is open-drain. */
#define PIO_OPENDRAIN               (1u << 2)

/**  The internal debouncing filter is active. */
#define PIO_DEBOUNCE                (1u << 3)

/**  Enable additional interrupt modes. */
#define PIO_IT_AIME                 (1u << 4)

/**  Interrupt High Level/Rising Edge detection is active. */
#define PIO_IT_RE_OR_HL             (1u << 5)
/**  Interrupt Edge detection is active. */
#define PIO_IT_EDGE                 (1u << 6)

/**  Low level interrupt is active */
#define PIO_IT_LOW_LEVEL            (0               | 0 | PIO_IT_AIME)
/**  High level interrupt is active */
#define PIO_IT_HIGH_LEVEL           (PIO_IT_RE_OR_HL | 0 | PIO_IT_AIME)
/**  Falling edge interrupt is active */
#define PIO_IT_FALL_EDGE            (0               | PIO_IT_EDGE | PIO_IT_AIME)
/**  Rising edge interrupt is active */
#define PIO_IT_RISE_EDGE            (PIO_IT_RE_OR_HL | PIO_IT_EDGE | PIO_IT_AIME)

/** Board pin to spi pin */
#define BOARD_PIN_TO_SPI_PIN(x) \
	(x==BOARD_SPI_SS0 ? PIN_SPI_SS0 : \
	(x==BOARD_SPI_SS1 ? PIN_SPI_SS1 : \
	(x==BOARD_SPI_SS2 ? PIN_SPI_SS2 : PIN_SPI_SS3 )))
/** Board pin to spi channel */
#define BOARD_PIN_TO_SPI_CHANNEL(x) \
	(x==BOARD_SPI_SS0 ? 0 : \
	(x==BOARD_SPI_SS1 ? 1 : \
	(x==BOARD_SPI_SS2 ? 2 : 3)))

// Interrupts
/** digital pin to interupt*/
#define digitalPinToInterrupt(p)  ((p) < NUM_DIGITAL_PINS ? (p) : -1)
/** analog input to digital pin */
#define analogInputToDigitalPin(p)  ((p < 12u) ? (p) + 54u : -1)
/** digital pin to port */
#define digitalPinToPort(P)        ( g_APinDescription[P].pPort )
/** digital pin to bitmask */
#define digitalPinToBitMask(P)     ( g_APinDescription[P].ulPin )
/** port output register */
#define portOutputRegister(port)   ( &(port->PIO_ODSR) )
/** port input register */
#define portInputRegister(port)    ( &(port->PIO_PDSR) )
/** check if pin has pwm */
#define digitalPinHasPWM(P)        ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )

#endif