主要内容：linux下按键驱动的编写--学习中断编程
1，file，inode，cdev之间的关系
2，readl/writel接口和gpio库函数的使用
3，ioctl接口的实现
4，新的注册设备号的方法
5，中断在linux驱动的中处理
6，文件io模型--阻塞和非阻塞
======================================================

1，file，inode，cdev之间的关系
	
	struct file: 描述进程中打开的文件，进程中只要调用了open就有一个该对象
			具体描述了打开文件的路径，权限，标志，内部偏移
	
		struct file {
				struct path		f_path;
				atomic_long_t		f_count;
				unsigned int 		f_flags;
				fmode_t			f_mode;
				loff_t			f_pos;
				void			*private_data;
				const struct file_operations	*f_op;
			
		}
		
			
	struct inode：描述一个具体文件的属性，类型，权限，用户id，组id，修改时间，大小，主设备号，次设备号
			linux每个文件都有一个inode
			
		struct inode {
			umode_t			i_mode;
			uid_t			i_uid;
			gid_t			i_gid;	
			dev_t			i_rdev;
			loff_t			i_size;
			struct timespec		i_atime;
			struct timespec		i_mtime;
			struct timespec		i_ctime;
			const struct file_operations	*i_fop;
		}
	struct cdev：描述一个字符设备，包含设备号，fops(文件操作接口)
			每个字符设备都有一个该对象
			
		struct cdev {
			const struct file_operations *ops;
			struct list_head list;
			dev_t dev;
		};
		
	//通过file去获取到inode
	struct inode *inode = filp->f_path.dentry->d_inode;
	int major = imajor(inode);
	int minor = iminor(inode);
	printk("major = %d, minor = %d\n", major, minor);
	
	
	// inode此时就可以去区分当前打开的是哪个次设备
	int major = imajor(inode);
	int minor = iminor(inode);
	
	
	
	//利用所有的函数中都有一个filp,将数据存放在private_data中，就可以实现各个函数之间的数据共享
	led_drv_open()
		|
		static int a = 34;
		filp->private_data = &a;
	led_drv_write()
		|
		int a = *((int *)filp->private_data);
		printk("a== %d\n", a);
		

===========================================================
2，readl/writel接口和gpio库函数的使用	
	操作寄存器的地址的方法
	a， 直接地址操作
	
		*gpc0conf &= ~(0xff<<12); 
		*gpc0conf |= (0x11<<12);
	
	b，linux提供的函数接口readl/writel--跨平台的统一
		long readl(unsinged long *addr) //读取地址对应空间的值
		void writel(long value,  unsinged long *addr)//将数据写入到一个地址对应空间
			
		例子：
			writel(readl(led_dev->reg_base + 4) | (0x3<<3), led_dev->reg_base + 4);
			
		另外一种写法：
			__raw_readl(a)
			__raw_writel(d,ad)
			
		例子；
			unsigned long tmp = __raw_readl(led_dev->reg_base);
			tmp &= ~(0xff<<12);
			tmp |= (0x11<<12);
			__raw_writel(tmp, led_dev->reg_base);
		
	c, 各种原厂提供的gpio的库函数--只针对gpio的操作--操作简单直观
		不需要去操作地址(不需要映射)，只需要知道gpio的号码就号码
		GPC0_3/4
			号码的定义：
				S5PV210_GPC0(3) ==》整数
				S5PV210_GPC0(4)
				S5PV210_GPH0(3)
		
		
		//申请gpio的使用权
		int gpio_request(unsigned gpio,const char * label)

		//设置gpio为输入功能
		int gpio_direction_input(unsigned gpio)
		//设置gpio为输出，并且同时可以输出高低电平
		gpio_direction_output(unsigned gpio,int value)
		// 获取到gpio的数据
		int gpio_get_value(unsigned gpio);
		// 设置到gpio的数据
		void gpio_set_value(unsigned gpio, int value)

		//通过gpio的号码获取到对应中断号码
		int gpio_to_irq(unsigned gpio)
		//通过中断的号码获取到对应gpio号码
		int irq_to_gpio(unsigned irqno)

		//用于配置gpio的其他功能
		s3c_gpio_cfgpin(unsigned int pin,unsigned int config)
		//用于配置gpio的内部上拉和下拉
		s3c_gpio_setpull(unsigned int pin,s3c_gpio_pull_t pull)

		//释放gpio的使用权
		gpio_free(unsigned gpio)

		
