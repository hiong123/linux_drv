��Ҫ���ݣ�LCD��������---Framebuffer��ϵͳ
1��framebuffer���������
2��LCD����ֲ����
3������logo���������Լ�Kconfig��Makefile��ʹ��
4��Ӧ�ó������lcd���ķ���
5��mmap�������е�ʵ��
6��framebuffer��ϵͳ����������ԭ��
==================================================


lcd�����漰����������
	
	--------------------------------------------------------------
	fbmem:�Դ�����,ʵ��fops
		�����Դ�ӳ�䵽�û������ǲ����Դ�����η���
		drivers/video/fbmem.c

	-------------------------------------------------------------
	xxx-fb�㣺 lcd�������������Դ�ķ��䣬��֪�����ӳ�䵽�û��ռ�
		drivers/video/s3c-fb.c(pdrv)
		
		
				---------------------------------------
				pdev:
					&s3c_device_fb,
					
					smdkv210_machine_init()
						|
						s3c_fb_set_platdata(&smdkv210_lcd0_pdata);//�����Զ�������smdkv210_lcd0_pdata
						platform_add_devices(smdkv210_devices, ARRAY_SIZE(smdkv210_devices));
			
				�Զ������ݣ�
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
					
					
					
					
ʱ��Ĺ�ϵ��
	soc                    	lcd               		   linux����
��ֱ����������ʱ�䣺
	VSPW                 tvpw  1<y<20		==>y=10      vsync_len=10
	VBPD                  tvb-tvpw  23-tvpw	==>13       upper_margin=13
	LINEVAL               tvd               ==>480      yres=480
	VFPD                  tvfp              ==>22		lower_margin=22
	
ˮƽ����������ʱ�䣺
	HSPW               thpw    1<x<40      ==>x=20    hsync_len=20
	HBPD			   thb-thpw  46-thpw   ==>26      left_margin =26
	HOZVAL               thd               ===>800   xres=800
	HFPD����������������thfp               ==>210   right_margin=210

Documentation/fb$ vim framebuffer.txt	
  +----------+---------------------------------------------+----------+-------+
  |          |                ��                            |          |       |
  |          |                |upper_margin                |          |       |
  |          |                ��                            |          |       |
  +----------###############################################----------+-------+
  |          #                ��                            #          |       |
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
  |          #                ��                            #          |       |
  +----------###############################################----------+-------+
  |          |                ��                            |          |       |
  |          |                |lower_margin                |          |       |
  |          |                ��                            |          |       |
  +----------+---------------------------------------------+----------+-------+
  |          |                ��                            |          |       |
  |          |                |vsync_len                   |          |       |
  |          |                ��                            |          |       |
  +----------+---------------------------------------------+----------+-------+
					

������ֲ��arch/arm/mach-s5pv210/mach-smdkv210.c

�򿪱��⣺GPD0_0--�ߵ�ƽ
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
	.vidcon1	= VIDCON1_INV_HSYNC | VIDCON1_INV_VSYNC, //���Ĵ���׼����ֵ
	.setup_gpio	= s5pv210_fb_gpio_setup_24bpp,
};					
					
					
��֤lcd�Ƿ�ok��
	�����ںˣ�
		 make menuconfig
			Device Drivers  --->
				Graphics support  --->
					<*> Support for frame buffer devices  ---> //  fbmem.c
						<*>   Samsung S3C framebuffer support  //s3c-fb.c  ==>�ṩ��һ���Դ�
					
					Console display driver support  --->//
						<*> Framebuffer Console support //������ʱ��ͼƬ���뵽�Դ���ȥ
					[*] Bootup logo  --->//�ṩ��logoͼƬ�����ҽ�ͼƬ�����uImage
						[*]   Standard black and white Linux logo (NEW)
						[*]   Standard 16-color Linux logo (NEW)
						[*]   Standard 224-color Linux logo (NEW)// �����������ͼƬ��Ĭ����ʾ���--С���
						 
						 
	
	�����ںˣ�
		make uImage
		
	�����ںˣ�
		cp -raf arch/arm/boot/uImage /tftpboot
						 
						 
