cmake_minimum_required(VERSION 3.21)
project(cw32f030c8t6 C)

set(CMAKE_C_STANDARD 99)

include_directories(Libraries/inc)
include_directories(User/inc)
include_directories(C:/Users/Legend/AppData/Local/Arm/Packs/ARM/CMSIS/5.8.0/CMSIS/Core/Include)

add_executable(cw32f030c8t6
        Libraries/inc/base_types.h
        Libraries/inc/cw32f030.h
        Libraries/inc/cw32f030_adc.h
        Libraries/inc/cw32f030_atim.h
        Libraries/inc/cw32f030_awt.h
        Libraries/inc/cw32f030_btim.h
        Libraries/inc/cw32f030_crc.h
        Libraries/inc/cw32f030_debug.h
        Libraries/inc/cw32f030_digitalsign.h
        Libraries/inc/cw32f030_dma.h
        Libraries/inc/cw32f030_flash.h
        Libraries/inc/cw32f030_gpio.h
        Libraries/inc/cw32f030_gtim.h
        Libraries/inc/cw32f030_i2c.h
        Libraries/inc/cw32f030_iwdt.h
        Libraries/inc/cw32f030_lvd.h
        Libraries/inc/cw32f030_pwr.h
        Libraries/inc/cw32f030_ram.h
        Libraries/inc/cw32f030_rcc.h
        Libraries/inc/cw32f030_rtc.h
        Libraries/inc/cw32f030_spi.h
        Libraries/inc/cw32f030_systick.h
        Libraries/inc/cw32f030_uart.h
        Libraries/inc/cw32f030_vc.h
        Libraries/inc/cw32f030_wwdt.h
        Libraries/inc/system_cw32f030.h
        Libraries/src/cw32f030_adc.c
        Libraries/src/cw32f030_atim.c
        Libraries/src/cw32f030_awt.c
        Libraries/src/cw32f030_btim.c
        Libraries/src/cw32f030_crc.c
        Libraries/src/cw32f030_debug.c
        Libraries/src/cw32f030_digitalsign.c
        Libraries/src/cw32f030_dma.c
        Libraries/src/cw32f030_flash.c
        Libraries/src/cw32f030_gpio.c
        Libraries/src/cw32f030_gtim.c
        Libraries/src/cw32f030_i2c.c
        Libraries/src/cw32f030_iwdt.c
        Libraries/src/cw32f030_lvd.c
        Libraries/src/cw32f030_pwr.c
        Libraries/src/cw32f030_ram.c
        Libraries/src/cw32f030_rcc.c
        Libraries/src/cw32f030_rtc.c
        Libraries/src/cw32f030_spi.c
        Libraries/src/cw32f030_systick.c
        Libraries/src/cw32f030_uart.c
        Libraries/src/cw32f030_vc.c
        Libraries/src/cw32f030_wwdt.c
        Libraries/src/system_cw32f030.c
        User/inc/interrupts_cw32f030.h
        User/inc/main.h
        User/src/interrupts_cw32f030.c
        User/src/main.c)
