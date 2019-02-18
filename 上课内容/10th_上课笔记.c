主要内容： 内核的裁剪和移植，运行原理理解
1，内核的编译步骤
2，内核裁剪,make menuconfig详细讲解
3，驱动编译进内核uImage的方法
4，内核的运行流程：
	理解module_init的原理
	理解挂载的原理
	理解祖先进程运行的原理
================================================
1，内核的编译步骤
	a,设置交叉工具链--Makefile
		 195 ARCH            ?= arm
		 196 CROSS_COMPILE   ?= arm-none-linux-gnueabi-

		在命令行中：临时传参数
			make uImage ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-
	b，选择一个soc--s5pv210
		make s5pv210_defconfig
			//内部： cp -raf arch/arm/configs/s5pv210_defconfig  .config
			//.config是所有的宏以及它的值全部收集到这里， 内核所有的Makefile都根据它的宏和值去编译代码
				14 CONFIG_ARM=y
				15 CONFIG_SYS_SUPPORTS_APM_EMULATION=y
				Makefile:
					obj-$(CONFIG_ARM) = xxx.o
			// Kconfig提供宏和界面，不同值，值是由用户去决定(.config是由用户来决定)
			// .config可以进行保存
	c，make menuconfig --读取Kconfig，显示图像，也会根据.config去显示最终效果

	d，make uImage
	
	
2,	 驱动编译进内核uImage的方法--s5pv210_led.c


3，内核的内部构造

uboot是可以启动linux，bsd,vxwork, 还可以启动不同的架构：arm，mips,x86

uImage中头部信息就包含了这些内容，uboot就可以通过读取头部信息，来采取不同方式去启动内核
zImage--uboot默认就将使用zImage认为是arm,linux内核

farsight@ubuntu:~$ mkimage
Usage: mkimage -l image
        
       mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
          -A ==> set architecture to 'arch'//指定架构： arm,mips,x86
          -O ==> set operating system to 'os'//指定操作系统：  linux,bsd,vxwork
          -T ==> set image type to 'type'//镜像类型：kernel, ramdisk
          -C ==> set compression type 'comp'//压缩方法：  none，gzip2,bzip2
          -a ==> set load address to 'addr' (hex)//加载到内存的地址(参考值)：0x20008000
          -e ==> set entry point to 'ep' (hex)//内核执行地址：0x20008000
          -n ==> set image name to 'name'//描述--自定义
          -d ==> use image data from 'datafile'//指定在哪个文件上增加64自己头部
          -x ==> set XIP (execute in place)
	
	
mkimage -A arm -O linux -T kernel -C none -a 0x20008000 -e 0x20008000 -n "linux-3.0.8-jihq" -d zImage   uImage333
Image Name:   linux-3.0.8-jihq
Created:      Tue Sep  6 02:02:32 2016
Image Type:   ARM Linux Kernel Image (uncompressed)
Data Size:    2386796 Bytes = 2330.86 kB = 2.28 MB
Load Address: 0x20008000
Entry Point:  0x20008000
				

======================================================================================
test1.c test2.c  ==> test_elf

ld -Txxx.lds  test1.o test2.o  -o test_elf
xxx.lds

ENTRY(_start)//入口函数--整个程序是从哪个函数运行
SECTIONS//内部的段描述
{
	.text : {
		test2.o (.text)
		* (.text)
	}
	.data :{
		* (.data)
	}
	
	
}
		
arch/arm/kernel/vmlinux.lds
OUTPUT_ARCH(arm)
ENTRY(stext)
SECTIONS//内部的段描述
{
 . = 0xC0000000 + 0x00008000;
 .init : { /* Init code and data                */
	  _stext = .; //.表示当前的位置
	  _sinittext = .;
	   *(.head.text)
	   *(.init.text) 
	   *(.cpuinit.text)
	   *(.meminit.text)

	  _einittext = .;
	  __proc_info_begin = .; 
	  *(.proc.info.init)
	  __proc_info_end = .;
	  __arch_info_begin = .;
	   *(.arch.info.init)
	  __arch_info_end = .;
	  __tagtable_begin = .;
	   *(.taglist.init)
	  __tagtable_end = .;
	  __pv_table_begin = .;
	   *(.pv_table)
	  __pv_table_end = .;
	  . = ALIGN(16); __setup_start = .; *(.init.setup) __setup_end = .;
	  __initcall_start = .; *(.initcallearly.init) 
	  
	  __early_initcall_end = .; //开始
	  *(.initcall0.init) *(.initcall0s.init) //全部都是函数指针
	  *(.initcall1.init) *(.initcall1s.init) 
	  *(.initcall2.init) *(.initcall2s.init)
	  *(.initcall3.init) *(.initcall3s.init)
	  *(.initcall4.init) *(.initcall4s.init) 
	  *(.initcall5.init) *(.initcall5s.init) 
	  *(.initcallrootfs.init)
	  *(.initcall6.init) *(.initcall6s.init)//全部都是函数指针
	  *(.initcall7.init) *(.initcall7s.init) 
	  __initcall_end = .;//结束
	}
   .text : { /* Real text segment         */
	  _text = .; /* Text and read-only data */
	   __exception_text_start = .;
	   *(.exception.text)
	   __exception_text_end = .;
   }
   
  
 }

 
 
 
 module_init(led_drv_init) //module_init实际是一个函数指针，指向括号中的参数
 
 
 typedef int (*initcall_t)(void);
 #define module_init(x)	__initcall(x);
 #define __initcall(fn) device_initcall(fn)
 #define device_initcall(fn)		__define_initcall("6",fn,6)
 #define __define_initcall(level,fn,id) \
	static initcall_t __initcall_##fn##id __used \
	__attribute__((__section__(".initcall" level ".init"))) = fn
 
  module_init(led_drv_init)==》
		  函数指针类型       变量名
	static initcall_t  	__initcall_led_drv_init6 __attribute__((__section__(".initcall6.init")))= led_drv_init
 
 
 
