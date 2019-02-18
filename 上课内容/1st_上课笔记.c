运行开发板：
1，uboot的用法：
	FS210 # help nand
	nand info             - show available NAND devices
	nand device [dev]     - show or set current device
	nand read[.jffs2]     - addr off|partition size
	nand write[.jffs2]    - addr off|partiton size - read/write `size' bytes starting
		at offset `off' to/from memory address `addr'
	nand write[.yaffs[1]] - addr off|partition size - write `size' byte yaffs image
		starting at offset `off' from memory address `addr' (.yaffs1 for 512+16 NAND)
	nand erase [clean] [off size] - erase `size' bytes from
		offset `off' (entire device if not specified)
	nand bad - show bad blocks
	nand dump[.oob] off - dump page
	nand scrub - really clean NAND erasing bad blocks (UNSAFE)
	nand markbad off - mark bad block at offset (UNSAFE)
	nand biterr off - make a bit error at offset (UNSAFE)
	nand lock [tight] [status] - bring nand to lock state or display locked pages
	nand unlock [offset] [size] - unlock section
	
	
	
	操作nandflash的命令：
		写nand：
			nand erase 0x100000 0x400000
			nand write 0x40008000 0x100000 0x400000
			
		读nand:
			nand read 0x40008000 0x100000 0x400000
		
	
2,在uboot中去设置参数，能够启动内核并让内容挂载根文件系统：
	1，启动内核：
		a，在ubuntu中，将uImage放在/tftpboot
		
			E:\Linux驱动开发-s5pv210-george\source\image\uImage308
	
		b,在开发板uboot中：
			bootcmd的作用：倒计时结束后ｕｂｏｏｔ需要做什么
			FS210 # set serverip 192.168.7.11 //ubuntu--tftp服务器的ip
			FS210 # set ipaddr 192.168.7.7 //开发板ip--ubuntu ip + 60
			FS210 # set bootcmd tftp 0x40008000 uImage \; bootm 0x40008000
			FS210 # save

	2,挂载根文件系统：
		a，将rootfs放入到ubuntu-/opt
			E:\Linux驱动开发-s5pv210-george\source\rootfs_fs210.tgz
			
			解压：
			sudo tar -xvf rootfs_fs210.tgz
		
		b，设置nfs服务器--设置/opt/rootfs能够被挂载
			sudo vim /etc/exports
				 /opt/rootfs             *(subtree_check,rw,no_root_squash,async)
			
			重启nfs服务器：
			sudo service nfs-kernel-server restart
		
		c,在开发板中操作：
			FS210 # set bootargs console=ttySAC0,115200 init=/linuxrc root=/dev/nfs nfsroot=192.168.7.11:/opt/rootfs ip=192.168.7.7
			FS210 # save

			bootargs ：传递给内核的参数
					console=ｘｘｘ: 内核启动的调试信息通过什么设备输出
					init=xxx: 告诉内核整个系统的祖先进程是什么
					root=xxx: 告诉内核根文件系统在哪里
						root=/dev/nfs  告诉内核根文件系统在网络远端
						nfsroot=ip:path
					ip=xx: 告诉内核开机为系统分配一个静态ip(开发板ip)
		
	
	

	
