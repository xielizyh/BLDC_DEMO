/**
  ******************************************************************************
  * @file			app_key.c
  * @brief			app_key function
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2019-07-18
  * @copyright		2019, EVECCA Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "app_key.h"
#include "app_debug.h"

/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct{
    app_key_evt_t   evt;        /*!< 按键事件 */
    app_key_cb_t    cb;         /*!< 按键回调 */
    uint16_t        ticks;      /*!< 计时 */
    uint8_t         int_ticks;  /*!< 多次按键间隔计时 */
    uint16_t        hold_ticks; /*!< 长按计时 */
    uint8_t         polarity:1; /*!< 极性 */     
    uint8_t         ispushed:1; /*!< 是否按下 */    
    uint8_t         debounce:6; /*!< 消抖计时 */    
}_key_t;

/* Private variables ---------------------------------------------------------*/
/**
* @brief 按键列表
*/
static _key_t _key_list[] = {
    {{BSP_KEY_NUM_RUN, BSP_KEY_IDLE, 0}, NULL, 0, 0, 0, 0, 0, 0},
    {{BSP_KEY_NUM_STOP, BSP_KEY_IDLE, 0}, NULL, 0, 0, 0, 0, 0, 0},
    {{BSP_KEY_NUM_UP, BSP_KEY_IDLE, 0}, NULL, 0, 0, 0, 0, 0, 0},
    {{BSP_KEY_NUM_DOWN, BSP_KEY_IDLE, 0}, NULL, 0, 0, 0, 0, 0, 0},
    {{BSP_KEY_NUM_DIR, BSP_KEY_IDLE, 0}, NULL, 0, 0, 0, 0, 0, 0},
};

/* Private function ----------------------------------------------------------*/
static void _key_state_change(_key_t *key, bsp_key_sta_t state);
static void _process_key_poll(void);
static void _key_default_callback(app_key_evt_t *event);

/**=============================================================================
 * @brief           按键状态变化
 *
 * @param[in]       key: 按键指针
 * @param[in]       state: 按键状态
 *
 * @return          none
 *============================================================================*/
static void _key_state_change(_key_t *key, bsp_key_sta_t state)
{
    key->evt.sta = state;

    /*
    if (state == BSP_KEY_IDLE) 
    {
        key->evt.clicks = 0;
    }
    */ 
    if (state != BSP_KEY_IDLE)  
    {
        key->cb(&key->evt);   /*!< 回调 */
    }
}

/**=============================================================================
 * @brief           处理按键轮询
 *
 * @param[in]       none
 *
 * @return          none
 *============================================================================*/
