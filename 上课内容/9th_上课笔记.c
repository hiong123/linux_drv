 ��Ҫ����--���ݴ���������
 1���������Ļ�������ԭ��
 2�����ݴ������������
 3��gt811���ݴ�������Ӳ�����ԺͶ�ȡ���귽ʽ
 4��linux�ж�㴥����Э��
 5��gt811�����ı�д
 =========================================
 
 Ӳ�����ӣ�
	gt811----------------- s5pv210
		SCL2 -----	GPD1_5
		SDA2  -----   GPD1_4
		INT		-----    XEINT14/GPH1_6
		RESET	-----    TOUT3/GPD0_3
 
gt811�ļĴ���:��ַΪ16bit���Ĵ��������ݿ��Ϊ8bit
	���üĴ���
	����Ĵ�����0x721�Ĵ�����ʼ����ȡ���е�����
		
 
gt811�ϵ�����У�
	1������INT����Ϊ����̬����reset���ó�gpio�ڲ�����
	2��reset���ó������(��ʼ��λ)����ʱ1ms���������ó�����̬
	3����ʱ����20ms��ͨ��i2cȥѰַgt811
	4���������Ӧ���Ϳ���һ�λ���д��106���üĴ���
 
��д������������
	


	1���ṩi2c client����Ҫ����Ϣ--arch/arm/mach-s5pv210/mach-smdkv210.c
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

	
	2,��дi2c_driver
	
		uint8_t config_info[] = {
			0x06,0xA2,   //�Ĵ�����ַ
			
			0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,0x05,0x55,0x15,0x55,0x25,0x55,\
			0x35,0x55,0x45,0x55,0x55,0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,\
			0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,0xF5,0x55,0x1B,0x03,0x00,0x00,0x00,0x13,\
			0x13,0x13,0x0F,0x0F,0x0A,0x50,0x30,0x0D,0x03,0x00,0x05,0x58,0x02,0x00,0x04,0x00,\
			0x00,0x32,0x2C,0x34,0x2E,0x00,0x00,0x04,0x14,0x22,0x04,0x00,0x00,0x00,0x00,0x00,\
			0x20,0x14,0xEC,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x30,\
			0x25,0x28,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x01\
	};
	
	
	
	���Э�飺
		�ϱ������㣺
			   ABS_MT_POSITION_X x[0]//��һ����
			   ABS_MT_POSITION_Y y[0]  
			   SYN_MT_REPORT //��һ�����ϱ����
			   ABS_MT_POSITION_X x[1]//�ڶ�����
			   ABS_MT_POSITION_Y y[1]
			   SYN_MT_REPORT//�ڶ������ϱ����
			    ABS_MT_POSITION_X x[2]//��������
			   ABS_MT_POSITION_Y y[2]
			   SYN_MT_REPORT//���������ϱ����
			   SYN_REPORT // ���е��ϱ����
		
		���������֣�
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	