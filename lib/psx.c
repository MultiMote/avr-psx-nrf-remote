#include "psx.h"
#include <util/delay.h>

// буфер
static uint8_t _psx_data[21];

// отправить байт программным SPI и получить ответ
uint8_t _psx_gamepad_shift(uint8_t transmit_byte) {
    uint8_t received_byte = 0;
    for(uint8_t i = 0; i < 8; i++) {
        PSX_SCK_PORT &= ~_BV(PSX_SCK_PIN);

        if (transmit_byte & (_BV(i))) {
            PSX_DC_PORT |= _BV(PSX_DC_PIN);
        }
        else {
            PSX_DC_PORT &= ~_BV(PSX_DC_PIN);
        }

        _delay_us(CTRL_CLK);

        PSX_SCK_PORT |= _BV(PSX_SCK_PIN);

        if(PSX_DATA_PINREG & _BV(PSX_DATA_PIN)) {
            received_byte |= _BV(i);
        }

        _delay_us(CTRL_CLK);
    }

    PSX_SCK_PORT |= _BV(PSX_SCK_PIN);

    return received_byte;
}

// отправить команду
void _psx_send_command(uint8_t send_data[], uint8_t size){
    PSX_CS_PORT &= ~(_BV(PSX_CS_PIN));
    PSX_DC_PORT |= _BV(PSX_DC_PIN);

    PSX_SCK_PORT |= _BV(PSX_SCK_PIN);

    for (uint8_t i = 0; i < size; i++){
        send_data[i] = _psx_gamepad_shift(send_data[i]);
    }

    PSX_CS_PORT |= _BV(PSX_CS_PIN);
}

// обновить состояние кнопок
void psx_read_gamepad() {
    _psx_data[0] = 0x01;
    _psx_data[1] = 0x42;
    for (uint8_t i = 2; i < 21; i++){
        _psx_data[i] = 0x00;
    }
    _psx_send_command(_psx_data, 21);
}

// инициализация геймпада
void psx_init(bool analog){
    
    PSX_SCK_DDR |= _BV(PSX_SCK_PIN);
    PSX_CS_DDR |= _BV(PSX_CS_PIN);
    PSX_DC_DDR |= _BV(PSX_DC_PIN);
    
    PSX_DATA_DDR &= ~(_BV(PSX_DATA_PIN));
    PSX_DATA_PORT |= _BV(PSX_DATA_PIN);

    PSX_SCK_PORT |= _BV(PSX_SCK_PIN);
    PSX_DC_PORT |= _BV(PSX_DC_PIN);

    psx_read_gamepad();

    if(!analog) return;

    // войти в режим конфигурации
    uint8_t enter_config_command[] = {0x01, 0x43, 0x00, 0x01, 0x00};
    _psx_send_command(enter_config_command, 5);

    // заблокирвать геймпад в аналоговом режиме
    uint8_t lock_analog_mode_command[] = {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    _psx_send_command(lock_analog_mode_command, 9);
    
    // выйти из режима конфигурации
    uint8_t exit_config_command[] = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
    _psx_send_command(exit_config_command, 9);
}

// получить состояние всех кнопок
uint16_t psx_buttons() {
    uint16_t buttons = *(uint16_t*)(_psx_data + 3); // получить 2 байта, содержащих позиции данных 3 и 4
    return ~buttons;
}

// получить состояниие кнопки (PSB_x
uint8_t psx_button(uint16_t button) {
    uint16_t buttons = psx_buttons();
    return ((buttons & button) > 0);
}

// получить состояниие оси джойстика (PSS_x)
uint8_t psx_stick(unsigned int stick) {
    return _psx_data[stick];
} 