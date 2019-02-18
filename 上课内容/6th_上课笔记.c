主要内容： 输入子系统
1，输入子系统框架
2，输入子系统的编程方式
3，输入子系统的对象分析
4，输入子系统和平台总线的结合
5，输入子系统的代码阅读(理解从上层到底层的运行原理)
============================================================
输入子系统：管理和兼容不同的输入设备
	输入设备：
		1，键值类：产生的数据是一个键值--回车键，esc
			按键， 全键盘
		2，绝对坐标类：大小有一个最大值和最小值
			ts，重力传感器gsensor,陀螺仪
				x, y,z
				
		3，相对坐标类：
			mouse
			
	如果每个输入设备都按照字符设备驱动框架的编程步骤去做，多个设备就有多个类似的驱动(至少要写很多次fop)
	输入子系统：
		1，提供一个统一的驱动编程方式,不需要做很多事情(比如，申请设备号，创建设备节点，实现fops)
		2，对于应用空间而言，输入子系统提供了统一的编程方法(永远都拿一个数据包： struct input_event)

	
	
输入子系统的框架：
	
		应用层
	-------------------------------------------------
	驱动层：
			input handler层: 数据的处理层(周六福)
				负责将数据交给应用空间(copy_to_user)，但是不知道数据
				是如何从硬件中拿到
				内核已经写好了：drivers/input/evdev.c(通用的handler), joydev.c,mousedev.c 
				
			-----------------------------------------
			input 核心层：与上下两层进行交互(运输公司)
				维护了两个链表：handler和input dev链表
				内核已经写好了： drivers/input/input.c
			
			-----------------------------------------
			input device层：表示一个具体的输入设备(金矿)
				完成的是从硬件中获取数据
				(在这层进行开发)
			
			-----------------------------------------
			硬件
			
	确保内核已经有：input handler层和	input 核心层
	make menuconfig
		Device Drivers  --->
			Input device support  --->
				-*- Generic input layer (needed for keyboard, mouse, ...) // input.c
				<*>   Mouse interface	//mousedev.c 
				<*>   Event interface //evdev.c(通用的handler)
				
	
	
	
2，输入子系统的编程方式
			
		a,分配一个input device对象
		b,初始化input device对象
		c,注册input device对象
		d，初始化硬件，并获取到数据
			
			
		
		接口：
			//分配一个input device
			struct input_dev *input_allocate_device(void)
			void input_free_device(struct input_dev *dev)
			
			//注册input device
			int input_register_device(struct input_dev *dev)
			void input_unregister_device(struct input_dev *dev)
			
			//参数1--哪个设备上报
			//参数2--上报什么类型数据--按键
			//参数3--上报哪个具体的code--按键值
			// 参数4--上报数据---按键的状态
			void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
			
			其他类似函数：
				void input_report_key(struct input_dev *dev, unsigned int code, int value)
				void input_report_rel(struct input_dev *dev, unsigned int code, int value)
				void input_report_abs(struct input_dev *dev, unsigned int code, int value)
				void input_sync(struct input_dev *dev)
			
			
			struct input_event { //不管是哪个输入设备，用户拿到的数据都是这种类型--数据包
				struct timeval time;
				__u16 type; //数据类型--EV_KEY, EV_ABS, EV_REL
				__u16 code; 
				__s32 value;
			};
			
	
	
	struct input_dev {//描述设备的名字，厂商，能够产生哪些数据类型type，哪些code,哪些value
		//为设备提供一些信息---/sys/目录中显示
		const char *name;
		const char *phys;
		const char *uniq;
		struct input_id id;

		
		//以下全是数组，表示位表
		// evbit表示能够去设置该设备能够产生哪些类型数据:
		/*
			#define EV_SYN			0x00
			#define EV_KEY			0x01
			#define EV_REL			0x02
			#define EV_ABS			0x03
			#define EV_MSC			0x04
			#define EV_SW			0x05
			#define EV_LED			0x11
			#define EV_SND			0x12
			#define EV_REP			0x14
			#define EV_FF			0x15
			#define EV_PWR			0x16
			#define EV_FF_STATUS		0x17
			#define EV_MAX			0x1f
			#define EV_CNT			(EV_MAX+1)
			如果要产生按键数据，将evbit中的第1bit置1
			如果要产生绝对数据，将evbit中的第3bit置1		
		*/
		unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
		// keybit表示能够去设置该设备能够产生哪些按键:
		/*
			#define KEY_ESC			1
			#define KEY_DOWN		108
			#define KEY_PAGEDOWN		109
			#define KEY_INSERT		110
			#define KEY_DELETE		111
			#define KEY_MACRO		112
			#define KEY_MUTE		113
			如果要产生KEY_DOWN，将keybit中的第108bit置1
			如果要产生KEY_ESC，将keybit中的第1bit置1
		*/
		unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];// BITS_TO_LONGS(KEY_CNT) = 768/32 =unsigned long keybit[24];
		// relbit表示能够去设置该设备能够产生哪些相对数据:
		/*
			#define REL_X			0x00
			#define REL_Y			0x01
			#define REL_Z			0x02
			
			keybit[108/32] |= 1 << (108%32)
		*/
		unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
		// absbit表示能够去设置该设备能够产生哪些绝对数据:
		unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];// unsigned long absbit[2]==64bit
		
		unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
		unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
		unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
		unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
		unsigned long swbit[BITS_TO_LONGS(SW_CNT)];
		
		struct device dev;//描述的对象---名词

		struct list_head	h_list;//和儿子关联
		struct list_head	node;
		
	}
	

	