=====================================================
3，ioctl接口的实现
	ioctl应用的文件io接口，给驱动发送指令(命令，cmd)，命令只是一个整数而已，应用发出来，驱动去识别
	
	需求：
		a,关闭某个灯  b，开某个灯  c，灯全开 d， 灯全灭
	
	
	模型：
			int ioctl(int fd, int cmd, ...);
		-----------------------------------------
		驱动中：long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
		xxx_ioctl()
		{
			switch(cmd){
				case 命令1 ：
					xxx
				case 命令2：
					xxx
				
			}
		}
		
		重要点：命令的定义(唯一)
			a, 直接用数字(不推荐，你自己定义的命令有可能和系统中某个已有的命令有冲突)
				#define LED_ON  3333
				#define LED_OFF 4444
				#define LED_NUM_ON  1111
				#define LED_NUM_OFF 2222
			b，用系统中提供的接口去定义命令
				_IO(type,nr)
				_IOR(type,nr,size)  ==》都是用于定义一个唯一的数字
				_IOW(type,nr,size)
				_IOWR(type,nr,size)
			
				参数：1，魔幻数--字符=='L', 'l'
					  2, 整数--区分不同命令
					  3，如果需要传递数据，size就是一个类型
					  
				#define LED_ON  _IO('L', 1000)
				#define LED_OFF _IO('L', 1001)
				#define LED_NUM_ON  _IOW('L', 1002, int)
				#define LED_NUM_OFF _IOW('L', 1003, int)
							
			

4，新的注册设备号的方法	

	1，旧的申请设备号的方式
		register_chrdev(0, "led_drv", &led_fops); 
			a， 申请了设备号而已
			b， 注册cdev
	
	2，新的申请设备号的方式
		静态：
			//参数1：指定设备号
			// 参数2：设备的个数
			//参数3 ：名字－－－/proc/devices显示
			int register_chrdev_region(dev_t from,unsigned count,const char * name)
		动态：
			int alloc_chrdev_region(dev_t * dev,unsigned baseminor,unsigned count,const char * name)
		以上两个函数都只是申请了设备号而已
		
		
		还要有以下代码进行配合：注册cdev
			cdev_alloc(void)
			cdev_init(struct cdev * cdev,const struct file_operations * fops)
			cdev_add(struct cdev * p,dev_t dev,unsigned count)
		
		
		
		编写的时候：
			struct s5pv210_led{
				 dev_t  devno;
				 struct cdev *cdev;
				 struct class *cls;
				 struct device *dev;
			};
		
		
		int major = 0;

		if(major > 0){
			led_dev->devno = MKDEV(major, 0);
			ret = register_chrdev_region(led_dev->devno, 1, "led_drv_new");
		}else{
		
			ret = alloc_chrdev_region(&led_dev->devno, 0,  1, "led_drv_new");
		}

		if(ret < 0){
			printk("register/alloc_chrdev_region error\n");
			goto err_0;
		}

		//构建cdev和注册cdev
		led_dev->cdev = cdev_alloc();
		cdev_init(led_dev->cdev, &led_fops);
		cdev_add(led_dev->cdev,led_dev->devno,1);
		
		
===============================================
5，中断在linux驱动的中处理
	外设和cpu进行异步交互的一种方式
	
	
	外设--------中断控制器vic-------fiq---------
							--------irq------cpu(A8)
	
	
	注册/申请中断
	int request_irq(unsigned int irq,irq_handler_t handler,unsigned long flags,const char * name,void * dev)
	//参数1--中断号码
			号码的定义：
	//参数2--中断处理方法
			irqreturn_t (*irq_handler_t)(int, void *);
			irqreturn_t key_irq_handle(int irqno, void *dev_id)
			{
				printk("---------*_*- %s----------\n", __FUNCTION__);

				return IRQ_HANDLED;

			}
	//参数3--触发方式
			#define IRQF_TRIGGER_NONE	0x00000000 //内部中断的触发方法
			#define IRQF_TRIGGER_RISING	0x00000001
			#define IRQF_TRIGGER_FALLING	0x00000002
			#define IRQF_TRIGGER_HIGH	0x00000004
			#define IRQF_TRIGGER_LOW	0x00000008
	//参数4--自定义字符串--/proc/interrupts
	//参数5--传递给参数2的值--可以填NULL
	//返回值--正确为0
	
	
	注销中断：
	void free_irq(unsigned int irq,void * dev_id)
	//参数1--中断号码
	//参数2--request_irq中断第五个参数是一样
	
	
	
	
	
	
	
	
	
	
	
	
		
		
		