=====================================================================
����logo��
	�ṩͼƬ��
	jpg/bmp ---->ppm��ʽ(������ͼƬ����)---> .c����һ��struct linux�ṹ������ͼ������
		
		�ں˿��Խ����ͼƬ�����ȥ��������ʱ�����ĸ�����Ҫȥָ����ָ��struct linux�ṹ
			drivers/video/logo/logo.c
					 65         if (depth >= 8) {
					 66 #ifdef CONFIG_LOGO_LINUX_CLUT224
					 67                 /* Generic Linux logo */
					 68                 logo = &logo_linux_clut224;
					 69 #endif

	
	������
		1����ȡͼƬ��ת��logo_bugadi_clut224.ppm
		2��logo_bugadi_clut224.ppm������ںˣ�
			Makefile��Kconfig
		
			drivers/video/logo$ vim Kconfig
				 30 config LOGO_BUGADI_CLUT224
				 31         bool "Linux BuGaDi 224-color logo"
				 32         help
				 33                 good car

			drivers/video/logo$ vim Makefile			 
						
				 4 obj-$(CONFIG_LOGO_BUGADI_CLUT224)   += logo_bugadi_clut224.o	
					
	
		3, make menuconfigѡ��ͼƬ
			Device Drivers  ---> 
				Graphics support  --->
					[*] Bootup logo  --->
						[ ]   Standard black and white Linux logo
						[ ]   Standard 16-color Linux logo
						[ ]   Standard 224-color Linux logo 
						[*]   Linux BuGaDi 224-color logo
						
		4��ָ���Լ���logo�� vim drivers/video/logo/logo.c
			33 extern const struct linux_logo logo_bugadi_clut224;

			// ���
			 65         if (depth >= 8) {
			 66 #ifdef CONFIG_LOGO_BUGADI_CLUT224
			 67                 logo = &logo_bugadi_clut224;
			 68 #endif
		
		5��make uImage -j2
		
			cp -raf arch/arm/boot/uImage  /tftpboot/
	
	=======================================================================================
	Makefile������Դ��
		obj-$(CONFIG_LOGO_LINUX_CLUT224)        += logo_linux_clut224.o
			
			obj-m += logo_linux_clut224.o // ==ko
			obj-y += logo_linux_clut224.o // ==uImage
			obj-n += logo_linux_clut224.o		
					
					
	Kconfig���ṩ�û�������ṩ��--make menuconfig					
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

					
����
	1���ҵ�lcd����soc�����������漰����ʱ���ϵ
	2��������ʱ������С���
	3�������Զ��������logo
	

	
	
	
struct fb_info �������Դ�(lcd)����Ϣ�������˷ֱ��ʣ�bpp��ʱ���Դ����ʼλ�ã���С

struct fb_info {
	struct fb_var_screeninfo var;	//�ɱ����--��ӵ�lcd���ǿ��Ա仯���ֱ��ʣ�bpp��ʱ��
	struct fb_fix_screeninfo fix;	//�̶�����--�Դ�һ������֮��Ͳ���仯--�Դ����ʼλ�ã���С
	char __iomem *screen_base;	/* Virtual address */
	unsigned long screen_size;	/* Amount of ioremapped VRAM or 0 */ 
}	

struct fb_var_screeninfo {//�ɱ����--��ӵ�lcd���ǿ��Ա仯
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
	
struct fb_fix_screeninfo {	//�̶�����--�Դ�һ������֮��Ͳ���仯
	unsigned long smem_start;	/* Start of frame buffer mem */
					/* (physical address) */
	__u32 smem_len;			/* Length of frame buffer mem */
}
	
	
	
	

Ӧ����mmap���÷���
	 void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
	 // ����1--ָ��ӳ�䵽�û��ռ���׵�ַ��һ����NULL����ʾ��ϵͳ�Զ�����
	 //����2--ӳ��ĳ���
	 //����3--������־��PROT_EXEC��PROT_READ��PROT_WRITE��PROT_NONE
	 //����4--�Ƿ�˽�У�MAP_SHARED��MAP_PRIVATE,һ�㶼����MAP_SHARED
	 //����5--open֮���fd
	 //����5--ƫ����ʼλ�ö��ٿ�ʼӳ�䣬һ�㶼��0
      int munmap(void *addr, size_t length);

	  
	 �÷���
		char *addr = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd,  0);
	
		ȥӳ�䣺
			    munmap(addr,  1024);
	
	
	������ʵ�֣�
			int led_drv_mmap(struct file *filp, struct vm_area_struct *vma)
			{
				// struct vm_area_struct��¼�û�mmap�еĸ�������

				//�õ�һ�������ַ
				unsigned long addr;
				
				addr = virt_to_phys(led_dev->virt_mem);

				//����1---vma����Ӧ���в�������
				//����2---ӳ�䵽�û��ռ��λ��
				//����3---�����ڴ��ҳ��ַ
				//����4---��С
				//����5---ӳ���Ȩ��

				if (io_remap_pfn_range(vma, vma->vm_start, addr >> PAGE_SHIFT,
								PAGE_SIZE, vma->vm_page_prot)) {
					printk(KERN_ERR "%s: io_remap_pfn_range failed\n",
						__func__);
					return -EAGAIN;
				}

				return 0;

			}
			
	
	
	lcd����ӳ�䣺
		fb_mmap(struct file *file, struct vm_area_struct * vma)
		{
			
			struct fb_info *info = file_fb_info(file); //struct fb_info��������--s3c-fb.c
						|
						int fbidx = iminor(inode);
						struct fb_info *info = registered_fb[fbidx];
			start = info->fix.smem_start; //����info�й̶�����fix��smem_start��info������
			off += start;// start+offset��ʾ�����￪ʼӳ�䣬���offset=0,��start��ʼӳ��
			// off�Ƕ���
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
			




			
lcd��Ļ�����Ҫ��һ��ͼƬ��ʾ��lcd����
		1��int fd = open("/dev/fb0");
		
		2,�������л�ȡlcd���Ĳ���--ioctl	
			FBIOGET_VSCREENINFO
			FBIOGET_FSCREENINFO
			
			struct fb_var_screeninfo var;
			struct fb_fix_screeninfo fix;
			
			//��ȡ�ɱ����
			ioctl(fd, FBIOGET_VSCREENINFO,  &var);
			//��ȡ�̶�����
			ioctl(fd, FBIOGET_FSCREENINFO,  &fix);
			
		
		
		2,mmap();//���Դ�ӳ�䵽�û��ռ�--ӳ���С����ξ���--lcd��һ֡��С
			int size = var.xres * var.yres * var.bits_per_pixel /8
		
			char *addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd,  0);
		
		3,��ͼƬ����д�뵽�Դ���
			jpg/bmp -->�ļ���ȡ-->������ bmp/jpg--> RGB565(LCD����ʾ�����ظ�ʽһ����rgb)
			
			//bufferͼƬ����, framebuffer_ptr--lcd���Դ���ʼλ��
			draw_bmp(buffer,(unsigned short *) framebuffer_ptr);
					|
					bmp_read_file(bmpfilename, &bmpdata); //����bmpͼƬ��ǰ54���ֽ�
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







		