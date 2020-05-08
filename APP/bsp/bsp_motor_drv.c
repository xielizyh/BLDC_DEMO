/**
  ******************************************************************************
  * @file			bsp_motor_drv.c
  * @brief			bsp_motor_drv function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-04-29
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "bsp_motor_drv.h"
#include "stm32f4xx.h" 
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

/* Private constants ---------------------------------------------------------*/
#define PHASE_UH_GPIO_PIN   (GPIO_Pin_8)    /*!< U相上桥臂IO */
#define PHASE_VH_GPIO_PIN   (GPIO_Pin_9)    /*!< V相上桥臂IO */
#define PHASE_WH_GPIO_PIN   (GPIO_Pin_10)   /*!< W相上桥臂IO */

#define PHASE_UL_GPIO_PIN   (GPIO_Pin_13)   /*!< U相下桥臂IO */
#define PHASE_VL_GPIO_PIN   (GPIO_Pin_14)   /*!< V相下桥臂IO */
#define PHASE_WL_GPIO_PIN   (GPIO_Pin_15)   /*!< W相下桥臂IO */

#define PWM_PREQ    16   /*!< 16K */
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
 * 
 * @note            H_PWM_L_ON
 *============================================================================*/
static void _motor_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启GPIO和TIM时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* 上桥臂GPIO配置 */
    GPIO_InitStructure.GPIO_Pin = PHASE_UH_GPIO_PIN | PHASE_VH_GPIO_PIN | PHASE_WH_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 上桥臂GPIO复用 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);

    /* 下桥臂GPIO配置 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**=============================================================================
 * @brief           PWM初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _motor_pwm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 定时器初始化 */
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = (168000 / PWM_PREQ) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* PWM初始化 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    /* 使能预装载寄存器 */
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* 死区初始化 */
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x6A;  /*!< 1.8us */
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    /* 定时器中断初始化 */
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);  
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);                            
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                         
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 使能PWM输出 */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**=============================================================================
 * @brief           电机驱动初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_motor_drv_init(void)
{
    _motor_gpio_init();
    _motor_pwm_init();
}
