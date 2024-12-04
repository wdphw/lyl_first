#include <ioCC2530.h>
#include <stdint.h>
#include "adc.h"

#define ADC_REFERENCE_VOLTAGE 3.3
#define ADC_RESOLUTION          12

void initLightSensor(void) 
{ 
    APCFG  |= 0x01 << 5;//0.5ģ��ioʹ�� 
}

uint16_t get_adcValue(void) 
{ 
    uint16_t value; 
    ADCIF = 0;   //��ADC �жϱ�־ 
    //���û�׼��ѹavdd5:3.3V��512�����ʣ�ͨ��5������ADת�� 
    ADCCON3 = (0x80 | 0x30 | 0x05); 
    while ( !ADCIF ) 
    { 
        ;  //�ȴ�ADת������ 
    } 
    value = ADCL; 
    value |= (ADCH<<8); 
    return (value>>4); //12λ��Ч���� 
}

float ADC_ValueToVoltage(uint32_t adcValue) 
{  
    
  // ����ÿ��ADC��λ�ĵ�ѹֵ  
   
  float voltagePerUnit = (float)ADC_REFERENCE_VOLTAGE / (1 << (ADC_RESOLUTION+1));  
  // �����ܵ�ѹ  
   
  float voltage = adcValue * voltagePerUnit;  
   
  // ���ص�ѹֵ����λ��mV��  
   
  return voltage;  
 
}