��Ҫ���ݣ� �ں˵Ĳü�����ֲ������ԭ�����
1���ں˵ı��벽��
2���ں˲ü�,make menuconfig��ϸ����
3������������ں�uImage�ķ���
4���ں˵��������̣�
	���module_init��ԭ��
	�����ص�ԭ��
	������Ƚ������е�ԭ��
================================================
1���ں˵ı��벽��
	a,���ý��湤����--Makefile
		 195 ARCH            ?= arm
		 196 CROSS_COMPILE   ?= arm-none-linux-gnueabi-

		���������У���ʱ������
			make uImage ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi-
	b��ѡ��һ��soc--s5pv210
		make s5pv210_defconfig
			//�ڲ��� cp -raf arch/arm/configs/s5pv210_defconfig  .config
			//.config�����еĺ��Լ�����ֵȫ���ռ������ �ں����е�Makefile���������ĺ��ֵȥ�������
				14 CONFIG_ARM=y
				15 CONFIG_SYS_SUPPORTS_APM_EMULATION=y
				Makefile:
					obj-$(CONFIG_ARM) = xxx.o
			// Kconfig�ṩ��ͽ��棬��ֵͬ��ֵ�����û�ȥ����(.config�����û�������)
			// .config���Խ��б���
	c��make menuconfig --��ȡKconfig����ʾͼ��Ҳ�����.configȥ��ʾ����Ч��

	d��make uImage
	
	
2,	 ����������ں�uImage�ķ���--s5pv210_led.c


3���ں˵��ڲ�����

uboot�ǿ�������linux��bsd,vxwork, ������������ͬ�ļܹ���arm��mips,x86

uImage��ͷ����Ϣ�Ͱ�������Щ���ݣ�uboot�Ϳ���ͨ����ȡͷ����Ϣ������ȡ��ͬ��ʽȥ�����ں�
zImage--ubootĬ�Ͼͽ�ʹ��zImage��Ϊ��arm,linux�ں�

farsight@ubuntu:~$ mkimage
Usage: mkimage -l image
        
       mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
          -A ==> set architecture to 'arch'//ָ���ܹ��� arm,mips,x86
          -O ==> set operating system to 'os'//ָ������ϵͳ��  linux,bsd,vxwork
          -T ==> set image type to 'type'//�������ͣ�kernel, ramdisk
          -C ==> set compression type 'comp'//ѹ��������  none��gzip2,bzip2
          -a ==> set load address to 'addr' (hex)//���ص��ڴ�ĵ�ַ(�ο�ֵ)��0x20008000
          -e ==> set entry point to 'ep' (hex)//�ں�ִ�е�ַ��0x20008000
          -n ==> set image name to 'name'//����--�Զ���
          -d ==> use image data from 'datafile'//ָ�����ĸ��ļ�������64�Լ�ͷ��
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

ENTRY(_start)//��ں���--���������Ǵ��ĸ���������
SECTIONS//�ڲ��Ķ�����
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
SECTIONS//�ڲ��Ķ�����
{
 . = 0xC0000000 + 0x00008000;
 .init : { /* Init code and data                */
	  _stext = .; //.��ʾ��ǰ��λ��
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
	  
	  __early_initcall_end = .; //��ʼ
	  *(.initcall0.init) *(.initcall0s.init) //ȫ�����Ǻ���ָ��
	  *(.initcall1.init) *(.initcall1s.init) 
	  *(.initcall2.init) *(.initcall2s.init)
	  *(.initcall3.init) *(.initcall3s.init)
	  *(.initcall4.init) *(.initcall4s.init) 
	  *(.initcall5.init) *(.initcall5s.init) 
	  *(.initcallrootfs.init)
	  *(.initcall6.init) *(.initcall6s.init)//ȫ�����Ǻ���ָ��
	  *(.initcall7.init) *(.initcall7s.init) 
	  __initcall_end = .;//����
	}
   .text : { /* Real text segment         */
	  _text = .; /* Text and read-only data */
	   __exception_text_start = .;
	   *(.exception.text)
	   __exception_text_end = .;
   }
   
  
 }

 
 
 
 module_init(led_drv_init) //module_initʵ����һ������ָ�룬ָ�������еĲ���
 
 
 typedef int (*initcall_t)(void);
 #define module_init(x)	__initcall(x);
 #define __initcall(fn) device_initcall(fn)
 #define device_initcall(fn)		__define_initcall("6",fn,6)
 #define __define_initcall(level,fn,id) \
	static initcall_t __initcall_##fn##id __used \
	__attribute__((__section__(".initcall" level ".init"))) = fn
 
  module_init(led_drv_init)==��
		  ����ָ������       ������
	static initcall_t  	__initcall_led_drv_init6 __attribute__((__section__(".initcall6.init")))= led_drv_init
 
 
 
