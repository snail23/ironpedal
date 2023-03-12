#include "myHAL.h"

// For demonstration purposes
#include "stdio.h"

#ifdef _WIN64
#include "time.h"

void SPI_TXBuffer(uint8_t *buffer, uint32_t len) {
    printf("I'm sending a buffer\n");
}

void SPI_TXByte(uint8_t* data) {
    printf("I'm sending a byte\n");
}

void GPIO_SetPin(uint16_t Port, uint16_t Pin) {
    printf("I'm setting a pin\n");
}

void GPIO_ResetPin(uint16_t Port, uint16_t Pin) {
    printf("I'm resetting a pin\n");
}

void HAL_Delay(uint16_t ms) {
    clock_t start = clock();
    clock_t elapsed = clock() - start;
    while (elapsed < ms) {
        // wait
        elapsed = clock() - start;
    }
}
#endif