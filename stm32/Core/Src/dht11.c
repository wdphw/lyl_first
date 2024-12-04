#include "dht11.h"

static void DHT11_PP_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * ************************************************************************
 * @brief 将DHT11配置为上拉输入模式
 * ************************************************************************
 */
static void DHT11_UP_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

/**
 * ************************************************************************
 * @brief 读取字节
 * @return temp
 * ************************************************************************
 */
uint8_t DHT11_ReadByte(void)
{
    uint8_t i, temp = 0;

    for (i = 0; i < 8; i++)
    {
        while (DHT11_ReadPin == 0)
            ; // 等待低电平结束

        m_delay_us(40); //	延时 40 微秒

        if (DHT11_ReadPin == 1)
        {
            while (DHT11_ReadPin == 1)
                ; // 等待高电平结束

            temp |= (uint8_t)(0X01 << (7 - i)); // 先发送高位
        }
        else
        {
            temp &= (uint8_t) ~(0X01 << (7 - i));
        }
    }
    return temp;
}

/**
 * ************************************************************************
 * @brief 读取一次数据
 * @param[in] DHT11_Data  定义的结构体变量
 * @return 0或1（数据校验是否成功）
 * @note 它首先向DHT11发送启动信号，然后等待DHT11的应答。如果DHT11正确应答，
 * 		 则继续读取湿度整数、湿度小数、温度整数、温度小数和校验和数据，
 * 		 并计算校验和以进行数据校验
 * ************************************************************************
 */
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
    DHT11_PP_OUT(); // 主机输出，主机拉低
    DHT11_PULL_0;
    HAL_Delay(18); // 延时 18 ms

    DHT11_PULL_1; // 主机拉高，延时 30 us
    m_delay_us(30);

    DHT11_UP_IN(); // 主机输入，获取 DHT11 数据

    if (DHT11_ReadPin == 0) // 收到从机应答
    {
        while (DHT11_ReadPin == 0)
            ; // 等待从机应答的低电平结束

        while (DHT11_ReadPin == 1)
            ; // 等待从机应答的高电平结束

        /*开始接收数据*/
        DHT11_Data->humi_int = DHT11_ReadByte();
        DHT11_Data->humi_dec = DHT11_ReadByte();
        DHT11_Data->temp_int = DHT11_ReadByte();
        DHT11_Data->temp_dec = DHT11_ReadByte();
        DHT11_Data->check_sum = DHT11_ReadByte();

        DHT11_PP_OUT(); // 读取结束，主机拉高
        DHT11_PULL_1;

        // 数据校验
        if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_dec + DHT11_Data->temp_int + DHT11_Data->temp_dec)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else // 未收到从机应答
    {
        return 0;
    }
}
