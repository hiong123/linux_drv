主要内容：LCD驱动开发---Framebuffer子系统
1，framebuffer的驱动框架
2，LCD的移植方法
3，启动logo的制作，以及Kconfig和Makefile的使用
4，应用程序控制lcd屏的方法
5，mmap在驱动中的实现
6，framebuffer子系统的驱动运行原理
==================================================


lcd屏所涉及到的驱动：
	
	--------------------------------------------------------------
	fbmem:显存抽像层,实现fops
		负责将显存映射到用户，但是不管显存是如何分配
		drivers/video/fbmem.c

	-------------------------------------------------------------
	xxx-fb层： lcd控制器，负责显存的分配，不知道如何映射到用户空间
		drivers/video/s3c-fb.c(pdrv)
		
		
				---------------------------------------
				pdev:
					&s3c_device_fb,
					
					smdkv210_machine_init()
						|
						s3c_fb_set_platdata(&smdkv210_lcd0_pdata);//设置自定义数据smdkv210_lcd0_pdata
						platform_add_devices(smdkv210_devices, ARRAY_SIZE(smdkv210_devices));
			
				自定义数据：
					static struct s3c_fb_pd_win smdkv210_fb_win0 = {
						.win_mode = {
							.left_margin	= 13,
							.right_margin	= 8,
							.upper_margin	= 7,
							.lower_margin	= 5,
							.hsync_len	= 3,
							.vsync_len	= 1,
							.xres		= 800,
							.yres		= 480,
						},
						.max_bpp	= 32,
						.default_bpp	= 24,
					};

					static struct s3c_fb_platdata smdkv210_lcd0_pdata __initdata = {
						.win[0]		= &smdkv210_fb_win0,
						.vidcon0	= VIDCON0_VIDOUT_RGB | VIDCON0_PNRMODE_RGB,
						.vidcon1	= VIDCON1_INV_HSYNC | VIDCON1_INV_VSYNC,
						.setup_gpio	= s5pv210_fb_gpio_setup_24bpp,
					};
					
					
					
					
时序的关系：
	soc                    	lcd               		   linux驱动
垂直方向所花的时间：
	VSPW                 tvpw  1<y<20		==>y=10      vsync_len=10
	VBPD                  tvb-tvpw  23-tvpw	==>13       upper_margin=13
	LINEVAL               tvd               ==>480      yres=480
	VFPD                  tvfp              ==>22		lower_margin=22
	
水平方向所花的时间：
	HSPW               thpw    1<x<40      ==>x=20    hsync_len=20
	HBPD			   thb-thpw  46-thpw   ==>26      left_margin =26
	HOZVAL               thd               ===>800   xres=800
	HFPD　　　　　　　　thfp               ==>210   right_margin=210

Documentation/fb$ vim framebuffer.txt	
  +----------+---------------------------------------------+----------+-------+
  |          |                ↑                            |          |       |
  |          |                |upper_margin                |          |       |
  |          |                ↓                            |          |       |
  +----------###############################################----------+-------+
  |          #                ↑                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |   left   #                |                            #  right   | hsync |
  |  margin  #                |       xres                 #  margin  |  len  |
  |<-------->#<---------------+--------------------------->#<-------->|<----->|
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |yres                        #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                |                            #          |       |
  |          #                ↓                            #          |       |
  +----------###############################################----------+-------+
  |          |                ↑                            |          |       |
  |          |                |lower_margin                |          |       |
  |          |                ↓                            |          |       |
  +----------+---------------------------------------------+----------+-------+
  |          |                ↑                            |          |       |
  |          |                |vsync_len                   |          |       |
  |          |                ↓                            |          |       |
  +----------+---------------------------------------------+----------+-------+
					

驱动移植：arch/arm/mach-s5pv210/mach-smdkv210.c

打开背光：GPD0_0--高电平
	smdkv210_machine_init(void)
		|
		gpio_request(S5PV210_GPD0(0), "gpd0_blacklight");
		gpio_direction_output(S5PV210_GPD0(0), 1);
		gpio_free(S5PV210_GPD0(0));


	
						
static struct s3c_fb_pd_win smdkv210_fb_win0 = {
	.win_mode = {
		.left_margin	= 26,
		.right_margin	= 210,
		.upper_margin	= 13,
		.lower_margin	=22,
		.hsync_len	= 20,
		.vsync_len	= 10,
		.xres		= 800,
		.yres		= 480,
	},
	.max_bpp	= 32,
	.default_bpp	= 16,
};

