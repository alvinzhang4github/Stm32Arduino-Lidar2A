/**
  ******************************************************************************
  * @file    bsp_LIDAR_usart.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   ����LIDAR��λ��Ϣ����
  ****************************************************************************
**/
  
#include "bsp_uart.h"
#include "lidar.h"
#include <Arduino.h>

T_TX_BUFF TxBuffer;
T_RX_BUFF RxBuffer;


 /**
  * @brief  USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval �� 
  */
void USART1_Init(int baud)
{
	Serial.begin(baud);
}

//�ض���c�⺯��printf������
int fputc(int ch, FILE *f) 
{	
	Serial.write(ch);
	return (ch);
}
/********************************************************************************************************************/
//��λ֡������ɱ�־
//����:    ��
//����ֵ:  ��
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
//д���ݵ����ջ���
//����:
//         data:    ���յ�������
//����ֵ:  ��
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
//�������ݴ�����
//����:    ��
//����ֵ:  ��
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
