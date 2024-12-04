#include <ioCC2530.h>
#include <stdint.h>
#include "adc.h"

#define ADC_REFERENCE_VOLTAGE 3.3
#define ADC_RESOLUTION          12

void initLightSensor(void) 
{ 
    APCFG  |= 0x01 << 5;//0.5模拟io使能 
}

uint16_t get_adcValue(void) 
{ 
    uint16_t value; 
    ADCIF = 0;   //清ADC 中断标志 
    //采用基准电压avdd5:3.3V，512采样率，通道5，启动AD转化 
    ADCCON3 = (0x80 | 0x30 | 0x05); 
    while ( !ADCIF ) 
    { 
        ;  //等待AD转化结束 
    } 
    value = ADCL; 
    value |= (ADCH<<8); 
    return (value>>4); //12位有效数据 
}

float ADC_ValueToVoltage(uint32_t adcValue) 
{  
    
  // 计算每个ADC单位的电压值  
   
  float voltagePerUnit = (float)ADC_REFERENCE_VOLTAGE / (1 << (ADC_RESOLUTION+1));  
  // 计算总电压  
   
  float voltage = adcValue * voltagePerUnit;  
   
  // 返回电压值（单位：mV）  
   
  return voltage;  
 
}