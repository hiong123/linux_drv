��Ҫ���ݣ� i2c��ϵͳ--at24c02��e2prom����
1��i2cЭ��
2��i2c��ϵͳ���
3��i2c��ϵͳ��̷�ʽ--���豸��������
4��i2c��ϵͳ�Ķ������
=============================================
1, i2c ���豸�ǳ��ࣺ
	ts(����/��������)
	camera(��������)
	gsensor/������mpu5060
	hdmi
	e2prom
	
	i2c��һ���������ߣ�scl��sda
		�����д�������
		

2,i2c��ϵͳ���


	-------------------------------------------
	i2c driver�㣺 ���豸������
		1����Ӧ�ý��н���
		2��֪�����ݣ����ǲ�֪����ξ������i2c����
		3����������ν�����д��Ӳ����ϸ��
	------------------------------------------------------------------
	i2c-core�㣺
		1��ά����i2c 
		2��Ϊ���������ṩ�ӿ�
	-------------------------------------------------------------------
	i2c��������--i2c adapter��
		1,��ʼ��i2c������
		2��֪����ν����ݲ�����Ӳ����ȥ�����ǲ�����������ʲô
		3,����i2c client
		
		
	struct i2c_client {
			unsigned short flags;		/* div., see below		*/
			unsigned short addr;// 7bit �Ĵ��豸��ַ
			char name[I2C_NAME_SIZE];//�������ڽ���ƥ��
			struct i2c_adapter *adapter;//����i2c  client��Ӧ��������
			struct i2c_driver *driver;	/* and our access routines	*/
			struct device dev;	//����
			int irq;			/* irq issued by device		*/
			struct list_head detected;
	};
		
	struct i2c_adapter {
		struct module *owner;
		unsigned int class;		  /* classes to allow probing for */
		const struct i2c_algorithm *algo;//�㷨
						|
						int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs, int num);
		void *algo_data;

		/* data fields that are valid for all devices	*/
		struct rt_mutex bus_lock;

		int timeout;			/* in jiffies */
		int retries;
		struct device dev;	//����

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
			struct device_driver driver;//����
			const struct i2c_device_id *id_table;//�ȶԵ�����
	}	
	struct i2c_msg {
		__u16 addr;	//��Ϣ�Ƿ��͸��ĸ����豸�ģ�һ��������i2c_client
		__u16 flags;//������д
		__u16 len;	//  ��д����
		__u8 *buf;	//ָ��һ��������
	};	
	


3��i2c��ϵͳ��̷�ʽ
	0--i2c���Ĳ��i2c �������������ںˣ�
		drivers/i2c/i2c-core.c
		drivers/i2c/busses/i2c-s3c2410.c
	
		make menuconfig
			Device Drivers  --->
				<*> I2C support  --->//ѡ��i2c-core.c
					I2C Hardware Bus support  --->
						<*> S3C2410 I2C Driver //ѡ��i2c-s3c2410.c
	
	1����Ҫһ��i2c_client,������һ��device��ע�ᵽ����
		i2c_client����Ҫֱ��ȥ������ֻ��Ҫ��ӵĴ����Ϳ�����
		arch/arm/mach-s5pv210/mach-smdkv210.c
		
		//ֻ��Ҫ�����豸�ĵ�ַ���������뵽���鼴�ɣ�i2c_board_info�����ݻᱻ���ڳ�ʼ��i2c_client
		static struct i2c_board_info smdkv210_i2c_devs0[] __initdata = {
			{ I2C_BOARD_INFO("at24c02", 0x50), //���ֿ�������������ƥ�䣬7bit��ַ},     /* Samsung S524AD0XD1 */
			{ I2C_BOARD_INFO("wm8580", 0x1b), },
		};//�޸Ĵ˴�

		static struct i2c_board_info smdkv210_i2c_devs1[] __initdata = {
			/* To Be Updated */
		};

		static struct i2c_board_info smdkv210_i2c_devs2[] __initdata = {
			/* To Be Updated */
		};
		
		smdkv210_machine_init()
			|
			//ע��i2c���豸����Ϣ��������Ϣ֮��ϵͳ���Զ��Ĵ���һ��i2c_client
			i2c_register_board_info(0, smdkv210_i2c_devs0,ARRAY_SIZE(smdkv210_i2c_devs0));
			i2c_register_board_info(1, smdkv210_i2c_devs1,ARRAY_SIZE(smdkv210_i2c_devs1));
			i2c_register_board_info(2, smdkv210_i2c_devs2,ARRAY_SIZE(smdkv210_i2c_devs2));
			
			
		
		�����ں˱���:
			make uImage -j2
			
		�����ںˣ�
			cp -raf arch/arm/boot/uImage /tftpboot
	
	
	2����Ҫһ��i2c driver��ע�ᵽ����
	
	
	
		�ӿڣ�
			int i2c_add_driver(struct i2c_driver *driver)
			void i2c_del_driver(struct i2c_driver *);
			
			//�����豸
			int i2c_master_recv(const struct i2c_client * client,char * buf,int count)
			//д���豸
			int i2c_master_send(const struct i2c_client * client,const char * buf,int count)

			//��д
			int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)

	

		int at24_i2c_read_combine(struct i2c_client *client, char *buf, int size)
		{

			int ret;
			struct i2c_adapter *adapter = client->adapter;


			struct i2c_msg msg[2];

			//д
			msg[0].addr = client->addr;
			msg[0].flags = 0;
			msg[0].len = 1;
			msg[0].buf = &buf[0];

			//��
			msg[1].addr = client->addr;
			msg[1].flags = I2C_M_RD;
			msg[1].len = size - 1;
			msg[1].buf = &buf[1];
			
			//����3---��Ϣ�ĸ���
			ret = i2c_transfer(adapter, msg, 2);

			return ret==2?size-1:ret;	

		}


	���񣺶�����--����
	
		1��i2c_register_board_info(0, smdkv210_i2c_devs0,
			ARRAY_SIZE(smdkv210_i2c_devs0));

		
		2��i2c client����δ�����
			struct i2c_client *i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info)
			
		3��i2c_new_device��˭���ã�
			i2c_add_numbered_adapter(struct i2c_adapter * adap)������
				 i2c_register_adapter(adap);


		4��i2c_add_numbered_adapter��˭���ã�
			s3c24xx_i2c_probe(struct platform_device *pdev)


	
	����˳��i2c-s3c2410.c
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
															device_register(&client->dev);//ע�ᵽ������
															
		}
		subsys_initcall(i2c_adap_s3c_init);