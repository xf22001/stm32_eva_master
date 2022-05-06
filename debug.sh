#!/bin/bash

#================================================================
#   Copyright (C) 2019年08月28日 肖飞 All rights reserved
#   
#   文件名称：debug.sh
#   创 建 者：肖飞
#   创建日期：2019年08月28日 星期三 08时45分08秒
#   修改日期：2020年04月27日 星期一 14时19分56秒
#   描    述：
#
#================================================================
function main() {
	#arm-none-eabi-gdb -x gdb_init
	#cgdb -d arm-none-eabi-gdb -- -x gdb_init
	gdb-multiarch -x gdb_init
}

main $@
