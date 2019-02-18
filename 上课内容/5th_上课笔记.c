��Ҫ���ݣ�ƽ̨����
1������ģ��---bus�� driver�� device
2��sysfs�ļ�ϵͳ
3��ƽ̨���ߵ�����
4��ƽ̨���ߵı��
5��ƽ̨�����е��Զ���ƽ̨����(platform_data)
6���ں����Դ���pdev
=====================================================


bus�� driver�� device��������Կ��ĵ�---/sysĿ¼
	/sys:��¼ϵͳ��������������Ϣ���Լ�֮��Ĺ�ϵ
		bus/
			devices
			drivers
			
���Ҫ����mybus, mydrv, mydev

��ʶ��������
	struct bus_type ����������һ�����ߣ��������֣�����ƥ�䷽��
		struct bus_type {
				const char		*name; //����
				int (*match)(struct device *dev, struct device_driver *drv);//ƥ�䷽��
				int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
				int (*probe)(struct device *dev);
				int (*remove)(struct device *dev);
				void (*shutdown)(struct device *dev);
		}
		
		int bus_register(struct bus_type * bus)
		void bus_unregister(struct bus_type * bus)
		
	struct device:�����豸����Ϣ����һ������
		struct device {
			struct kobject kobj;
			const char		*init_name; /* initial name of the device */
			struct bus_type	*bus; //��Ҫע�ᵽ�ĸ�����
			struct device_driver *driver;
			void		*platform_data; //�Զ�������
		}
	
		int device_register(struct device * dev)
		void  device_unregister(struct device * dev)

	struct device_driver�����豸�Ĳ�������--��һ������
		struct device_driver {
			const char		*name;
			struct bus_type		*bus;
		
			int (*probe) (struct device *dev);
			int (*remove) (struct device *dev);
			void (*shutdown) (struct device *dev);
			int (*suspend) (struct device *dev, pm_message_t state);
			int (*resume) (struct device *dev);	
		}
		int driver_register(struct device_driver * drv)
		void driver_unregister(struct device_driver * drv)

		
		
=========================================================
3��ƽ̨���ߵ����ã�
	����soc����������дһ��������Լ��ݶ��ƽ̨


ƽ̨���ߣ�
	struct bus_type platform_bus_type = {// ������ע�ᣬ����Ҫȥ����
		.name		= "platform",
		.dev_attrs	= platform_dev_attrs,
		.match		= platform_match,
		.uevent		= platform_uevent,
		.pm		= &platform_dev_pm_ops,
	};
	static int platform_match(struct device *dev, struct device_driver *drv)
	{
		struct platform_device *pdev = to_platform_device(dev);
		struct platform_driver *pdrv = to_platform_driver(drv);

		/* Attempt an OF style match first */
		if (of_driver_match_device(dev, drv))
			return 1;

		/* Then try to match against the id table */
		if (pdrv->id_table)
			return platform_match_id(pdrv->id_table, pdev) != NULL;

		/* fall-back to driver name match */
		return (strcmp(pdev->name, drv->name) == 0);
	}


	struct platform_device {//������һ����ͬƽ̨�µ�Ӳ����Ϣ�� ��ַ���ж�
		const char	* name; //������ƥ��
		int		id; // һ�㶼���ó�-1
		struct device	dev; //����
		u32		num_resources;
		struct resource	* resource; //ͳһ�������жϺ͵�ַ����Ϣ�ĸ�ʽ

	};
	
	struct resource {
		resource_size_t start; //��ʼ
		resource_size_t end; //����
		const char *name; //�Զ���
		unsigned long flags;//���ֵ�ַ���ж�
		struct resource *parent, *sibling, *child;
	};
	
	//��ȡ��Դ
	struct resource *platform_get_resource(struct platform_device * dev,unsigned int type,unsigned int num)
	
	
	struct platform_driver {
		int (*probe)(struct platform_device *);
		int (*remove)(struct platform_device *);
		
		struct device_driver driver;//����
		const struct platform_device_id *id_table;//���ں�pdev����ƥ��
	};


	��дһ��������led�������������ڶ��ƽ̨��
	
	
	
	


5��ƽ̨�����е��Զ���ƽ̨����(platform_data)
��pdev�� ��
����1��
	//ƽ̨�Զ��������--pdev�е�
	struct led_platdata led_info = {
		.name = "GPC0_3/4",
		.shift = 3,
	};
	
	struct platform_device  led_pdev = {
		.name = "s5pv210_led",
		.id = -1,
		.num_resources = ARRAY_SIZE(led_res),
		.resource = led_res,
		.dev = {
			.platform_data = &led_info,
		},

	};

