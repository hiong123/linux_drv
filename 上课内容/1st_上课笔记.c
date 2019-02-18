���п����壺
1��uboot���÷���
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
	
	
	
	����nandflash�����
		дnand��
			nand erase 0x100000 0x400000
			nand write 0x40008000 0x100000 0x400000
			
		��nand:
			nand read 0x40008000 0x100000 0x400000
		
	
2,��uboot��ȥ���ò������ܹ������ں˲������ݹ��ظ��ļ�ϵͳ��
	1�������ںˣ�
		a����ubuntu�У���uImage����/tftpboot
		
			E:\Linux��������-s5pv210-george\source\image\uImage308
	
		b,�ڿ�����uboot�У�
			bootcmd�����ã�����ʱ��������������Ҫ��ʲô
			FS210 # set serverip 192.168.7.11 //ubuntu--tftp��������ip
			FS210 # set ipaddr 192.168.7.7 //������ip--ubuntu ip + 60
			FS210 # set bootcmd tftp 0x40008000 uImage \; bootm 0x40008000
			FS210 # save

	2,���ظ��ļ�ϵͳ��
		a����rootfs���뵽ubuntu-/opt
			E:\Linux��������-s5pv210-george\source\rootfs_fs210.tgz
			
			��ѹ��
			sudo tar -xvf rootfs_fs210.tgz
		
		b������nfs������--����/opt/rootfs�ܹ�������
			sudo vim /etc/exports
				 /opt/rootfs             *(subtree_check,rw,no_root_squash,async)
			
			����nfs��������
			sudo service nfs-kernel-server restart
		
		c,�ڿ������в�����
			FS210 # set bootargs console=ttySAC0,115200 init=/linuxrc root=/dev/nfs nfsroot=192.168.7.11:/opt/rootfs ip=192.168.7.7
			FS210 # save

			bootargs �����ݸ��ں˵Ĳ���
					console=������: �ں������ĵ�����Ϣͨ��ʲô�豸���
					init=xxx: �����ں�����ϵͳ�����Ƚ�����ʲô
					root=xxx: �����ں˸��ļ�ϵͳ������
						root=/dev/nfs  �����ں˸��ļ�ϵͳ������Զ��
						nfsroot=ip:path
					ip=xx: �����ں˿���Ϊϵͳ����һ����̬ip(������ip)
		
	
	

	
�����ں�
	1����ȡ�ں�Դ��
		www.kernel.org

		ѡ��һ���ܹ�֧�����soc���ں�
		3.0.8 ==>android�ײ�
		
	2��tar -xvf linux-3.0.8.tar.bz2

	3�����룺��������ο�---E:\Linux��������-s5pv210-george\��������\��һ��\ʵ��\ϵͳ�������ֲ�ĵ�.doc
	
		0--���ý��湤������E:\Linux��������-s5pv210-george\source\toolchain-4.5.1-farsight.tar.bz2
			��ѹ��ubuntu��
				sudo mkdir /usr/local/arm
				
				sudo tar -xvf toolchain-4.5.1-farsight.tar.bz2  -C /usr/local/arm/
			
			���û���PATH(ֱ�ӿ��ٵ�ʹ��arm-none-linux-gnueabi-gcc)��
				vim ~/.bashrc
					export PATH=$PATH:/usr/local/arm/toolchain-4.5.1-farsight/bin/
				�������£�
					source ~/.bashrc
			
		a, ���ý��湤����--arm�ı�����--�ں�Դ���Makefile
			 195 ARCH            ?= arm
			 196 CROSS_COMPILE   ?= arm-none-linux-gnueabi-

		b��ѡ��һ��soc(�������s5pv210��ش���)
			make s5pv210_defconfig
					//�ڲ��У� cp -raf  arch/arm/configs/s5pv210_defconfig   .config
		
		c��make menuconfig --�ں˲ü�--ѡ������Ҫ
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
					
			
			ע�⣺
				��������ѡ�񣬼ǵ�Ҫ��dm9000����ֲ
				
			============================================================
			���make menuconfig����
				lpf@ubuntu:/home/farsight/linux_source/linux-3.0.8$ make menuconfig
				 *** Unable to find the ncurses libraries or the
				 *** required header files.
				 *** 'make menuconfig' requires the ncurses libraries.
				 ***
				 *** Install ncurses (ncurses-devel) and try again.
				 ***
				�����
				sudo apt-get install libncurses5-dev

			============================================================
		d�������ں�--make uImage
			make uImage 
		
		
		��ע��
			�����ںˣ�
				make uImage  ==>����zImage��uImage(��Ҫmkimage��֧��, ���뵽ubuntu---/usr/bin/)
						E:\Linux��������-s5pv210-george\��������\��һ��\ʵ��\mkimage
						
						 sudo mv  mkimage /usr/bin/
						 sudo chmod 777 /usr/bin/mkimage
				make zImage  ==��ֻ����zImage
		
		e��ʹ���Լ�������ںˣ�
			cp arch/arm/boot/uImage  /tftpboot/
		
��д����:
	.c +  Makefile
	
	���������д��
		1, ͷ�ļ�
			#include <linux/init.h>
			#include <linux/module.h>
			

		2, ģ���װ�غ�ж����ں���������
			module_init(hello_drv_init);
			module_exit(hello_drv_exit);

		3, GPL����֤
			MODULE_LICENSE("GPL");
		4, ģ���װ�غ�ж����ں�����ʵ��
			static int __init hello_drv_init(void)
			{

					return 0;
			}


			static void __exit hello_drv_exit(void)
			{



			}

	Makefile��
		#KERNELRELEASE���ں�Դ����һ������
		ifeq ($(KERNELRELEASE),)
		#��ȡ�ں�·��
		KERN_DIR = /home/farsight/linux_system/kernel/linux-3.0.8
		#��ǰ����·��
		CUR_DIR = $(shell pwd)

		all :
				#���뵽�ں�Ŀ¼��ִ���ں�Makefile�����Ҹ����ں˽���ǰ��Դ������ģ��
				make -C $(KERN_DIR) M=$(CUR_DIR) modules

		clean :
				make -C $(KERN_DIR) M=$(CUR_DIR) clean

		install :
				cp -raf  *.ko  /opt/rootfs/drv_module


		else
		#����ǰ��hello.c�����hello.ko
		#ָ�������ĸ�
		obj-m += hello.o

		endif

	
	
����ko��
	[root@farsight /drv_module]# insmod hello.ko
	--------^_^ hello_drv_init--------
	[root@farsight /drv_module]# rmmod hello
	--------^_^ hello_drv_exit--------
	[root@farsight /drv_module]# insmod hello.ko
	--------^_^ hello_drv_init--------
	[root@farsight /drv_module]# lsmod
	hello 755 0 - Live 0xbf004000

		
		

�Ƽ����ʹ��sourceinsight��д���룺
	E:\Linux��������-s5pv210-george\source\si_linux308-ori.tgz���Ѿ����ù��̣�һ����ѹ��Դ��Ŀ¼��






	
ģ�鴫�ݲ����� insmod  hello.ko
		���ߣ�		insmod hello.ko  myname="xxx" age=777


	�����У�
			module_param(myname,charp, 0644);
			module_param(age, int, 0644);



����
1������������������--���ṩ�õ�
2�������ں�--��ֲdm9000����--���Լ�������ں�
3��������򵥵�����ģ��
4����дMakefile
5����sourceinsightȥд��������
==================================================



		
		
		