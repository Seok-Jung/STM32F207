/*********************************************************************************************************
*
* File                : ws_AT45DBXX.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/


#include "stm32f2xx.h"
#include "usart.h"
#include "ws_AT45DBXX.h"

void AT45DBXX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_Configuration();

	printf("SPI is ready!\r\n");

  RCC_AHB1PeriphClockCmd(Open207Z_SPIx_CS_GPIO_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = Open207Z_SPIx_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(Open207Z_SPIx_CS_GPIO_PORT, &GPIO_InitStructure);
  	
  AT45DBXX_Disable;
}

static void AT45DBXX_BUSY(void)
{
	u8 AT45DBXXStruct;
	AT45DBXX_Enable;
	SPIx_Send_byte(READ_STATE_REGISTER);
	do
	{AT45DBXXStruct = SPIx_Receive_byte();}
	while(!(AT45DBXXStruct & 0x80));
	AT45DBXX_Disable;	
}

void AT45DBXX_Read_ID(u8 *IData)
{
	u8 i;
	AT45DBXX_BUSY();
	AT45DBXX_Enable;
	SPIx_Send_byte(Read_ID); //ִ�ж�ȡid����		  	
  	for(i=0;i<4;i++)
  	{
  		IData[i] = SPIx_Receive_byte();
  	}
  	AT45DBXX_Disable;
}

void write_buffer(u16 BufferOffset,u8 Data)
{			
	AT45DBXX_Enable;
	SPIx_Send_byte(0x84);			
	SPIx_Send_byte(0xff);
	SPIx_Send_byte((u8)BufferOffset>>8);
	SPIx_Send_byte((u8)BufferOffset);		
	SPIx_Send_byte(Data);
	AT45DBXX_Disable;
}

u8 read_buffer(u16 BufferOffset)
{		
	u8 temp=0;

	AT45DBXX_Enable;
 	SPIx_Send_byte(0xd4);
	SPIx_Send_byte(0xff);
	SPIx_Send_byte((u8)BufferOffset>>8);
	SPIx_Send_byte((u8)BufferOffset);  
	SPIx_Send_byte(0xff);	
	temp=SPIx_Receive_byte();
	AT45DBXX_Disable;
	return temp;		
}
