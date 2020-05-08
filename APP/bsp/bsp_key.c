/**
  ******************************************************************************
  * @file			bsp_key.c
  * @brief			bsp_key function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2019-07-18
  * @copyright		2019, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "bsp_key.h"
#include "stm32f4xx_gpio.h"

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/**
 * @brief 按键引脚定义
 */
#define KEY_RUN_GPIO_PORT       (GPIOC)
#define KEY_STOP_GPIO_PORT      (GPIOB)
#define KEY_UP_GPIO_PORT        (GPIOB)
#define KEY_DOWN_GPIO_PORT      (GPIOB)
#define KEY_DIR_GPIO_PORT       (GPIOB)

#define KEY_RUN_GPIO_PIN    (GPIO_Pin_5)	
#define KEY_STOP_GPIO_PIN	(GPIO_Pin_1)	
#define KEY_UP_GPIO_PIN	    (GPIO_Pin_10)	
#define KEY_DOWN_GPIO_PIN	(GPIO_Pin_11)
#define KEY_DIR_GPIO_PIN	(GPIO_Pin_12)	

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**=============================================================================
 * @brief           按键驱动初始化
 *
 * @param[in]       none
 * 
 * @return          XLI_ERR_NONE：成功
 *============================================================================*/
void bsp_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 开启GPIO时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

    /* GPIOC */
    GPIO_InitStructure.GPIO_Pin = KEY_RUN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    /* GPIOB */
    GPIO_InitStructure.GPIO_Pin = KEY_UP_GPIO_PIN | KEY_DOWN_GPIO_PIN | \
                                KEY_STOP_GPIO_PIN | KEY_DIR_GPIO_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                            
}

/**=============================================================================
 * @brief           获取按键状态
 *
 * @param[in]       idx: 索引
 * 
 * @return          0/1： 按键高低电平
 *============================================================================*/
uint8_t bsp_key_get_is_pushed(uint8_t idx)
{
    if (idx >= BSP_KEY_NUM_MAX)
    {
        return -1;
    }
    
    uint8_t value = 0;
    switch (idx)
    {
    case BSP_KEY_NUM_RUN:
        value = !GPIO_ReadInputDataBit(GPIOC, KEY_RUN_GPIO_PIN);
        break;

    case BSP_KEY_NUM_STOP:
        value = !GPIO_ReadInputDataBit(GPIOB, KEY_STOP_GPIO_PIN);
        break;

    case BSP_KEY_NUM_UP:
        value = !GPIO_ReadInputDataBit(GPIOB, KEY_UP_GPIO_PIN);
        break;        

    case BSP_KEY_NUM_DOWN:
        value = !GPIO_ReadInputDataBit(GPIOB, KEY_DOWN_GPIO_PIN);
        break;

    case BSP_KEY_NUM_DIR:
        value = !GPIO_ReadInputDataBit(GPIOB, KEY_DIR_GPIO_PIN);
        break;

    default:
        break;
    }

    return value;
}