static struct s3c_fb_platdata smdkv210_lcd0_pdata __initdata = {
	.win[0]		= &smdkv210_fb_win0,
	.vidcon0	= VIDCON0_VIDOUT_RGB | VIDCON0_PNRMODE_RGB,
	.vidcon1	= VIDCON1_INV_HSYNC | VIDCON1_INV_VSYNC, //给寄存器准备的值
	.setup_gpio	= s5pv210_fb_gpio_setup_24bpp,
};					
					
					
验证lcd是否ok：
	配置内核：
		 make menuconfig
			Device Drivers  --->
				Graphics support  --->
					<*> Support for frame buffer devices  ---> //  fbmem.c
						<*>   Samsung S3C framebuffer support  //s3c-fb.c  ==>提供了一个显存
					
					Console display driver support  --->//
						<*> Framebuffer Console support //开机的时候将图片放入到显存中去
					[*] Bootup logo  --->//提供了logo图片，并且将图片编译进uImage
						[*]   Standard black and white Linux logo (NEW)
						[*]   Standard 16-color Linux logo (NEW)
						[*]   Standard 224-color Linux logo (NEW)// 这个三个都是图片，默认显示这个--小企鹅
						 
						 
	
	编译内核：
		make uImage
		
	更新内核：
		cp -raf arch/arm/boot/uImage /tftpboot
						 
						 
=====================================================================
更改logo：
	提供图片：
	jpg/bmp ---->ppm格式(包含了图片数据)---> .c包含一个struct linux结构，包含图像数据
		
		内核可以将多个图片编译进去，开机的时候用哪个是需要去指定，指定struct linux结构
			drivers/video/logo/logo.c
					 65         if (depth >= 8) {
					 66 #ifdef CONFIG_LOGO_LINUX_CLUT224
					 67                 /* Generic Linux logo */
					 68                 logo = &logo_linux_clut224;
					 69 #endif

	
	操作：
		1，获取图片，转换logo_bugadi_clut224.ppm
		2，logo_bugadi_clut224.ppm编译进内核：
			Makefile和Kconfig
		
			drivers/video/logo$ vim Kconfig
				 30 config LOGO_BUGADI_CLUT224
				 31         bool "Linux BuGaDi 224-color logo"
				 32         help
				 33                 good car

			drivers/video/logo$ vim Makefile			 
						
				 4 obj-$(CONFIG_LOGO_BUGADI_CLUT224)   += logo_bugadi_clut224.o	
					
	
		3, make menuconfig选择图片
			Device Drivers  ---> 
				Graphics support  --->
					[*] Bootup logo  --->
						[ ]   Standard black and white Linux logo
						[ ]   Standard 16-color Linux logo
						[ ]   Standard 224-color Linux logo 
						[*]   Linux BuGaDi 224-color logo
						
		4，指定自己的logo： vim drivers/video/logo/logo.c
			33 extern const struct linux_logo logo_bugadi_clut224;

			// 添加
			 65         if (depth >= 8) {
			 66 #ifdef CONFIG_LOGO_BUGADI_CLUT224
			 67                 logo = &logo_bugadi_clut224;
			 68 #endif
		
		5，make uImage -j2
		
			cp -raf arch/arm/boot/uImage  /tftpboot/
	
	=======================================================================================
	Makefile：控制源码
		obj-$(CONFIG_LOGO_LINUX_CLUT224)        += logo_linux_clut224.o
			
			obj-m += logo_linux_clut224.o // ==ko
			obj-y += logo_linux_clut224.o // ==uImage
			obj-n += logo_linux_clut224.o		
					
					
	Kconfig：提供用户界面和提供宏--make menuconfig					
	vim arch/arm/Kconfig
		menu "Test support"

		config MYTEST
				tristate "This is a simple test config"
				help
				 test test test test help help help,you can you up

		config BAOBAO
				tristate "Baobao is very hard"
				help
						you can you up

		choice
				prompt "Fruit Type"

		config APPLE
				bool "Apple is very small"
				help
						you can you up

		config BANANA
				bool "Banna is very yellow"
				help
						you can you up

		config ORANGE
				bool "orange is very green"
				help
						you can you up

		endchoice


		endmenu

					
任务：
	1，找到lcd屏，soc，驱动中所涉及到的时序关系
	2，开机的时候启动小企鹅
	3，制作自定义的启动logo
	

	
	
	
struct fb_info ：描述显存(lcd)的信息，描述了分辨率，bpp，时序，显存的起始位置，大小

struct fb_info {
	struct fb_var_screeninfo var;	//可变参数--外接的lcd屏是可以变化：分辨率，bpp，时序
	struct fb_fix_screeninfo fix;	//固定参数--显存一旦分配之后就不会变化--显存的起始位置，大小
	char __iomem *screen_base;	/* Virtual address */
	unsigned long screen_size;	/* Amount of ioremapped VRAM or 0 */ 
}	

struct fb_var_screeninfo {//可变参数--外接的lcd屏是可以变化
	__u32 xres;			/* visible resolution		*/
	__u32 yres;
	__u32 bits_per_pixel;		/* guess what			*/
	
	_u32 pixclock;			/* pixel clock in ps (pico seconds) */
	__u32 left_margin;		/* time from sync to picture	*/
	__u32 right_margin;		/* time from picture to sync	*/
	__u32 upper_margin;		/* time from sync to picture	*/
	__u32 lower_margin;
	__u32 hsync_len;		/* length of horizontal sync	*/
	__u32 vsync_len;		/* length of vertical sync	*/
}
	
struct fb_fix_screeninfo {	//固定参数--显存一旦分配之后就不会变化
	unsigned long smem_start;	/* Start of frame buffer mem */
					/* (physical address) */
	__u32 smem_len;			/* Length of frame buffer mem */
}
	
	
	
	

应用中mmap的用法：
	 void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
	 // 参数1--指定映射到用户空间的首地址，一般填NULL，表示由系统自动分配
	 //参数2--映射的长度
	 //参数3--保护标志：PROT_EXEC，PROT_READ，PROT_WRITE，PROT_NONE
	 //参数4--是否私有，MAP_SHARED，MAP_PRIVATE,一般都会填MAP_SHARED
	 //参数5--open之后的fd
	 //参数5--偏移起始位置多少开始映射，一般都填0
      int munmap(void *addr, size_t length);

	  
	 用法：
		char *addr = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd,  0);
	
		去映射：
			    munmap(addr,  1024);
	
	
	驱动中实现：
			int led_drv_mmap(struct file *filp, struct vm_area_struct *vma)
			{
				// struct vm_area_struct记录用户mmap中的各个参数

				//得到一个物理地址
				unsigned long addr;
				
				addr = virt_to_phys(led_dev->virt_mem);

				//参数1---vma描述应用中参数对象
				//参数2---映射到用户空间的位置
				//参数3---物理内存的页地址
				//参数4---大小
				//参数5---映射的权限

				if (io_remap_pfn_range(vma, vma->vm_start, addr >> PAGE_SHIFT,
								PAGE_SIZE, vma->vm_page_prot)) {
					printk(KERN_ERR "%s: io_remap_pfn_range failed\n",
						__func__);
					return -EAGAIN;
				}

				return 0;

			}
			
	
	
	lcd屏中映射：
		fb_mmap(struct file *file, struct vm_area_struct * vma)
		{
			
			struct fb_info *info = file_fb_info(file); //struct fb_info从哪里来--s3c-fb.c
						|
						int fbidx = iminor(inode);
						struct fb_info *info = registered_fb[fbidx];
			start = info->fix.smem_start; //来自info中固定参数fix中smem_start，info从哪里
			off += start;// start+offset表示从哪里开始映射，如果offset=0,从start开始映射
			// off是多少
			if (io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
			     vma->vm_end - vma->vm_start, vma->vm_page_prot))
			return -EAGAIN;
			
		}
		
	===============================================
	s3c-fb.c

			register_framebuffer(struct fb_info * fb_info)
				|
				do_register_framebuffer(fb_info);
					|
					fb_info->dev = device_create(fb_class, fb_info->device,MKDEV(FB_MAJOR, i), NULL, "fb%d", i);
					registered_fb[i] = fb_info;
			




			
