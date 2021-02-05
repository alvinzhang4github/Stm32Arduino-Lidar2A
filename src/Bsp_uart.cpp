/**
  ******************************************************************************
  * @file    bsp_LIDAR_usart.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   接收LIDAR定位信息串口
  ****************************************************************************
**/
  
#include "bsp_uart.h"
#include "lidar.h"
#include <Arduino.h>

T_TX_BUFF TxBuffer;
T_RX_BUFF RxBuffer;


 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无 
  */
void USART1_Init(int baud)
{
	Serial.begin(baud);
}

//重定向c库函数printf到串口
int fputc(int ch, FILE *f) 
{	
	Serial.write(ch);
	return (ch);
}
/********************************************************************************************************************/
//置位帧接收完成标志
//参数:    无
//返回值:  无
/********************************************************************************************************************/
 
void RxDataComplete(void)
{
	if((RxBuffer.Rdy == 0)&&(RxBuffer.Len > 0))
	{
		if(RxBuffer.Len > 3)
			RxBuffer.Rdy = 1;
		else
			RxBuffer.Len = 0;
	}
}

/********************************************************************************************************************/
//写数据到接收缓存
//参数:
//         data:    接收到的数据
//返回值:  无
/********************************************************************************************************************/
void UartReceive(uint8_t data)
{
	if(RxBuffer.Rdy == 0)
	{
		RxBuffer.Buff[RxBuffer.Len++] = data;
		if(RxBuffer.Len >= sizeof(RxBuffer.Buff))
			RxBuffer.Rdy = 1;
	}
}

/********************************************************************************************************************/
//接收数据处理函数
//参数:    无
//返回值:  无
/********************************************************************************************************************/
void ProcessUartRxData(void)
{
	while (Serial.available()) 
	{
		UartReceive(Serial.read());
	}
	RxDataComplete();

	if(RxBuffer.Rdy == 0)
		return;

	if(RxBuffer.Rdy > 0)
		P_Cmd_Process();
	RxBuffer.Len = 0;
	RxBuffer.Rdy = 0;
}


/*********************************************END OF FILE**********************/
