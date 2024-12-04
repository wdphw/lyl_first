#ifndef __DHT11__H
#define __DHT11_H

#include <stdint.h>

void GPIO_OUT_Init(void);
void GPIO_IN_Init(void);
void dht11_start(void);
int dht11_check(void);
uint8_t Dht11_ReceOneByte (void);
uint8_t Dht11_GetData (double *temp, double *humi);

#endif