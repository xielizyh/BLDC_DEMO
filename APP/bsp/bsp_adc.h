/**
  ******************************************************************************
  * @file			bsp_adc.h
  * @brief			bsp_adc header file
  * @author			Xli
  * @email			xieliyzh@163.com
  * @version		1.0.0
  * @date			2020-05-08
  * @copyright		2020, XIELI Co.,Ltd. All rights reserved
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/**
* @brief ADC索引定义
*/
typedef enum {
    BSP_ADC_PHASE_U_CUR,		/*!< U相电流 */
    BSP_ADC_PHASE_V_CUR,		/*!< V相电流 */
    BSP_ADC_PHASE_W_CUR,		/*!< W相电流 */
    BSP_ADC_BUS_VOL,		    /*!< 总线电压 */
    BSP_ADC_POT_VOL,		    /*!< 端子电压 */
    BSP_ADC_NUM_MAX
}bsp_adc_num_t;

/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */
void bsp_adc_init(uint16_t *sample_buf);

#ifdef __cplusplus
}
#endif

#endif  /* __BSP_ADC_H_ */
