��Ҫ���ݣ� ������ϵͳ
1��������ϵͳ���
2��������ϵͳ�ı�̷�ʽ
3��������ϵͳ�Ķ������
4��������ϵͳ��ƽ̨���ߵĽ��
5��������ϵͳ�Ĵ����Ķ�(�����ϲ㵽�ײ������ԭ��)
============================================================
������ϵͳ������ͼ��ݲ�ͬ�������豸
	�����豸��
		1����ֵ�ࣺ������������һ����ֵ--�س�����esc
			������ ȫ����
		2�����������ࣺ��С��һ�����ֵ����Сֵ
			ts������������gsensor,������
				x, y,z
				
		3����������ࣺ
			mouse
			
	���ÿ�������豸�������ַ��豸������ܵı�̲���ȥ��������豸���ж�����Ƶ�����(����Ҫд�ܶ��fop)
	������ϵͳ��
		1���ṩһ��ͳһ��������̷�ʽ,����Ҫ���ܶ�����(���磬�����豸�ţ������豸�ڵ㣬ʵ��fops)
		2������Ӧ�ÿռ���ԣ�������ϵͳ�ṩ��ͳһ�ı�̷���(��Զ����һ�����ݰ��� struct input_event)

	
	
������ϵͳ�Ŀ�ܣ�
	
		Ӧ�ò�
	-------------------------------------------------
	�����㣺
			input handler��: ���ݵĴ����(������)
				�������ݽ���Ӧ�ÿռ�(copy_to_user)�����ǲ�֪������
				����δ�Ӳ�����õ�
				�ں��Ѿ�д���ˣ�drivers/input/evdev.c(ͨ�õ�handler), joydev.c,mousedev.c 
				
			-----------------------------------------
			input ���Ĳ㣺������������н���(���乫˾)
				ά������������handler��input dev����
				�ں��Ѿ�д���ˣ� drivers/input/input.c
			
			-----------------------------------------
			input device�㣺��ʾһ������������豸(���)
				��ɵ��Ǵ�Ӳ���л�ȡ����
				(�������п���)
			
			-----------------------------------------
			Ӳ��
			
	ȷ���ں��Ѿ��У�input handler���	input ���Ĳ�
	make menuconfig
		Device Drivers  --->
			Input device support  --->
				-*- Generic input layer (needed for keyboard, mouse, ...) // input.c
				<*>   Mouse interface	//mousedev.c 
				<*>   Event interface //evdev.c(ͨ�õ�handler)
				
	
	
	
2��������ϵͳ�ı�̷�ʽ
			
		a,����һ��input device����
		b,��ʼ��input device����
		c,ע��input device����
		d����ʼ��Ӳ��������ȡ������
			
			
		
		�ӿڣ�
			//����һ��input device
			struct input_dev *input_allocate_device(void)
			void input_free_device(struct input_dev *dev)
			
			//ע��input device
			int input_register_device(struct input_dev *dev)
			void input_unregister_device(struct input_dev *dev)
			
			//����1--�ĸ��豸�ϱ�
			//����2--�ϱ�ʲô��������--����
			//����3--�ϱ��ĸ������code--����ֵ
			// ����4--�ϱ�����---������״̬
			void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
			
			�������ƺ�����
				void input_report_key(struct input_dev *dev, unsigned int code, int value)
				void input_report_rel(struct input_dev *dev, unsigned int code, int value)
				void input_report_abs(struct input_dev *dev, unsigned int code, int value)
				void input_sync(struct input_dev *dev)
			
			
			struct input_event { //�������ĸ������豸���û��õ������ݶ�����������--���ݰ�
				struct timeval time;
				__u16 type; //��������--EV_KEY, EV_ABS, EV_REL
				__u16 code; 
				__s32 value;
			};
			
	
	
	struct input_dev {//�����豸�����֣����̣��ܹ�������Щ��������type����Щcode,��Щvalue
		//Ϊ�豸�ṩһЩ��Ϣ---/sys/Ŀ¼����ʾ
		const char *name;
		const char *phys;
		const char *uniq;
		struct input_id id;

		
		//����ȫ�����飬��ʾλ��
		// evbit��ʾ�ܹ�ȥ���ø��豸�ܹ�������Щ��������:
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
			���Ҫ�����������ݣ���evbit�еĵ�1bit��1
			���Ҫ�����������ݣ���evbit�еĵ�3bit��1		
		*/
		unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
		// keybit��ʾ�ܹ�ȥ���ø��豸�ܹ�������Щ����:
		/*
			#define KEY_ESC			1
			#define KEY_DOWN		108
			#define KEY_PAGEDOWN		109
			#define KEY_INSERT		110
			#define KEY_DELETE		111
			#define KEY_MACRO		112
			#define KEY_MUTE		113
			���Ҫ����KEY_DOWN����keybit�еĵ�108bit��1
			���Ҫ����KEY_ESC����keybit�еĵ�1bit��1
		*/
		unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];// BITS_TO_LONGS(KEY_CNT) = 768/32 =unsigned long keybit[24];
		// relbit��ʾ�ܹ�ȥ���ø��豸�ܹ�������Щ�������:
		/*
			#define REL_X			0x00
			#define REL_Y			0x01
			#define REL_Z			0x02
			
			keybit[108/32] |= 1 << (108%32)
		*/
		unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
		// absbit��ʾ�ܹ�ȥ���ø��豸�ܹ�������Щ��������:
		unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];// unsigned long absbit[2]==64bit
		
		unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
		unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
		unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
		unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
		unsigned long swbit[BITS_TO_LONGS(SW_CNT)];
		
		struct device dev;//�����Ķ���---����

		struct list_head	h_list;//�Ͷ��ӹ���
		struct list_head	node;
		
	}
	

	
