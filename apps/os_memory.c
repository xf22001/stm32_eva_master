

/*================================================================
 *   
 *   
 *   文件名称：os_memory.c
 *   创 建 者：肖飞
 *   创建日期：2021年02月04日 星期四 10时23分08秒
 *   修改日期：2021年02月04日 星期四 10时24分16秒
 *   描    述：
 *
 *================================================================*/
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

void *port_malloc(size_t size)
{
	return pvPortMalloc(size);
}

void port_free(void *p)
{
	vPortFree(p);
}

uint32_t get_total_heap_size(void)
{
	return (uint32_t)configTOTAL_HEAP_SIZE;
}