����2���ṩ��һ�������ܹ���̬��ʱ���Ը����Զ�������
	// ������ָ������ݾ����Զ���ƽ̨����
	void __init s5pv210_led_set_platdata(struct led_platdata *pd)
	{
		struct led_platdata *npd;

		if (!pd) {
			printk(KERN_ERR "%s: no platform data\n", __func__);
			return;
		}

		npd = kmemdup(pd, sizeof(struct led_platdata), GFP_KERNEL);
		if (!npd)
			printk(KERN_ERR "%s: no memory for platform data\n", __func__);

		led_pdev.dev.platform_data = npd;
	}
	
	ʹ�ã�
		s5pv210_led_set_platdata(&led_info);//led_info�ͱ�����Զ�������



		
		

6���ں����Դ���pdev-- make  s5pv210_defconfig
	arch/arm/mach-s5pv210/mach-smdkv210.c
	
	static struct platform_device *smdkv210_devices[] __initdata = {
		&s3c_device_adc,
		&s3c_device_cfcon,
		&s3c_device_fb,
		&s3c_device_hsmmc0,
		&s3c_device_hsmmc1,
		&s3c_device_hsmmc2,
		&s3c_device_hsmmc3,
		&s3c_device_i2c0,
		&s3c_device_i2c1,
		&s3c_device_i2c2,
		&s3c_device_rtc,
		&s3c_device_ts,
		&s3c_device_wdt,
		&s5pv210_device_ac97,
		&s5pv210_device_iis0,
		&s5pv210_device_spdif,
		&samsung_asoc_dma,
		&samsung_device_keypad,
		&smdkv210_dm9000,
		&smdkv210_lcd_lte480wv,
		&s3c_device_timer[3],
		&smdkv210_backlight_device,
	};
	smdkv210_machine_init(void)//�ڿ�����ʱ����Զ�����
		|
		s3c_fb_set_platdata(&smdkv210_lcd0_pdata);
		platform_add_devices(smdkv210_devices, ARRAY_SIZE(smdkv210_devices));
			|
			for (i = 0; i < num; i++) {
					ret = platform_device_register(devs[i]);
		
	����Լ������pdev��Ҫ�ڿ�����ʱ��(�����ں�����)��ȥ�Զ���ע�ᵽ������ȥ��
		��pdev���뵽����smdkv210_devices
	




============================================================================
��չ�� ���ں˴���
	driver��deviceƥ��ɹ�֮�󣬻��Զ�����drv�е�probe����
	
	
	driver_register(drv)
		|
		other = driver_find(drv->name, drv->bus);//�������в鿴�Ƿ��Ѿ�ע����
		ret = bus_add_driver(drv);
			|
			bus = bus_get(drv->bus);//��ȡ����
			//���䲢��ʼ��priv����
			priv = kzalloc(sizeof(*priv), GFP_KERNEL);
			klist_init(&priv->klist_devices, NULL, NULL);
			priv->driver = drv;//�����˵�ǰҪע���driver
			drv->p = priv;
			priv->kobj.kset = bus->p->drivers_kset;
			error = kobject_init_and_add(&priv->kobj, &driver_ktype, NULL,"%s", drv->name);
			
			driver_attach(drv);//����device������ƥ�䣬��ִ��driver��probe
				|
				bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);
					|
					klist_iter_init_node(&bus->p->klist_devices, &i,(start ? &start->p->knode_bus : NULL));
					while ((dev = next_device(&i)) && !error)//�������е�device
						error = fn(dev, data);//__driver_attach(dev,drv)���ö��
							|
							if (!driver_match_device(drv, dev))
									|
								//���busû��match������drv��device��ƥ��
								drv->bus->match ? drv->bus->match(dev, drv) : 1;
								return 0;
							driver_probe_device(drv, dev);
								|
								really_probe(dev, drv);
									|
									if (dev->bus->probe) {//�����������probe��ֱ�ӵ������ߵ�probe
										ret = dev->bus->probe(dev);
										if (ret)
											goto probe_failed;
									} else if (drv->probe) {//�������û��probe����ֱ�ӵ���drv�е�probe
											ret = drv->probe(dev);
						
			
			klist_add_tail(&priv->knode_bus, &bus->p->klist_drivers);//��driver���뵽����
			
			module_add_driver(drv->owner, drv);
			
			driver_create_file(drv, &driver_attr_uevent);// ����/sys/bus/xxx/drivers/xxx/uevent
			
		ret = driver_add_groups(drv, drv->groups);