4��������ϵͳ��ƽ̨���ߵĽ��
	ƽ̨��������������õ�����
			ֻҪ�漰�������������ļĴ�����ַ���ж϶�������ƽ̨����
			ƽ̨�����ص㣺 ������Ӳ���ķ�������Դ(Ӳ����Ϣ)����
			ƽ̨������һ�����ܴ���
			
			���������ϵͳ����Ҫ��������������Դ����---������ƽ̨����




	������ϵͳ�����������������豸
				������ɽ������ϱ����û��������豸�ţ������豸�ڵ㣬ʵ��fops��

				
	

	input + plat: ��дһ�������豸���������������ܹ��ڶ��socƽ̨����������
	
	


5��������ϵͳ�Ĵ����Ķ�(�����ϲ㵽�ײ������ԭ��)--���ȥ����ܴ���



input  handler��evdev.c

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
				//��ʼ������
				INIT_LIST_HEAD(&handler->h_list);
				//��evdev_handler����input_table[2]
				input_table[handler->minor >> 5] = handler;
				
				//ע�ᵽ����input_handler_list
				list_add_tail(&handler->node, &input_handler_list);

				//�������е�input device,����ƥ�䣬ƥ��ɹ�����handler->connect
				list_for_each_entry(dev, &input_dev_list, node)
					input_attach_handler(dev, handler);
						|
						//evdev_handler����ƥ������
						id = input_match_device(handler, dev);
						error = handler->connect(handler, dev, id);//������evdev_connect

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
			//ע����һ����/sys/class/input
			err = class_register(&input_class);
			
			// ��procĿ¼�´���/proc/bus/input/devices��handlers
			err = input_proc_init();
			// ע�������豸�ź�fops
			err = register_chrdev(INPUT_MAJOR, "input", &input_fops);



===================================================
simple_key_input.c
	input_register_device(keyinput);
			|
			list_add_tail(&dev->node, &input_dev_list);

			list_for_each_entry(handler, &input_handler_list, node)
				input_attach_handler(dev, handler);
	

	
==============================================================



������evdev.c
evdev_connect
	|
	//��evdev_table��һ��û��ռ�õ�λ�ã���һ�ε���minor=0
	for (minor = 0; minor < EVDEV_MINORS; minor++)
		if (!evdev_table[minor])
	
	//������һ������evdev
	evdev = kzalloc(sizeof(struct evdev), GFP_KERNEL);	

	//��ʼ��evdev
	INIT_LIST_HEAD(&evdev->client_list);
	spin_lock_init(&evdev->client_lock);
	mutex_init(&evdev->mutex);
	//��ʼ���ȴ�����--��������
	init_waitqueue_head(&evdev->wait);
	
	//��������: event0
	dev_set_name(&evdev->dev, "event%d", minor);
	evdev->exist = true;
	evdev->minor = minor;

	//ͨ��handleȥ��¼input handler��input dev
	evdev->handle.dev = input_get_device(dev);
	evdev->handle.name = dev_name(&evdev->dev);
	evdev->handle.handler = handler;
	evdev->handle.private = evdev;


	//�����豸�ڵ� struct device *device_create()
	//  ������/dev/event0===/sys/class/input/event0
	evdev->dev.devt = MKDEV(INPUT_MAJOR, EVDEV_MINOR_BASE + minor);
	evdev->dev.class = &input_class;
	evdev->dev.parent = &dev->dev;
	evdev->dev.release = evdev_free;
	device_initialize(&evdev->dev);
	device_add(&evdev->dev);
	
	// handler��inputdev����ĺ�input handle���ӽ��й���
	error = input_register_handle(&evdev->handle);
			|
			list_add_tail_rcu(&handle->d_node, &dev->h_list);
			list_add_tail_rcu(&handle->h_node, &handler->h_list);
	
	//����ǰevdev���뵽ȫ�ֵ�������evdev_table
	error = evdev_install_chrdev(evdev);
			|
			evdev_table[evdev->minor] = evdev;





�����豸�ڵ㣺��ͬ��device_create()
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
Ӧ�ã�
	open()
	read();
=======================
	sys_open   sys_read
==============================
input.c��open
	.open = input_open_file,
			|
			//�õ�evdev handler
			handler = input_table[iminor(inode) >> 5];
			//�õ�handler��fops--evdev_fops
			if (handler)
				new_fops = fops_get(handler->fops);
			
			
			//evdev handler��fops����file->f_op,����read/write..xxx���ն���ĵ���
			//evdev handler��fops��������
			file->f_op = new_fops;
			//����evdev handler��fops�е�open
			err = new_fops->open(inode, file); //ʵ����evdev_fops->open()
		

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
	//����һ������������
	client = kzalloc(sizeof(struct evdev_client) +
				bufsize * sizeof(struct input_event),
			 GFP_KERNEL);
	
	client->bufsize = bufsize;
	spin_lock_init(&client->buffer_lock);
	client->evdev = evdev;
	//��client���뵽evdev������
	evdev_attach_client(evdev, client);
	//��client��������¼��file���Ը������� read/writeȥ����
	file->private_data = client;














	