编译内核
	1，获取内核源码
		www.kernel.org

		选择一个能够支持你的soc的内核
		3.0.8 ==>android底层
		
	2，tar -xvf linux-3.0.8.tar.bz2

	3，编译：具体操作参考---E:\Linux驱动开发-s5pv210-george\初级驱动\第一天\实验\系统环境搭建移植文档.doc
	
		0--设置交叉工具链：E:\Linux驱动开发-s5pv210-george\source\toolchain-4.5.1-farsight.tar.bz2
			解压到ubuntu：
				sudo mkdir /usr/local/arm
				
				sudo tar -xvf toolchain-4.5.1-farsight.tar.bz2  -C /usr/local/arm/
			
			设置环境PATH(直接快速的使用arm-none-linux-gnueabi-gcc)：
				vim ~/.bashrc
					export PATH=$PATH:/usr/local/arm/toolchain-4.5.1-farsight/bin/
				保存后更新：
					source ~/.bashrc
			
		a, 设置交叉工具链--arm的编译器--内核源码的Makefile
			 195 ARCH            ?= arm
			 196 CROSS_COMPILE   ?= arm-none-linux-gnueabi-

		b，选择一个soc(挑出针对s5pv210相关代码)
			make s5pv210_defconfig
					//内部中： cp -raf  arch/arm/configs/s5pv210_defconfig   .config
		
		c，make menuconfig --内核裁剪--选择你想要
			General setup  --->     
				[*] Configure standard kernel features (expert users)  --->
			System Type  --->      
			    (0) S3C UART to use for low-level messages    
			  
			Kernel Features  --->        
					 Memory split (3G/1G user/kernel split)  --->
			
			[ * ] Networking support  --->     
					Networking options  ---> 
						<*>Packet sockets 
						<*> Unix domain sockets 
						[*] TCP/IP networking
							  [*]   IP: multicasting  
							  [*]IP: kernel level autoconfiguration
									[*]     IP: DHCP support
									[*]     IP: BOOTP support 
									[*]     IP: RARP support
			Device Drivers  --->
				[*] Network device support  --->
					   [*]   Ethernet (10 or 100Mbit)  --->
								<*>   DM9000 support
			File systems  --->
				[*] Network File Systems (NEW)  --->
					<*>   NFS client support
					[*]     NFS client support for NFS version 3
					[*]       NFS client support for the NFSv3 ACL protocol extension
					[*]   Root file system on NFS
					
			
			注意：
				除了以上选择，记得要做dm9000的移植
				
			============================================================
			如果make menuconfig报错：
				lpf@ubuntu:/home/farsight/linux_source/linux-3.0.8$ make menuconfig
				 *** Unable to find the ncurses libraries or the
				 *** required header files.
				 *** 'make menuconfig' requires the ncurses libraries.
				 ***
				 *** Install ncurses (ncurses-devel) and try again.
				 ***
				解决：
				sudo apt-get install libncurses5-dev

			============================================================
		d，编译内核--make uImage
			make uImage 
		
		
		备注：
			编译内核：
				make uImage  ==>生成zImage和uImage(需要mkimage的支持, 放入到ubuntu---/usr/bin/)
						E:\Linux驱动开发-s5pv210-george\初级驱动\第一天\实验\mkimage
						
						 sudo mv  mkimage /usr/bin/
						 sudo chmod 777 /usr/bin/mkimage
				make zImage  ==》只生成zImage
		
		e，使用自己编译的内核：
			cp arch/arm/boot/uImage  /tftpboot/
		
编写代码:
	.c +  Makefile
	
	驱动代码编写：
		1, 头文件
			#include <linux/init.h>
			#include <linux/module.h>
			

		2, 模块的装载和卸载入口函数的声明
			module_init(hello_drv_init);
			module_exit(hello_drv_exit);

		3, GPL的认证
			MODULE_LICENSE("GPL");
		4, 模块的装载和卸载入口函数的实现
			static int __init hello_drv_init(void)
			{

					return 0;
			}


			static void __exit hello_drv_exit(void)
			{



			}

	Makefile：
		#KERNELRELEASE是内核源码中一个变量
		ifeq ($(KERNELRELEASE),)
		#获取内核路径
		KERN_DIR = /home/farsight/linux_system/kernel/linux-3.0.8
		#当前代码路径
		CUR_DIR = $(shell pwd)

		all :
				#进入到内核目录，执行内核Makefile，并且告诉内核将当前的源码编译成模块
				make -C $(KERN_DIR) M=$(CUR_DIR) modules

		clean :
				make -C $(KERN_DIR) M=$(CUR_DIR) clean

		install :
				cp -raf  *.ko  /opt/rootfs/drv_module


		else
		#将当前的hello.c编译成hello.ko
		#指定编译哪个
		obj-m += hello.o

		endif

	
	
运行ko：
	[root@farsight /drv_module]# insmod hello.ko
	--------^_^ hello_drv_init--------
	[root@farsight /drv_module]# rmmod hello
	--------^_^ hello_drv_exit--------
	[root@farsight /drv_module]# insmod hello.ko
	--------^_^ hello_drv_init--------
	[root@farsight /drv_module]# lsmod
	hello 755 0 - Live 0xbf004000

		
		

推荐大家使用sourceinsight编写代码：
	E:\Linux驱动开发-s5pv210-george\source\si_linux308-ori.tgz（已经做好工程，一定解压到源码目录）






	
模块传递参数： insmod  hello.ko
		或者：		insmod hello.ko  myname="xxx" age=777


	驱动中：
			module_param(myname,charp, 0644);
			module_param(age, int, 0644);



任务：
1，将开发板运行起来--用提供好的
2，编译内核--移植dm9000网卡--用自己编译的内核
3，编译最简单的驱动模块
4，编写Makefile
5，用sourceinsight去写参数传递
==================================================



		
		
		