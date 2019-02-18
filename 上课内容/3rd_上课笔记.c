��Ҫ���ݣ�linux�°��������ı�д--ѧϰ�жϱ��
1��file��inode��cdev֮��Ĺ�ϵ
2��readl/writel�ӿں�gpio�⺯����ʹ��
3��ioctl�ӿڵ�ʵ��
4���µ�ע���豸�ŵķ���
5���ж���linux�������д���
6���ļ�ioģ��--�����ͷ�����
======================================================

1��file��inode��cdev֮��Ĺ�ϵ
	
	struct file: ���������д򿪵��ļ���������ֻҪ������open����һ���ö���
			���������˴��ļ���·����Ȩ�ޣ���־���ڲ�ƫ��
	
		struct file {
				struct path		f_path;
				atomic_long_t		f_count;
				unsigned int 		f_flags;
				fmode_t			f_mode;
				loff_t			f_pos;
				void			*private_data;
				const struct file_operations	*f_op;
			
		}
		
			
	struct inode������һ�������ļ������ԣ����ͣ�Ȩ�ޣ��û�id����id���޸�ʱ�䣬��С�����豸�ţ����豸��
			linuxÿ���ļ�����һ��inode
			
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
	struct cdev������һ���ַ��豸�������豸�ţ�fops(�ļ������ӿ�)
			ÿ���ַ��豸����һ���ö���
			
		struct cdev {
			const struct file_operations *ops;
			struct list_head list;
			dev_t dev;
		};
		
	//ͨ��fileȥ��ȡ��inode
	struct inode *inode = filp->f_path.dentry->d_inode;
	int major = imajor(inode);
	int minor = iminor(inode);
	printk("major = %d, minor = %d\n", major, minor);
	
	
	// inode��ʱ�Ϳ���ȥ���ֵ�ǰ�򿪵����ĸ����豸
	int major = imajor(inode);
	int minor = iminor(inode);
	
	
	
	//�������еĺ����ж���һ��filp,�����ݴ����private_data�У��Ϳ���ʵ�ָ�������֮������ݹ���
	led_drv_open()
		|
		static int a = 34;
		filp->private_data = &a;
	led_drv_write()
		|
		int a = *((int *)filp->private_data);
		printk("a== %d\n", a);
		

===========================================================
2��readl/writel�ӿں�gpio�⺯����ʹ��	
	�����Ĵ����ĵ�ַ�ķ���
	a�� ֱ�ӵ�ַ����
	
		*gpc0conf &= ~(0xff<<12); 
		*gpc0conf |= (0x11<<12);
	
	b��linux�ṩ�ĺ����ӿ�readl/writel--��ƽ̨��ͳһ
		long readl(unsinged long *addr) //��ȡ��ַ��Ӧ�ռ��ֵ
		void writel(long value,  unsinged long *addr)//������д�뵽һ����ַ��Ӧ�ռ�
			
		���ӣ�
			writel(readl(led_dev->reg_base + 4) | (0x3<<3), led_dev->reg_base + 4);
			
		����һ��д����
			__raw_readl(a)
			__raw_writel(d,ad)
			
		���ӣ�
			unsigned long tmp = __raw_readl(led_dev->reg_base);
			tmp &= ~(0xff<<12);
			tmp |= (0x11<<12);
			__raw_writel(tmp, led_dev->reg_base);
		
	c, ����ԭ���ṩ��gpio�Ŀ⺯��--ֻ���gpio�Ĳ���--������ֱ��
		����Ҫȥ������ַ(����Ҫӳ��)��ֻ��Ҫ֪��gpio�ĺ���ͺ���
		GPC0_3/4
			����Ķ��壺
				S5PV210_GPC0(3) ==������
				S5PV210_GPC0(4)
				S5PV210_GPH0(3)
		
		
		//����gpio��ʹ��Ȩ
		int gpio_request(unsigned gpio,const char * label)

		//����gpioΪ���빦��
		int gpio_direction_input(unsigned gpio)
		//����gpioΪ���������ͬʱ��������ߵ͵�ƽ
		gpio_direction_output(unsigned gpio,int value)
		// ��ȡ��gpio������
		int gpio_get_value(unsigned gpio);
		// ���õ�gpio������
		void gpio_set_value(unsigned gpio, int value)

		//ͨ��gpio�ĺ����ȡ����Ӧ�жϺ���
		int gpio_to_irq(unsigned gpio)
		//ͨ���жϵĺ����ȡ����Ӧgpio����
		int irq_to_gpio(unsigned irqno)

		//��������gpio����������
		s3c_gpio_cfgpin(unsigned int pin,unsigned int config)
		//��������gpio���ڲ�����������
		s3c_gpio_setpull(unsigned int pin,s3c_gpio_pull_t pull)

		//�ͷ�gpio��ʹ��Ȩ
		gpio_free(unsigned gpio)

		
