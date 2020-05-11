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

/* Figure --------------------------------------------------------------------*/
/*  逆变电路编号
		   -------------------------------------
		   |			  |				   |
	1-----||	   2-----||			3-----||
		   |			  | 			   |
		   |---U		  |---V			   |---W
		   |			  |				   |
	4-----||	   5-----||			6-----||
		   |			  |    			   |
		   -------------------------------------
*/

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
static bsp_motor_it_update_callback _motor_it_update_cb;

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
    GPIO_InitStructure.GPIO_Pin = PHASE_UL_GPIO_PIN | PHASE_VL_GPIO_PIN | PHASE_WL_GPIO_PIN;
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
    TIM_TimeBaseStructure.TIM_Period = (168000 / PWM_PREQ) - 1; /*!< 10500=16K */
    //TIM_TimeBaseStructure.TIM_Period = 10000; /*!< 10000=16.8K */
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
 * @brief           定时器更新中断
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void TIM1_UP_TIM10_IRQHandler(void)
{
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);   /*!< 清更新中断 */
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)    /*!< 溢出中断 */
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
        _motor_it_update_cb();
    }
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q51PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare1(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_UL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_WL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_VL_GPIO_PIN);
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q16PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare1(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_UL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_VL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_WL_GPIO_PIN);
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q62PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare2(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_UL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_VL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_WL_GPIO_PIN);
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q24PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare3(TIM1, 0);
    TIM_SetCompare2(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_VL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_WL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_UL_GPIO_PIN);
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q43PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_VL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_WL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_UL_GPIO_PIN);
}

/**=============================================================================
 * @brief           六步换相
 *
 * @param[in]       duty:占空比
 *
 * @return          none
 *============================================================================*/
void bsp_motor_MOS_Q35PWM(uint8_t duty)
{
    uint32_t compare = 168000 / PWM_PREQ / 100 * duty - 1;
    
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, compare);

    GPIO_ResetBits(GPIOB, PHASE_UL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_WL_GPIO_PIN);
    GPIO_SetBits(GPIOB, PHASE_VL_GPIO_PIN);
}

/**=============================================================================
 * @brief           电机运行
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_motor_start(void)
{
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable);

	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable);

}

/**=============================================================================
 * @brief           电机停止
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_motor_stop(void)
{
/*    
    TIM_SetCompare1(TIM1, 0);
    TIM_SetCompare2(TIM1, 0);
    TIM_SetCompare3(TIM1, 0); 

    GPIO_ResetBits(GPIOB, PHASE_UL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_VL_GPIO_PIN); 
    GPIO_ResetBits(GPIOB, PHASE_WL_GPIO_PIN);  
*/ 

	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);

	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable);

	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable);

}

/**=============================================================================
 * @brief           电机驱动初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void bsp_motor_drv_init(bsp_motor_it_update_callback cb)
{
    _motor_gpio_init();
    _motor_pwm_init();
    _motor_it_update_cb = cb;
}