4，输入子系统和平台总线的结合
	平台总线在哪里可以用到：　
			只要涉及到操作控制器的寄存器地址和中断都可以用平台总线
			平台总线特点： 将操作硬件的方法和资源(硬件信息)分离
			平台总线是一个功能代码
			
			如果输入子系统中需要将操作方法和资源分离---可以用平台总线




	输入子系统：１，管理了输入设备
				２，完成将数据上报给用户（申请设备号，创建设备节点，实现fops）

				
	

	input + plat: 编写一个输入设备的驱动，该驱动能够在多个soc平台中驱动按键
	
	


5，输入子系统的代码阅读(理解从上层到底层的运行原理)--如何去理解框架代码



input  handler：evdev.c

module_init(evdev_init);
module_exit(evdev_exit);

static struct input_handler evdev_handler = {
	.event		= evdev_event,
	.connect	= evdev_connect,
	.disconnect	= evdev_disconnect,
	.fops		= &evdev_fops,
	.minor		= EVDEV_MINOR_BASE,
	.name		= "evdev",
	.id_table	= evdev_ids,
};
	evdev_init()
		|
		input_register_handler(&evdev_handler);
				|
				//初始化儿子
				INIT_LIST_HEAD(&handler->h_list);
				//将evdev_handler放在input_table[2]
				input_table[handler->minor >> 5] = handler;
				
				//注册到链表input_handler_list
				list_add_tail(&handler->node, &input_handler_list);

				//遍历所有的input device,进行匹配，匹配成功调用handler->connect
				list_for_each_entry(dev, &input_dev_list, node)
					input_attach_handler(dev, handler);
						|
						//evdev_handler可以匹配所有
						id = input_match_device(handler, dev);
						error = handler->connect(handler, dev, id);//调用了evdev_connect

======================================================================================
input core: input.c


static const struct file_operations input_fops = {
	.owner = THIS_MODULE,
	.open = input_open_file,
	.llseek = noop_llseek,
};

subsys_initcall(input_init);
module_exit(input_exit);
		input_init()
			|
			//注册了一个类/sys/class/input
			err = class_register(&input_class);
			
			// 在proc目录下创建/proc/bus/input/devices和handlers
			err = input_proc_init();
			// 注册了主设备号和fops
			err = register_chrdev(INPUT_MAJOR, "input", &input_fops);



===================================================
simple_key_input.c
	input_register_device(keyinput);
			|
			list_add_tail(&dev->node, &input_dev_list);

			list_for_each_entry(handler, &input_handler_list, node)
				input_attach_handler(dev, handler);
	

	
==============================================================



