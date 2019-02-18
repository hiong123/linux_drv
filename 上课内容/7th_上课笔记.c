主要内容： i2c子系统--at24c02，e2prom驱动
1，i2c协议
2，i2c子系统框架
3，i2c子系统编程方式--从设备驱动开发
4，i2c子系统的对象分析
=============================================
1, i2c 从设备非常多：
	ts(坐标/控制命令)
	camera(控制命令)
	gsensor/陀螺仪mpu5060
	hdmi
	e2prom
	
	i2c是一个物理总线：scl和sda
		总线中传输数据
		

2,i2c子系统框架


	-------------------------------------------
	i2c driver层： 从设备驱动层
		1，与应用进行交互
		2，知道数据，但是不知道如何具体操作i2c控制
		3，不关心如何将数据写入硬件的细节
	------------------------------------------------------------------
	i2c-core层：
		1，维护了i2c 
		2，为上下两层提供接口
	-------------------------------------------------------------------
	i2c适配器层--i2c adapter层
		1,初始化i2c控制器
		2，知道如何将数据操作到硬件中去，但是不关心数据是什么
		3,创建i2c client
		
		
	struct i2c_client {
			unsigned short flags;		/* div., see below		*/
			unsigned short addr;// 7bit 的从设备地址
			char name[I2C_NAME_SIZE];//名字用于进行匹配
			struct i2c_adapter *adapter;//创建i2c  client对应的适配器
			struct i2c_driver *driver;	/* and our access routines	*/
			struct device dev;	//父类
			int irq;			/* irq issued by device		*/
			struct list_head detected;
	};
		
	struct i2c_adapter {
		struct module *owner;
		unsigned int class;		  /* classes to allow probing for */
		const struct i2c_algorithm *algo;//算法
						|
						int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs, int num);
		void *algo_data;

		/* data fields that are valid for all devices	*/
		struct rt_mutex bus_lock;

		int timeout;			/* in jiffies */
		int retries;
		struct device dev;	//父类

		int nr;
		char name[48];
		struct completion dev_released;

		struct mutex userspace_clients_lock;
		struct list_head userspace_clients;
	};
	
	struct i2c_driver {
			/* Standard driver model interfaces */
			int (*probe)(struct i2c_client *, const struct i2c_device_id *);
			int (*remove)(struct i2c_client *);
			struct device_driver driver;//父类
			const struct i2c_device_id *id_table;//比对的名字
	}	
	struct i2c_msg {
		__u16 addr;	//消息是发送给哪个从设备的，一般来自于i2c_client
		__u16 flags;//读还是写
		__u16 len;	//  读写多少
		__u8 *buf;	//指向一个缓冲区
	};	
	


3，i2c子系统编程方式
	0--i2c核心层和i2c 适配器层编译进内核：
		drivers/i2c/i2c-core.c
		drivers/i2c/busses/i2c-s3c2410.c
	
		make menuconfig
			Device Drivers  --->
				<*> I2C support  --->//选择i2c-core.c
					I2C Hardware Bus support  --->
						<*> S3C2410 I2C Driver //选择i2c-s3c2410.c
	
	1，需要一个i2c_client,类似于一个device，注册到总线
		i2c_client不需要直接去创建，只需要间接的创建就可以了
		arch/arm/mach-s5pv210/mach-smdkv210.c
		
		//只需要将从设备的地址和名字填入到数组即可：i2c_board_info中内容会被用于初始化i2c_client
		static struct i2c_board_info smdkv210_i2c_devs0[] __initdata = {
			{ I2C_BOARD_INFO("at24c02", 0x50), //名字可以随便给，用于匹配，7bit地址},     /* Samsung S524AD0XD1 */
			{ I2C_BOARD_INFO("wm8580", 0x1b), },
		};//修改此处

		static struct i2c_board_info smdkv210_i2c_devs1[] __initdata = {
			/* To Be Updated */
		};

		static struct i2c_board_info smdkv210_i2c_devs2[] __initdata = {
			/* To Be Updated */
		};
		
		smdkv210_machine_init()
			|
			//注册i2c从设备的信息，有了信息之后，系统会自动的创建一个i2c_client
			i2c_register_board_info(0, smdkv210_i2c_devs0,ARRAY_SIZE(smdkv210_i2c_devs0));
			i2c_register_board_info(1, smdkv210_i2c_devs1,ARRAY_SIZE(smdkv210_i2c_devs1));
			i2c_register_board_info(2, smdkv210_i2c_devs2,ARRAY_SIZE(smdkv210_i2c_devs2));
			
			
		
		重新内核编译:
			make uImage -j2
			
		更新内核：
			cp -raf arch/arm/boot/uImage /tftpboot
	
	
	2，需要一个i2c driver，注册到总线
	
	
	
		接口：
			int i2c_add_driver(struct i2c_driver *driver)
			void i2c_del_driver(struct i2c_driver *);
			
			//读从设备
			int i2c_master_recv(const struct i2c_client * client,char * buf,int count)
			//写从设备
			int i2c_master_send(const struct i2c_client * client,const char * buf,int count)

			//读写
			int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)

	

		int at24_i2c_read_combine(struct i2c_client *client, char *buf, int size)
		{

			int ret;
			struct i2c_adapter *adapter = client->adapter;


			struct i2c_msg msg[2];

			//写
			msg[0].addr = client->addr;
			msg[0].flags = 0;
			msg[0].len = 1;
			msg[0].buf = &buf[0];

			//读
			msg[1].addr = client->addr;
			msg[1].flags = I2C_M_RD;
			msg[1].len = size - 1;
			msg[1].buf = &buf[1];
			
			//参数3---消息的个数
			ret = i2c_transfer(adapter, msg, 2);

			return ret==2?size-1:ret;	

		}


	任务：读代码--倒叙
	
		1，i2c_register_board_info(0, smdkv210_i2c_devs0,
			ARRAY_SIZE(smdkv210_i2c_devs0));

		
		2，i2c client是如何创建的
			struct i2c_client *i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info)
			
		3，i2c_new_device是谁调用：
			i2c_add_numbered_adapter(struct i2c_adapter * adap)或者是
				 i2c_register_adapter(adap);


		4，i2c_add_numbered_adapter是谁调用？
			s3c24xx_i2c_probe(struct platform_device *pdev)


	
	正常顺序：i2c-s3c2410.c
		static int __init i2c_adap_s3c_init(void)
		{
			return platform_driver_register(&s3c24xx_i2c_driver);
						|
						s3c24xx_i2c_probe(struct platform_device *pdev)
							|
							i2c_add_numbered_adapter(&i2c->adap);
								|
								 i2c_register_adapter(adap);
									|
									i2c_scan_static_board_info(adap);
										|
										list_for_each_entry(devinfo, &__i2c_board_list, list) {
											if (devinfo->busnum == adapter->nr
													&& !i2c_new_device(adapter,&devinfo->board_info))
															|
															device_register(&client->dev);//注册到总线中
															
		}
		subsys_initcall(i2c_adap_s3c_init);