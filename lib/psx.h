#ifndef PSX_lib_h
#define PSX_lib_h

#include <avr/io.h>
#include <stdbool.h>


#define PSX_DATA_PORT   PORTD
#define PSX_SCK_PORT    PORTD
#define PSX_DC_PORT     PORTD
#define PSX_CS_PORT     PORTD

#define PSX_DATA_PINREG PIND

#define PSX_DATA_DDR    DDRD
#define PSX_SCK_DDR     DDRD
#define PSX_DC_DDR      DDRD
#define PSX_CS_DDR      DDRD

#define  PSX_SCK_PIN    4
#define  PSX_DC_PIN     7
#define  PSX_CS_PIN     5
#define  PSX_DATA_PIN   6

// задержка тактов SPI, us
#define CTRL_CLK        10

// кнопкм
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000

// джойстики
#define PSS_RX 5
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8


// инициализация геймпада
void psx_init(bool analog);

// получить состояние всех кнопок
uint16_t psx_buttons();

// получить состояниие кнопки (PSB_x)
uint8_t psx_button(uint16_t);

// получить состояниие оси джойстика (PSS_x)
uint8_t psx_stick(unsigned int);

// обновить состояние кнопок
void psx_read_gamepad();

#endif