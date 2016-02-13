CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)

#============================================================================================

PROJECT(avr-psx-nrf-remote)

SET(DEVICE "atmega8")
SET(FREQ "8000000")

#============================================================================================

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CMAKE_C_FLAGS  "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -Wl,--gc-sections")

SET(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

#============================================================================================

set(LIB_SRC lib/nrf24.c lib/psx.c)

ADD_EXECUTABLE(host.elf host.c ${LIB_SRC})
ADD_EXECUTABLE(turtle.elf turtle.c ${LIB_SRC})

set_target_properties(turtle.elf PROPERTIES COMPILE_DEFINITIONS "TURTLE")

#============================================================================================

macro(MAKE_HEX elf)
    ADD_CUSTOM_COMMAND(TARGET ${elf} POST_BUILD COMMAND avr-objcopy -O ihex -R.eeprom ${BIN_DIR}/${elf} ${BIN_DIR}/${elf}.hex)
    ADD_CUSTOM_COMMAND(TARGET ${elf} POST_BUILD COMMAND avr-size ${BIN_DIR}/${elf} --mcu=${DEVICE} --format=avr)
endmacro()


MAKE_HEX(host.elf)
MAKE_HEX(turtle.elf)




