 主要内容--电容触摸屏驱动
 1，触摸屏的基本工作原理
 2，电容触摸屏驱动框架
 3，gt811电容触摸屏的硬件特性和读取坐标方式
 4，linux中多点触摸的协议
 5，gt811驱动的编写
 =========================================
 
 硬件连接：
	gt811----------------- s5pv210
		SCL2 -----	GPD1_5
		SDA2  -----   GPD1_4
		INT		-----    XEINT14/GPH1_6
		RESET	-----    TOUT3/GPD0_3
 
gt811的寄存器:地址为16bit，寄存器的数据宽度为8bit
	配置寄存器
	输出寄存器：0x721寄存器开始，读取所有的坐标
		
 
gt811上电的序列：
	1，设置INT引脚为输入态，将reset设置成gpio内部上拉
	2，reset设置成输出低(开始复位)，延时1ms，并且设置成输入态
	3，延时至少20ms，通过i2c去寻址gt811
	4，如果有响应，就可以一次或多次写入106配置寄存器
 
编写触摸屏驱动：
	


	1，提供i2c client所需要的信息--arch/arm/mach-s5pv210/mach-smdkv210.c
		i2c_scl2 
		i2c_sda2
 
	static struct i2c_board_info smdkv210_i2c_devs2[] __initdata = {
		/* To Be Updated */
		{ I2C_BOARD_INFO("gt811_ts", 0x5d),}, 
	};
	
	[root@farsight /]# ls /sys/bus/i2c/devices/
		0-001b  0-0050  2-005d  i2c-0   i2c-1   i2c-2
	[root@farsight /]# cat /sys/bus/i2c/devices/2-005d/name
		gt811_ts

	
	2,编写i2c_driver
	
		uint8_t config_info[] = {
			0x06,0xA2,   //寄存器地址
			
			0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x05,0x55,0x15,0x55,0x25,0x55,\
			0x35,0x55,0x45,0x55,0x55,0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,\
			0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,0xF5,0x55,0x1B,0x03,0x00,0x00,0x00,0x13,\
			0x13,0x13,0x0F,0x0F,0x0A,0x50,0x30,0x0D,0x03,0x00,0x05,0x58,0x02,0x00,0x04,0x00,\
			0x00,0x32,0x2C,0x34,0x2E,0x00,0x00,0x04,0x14,0x22,0x04,0x00,0x00,0x00,0x00,0x00,\
			0x20,0x14,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,\
			0x25,0x28,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x01\
	};
	
	
	
	多点协议：
		上报两个点：
			   ABS_MT_POSITION_X x[0]//第一个点
			   ABS_MT_POSITION_Y y[0]  
			   SYN_MT_REPORT //第一个点上报完毕
			   ABS_MT_POSITION_X x[1]//第二个点
			   ABS_MT_POSITION_Y y[1]
			   SYN_MT_REPORT//第二个点上报完毕
			    ABS_MT_POSITION_X x[2]//第三个点
			   ABS_MT_POSITION_Y y[2]
			   SYN_MT_REPORT//第三个点上报完毕
			   SYN_REPORT // 所有点上报完毕
		
		代码中体现：
			input_event(inputdev, EV_ABS, ABS_MT_POSITION_X, 111);
			input_event(inputdev, EV_ABS, ABS_MT_POSITION_Y, 222);
			input_event(inputdev, EV_ABS, ABS_MT_PRESSURE, 222);
			input_event(inputdev, EV_ABS, ABS_MT_TRACKING_ID, 3);
			input_mt_sync(inputdev); // SYN_MT_REPORT
			
			input_event(inputdev, EV_ABS, ABS_MT_POSITION_X, 11);
			input_event(inputdev, EV_ABS, ABS_MT_POSITION_Y, 22);
			input_event(inputdev, EV_ABS, ABS_MT_PRESSURE, 22);
			input_event(inputdev, EV_ABS, ABS_MT_TRACKING_ID, 2);
			input_mt_sync(inputdev); // SYN_MT_REPORT
			
			input_sync(inputdev) //SYN_REPORT
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	