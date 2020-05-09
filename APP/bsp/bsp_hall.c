/**
  ******************************************************************************
  * @file			bsp_hall.c
  * @brief			bsp_hall function
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

/* Private constants ---------------------------------------------------------*/
#define HALL_A_GPIO_PIN     (GPIO_Pin_6)
#define HALL_B_GPIO_PIN     (GPIO_Pin_7)
#define HALL_C_GPIO_PIN     (GPIO_Pin_8)

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           获取霍尔状态
 *
 * @param[in]       idx: 索引
 * 
 * @return          0/1： 高低电平
 *============================================================================*/
uint8_t bsp_hall_get_level(uint8_t idx)
{
    if (idx >= BSP_HALL_MAX)
    {
        return -1;
    }
    
    uint8_t value = 0;
    switch (idx)
    {
    case BSP_HALL_A:
        value = GPIO_ReadInputDataBit(GPIOC, HALL_A_GPIO_PIN);
        break;

    case BSP_HALL_B:
        value = GPIO_ReadInputDataBit(GPIOC, HALL_B_GPIO_PIN);
        break;

    case BSP_HALL_C:
        value = GPIO_ReadInputDataBit(GPIOC, HALL_C_GPIO_PIN);
        break;        

    default:
        break;
    }

    return value ? 1 : 0;
}

/**=============================================================================
 * @brief           霍尔初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_hall_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 开启GPIO时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = HALL_A_GPIO_PIN | HALL_B_GPIO_PIN | HALL_C_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);   
}