lcd屏幕：如果要将一个图片显示到lcd屏：
		1，int fd = open("/dev/fb0");
		
		2,从驱动中获取lcd屏的参数--ioctl	
			FBIOGET_VSCREENINFO
			FBIOGET_FSCREENINFO
			
			struct fb_var_screeninfo var;
			struct fb_fix_screeninfo fix;
			
			//获取可变参数
			ioctl(fd, FBIOGET_VSCREENINFO,  &var);
			//获取固定参数
			ioctl(fd, FBIOGET_FSCREENINFO,  &fix);
			
		
		
		2,mmap();//将显存映射到用户空间--映射大小是如何决定--lcd屏一帧的小
			int size = var.xres * var.yres * var.bits_per_pixel /8
		
			char *addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd,  0);
		
		3,将图片数据写入到显存中
			jpg/bmp -->文件读取-->解析： bmp/jpg--> RGB565(LCD屏显示的像素格式一定是rgb)
			
			//buffer图片数据, framebuffer_ptr--lcd的显存起始位置
			draw_bmp(buffer,(unsigned short *) framebuffer_ptr);
					|
					bmp_read_file(bmpfilename, &bmpdata); //跳过bmp图片的前54个字节
					bmp2fb16_rgb565(bmpdata, fb);
							|
							unsigned short * fb16_buff;

					fb16_buff = fb16;
					for (y = ysize; y > 0 ; y--)
					{
						for (x = 0 ; x < xsize; x++) /*copy one line to frame buffer*/
						{	
							/*copy one pixel to frame buffer*/
							b = *bmpdata;
							bmpdata++;
							b >>= 3;
							g = *bmpdata;
							bmpdata++;
							g >>= 2;
							r = *bmpdata;
							bmpdata++;
							r >>= 3;
							pixel16 = (unsigned short) ((r << 11) | (g << 5) | b);	
							*( fb16_buff +  (y* xsize + x)  ) = pixel16;
						}
					}







		