static void _process_key_poll(void)
{
    _key_t *key = NULL;
    uint8_t ispushed = 0;

    for (uint8_t i = 0; i < BSP_KEY_NUM_MAX; i++)
    {
        key = &_key_list[i];
        if(key->evt.sta != BSP_KEY_IDLE)
        {
            key->ticks++;
        }
        ispushed = bsp_key_get_is_pushed(i);
        if ((ispushed != key->ispushed) && (++key->debounce >= BSP_KEY_DEBOUNCE_DISCERN_TICKS)) //消抖
        {
            key->ticks = 0;
            key->ispushed = ispushed;
            key->debounce = 0;
        }
        switch (key->evt.sta)
        {
        case BSP_KEY_IDLE:
            if (key->ispushed)  //检测到有效按下
            {
                _key_state_change(key, BSP_KEY_DOWN);
            }
            else if (key->evt.clicks)   /*!< 有多击要判断 */
            {
                if (++key->int_ticks >= BSP_KEY_MULTI_CLICK_INTERVAL)  /*!< 300ms没有第二次单击 */
                {
                    key->int_ticks = 0;
                    switch (key->evt.clicks)
                    {
                    case 1:
                        _key_state_change(key, BSP_KEY_CLICK_SINGLE);
                        break;
                    case 2:
                        _key_state_change(key, BSP_KEY_CLICK_DOUBLE);
                        break;
                    case 3:
                        _key_state_change(key, BSP_KEY_CLICK_TRIPLE);
                        break;
                    case 4:
                        _key_state_change(key, BSP_KEY_CLICK_QUADR);
                        break;
                    case 5:
                        _key_state_change(key, BSP_KEY_CLICK_PENTA);
                        break;    
                    default:
                        break;
                    }

                    key->evt.clicks = 0;
                }
            }
            break;

        case BSP_KEY_DOWN:
            if (key->ispushed)
            {
                if(key->ticks >= BSP_KEY_HOLDING_DISCERN_TICKS) //长按
                {
                    key->hold_ticks = BSP_KEY_HOLDING_DISCERN_TICKS;
                    _key_state_change(key, BSP_KEY_HOLDING_TICK_1ST);
                }
            }
            else
            {
                _key_state_change(key, BSP_KEY_UP); //弹起
            }
            break;
        
        case BSP_KEY_UP:
            if (key->ispushed) 
			{
                _key_state_change(key, BSP_KEY_DOWN);
            } 
			else 
			{
                if (key->ticks >= BSP_KEY_CLICK_DISCERN_TICKS) 
				{
                    _key_state_change(key, BSP_KEY_CLICK);
                }
            }        
            break;

        case BSP_KEY_CLICK:
            _key_state_change(key, BSP_KEY_IDLE);

            /* 需要进行多击判断 */
            key->int_ticks = 0; /*!< 有单击，复位多击间隔计数 */
            key->evt.clicks++;
            break;

        case BSP_KEY_CLICK_SINGLE:
        case BSP_KEY_CLICK_DOUBLE:
        case BSP_KEY_CLICK_TRIPLE:
        case BSP_KEY_CLICK_QUADR:
        case BSP_KEY_CLICK_PENTA:
            _key_state_change(key, BSP_KEY_IDLE);
            break;

        case BSP_KEY_HOLDING_TICK_1ST:  /*!< 长按1st~2nd之间 */
            if (!key->ispushed)     /*!< 弹起 */
            {
                _key_state_change(key, BSP_KEY_HOLDING_SINGLE);
            }
            else if (++key->hold_ticks >= BSP_KEY_HOLDING_DISCERN_TICKS * 2)
            {
                _key_state_change(key, BSP_KEY_HOLDING_TICK_2ND);
            }            
            break;

        case BSP_KEY_HOLDING_TICK_2ND:  /*!< 长按2nd~3rd之间 */
            if (!key->ispushed)     /*!< 弹起 */
            {
                _key_state_change(key, BSP_KEY_HOLDING_DOUBLE);
            }
            else if (++key->hold_ticks >= BSP_KEY_HOLDING_DISCERN_TICKS * 3)
            {
                _key_state_change(key, BSP_KEY_HOLDING_TICK_3RD);
            }            
            break;

        case BSP_KEY_HOLDING_TICK_3RD:  /*!< 长按3rd~4th之间 */
            if (!key->ispushed)     /*!< 弹起 */
            {
                _key_state_change(key, BSP_KEY_HOLDING_TRIPLE);
            }
            else if (++key->hold_ticks >= BSP_KEY_HOLDING_DISCERN_TICKS * 4)
            {
                //_key_state_change(key, BSP_KEY_HOLDING_TICK_4TH);
                RTU_LOGW(LOG_TAG, "@TODO: support to more times holding");
                _key_state_change(key, BSP_KEY_IDLE);

            }            
            break;

        case BSP_KEY_HOLDING_SINGLE:    /*!< 一倍长按 */
        case BSP_KEY_HOLDING_DOUBLE:    /*!< 两倍长按 */
        case BSP_KEY_HOLDING_TRIPLE:    /*!< 三倍长按 */
            key->hold_ticks = 0;
            if (!key->ispushed) 
			{
                _key_state_change(key, BSP_KEY_UP);
                _key_state_change(key, BSP_KEY_IDLE);
            }
            break;

        default:
            break;
        }
    }
}

/**=============================================================================
 * @brief           按键默认回调
 *
 * @param[in]       event: 按键事件指针
 *
 * @return          none
 *============================================================================*/
static void _key_default_callback(app_key_evt_t *event)
{
    switch (event->sta)
    {
    case BSP_KEY_HOLDING_TICK_1ST:
        RTU_LOGI(LOG_TAG, "Holding");
        break;

    case BSP_KEY_DOWN:
        RTU_LOGI(LOG_TAG, "Down");
        break;

    case BSP_KEY_UP:
        RTU_LOGI(LOG_TAG, "Up");
        break;

    case BSP_KEY_CLICK:
        RTU_LOGI(LOG_TAG, "Click");
        break;

    default:
        break;
    }
}

/**=============================================================================
 * @brief           按键注册
 *
 * @param[in]       key_num: 按键序号
 * @param[in]       callback: 按键回调
 *
 * @return          - -1：失败
 *                  - 0：成功
 *============================================================================*/
int app_key_register_callback(bsp_key_num_t key_num, app_key_cb_t callback)
{
    if (callback == NULL || key_num >= BSP_KEY_NUM_MAX) return -1;

    _key_list[key_num].cb = callback;

    return 0;
}

/**=============================================================================
 * @brief           按键任务
 *
 * @param[in]       arg: 任务参数指针
 *
 * @return          none
 *============================================================================*/
void app_key_task(void *arg)
{ 
    _process_key_poll();   
}

/**=============================================================================
 * @brief           按键初始化
 *
 * @param[in]       none
 *
 * @return          - XLI_ERR_NONE：成功
 *                  - others：失败
 *============================================================================*/
void app_key_init(void)
{
    bsp_key_init();

    for(uint8_t i = 0;i < BSP_KEY_NUM_MAX; i++)
    {
        _key_list[i].evt.sta = BSP_KEY_IDLE;
        _key_list[i].evt.clicks = 0;
        _key_list[i].cb = _key_default_callback;
    }

}
