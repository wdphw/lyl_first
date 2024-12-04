#include "air.h"

void Air_on(void) // PB7置1
{
    HAL_GPIO_WritePin(GPIOB, AIR_Pin, GPIO_PIN_SET);
}

void Air_off(void) // PB7置0
{
    HAL_GPIO_WritePin(GPIOB, AIR_Pin, GPIO_PIN_RESET);
}
