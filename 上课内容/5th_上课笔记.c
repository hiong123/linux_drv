主要内容：平台总线
1，驱动模型---bus， driver， device
2，sysfs文件系统
3，平台总线的作用
4，平台总线的编程
5，平台总线中的自定义平台数据(platform_data)
6，内核中自带的pdev
=====================================================


bus， driver， device在哪里可以看的到---/sys目录
	/sys:记录系统中所有驱动的信息，以及之间的关系
		bus/
			devices
			drivers
			
如果要构建mybus, mydrv, mydev

认识三个对象：
	struct bus_type ：描述的是一个总线，比如名字，还是匹配方法
		struct bus_type {
				const char		*name; //名字
				int (*match)(struct device *dev, struct device_driver *drv);//匹配方法
				int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
				int (*probe)(struct device *dev);
				int (*remove)(struct device *dev);
				void (*shutdown)(struct device *dev);
		}
		
		int bus_register(struct bus_type * bus)
		void bus_unregister(struct bus_type * bus)
		
	struct device:描述设备的信息，是一个名词
		struct device {
			struct kobject kobj;
			const char		*init_name; /* initial name of the device */
			struct bus_type	*bus; //需要注册到哪个总线
			struct device_driver *driver;
			void		*platform_data; //自定义数据
		}
	
		int device_register(struct device * dev)
		void  device_unregister(struct device * dev)

	struct device_driver：对设备的操作方法--是一个动词
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
3，平台总线的作用：
	用于soc的升级，编写一个代码可以兼容多个平台


平台总线：
	struct bus_type platform_bus_type = {// 开机就注册，不需要去操作
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


	struct platform_device {//描述是一个不同平台下的硬件信息： 地址和中断
		const char	* name; //用于做匹配
		int		id; // 一般都设置成-1
		struct device	dev; //父类
		u32		num_resources;
		struct resource	* resource; //统一的描述中断和地址的信息的格式

	};
	
	struct resource {
		resource_size_t start; //起始
		resource_size_t end; //结束
		const char *name; //自定义
		unsigned long flags;//区分地址和中断
		struct resource *parent, *sibling, *child;
	};
	
	//获取资源
	struct resource *platform_get_resource(struct platform_device * dev,unsigned int type,unsigned int num)
	
	
	struct platform_driver {
		int (*probe)(struct platform_device *);
		int (*remove)(struct platform_device *);
		
		struct device_driver driver;//父类
		const struct platform_device_id *id_table;//用于和pdev进行匹配
	};


	编写一个驱动：led驱动，可以用在多个平台下
	
	
	
	


5，平台总线中的自定义平台数据(platform_data)
在pdev中 ：
方法1：
	//平台自定义的数据--pdev中的
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

方法2：提供了一个函数能够动态随时可以更换自定义数据
	// 参数所指向的数据就是自定义平台数据
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
	
	使用：
		s5pv210_led_set_platdata(&led_info);//led_info就变成了自定义数据



		
		

6，内核中自带的pdev-- make  s5pv210_defconfig
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
	smdkv210_machine_init(void)//在开机的时候就自定运行
		|
		s3c_fb_set_platdata(&smdkv210_lcd0_pdata);
		platform_add_devices(smdkv210_devices, ARRAY_SIZE(smdkv210_devices));
			|
			for (i = 0; i < num; i++) {
					ret = platform_device_register(devs[i]);
		
	如果自己定义的pdev需要在开机的时候(随着内核启动)而去自动的注册到总线中去：
		将pdev加入到数组smdkv210_devices
	




============================================================================
扩展： 读内核代码
	driver和device匹配成功之后，会自动调用drv中的probe方法
	
	
	driver_register(drv)
		|
		other = driver_find(drv->name, drv->bus);//在总线中查看是否已经注册了
		ret = bus_add_driver(drv);
			|
			bus = bus_get(drv->bus);//获取总线
			//分配并初始化priv对象
			priv = kzalloc(sizeof(*priv), GFP_KERNEL);
			klist_init(&priv->klist_devices, NULL, NULL);
			priv->driver = drv;//关联了当前要注册的driver
			drv->p = priv;
			priv->kobj.kset = bus->p->drivers_kset;
			error = kobject_init_and_add(&priv->kobj, &driver_ktype, NULL,"%s", drv->name);
			
			driver_attach(drv);//遍历device，并且匹配，并执行driver中probe
				|
				bus_for_each_dev(drv->bus, NULL, drv, __driver_attach);
					|
					klist_iter_init_node(&bus->p->klist_devices, &i,(start ? &start->p->knode_bus : NULL));
					while ((dev = next_device(&i)) && !error)//遍历所有的device
						error = fn(dev, data);//__driver_attach(dev,drv)调用多次
							|
							if (!driver_match_device(drv, dev))
									|
								//如果bus没有match方法，drv和device恒匹配
								drv->bus->match ? drv->bus->match(dev, drv) : 1;
								return 0;
							driver_probe_device(drv, dev);
								|
								really_probe(dev, drv);
									|
									if (dev->bus->probe) {//如果总线中有probe就直接调用总线的probe
										ret = dev->bus->probe(dev);
										if (ret)
											goto probe_failed;
									} else if (drv->probe) {//如果总线没有probe，就直接调用drv中的probe
											ret = drv->probe(dev);
						
			
			klist_add_tail(&priv->knode_bus, &bus->p->klist_drivers);//将driver加入到总线
			
			module_add_driver(drv->owner, drv);
			
			driver_create_file(drv, &driver_attr_uevent);// 创建/sys/bus/xxx/drivers/xxx/uevent
			
		ret = driver_add_groups(drv, drv->groups);











