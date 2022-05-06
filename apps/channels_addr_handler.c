

/*================================================================
 *
 *
 *   文件名称：channels_addr_handler.c
 *   创 建 者：肖飞
 *   创建日期：2021年07月16日 星期五 14时03分28秒
 *   修改日期：2022年02月15日 星期二 16时01分09秒
 *   描    述：
 *
 *================================================================*/
#include "modbus_data_value.h"
#include "app.h"
#include "channels.h"
#include "display.h"

#include "log.h"

void channels_modbus_data_action(void *fn_ctx, void *chain_ctx)
{
	channels_info_t *channels_info = (channels_info_t *)fn_ctx;
	channels_settings_t *channels_settings = &channels_info->channels_settings;
	modbus_data_ctx_t *modbus_data_ctx = (modbus_data_ctx_t *)chain_ctx;

	switch(modbus_data_ctx->addr) {
		case 5: {//枪数设置	0：单枪 1：双枪
			modbus_data_value_r(modbus_data_ctx, channels_info->channel_number);
		}
		break;

		case 6: {//是否刷卡	0：否 1：是
			modbus_data_value_rw(modbus_data_ctx, channels_settings->authorize);
		}
		break;

		case 7: {//后台设置	0:无 1：SSE 2:OCPP
		}
		break;

		case 8 ... 11: {//IP	ASCII
		}
		break;

		case 12: {//端口号	ASCII
		}
		break;

		case 13 ... 17: {//预留IP	ASCII
		}
		break;

		case 18: {//服务费设置	0.01元
		}
		break;

		case 19: {//时间小时	BCD
		}
		break;

		case 20: {//时间分钟	BCD
		}
		break;

		case 21: {//电价	0.01元
		}
		break;

		case 22 ... 78: {//计费设置	同上
		}
		break;

		case 79 ... 94: {//设备编号	ASCII
		}
		break;

		case 95: {//A枪额定电流	 0：16  1：32  2：63
		}
		break;

		case 96: {//是否验证标记	0：否 1：是
		}
		break;

		case 97: {//是否锁卡	0：否 1：是
		}
		break;

		case 98: {//预付款高	0.01元
		}
		break;

		case 99: {//预付款低	0.01元
		}
		break;

		case 100: {//A枪额定电流	1A
		}
		break;

		case 101: {//联网方式	0：网口 1：DTU
		}
		break;

		case 102: {//刷卡板类型	0:626 1:628 3：ZLG
		}
		break;

		case 103: {//电表设置	0：无 1:645 2：CED
		}
		break;

		case 104: {//HMI 设置	0：DW 2:BK 3:无
		}
		break;

		case 105: {//交流桩类型	0：单相 1：三相
		}
		break;

		case 106: {//预留
		}
		break;

		case 107: {//PE检测	0：是 1：否
		}
		break;

		case 108 ... 109: {//标记设置	ASCII
		}
		break;

		case 110 ... 299: {//预留
		}
		break;

		case 300: {//主板版本号	BCD
		}
		break;

		case 301: {//系统时间 秒	BCD
		}
		break;

		case 302: {//系统时间 分	BCD
		}
		break;

		case 303: {//系统时间 时	BCD
		}
		break;

		case 304: {//系统时间 日	BCD
		}
		break;

		case 305: {//系统时间 月	BCD
		}
		break;

		case 306: {//系统时间 年	BCD
		}
		break;

		case 307: {//系统时间 周	BCD
		}
		break;

		case 308: {//故障状态
		}
		break;

		case 309: {//输入电压
		}
		break;

		case 310: {//刷卡板状态
		}
		break;

		case 311: {//网络状态
		}
		break;

		case 312: {//系统运行时间	1min
		}
		break;

		case 313: {//CPU使用率	%
		}
		break;

		case 314: {//提示地址
		}
		break;

		case 315: {//当前电价	0.01元
		}
		break;

		case 316: {//主板上传信息
		}
		break;

		case 317: {//屏下发消息
		}
		break;

		case 318 ... 333: {//密码输入区	ASCII
		}
		break;

		case 334 ... 349: {//账户名输入区	ASCII
		}
		break;

		case 366: {//账户余额高
		}
		break;

		case 367: {//账户余额低
		}
		break;

		case 368: {//控制板温度
		}
		break;

		case 369: {//显示屏状态
		}
		break;

		case 370: {//时间校正标记
		}
		break;

		case 371 ... 499: {//预留
		}
		break;

		case 500: {//充电状态
		}
		break;

		case 501: {//充电电压
		}
		break;

		case 502: {//充电电流
		}
		break;

		case 503: {//充电电量高
		}
		break;

		case 504: {//充电电量低
		}
		break;

		case 505: {//充电时长
		}
		break;

		case 506 ... 521: {//充电账户
		}
		break;

		case 522: {//账户余额高
		}
		break;

		case 523: {//账户余额低
		}
		break;

		case 524: {//消费金额高
		}
		break;

		case 525: {//消费金额低
		}
		break;

		case 526: {//电量读数低
		}
		break;

		case 527: {//电量读数高
		}
		break;

		case 528: {//充电电压B
		}
		break;

		case 529: {//充电电流B
		}
		break;

		case 530: {//充电电压C
		}
		break;

		case 531: {//充电电流C
		}
		break;

		case 532: {//二维码显示
		}
		break;

		case 533: {//充电功率高
		}
		break;

		case 534: {//充电功率低
		}
		break;

		case 544 ... 599: {//预留
		}
		break;

		case 600: {//开关机状态
		}
		break;

		case 601: {//插枪状态
		}
		break;

		case 602: {//电表状态
		}
		break;

		case 603: {//停止充电原因
		}
		break;

		case 604: {//开始充电原因
		}
		break;

		case 605: {//枪是否锁定
		}
		break;

		case 606: {//枪头温度
		}
		break;

		case 607: {//CP电压
		}
		break;

		case 608: {//占空比
		}
		break;

		case 609: {//频率
		}
		break;

		//case 610 ... 699: {//预留
		//}
		//break;

		case 650: {//充电模式设置
		}
		break;

		case 651: {//设定充电金额
		}
		break;

		case 652: {//开始充电时间时	BCD
		}
		break;

		case 653: {//开始充电时间分	BCD
		}
		break;

		case 654: {//结束充电时间时	BCD
		}
		break;

		case 655: {//结束充电时间分	BCD
		}
		break;

		case 656: {//设定充电电量
		}
		break;

		case 657: {//设定充电时间
		}
		break;

		case 658 ... 699: {//预留
		}
		break;

		case 700 ... 899: {//B枪
		}
		break;

		case 900: {//记录查询 年	BCD
		}
		break;

		case 901: {//记录查询 月	BCD
		}
		break;

		case 902: {//记录查询 日	BCD
		}
		break;

		case 903: {//查询确认	按键 下发1
		}
		break;

		case 904: {//未上传记录数
		}
		break;

		case 905: {//记录总数
		}
		break;

		case 906: {//枪号
		}
		break;

		case 907 ... 922: {//账户号	ASCII
		}
		break;

		case 923: {//开始时间时+分 	BCD
		}
		break;

		case 924: {//结束时间时+分	BCD
		}
		break;

		case 925: {//充电量高
		}
		break;

		case 926: {//充电量低
		}
		break;

		case 927: {//消费金额高
		}
		break;

		case 928: {//消费金额低
		}
		break;

		case 929: {//开始充电时间
		}
		break;

		case 930: {//结束充电时间
		}
		break;

		default:
			debug("error! op:%s, addr:%d",
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_GET) ? "get" :
			      (modbus_data_ctx->action == MODBUS_DATA_ACTION_SET) ? "set" :
			      "unknow",
			      modbus_data_ctx->addr);
			break;
	}
}
