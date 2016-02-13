#define NRF24_IO_PORT PORTC
#define NRF24_IO_PINREG PINC
#define NRF24_IO_DDR DDRC

#include <avr/io.h>
#include <util/delay.h>
#include "lib/nrf24.h"
#include "lib/psx.h"

//      ATMega8        nrf24
//     ________      _________
//    |    PC0 |----| CE      |
//    |    PC1 |----| CSN     |
//    |    PC2 |----| SCK     |
//    |    PC3 |----| MOSI    |
//    |    PC4 |----| MISO    |
//    |        |    |_________|
//    |        |
//    |        |      Геймпад
//    |        |     _________
//    |    PD4 |----| SCK     |
//    |    PD5 |----| CS      |
//    |    PD6 |----| DATA    |
//    |    PD7 |----| DC      |
//    |________|    |_________|


uint8_t data_array[2];
uint8_t tx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
uint8_t rx_address[5] = {0xD7, 0xD7, 0xD7, 0xD7, 0xD7};


int main() {

    nrf24_init();

    nrf24_config(2, 2); // канал 2, размер пакета 2

    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);

    psx_init(false);

    while (1) {

        psx_read_gamepad(); // чтение геймпада

        data_array[0] = 0xFE; // проверочное значение

        // вверх       - левое  шасси вперёд
        // вниз        - левое  шасси назад
        // треугольник - правое шасси вперёд
        // крест       - правое шасси назад
        data_array[1] = (psx_button(PSB_PAD_UP) << 1) | (psx_button(PSB_PAD_DOWN) << 2)
                        | (psx_button(PSB_TRIANGLE) << 3) | (psx_button(PSB_CROSS) << 4); // запихиваем кнопки

        nrf24_send(data_array); // отправляем данные
        while (!nrf24_isSending());

        _delay_ms(20);

    }
}
