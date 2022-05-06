

/*================================================================
 *
 *
 *   文件名称：gpio_map.c
 *   创 建 者：肖飞
 *   创建日期：2021年06月04日 星期五 22时59分24秒
 *   修改日期：2021年06月04日 星期五 23时00分31秒
 *   描    述：
 *
 *================================================================*/
#include "app_platform.h"
#include "cmsis_os.h"

#include "main.h"

void set_lan_led(GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(led_lan_GPIO_Port, led_lan_Pin, PinState);
}
