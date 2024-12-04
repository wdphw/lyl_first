#include <ioCC2530.h>
#include <stdio.h>
#include <stdint.h>
#include "dht11.h"
//typedef unsigned char           uint8_t;
#define u32 unsigned int 

#define DHT11 P0_4

void delayUs(u32 n);
void delayMs(u32 n);

void delayUs(u32 n) 
{ 
  while(n--)   
  { 
    asm("NOP"); 
    asm("NOP");	
    asm("NOP"); 
  } 
} 
 
//毫秒级延时 
void delayMs(u32 n) 
{ 
  while(n--) 
  { 
      delayUs(1000); 
  } 
}

void GPIO_OUT_Init(void){
  P0SEL &=~(1<<4);
  P0DIR |= (1<<4);
  DHT11 = 1;
}

void GPIO_IN_Init(void){
   P0SEL &=~(1<<4);
   P0DIR &=~(1<<4);
}

void dht11_start(){
  GPIO_OUT_Init();
  DHT11=0;
  delayMs(20);
  DHT11=1;
  delayUs(30);

  
}

int dht11_check(){
  uint8_t retry=0;
  GPIO_IN_Init();
	
  while((DHT11==0)&& retry<100){ 
		retry++;
                delayUs(1);
	}
	if(retry>=100)return 1; 	
	else retry=0;

  
  while ((DHT11==1) && retry<100) {  
        retry++;
        delayUs(1);
    }	    
    if(retry>=100)return 1;	    
    return 0;

}

uint8_t Dht11_ReceOneByte (void)
{
    uint8_t tempVar = 0;   
    uint8_t receData = 0;   
    
    for (tempVar = 0;tempVar < 8;tempVar ++)
    {  
        
        while (DHT11 == 0);   //等待低电平过去
        delayUs(40);   
        
        
        if (DHT11 == 1)
        {
           while (DHT11 == 1);
            receData |= (uint8_t)(0x01<<(7-tempVar));   
        }
        else
        {
            receData &= (uint8_t)~(0x01<<(7-tempVar));   
        }
    }
    return receData;
}

uint8_t Dht11_GetData (double *temp, double *humi)
{
  double buf[5]={0};
    uint8_t i;
    dht11_start();   // 复位
    
    if(dht11_check() == 0)
    {
        for(i = 0; i < 5; i++) //读取40位数据
        {
            buf[i] = Dht11_ReceOneByte();
        }
	
        if(buf[0]!=0){
         printf("success\n");
          }
        
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]+buf[1]/10.00;
            *temp = buf[2]+buf[3]/10.00;
        }
    }
    else
        {
            return 1;
        }
    return 0;
}