分析：evdev.c
evdev_connect
	|
	//在evdev_table找一个没有占用的位置，第一次调用minor=0
	for (minor = 0; minor < EVDEV_MINORS; minor++)
		if (!evdev_table[minor])
	
	//分配了一个对象evdev
	evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);	

	//初始化evdev
	INIT_LIST_HEAD(&evdev->client_list);
	spin_lock_init(&evdev->client_lock);
	mutex_init(&evdev->mutex);
	//初始化等待队列--用于阻塞
	init_waitqueue_head(&evdev->wait);
	
	//设置名字: event0
	dev_set_name(&evdev->dev, "event%d", minor);
	evdev->exist = true;
	evdev->minor = minor;

	//通过handle去记录input handler和input dev
	evdev->handle.dev = input_get_device(dev);
	evdev->handle.name = dev_name(&evdev->dev);
	evdev->handle.handler = handler;
	evdev->handle.private = evdev;


	//创建设备节点 struct device *device_create()
	//  创建了/dev/event0===/sys/class/input/event0
	evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);
	evdev->dev.class = &input_class;
	evdev->dev.parent = &dev->dev;
	evdev->dev.release = evdev_free;
	device_initialize(&evdev->dev);
	device_add(&evdev->dev);
	
	// handler和inputdev反向的和input handle儿子进行关联
	error = input_register_handle(&evdev->handle);
			|
			list_add_tail_rcu(&handle->d_node, &dev->h_list);
			list_add_tail_rcu(&handle->h_node, &handler->h_list);
	
	//将当前evdev放入到全局的数组中evdev_table
	error = evdev_install_chrdev(evdev);
			|
			evdev_table[evdev->minor] = evdev;





创建设备节点：等同于device_create()
	dev_set_name(&evdev->dev, "event%d", minor);
	evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);
	evdev->dev.class = &input_class;
	evdev->dev.parent = &dev->dev;
	evdev->dev.release = evdev_free;
	device_initialize(&evdev->dev);
	device_add(&evdev->dev);
	
	
	device_create()
		|
		device_create_vargs(class, parent, devt, drvdata, fmt, vargs);
			|
			dev = kzalloc(sizeof(*dev), GFP_KERNEL);
			dev->devt = devt;
			dev->class = class;
			dev->parent = parent;
			dev->release = device_create_release;
			dev_set_drvdata(dev, drvdata);
			device_register(dev);
				|
				device_initialize(dev);
				return device_add(dev);



				
===========================================================
应用：
	open()
	read();
=======================
	sys_open   sys_read
==============================
input.c中open
	.open = input_open_file,
			|
			//得到evdev handler
			handler = input_table[iminor(inode) >> 5];
			//得到handler中fops--evdev_fops
			if (handler)
				new_fops = fops_get(handler->fops);
			
			
			//evdev handler中fops给了file->f_op,后面read/write..xxx最终都会的调用
			//evdev handler中fops各个函数
			file->f_op = new_fops;
			//调用evdev handler中fops中的open
			err = new_fops->open(inode, file); //实际是evdev_fops->open()
		

static const struct file_operations evdev_fops = {
	.owner		= THIS_MODULE,
	.read		= evdev_read,
	.write		= evdev_write,
	.poll		= evdev_poll,
	.open		= evdev_open,
	.release	= evdev_release,
	.unlocked_ioctl	= evdev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= evdev_ioctl_compat,
#endif
	.fasync		= evdev_fasync,
	.flush		= evdev_flush,
	.llseek		= no_llseek,
};



evdev_open(struct inode *inode, struct file *file)
	|
	// event0==>i=0
	int i = iminor(inode) - EVDEV_MINOR_BASE;
	//分配一个缓冲区队列
	client = kzalloc(sizeof(struct evdev_client) +
				bufsize * sizeof(struct input_event),
			 GFP_KERNEL);
	
	client->bufsize = bufsize;
	spin_lock_init(&client->buffer_lock);
	client->evdev = evdev;
	//将client放入到evdev的链表
	evdev_attach_client(evdev, client);
	//将client缓冲区记录到file中以给其他的 read/write去共享
	file->private_data = client;














	