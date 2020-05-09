/**
  ******************************************************************************
  * @file			app_motor.c
  * @brief			app_motor function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-08
  * @copyright		2020, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "app_motor.h"
#include "bsp_motor_drv.h"
#include "bsp_adc.h"
#include "bsp_hall.h"
#include "app_debug.h"
#include "app_key.h"

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/**
* @brief 霍尔结构
*/
typedef struct {
    uint8_t level[BSP_HALL_MAX];	/*!< 三路霍尔电平 */
    uint8_t old_step;               /*!< 6步(旧转子位置) */
    uint8_t new_step;               /*!< 6步(新转子位置) */
}_hall_t;

/**
* @brief 电机结构
*/
typedef struct {
    uint8_t aim_duty;		/*!< 目标占空比 */
}_motor_t;

/* Private variables ---------------------------------------------------------*/
static _hall_t hall;
static _motor_t motor;

static uint16_t adc_conv_result[BSP_ADC_NUM_MAX];

/* Private function ----------------------------------------------------------*/

/**=============================================================================
 * @brief           霍尔换相
 *
 * @param[in]       none
 *
 * @return          none
 * 
 * @note            5-1-3-2-6-4
 *============================================================================*/
static void _motor_hall_commutate(void)
{
    /* 读取霍尔状态 */
    for (uint8_t i = 0; i < BSP_HALL_MAX; i++)
    {
        hall.level[i] = bsp_hall_get_level(i);
    }
    
    hall.new_step = hall.level[0] + (hall.level[1] << 1) + (hall.level[2] << 2);

    if (hall.new_step != hall.old_step) /*!< 转子位置变化 */
    {
        switch (hall.new_step)
        {
        case 5:
            bsp_motor_MOS_Q51PWM(motor.aim_duty);
            break;

        case 1:
            bsp_motor_MOS_Q16PWM(motor.aim_duty);
            break;

        case 3:
            bsp_motor_MOS_Q62PWM(motor.aim_duty);
            break;

        case 2:
            bsp_motor_MOS_Q24PWM(motor.aim_duty);
            break;

        case 6:
            bsp_motor_MOS_Q43PWM(motor.aim_duty);
            break;

        case 4:
            bsp_motor_MOS_Q35PWM(motor.aim_duty);
            break;

        default:
            bsp_motor_stop();
            break;
        }

        hall.old_step = hall.new_step;
    }
    
}

/**=============================================================================
 * @brief           电机定时器更新中断处理
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _motor_it_update_callback(void)
{
    //_motor_hall_commutate();
    RTU_LOGD(LOG_TAG, "debug time it update");
}

/**=============================================================================
 * @brief           电机任务
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void app_motor_task(void *arg)
{
#if 0
    RTU_LOGI(LOG_TAG, "U current = %d", adc_conv_result[0]);
    RTU_LOGI(LOG_TAG, "V current = %d", adc_conv_result[1]);
    RTU_LOGI(LOG_TAG, "W current = %d", adc_conv_result[2]);
    RTU_LOGI(LOG_TAG, "Bus voltage = %d", adc_conv_result[3]);
    RTU_LOGI(LOG_TAG, "Pot voltage = %d", adc_conv_result[4]);
#endif    
}

/**=============================================================================
 * @brief           用户运行按键回调
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _user_key_run_callback(app_key_evt_t *evt)
{
    switch (evt->sta)
    {
    case BSP_KEY_DOWN:
        RTU_LOGI(LOG_TAG, "Down");
        break;

    case BSP_KEY_UP:
        RTU_LOGI(LOG_TAG, "Up");
        break;        

    case BSP_KEY_CLICK:
        RTU_LOGI(LOG_TAG, "Click");
        bsp_motor_start();
        break;   

    default:
        break;    
    }    
}

/**=============================================================================
 * @brief           电机初始化
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
void app_motor_init(void)
{
    bsp_adc_init(adc_conv_result);
    bsp_motor_drv_init(_motor_it_update_callback);

    app_key_register_callback(BSP_KEY_NUM_RUN, _user_key_run_callback);
}
