

/*================================================================
 *   
 *   
 *   文件名称：log_config.h
 *   创 建 者：肖飞
 *   创建日期：2021年03月22日 星期一 16时50分40秒
 *   修改日期：2022年03月18日 星期五 13时03分34秒
 *   描    述：
 *
 *================================================================*/
#ifndef _LOG_CONFIG_H
#define _LOG_CONFIG_H
#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(LOG_NONE)
#if !defined(LOG_UDP) && !defined(LOG_UART) && !defined(LOG_FILE)
#define LOG_UDP
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif //_LOG_CONFIG_H
