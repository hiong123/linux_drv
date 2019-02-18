主要内容：linux下led驱动的编写
1，设备号注册
2，设备文件
3，映射
4，用户和内核数据交互
5，出错处理
6，gpio库函数的使用
==================================================
设备文件：
farsight@ubuntu:~$ ls /dev/input/event0 -l
crw-r----- 1 root root 13, 64 Aug 17 19:30 /dev/input/event0

设备号(32bit整数)== 主设备号+次设备号
					 12bit    20bit
					 
	主设备号：某一类设备---摄像头
	次设备号：某一类中某个设备--前置和后置摄像头
	
	
设备号 ==> dev_t == (250<<20)|0 <=== 250 + 0
	知道设备号，想要获取主设备/次设备号

	#define MAJOR(dev)	((unsigned int) ((dev) >> 20))
	#define MINOR(dev)	((unsigned int) ((dev) & 0xfffff))
	#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi)) //组合成设备号

申请主设备号：
	int register_chrdev(unsigned int major,const char * name,const struct file_operations * fops)
	参数1：可以指定你需要在主设备号--250以上（静态）
			此处填0，由系统自动分配，分配之后的主设备号通过返回值得到
			
	参数2：设备的描述，自定义－－/proc/devices
		如果去判断驱动主设备号，是否申请成功，查看/proc/devices文件
	
	参数3：文件操作集合对象
	
	
	返回值：正确返回０，或者大于０（动态分配）
			错误：返回负数

释放主设备号：
	void unregister_chrdev(unsigned int major,const char * name)
	参数1：需要释放的主设备号
	参数2：设备的描述
	
	
=============================================================
创建设备文件(设备节点)：应用如果需要去控制驱动，只能通过文件去控制
	1,手动创建
	
		mknod 文件名 设备类型 主设备号 次设备号
		
		
		例子：
			mknod /dev/cat  c  250 0   //主设备号一定要和驱动中申请的保持一致，次设备号可以自己定义，一般从0开始
		
		
		有缺陷：/dev/目录下文件都是在内存中，每次开机之后都需要手动创建，比较麻烦
	
	
	2，在代码中自动创建--驱动一旦装载后，就可以自动的创建设备节点
		
		//创建一个类
		struct  class *class_create(owner,name)
		
		用法：
			//自动创建设备节点  /dev/xxx  主  次
			//创建类
			//参数1--THIS_MODULE
			//参数2--字符串，自定义
			//返回值: 返回一个class指针
			simple_cls = class_create(THIS_MODULE, "simple_cls");
			
			删除类：
				class_destroy(struct class * cls)
		
		
		device_create(struct class * class,struct device * parent,dev_t devt,void * drvdata,const char * fmt,...)

		用法：
			// 创建设备节点
			// 参数1--class_create创建之后的类
			// 参数2--设备的父类--NULL
			// 参数3--设备号--主设备号+次设备号
			//参数4---私有数据--NULL
			//参数5--可变参数--表示设备节点的名字
			//返回值: 返回一个device指针
			simple_dev = device_create(simple_cls,NULL, MKDEV(dev_major, 0), NULL, "simple%d", 0);
	
		//最终效果--/dev/simple0,同时还创建/sys/class/simple_cls/simple0
	
		删除设备节点：
			// 参数1--class_create创建之后的类
			// 参数2--设备号--主设备号+次设备号
			device_destroy(struct class * class,dev_t devt)
=================================================================================
在驱动中去操作硬件的方法---就是操作硬件的虚拟地址
	虚拟地址： 将物理地址在mmu的协助下，转换而成的
	
	看数据手册的时候：看到的是寄存器的物理地址
	
	
	
	
	
	gpio: 多功能的引脚： 输入，输出，中断，i2c，spi, vsync等等
		1, 配置gpio的功能--输出
				GPC0CON, R/W, Address = 0xE0200060
				
				将GPC0_3/4 配置成输出---0001
				
				GPC0CON &= ~(0xff<<12); 
				GPC0CON |= (0x11<<12);
		
		2，输出一个高低电平
				GPC0DAT, R/W, Address = 0xE0200064
				
				点亮：
					GPC0DAT |= 0x3<<3;
				点亮：
					GPC0DAT &= ~(0x3<<3);	
					
	
	在代码中需要将物理地址转换成虚拟地址，然后再操作：
		void *ioremap(cookie,size)
		参数1：物理地址
		参数2：映射大小
		返回值：虚拟地址
		
		//取消映射
		void iounmap(void *addr);
		参数1：虚拟地址
		

	例子：
		volatile unsigned long *gpc0conf = ioremap(0xE0200060, 8);
		volatile unsigned long *gpc0data = gpc0conf + 1;
		 
		*gpc0conf &= ~(0xff<<12); 
		*gpc0conf |= (0x11<<12);


		*gpc0data |= 0x3<<3;


	
	
	// 0，构建一个全局设备对象--面向对象
	// 1， 申请主设备号
	// 2, 创建设备节点
	// 3,硬件的初始化---地址映射
	// 4,实现fops中各个 接口


	==========================================================
	应用程序和驱动数据交互：
	1，从用户空间中拷贝数据到驱动(在驱动的xxx_write)
		 long copy_from_user(void *to,const void __user * from, unsigned long n)
		//参数1：目的地,用户空间传递过来的地址
		//参数2：源头
		//参数3：个数
		返回值：还剩下多少没有拷贝成功 
				大于0表示出错
				等于0表示正确

	2，将驱动的数据拷贝到用户空间(在驱动的xxx_read)
		long copy_to_user(void __user *to,const void *from, unsigned long n)






任务：编写led驱动
1，熟悉字符设备驱动编程的步骤
2，熟悉各个函数的使用
3，做出错处理(a, 先不管出错， b，写led驱动的要出错处理)


samba配置：
[farsight]
path = /home/farsight/
available  = yes
browseable = yes
public     = yes
writable   = yes

改成：
[farsight]
path = /home/farsight/
available  = yes
browseable = yes
;public     = yes
valid users = farsight
writable   = yes


//为samba登陆设置密码
farsight@ubuntu:~$ sudo smbpasswd -a farsight
New SMB password:
Retype new SMB password:

//重启samba
farsight@ubuntu:~$ sudo service smbd restart
smbd stop/waiting
smbd start/running, process 12920






	
	