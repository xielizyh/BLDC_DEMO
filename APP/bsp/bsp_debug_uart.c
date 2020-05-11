/**
  ******************************************************************************
  * @file			bsp_debug_uart.c
  * @brief			bsp_debug_uart function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-30
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "bsp_debug_uart.h"
#include "stm32f4xx.h" 
#include "stm32f4xx_usart.h"


/* Private constants ---------------------------------------------------------*/
#define UART_TX_ENABLE_DMA	(1)

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/**
* @brief 串口接收
*/
typedef struct {
	uint8_t buffer[UART_BUFFER_MAX_SIZE];		/*!< 接收缓存 */
	uint16_t size;	/*!< 大小 */
	bsp_debug_uart_rx_callback callback;	/*!< 接收回调 */
}uart_rx_t;

/* Private variables ---------------------------------------------------------*/
static uint8_t uart_tx_buffer[UART_BUFFER_MAX_SIZE];
static uart_rx_t uart_rx;

/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           串口初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _uart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 1、使能GPIOB、USART1、DMA1时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 
	/* 2、串口1对应引脚复用映射 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
	/* 3、USART1端口配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* 4、USART1初始化设置 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(USART1, &USART_InitStructure);
	
    /* 5、使能串口 */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	/* 6、接收使能 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);
}

/**=============================================================================
 * @brief           DMA发送初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _dma_tx_init(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_DeInit(DMA2_Stream7);
	
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){};

    /* DMA发送 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr =  (uint32_t)uart_tx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = /*UART_BUFFER_MAX_SIZE*/0;		/*!< 初始化填0 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable/*DMA_FIFOMode_Enable*/;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
	
	//DMA_SetCurrDataCounter(DMA2_Stream7, 0);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/**=============================================================================
 * @brief           DMA接收初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _dma_rx_init(uint8_t *rx_buf)
{
	DMA_InitTypeDef DMA_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_DeInit(DMA2_Stream2);

	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE){};	
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 						
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);		
	DMA_InitStructure.DMA_Memory0BaseAddr =  (uint32_t)rx_buf;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;				
	DMA_InitStructure.DMA_BufferSize = UART_BUFFER_MAX_SIZE;					
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   				
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;		
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
			
	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);							
	DMA_Cmd(DMA2_Stream2, ENABLE);											
/*	DMA接收中断意味着数据接收满才会产生
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
*/	
}

/**=============================================================================
 * @brief           串口是否正在发送
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
uint8_t bsp_debug_uart_is_transferring(void)
{
#if UART_TX_ENABLE_DMA
	return (DMA_GetCurrDataCounter(DMA2_Stream7) != 0);
#else
	return 0;
#endif	
}

/**=============================================================================
 * @brief           串口输出
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
int bsp_debug_uart_send(uint8_t *pbuf, uint16_t size)
{
#if UART_TX_ENABLE_DMA
    /* 参数校验 */
	if (!pbuf || (size > UART_BUFFER_MAX_SIZE)) return -1;
    /* DMA非空闲 */
	if (DMA_GetCurrDataCounter(DMA2_Stream7) != 0)	return -1;
    
    /* 拷贝发送数据 */
	memcpy(uart_tx_buffer, pbuf, size);
	DMA_Cmd(DMA2_Stream7, DISABLE);
	/* 设置传输数据大小 */
	DMA_SetCurrDataCounter(DMA2_Stream7, size);
	DMA_Cmd(DMA2_Stream7, ENABLE);
#else
	for(uint16_t i=0;i< size; i++)
	{
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = (u8) pbuf[i];  
	}
#endif
	return 0;
}

/**=============================================================================
 * @brief           串口1中断
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)	/*!< 串口空闲中断 */
	{
		DMA_Cmd(DMA2_Stream2, DISABLE);
		/* 清除空闲中断 */
		USART1->SR;
        USART1->DR;
		/* 读取数据长度 */
		uart_rx.size = UART_BUFFER_MAX_SIZE - DMA_GetCurrDataCounter(DMA2_Stream2);
		uart_rx.callback(uart_rx.buffer, uart_rx.size);	/*!< 回调 */
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_SetCurrDataCounter(DMA2_Stream2, UART_BUFFER_MAX_SIZE);
		DMA_Cmd(DMA2_Stream2, ENABLE);
	}
}

/**=============================================================================
 * @brief           DMA接收完成中断
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void DMA2_Stream2_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) != RESET)	    
	{
		DMA_Cmd(DMA2_Stream2, DISABLE);
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);										
	}
}

/**=============================================================================
 * @brief           DMA发送完成中断
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void DMA2_Stream7_IRQHandler(void)
{
	/* TCIF7不代表是否正在传输:0->无传输完成事件;1->发送传输完成事件 */
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)!= RESET)	    
	{
		DMA_Cmd(DMA2_Stream7, DISABLE);
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);									
	}
}

/**=============================================================================
 * @brief           串口初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_debug_uart_init(bsp_debug_uart_rx_callback recv_cb)
{
    _uart_init();
    _dma_tx_init();

	uart_rx.callback = recv_cb;
    _dma_rx_init(uart_rx.buffer);
}
