#include "freertos_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "usart.h"
#include "dht11.h"
#include <string.h>
#include <stdio.h>
#include "air.h"
#include "tim.h"
#include "adc.h"
#include "dma.h"

#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

#define TASK1_STACK 256
#define TASK1_PRIORITY 1
TaskHandle_t task1_handle;
void task1(void *pvParameters);

#define TASK2_STACK 128
#define TASK2_PRIORITY 1
TaskHandle_t task2_handle;
void task2(void *pvParameters);

#define TASK3_STACK 128
#define TASK3_PRIORITY 1
TaskHandle_t task3_handle;
void task3(void *pvParameters);

uint8_t air_flag = 0;
uint8_t w_flag = 0;
uint8_t led_flag = 0;
uint8_t beep_flag = 0;

void FreeRtos_Start(void)
{
    // 创建一个启动任务
    xTaskCreate((TaskFunction_t)start_task,               // 任务函数的地址
                (char *)"start_task",                     // 任务名
                (configSTACK_DEPTH_TYPE)START_TASK_STACK, // 任务栈大小，默认128，单位4字节
                NULL,                                     // 传递给任务的形参
                (UBaseType_t)START_TASK_PRIORITY,         // 任务优先级
                (TaskHandle_t *)&start_task_handle);      // 任务句柄的地址
    // 启动任务调度器 会自动创建空闲任务
    vTaskStartScheduler();
}

void start_task(void *pvParameters)
{ // 进入临界区
    taskENTER_CRITICAL();
    BaseType_t xReturned = xTaskCreate((TaskFunction_t)task1,               // 任务函数的地址
                                       (char *)"task1",                     // 任务名
                                       (configSTACK_DEPTH_TYPE)TASK1_STACK, // 任务栈大小，默认128，单位4字节
                                       NULL,                                // 传递给任务的形参
                                       (UBaseType_t)TASK1_PRIORITY,         // 任务优先级
                                       (TaskHandle_t *)&task1_handle);      // 任务句柄的地址

    xTaskCreate((TaskFunction_t)task2,               // 任务函数的地址
                (char *)"task2",                     // 任务名
                (configSTACK_DEPTH_TYPE)TASK2_STACK, // 任务栈大小，默认128，单位4字节
                NULL,                                // 传递给任务的形参
                (UBaseType_t)TASK2_PRIORITY,         // 任务优先级
                (TaskHandle_t *)&task2_handle);      // 任务句柄的地址

    xTaskCreate((TaskFunction_t)task3,               // 任务函数的地址
                (char *)"task3",                     // 任务名
                (configSTACK_DEPTH_TYPE)TASK3_STACK, // 任务栈大小，默认128，单位4字节
                NULL,                                // 传递给任务的形参
                (UBaseType_t)TASK3_PRIORITY,         // 任务优先级
                (TaskHandle_t *)&task3_handle);      // 任务句柄的地址

    // 删除自身
    vTaskDelete(NULL);

    // 退出临界区
    taskEXIT_CRITICAL();
}

void task1(void *pvParameters)
{
    uint16_t data[3] = {0};
    uint8_t ligh = 0;
    float temp = 0;
    uint8_t humi = 0;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)data, 3);
    char buffer[150];
    char zbuf[50];
    DHT11_Data_TypeDef DHT11_Data;
    while (1)
    {
        printf("任务1运行\n");
        if (DHT11_ReadData(&DHT11_Data))
        {
            temp = DHT11_Data.temp_int + DHT11_Data.temp_dec / 10;
            humi = DHT11_Data.humi_int;
        }
        if (data[2] < 500)
        {
            HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_RESET);
        }
        else if (data[1] < 500)
        {
            HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_RESET);
        }
        ligh = 200 - data[0] * 200 / 4095;
        sprintf(zbuf, "T%.1fH%dL%d\n", temp, humi, ligh);
        sprintf(buffer, "{\"temp\":{\"value\":%.1f},\"humi\":{\"value\":%d},\"light\":{\"value\":%d},\"fire\":{\"value\":%d},\"air\":{\"value\":%s},\"led\":{\"value\":%s},\"window\":{\"value\":%s}}\n", temp, humi, ligh, data[1], air_flag ? "true" : "false", led_flag ? "true" : "false", w_flag ? "true" : "false");
        HAL_UART_Transmit(&huart1, (uint8_t *)zbuf, strlen(zbuf), 1000);
        HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), 1000);

        vTaskDelay(1000);
    }
}

void task2(void *pvParameters)
{
    uint8_t rbuf[3];

    while (1)
    {
        printf("任务2运行\n");
        HAL_UART_Receive(&huart3, rbuf, 2, 1000);

        if (rbuf[0] == 'A')
        {
            sscanf((char *)rbuf, "A%d", &air_flag);
            if (air_flag)
            {
                Air_on();
            }
            else
            {
                Air_off();
            }
        }
        else if (rbuf[0] == 'W')
        {
            sscanf((char *)rbuf, "W%d", &w_flag);
            if (w_flag)
            {
                Servo_SetAngle(90);
            }
            else
            {
                Servo_SetAngle(0);
            }
        }
        else if (rbuf[0] == 'L')
        {
            sscanf((char *)rbuf, "L%d", &led_flag);
            if (led_flag)
            {
                HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_RESET);
                led_flag = 0;
            }
            else
            {
                HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_SET);
            }
        }
        else if (rbuf[0] == 'B')
        {
            sscanf((char *)rbuf, "B%d", &beep_flag);
            if (beep_flag)
            {
                HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_SET);
            }
        }

        vTaskDelay(1000);
    }
}

void task3(void *pvParameters)
{

    uint8_t rrbuf[3];
    while (1)
    {
        printf("任务3运行\n");
        HAL_UART_Receive(&huart1, rrbuf, 2, 1000);

        if (rrbuf[0] == 'A')
        {
            sscanf((char *)rrbuf, "A%d", &air_flag);
            if (air_flag)
            {
                Air_on();
            }
            else
            {
                Air_off();
            }
        }
        else if (rrbuf[0] == 'W')
        {
            sscanf((char *)rrbuf, "W%d", &w_flag);
            if (w_flag)
            {
                Servo_SetAngle(90);
            }
            else
            {
                Servo_SetAngle(0);
            }
        }
        else if (rrbuf[0] == 'L')
        {
            sscanf((char *)rrbuf, "L%d", &led_flag);
            if (led_flag)
            {
                HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_RESET);
                led_flag = 0;
            }
            else
            {
                HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_SET);
            }
        }
        else if (rrbuf[0] == 'B')
        {
            sscanf((char *)rrbuf, "B%d", &beep_flag);
            if (beep_flag)
            {
                HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_SET);
            }
        }

        vTaskDelay(1000);
    }
}
