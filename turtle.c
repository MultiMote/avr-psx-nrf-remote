#include <avr/io.h>
#include "lib/nrf24.h"
#include <util/delay.h>
#include <stdbool.h>

//      ATMega8        nrf24
//     ________      _________
//    |    PC0 |----| CE      |
//    |    PC1 |----| CSN     |
//    |    PC2 |----| SCK     |
//    |    PC3 |----| MOSI    |
//    |    PC4 |----| MISO    |
//    |        |    |_________|
//    |        |
//    |        |             Шасси
//    |    PD0 |-(2)  (1)--[/\   /\]--(3)
//    |    PD1 |-(1)       |L|   |R|
//    |    PD2 |-(3)       |L|   |R|
//    |    PD3 |-(4)  (2)--[\/   \/]--(4)
//    |________|

uint8_t data_array[2];
uint8_t tx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};
uint8_t rx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

#define ENGINES_PORT            PORTD
#define ENGINES_DDR             DDRD

#define LEFT_CHASSIS_FWD_PIN    1
#define LEFT_CHASSIS_REV_PIN    0

#define RIGHT_CHASSIS_FWD_PIN   3
#define RIGHT_CHASSIS_REV_PIN   2

int main() {

    ENGINES_DDR |= 0b00001111;
    nrf24_init();

    nrf24_config(2, 2); // канал 2, размер пакета 2

    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);


    while (1) {

        if (nrf24_dataReady()) nrf24_getData(data_array);

        if (data_array[0] == 0xFE) { // проверка типа пакета
            bool left_fw = (data_array[1] & (1 << 1)) != 0;
            bool left_bck = (data_array[1] & (1 << 2)) != 0;
            bool right_fw = (data_array[1] & (1 << 3)) != 0;
            bool right_bck = (data_array[1] & (1 << 4)) != 0;
            ENGINES_PORT = (left_fw << LEFT_CHASSIS_FWD_PIN) | (right_fw << RIGHT_CHASSIS_FWD_PIN) |
                           (left_bck << LEFT_CHASSIS_REV_PIN) | (right_bck << RIGHT_CHASSIS_REV_PIN);
        }

        _delay_ms(20);

    }
}
/* ------------------------------------------------------------------------- */