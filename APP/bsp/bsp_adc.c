/**
  ******************************************************************************
  * @file			bsp_adc.c
  * @brief			bsp_adc function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-08
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "bsp_hall.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"

/* Private constants ---------------------------------------------------------*/
#define PHASE_U_CUR_GPIO_PIN        (GPIO_Pin_6)
#define PHASE_V_CUR_GPIO_PIN        (GPIO_Pin_7)
#define PHASE_W_CUR_GPIO_PIN        (GPIO_Pin_4)

#define BUS_VOL_GPIO_PIN            (GPIO_Pin_1)
#define POT_VOL_GPIO_PIN            (GPIO_Pin_0)

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           GPIO初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _adc_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 开启GPIO时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = PHASE_U_CUR_GPIO_PIN | PHASE_V_CUR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);   

    GPIO_InitStructure.GPIO_Pin = PHASE_W_CUR_GPIO_PIN | BUS_VOL_GPIO_PIN;    
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = POT_VOL_GPIO_PIN;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**=============================================================================
 * @brief           ADC DMA初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _adc_dma_init(uint16_t *sample_buf)
{
    DMA_InitTypeDef DMA_InitStructure; 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream0);
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){};

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr =  (uint32_t)sample_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = BSP_ADC_NUM_MAX;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);    
    DMA_Cmd(DMA2_Stream0, ENABLE);

}

/**=============================================================================
 * @brief           ADC配置初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _adc_cfg_init(void)
{

}

/**=============================================================================
 * @brief           ADC初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_adc_init(uint16_t *sample_buf)
{
    _adc_gpio_init();
    _adc_dma_init(sample_buf);
}