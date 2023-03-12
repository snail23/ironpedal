# Part of Ironpedal
# https://github.com/snail23/ironpedal

TARGET = ironpedal
OPT = -O3 -flto

ifdef DEBUG
	OPT = -O1 -ggdb
endif

C_INCLUDES += \
-Idep/Adafruit-GFX-Library \
-Idep/DaisySP/Source \
-Idep/SSD1351-Driver-Library/src

CPP_SOURCES = \
\
$(LIBDAISY_DIR)/src/daisy_seed.cpp \
\
$(LIBDAISY_DIR)/src/dev/codec_ak4556.cpp \
$(LIBDAISY_DIR)/src/dev/codec_wm8731.cpp \
$(LIBDAISY_DIR)/src/dev/sdram.cpp \
\
$(LIBDAISY_DIR)/src/hid/audio.cpp \
$(LIBDAISY_DIR)/src/hid/ctrl.cpp \
$(LIBDAISY_DIR)/src/hid/parameter.cpp \
$(LIBDAISY_DIR)/src/hid/switch.cpp \
$(LIBDAISY_DIR)/src/hid/usb.cpp \
\
$(LIBDAISY_DIR)/src/per/adc.cpp \
$(LIBDAISY_DIR)/src/per/i2c.cpp \
$(LIBDAISY_DIR)/src/per/gpio.cpp \
$(LIBDAISY_DIR)/src/per/qspi.cpp \
$(LIBDAISY_DIR)/src/per/rng.cpp \
$(LIBDAISY_DIR)/src/per/sai.cpp \
$(LIBDAISY_DIR)/src/per/spi.cpp \
$(LIBDAISY_DIR)/src/per/tim.cpp \
$(LIBDAISY_DIR)/src/per/uart.cpp \
\
$(LIBDAISY_DIR)/src/sys/dma.cpp \
$(LIBDAISY_DIR)/src/sys/system.cpp \
\
dep/DaisySP/Source/Drums/analogbassdrum.cpp \
dep/DaisySP/Source/Dynamics/compressor.cpp \
\
dep/DaisySP/Source/Effects/autowah.cpp \
dep/DaisySP/Source/Effects/chorus.cpp \
dep/DaisySP/Source/Effects/overdrive.cpp \
dep/DaisySP/Source/Effects/reverbsc.cpp \
dep/DaisySP/Source/Effects/tremolo.cpp \
\
dep/DaisySP/Source/Filters/svf.cpp \
dep/DaisySP/Source/PhysicalModeling/resonator.cpp \
dep/DaisySP/Source/Synthesis/oscillator.cpp \
dep/DaisySP/Source/Utility/metro.cpp \
\
dep/SSD1351-Driver-Library/src/ssd1351.cpp \
\
src/ironpedal.cpp

C_SOURCES = \
\
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sai.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
\
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c \
$(LIBDAISY_DIR)/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c \
\
$(LIBDAISY_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
$(LIBDAISY_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
$(LIBDAISY_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
\
$(LIBDAISY_DIR)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
$(LIBDAISY_DIR)/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c \
\
$(LIBDAISY_DIR)/src/sys/system_stm32h7xx.c \
\
$(LIBDAISY_DIR)/src/usbd/usbd_conf.c \
$(LIBDAISY_DIR)/src/usbh/usbh_conf.c \
\
$(LIBDAISY_DIR)/src/util/hal_map.c

LIBDAISY_DIR = dep/libDaisy
OCD_DIR = dep/openocd
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core

include $(SYSTEM_FILES_DIR)/Makefile
LIBS =
