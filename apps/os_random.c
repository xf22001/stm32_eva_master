

/*================================================================
 *
 *
 *   文件名称：os_random.c
 *   创 建 者：肖飞
 *   创建日期：2021年08月25日 星期三 12时31分24秒
 *   修改日期：2021年08月25日 星期三 12时48分54秒
 *   描    述：
 *
 *================================================================*/
#include "app_platform.h"
#include "cmsis_os.h"

extern RNG_HandleTypeDef hrng;

int os_rand(void)
{
	uint32_t value;

	if(HAL_RNG_GenerateRandomNumber(&hrng, &value) != HAL_OK) {
	}

	return value;
}