=====================================================
3��ioctl�ӿڵ�ʵ��
	ioctlӦ�õ��ļ�io�ӿڣ�����������ָ��(���cmd)������ֻ��һ���������ѣ�Ӧ�÷�����������ȥʶ��
	
	����
		a,�ر�ĳ����  b����ĳ����  c����ȫ�� d�� ��ȫ��
	
	
	ģ�ͣ�
			int ioctl(int fd, int cmd, ...);
		-----------------------------------------
		�����У�long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
		xxx_ioctl()
		{
			switch(cmd){
				case ����1 ��
					xxx
				case ����2��
					xxx
				
			}
		}
		
		��Ҫ�㣺����Ķ���(Ψһ)
			a, ֱ��������(���Ƽ������Լ�����������п��ܺ�ϵͳ��ĳ�����е������г�ͻ)
				#define LED_ON  3333
				#define LED_OFF 4444
				#define LED_NUM_ON  1111
				#define LED_NUM_OFF 2222
			b����ϵͳ���ṩ�Ľӿ�ȥ��������
				_IO(type,nr)
				_IOR(type,nr,size)  ==���������ڶ���һ��Ψһ������
				_IOW(type,nr,size)
				_IOWR(type,nr,size)
			
				������1��ħ����--�ַ�=='L', 'l'
					  2, ����--���ֲ�ͬ����
					  3�������Ҫ�������ݣ�size����һ������
					  
				#define LED_ON  _IO('L', 1000)
				#define LED_OFF _IO('L', 1001)
				#define LED_NUM_ON  _IOW('L', 1002, int)
				#define LED_NUM_OFF _IOW('L', 1003, int)
							
			

4���µ�ע���豸�ŵķ���	

	1���ɵ������豸�ŵķ�ʽ
		register_chrdev(0, "led_drv", &led_fops); 
			a�� �������豸�Ŷ���
			b�� ע��cdev
	
	2���µ������豸�ŵķ�ʽ
		��̬��
			//����1��ָ���豸��
			// ����2���豸�ĸ���
			//����3 �����֣�����/proc/devices��ʾ
			int register_chrdev_region(dev_t from,unsigned count,const char * name)
		��̬��
			int alloc_chrdev_region(dev_t * dev,unsigned baseminor,unsigned count,const char * name)
		��������������ֻ���������豸�Ŷ���
		
		
		��Ҫ�����´��������ϣ�ע��cdev
			cdev_alloc(void)
			cdev_init(struct cdev * cdev,const struct file_operations * fops)
			cdev_add(struct cdev * p,dev_t dev,unsigned count)
		
		
		
		��д��ʱ��
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

		//����cdev��ע��cdev
		led_dev->cdev = cdev_alloc();
		cdev_init(led_dev->cdev, &led_fops);
		cdev_add(led_dev->cdev,led_dev->devno,1);
		
		
===============================================
5���ж���linux�������д���
	�����cpu�����첽������һ�ַ�ʽ
	
	
	����--------�жϿ�����vic-------fiq---------
							--------irq------cpu(A8)
	
	
	ע��/�����ж�
	int request_irq(unsigned int irq,irq_handler_t handler,unsigned long flags,const char * name,void * dev)
	//����1--�жϺ���
			����Ķ��壺
	//����2--�жϴ�����
			irqreturn_t (*irq_handler_t)(int, void *);
			irqreturn_t key_irq_handle(int irqno, void *dev_id)
			{
				printk("---------*_*- %s----------\n", __FUNCTION__);

				return IRQ_HANDLED;

			}
	//����3--������ʽ
			#define IRQF_TRIGGER_NONE	0x00000000 //�ڲ��жϵĴ�������
			#define IRQF_TRIGGER_RISING	0x00000001
			#define IRQF_TRIGGER_FALLING	0x00000002
			#define IRQF_TRIGGER_HIGH	0x00000004
			#define IRQF_TRIGGER_LOW	0x00000008
	//����4--�Զ����ַ���--/proc/interrupts
	//����5--���ݸ�����2��ֵ--������NULL
	//����ֵ--��ȷΪ0
	
	
	ע���жϣ�
	void free_irq(unsigned int irq,void * dev_id)
	//����1--�жϺ���
	//����2--request_irq�жϵ����������һ��
	
	
	
	
	
	
	
	
	
	
	
	
		
		
		