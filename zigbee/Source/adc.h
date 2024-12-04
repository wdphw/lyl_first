#ifndef __ADC_H
#define __ADC_H

void initLightSensor(void);
uint16_t get_adcValue(void);
float ADC_ValueToVoltage(uint32_t adcValue);

#endif