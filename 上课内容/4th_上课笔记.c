��Ҫ���ݣ�
1,�ж���linux�������д���
2,�ļ�ioģ��--�����ͷ�����, ��·����
3���ж��°벿--tasklet��work
4��Ӧ��open���õ��豸������open���̴���
=================================================
�����cpu�����첽������һ�ַ�ʽ
	
	
	����--------�жϿ�����vic-------fiq---------
							--------irq------cpu(A8)
	
	
	ע��/�����ж�
	int request_irq(unsigned int irq,irq_handler_t handler,unsigned long flags,const char * name,void * dev)
	//����1--�жϺ���
			����Ķ��壺
				a, IRQ_EINT(x);---�����ⲿ�ж�
				
				b,	GPH0_3---EINT3 //gpio�����жϹ��ܵ�
						int irqno = gpio_to_irq(S5PV210_GPH0(3));
				
				c,�ڲ��ж�--irqs.h
					#define IRQ_TIMER0_VIC		S5P_IRQ_VIC0(21)
					#define IRQ_TIMER1_VIC		S5P_IRQ_VIC0(22)
					#define IRQ_TIMER2_VIC		S5P_IRQ_VIC0(23)
					#define IRQ_TIMER3_VIC		S5P_IRQ_VIC0(24)
					#define IRQ_TIMER4_VIC		S5P_IRQ_VIC0(25)
					#define IRQ_SYSTIMER		S5P_IRQ_VIC0(26)
					#define IRQ_WDT			S5P_IRQ_VIC0(27)
				
	//����2--�жϴ�����
			irqreturn_t (*irq_handler_t)(int, void *);
			//�������Ĳ�����
			//1,��ǰ�����жϵĺ���
			// 2, request_irq���������
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
	
	
========================================================
������ ������Դ���ɴ��ʱ�򣬽���Ҫ����(������ȥ����)
	linux�󲿷ֵĺ���Ĭ�϶���������
		scanf(), read(), accept(); fgets()...

	������
		add_wait_queue(wait_queue_head_t * q,wait_queue_t * wait)
		set_current_state(state_value)
		schedule(void)

	
	������һ����
	��ʼ��һ���ȴ�����ͷ��
		wait_queue_head_t wq_head;

			init_waitqueue_head(wait_queue_head_t *q)
		
		�÷���
			init_waitqueue_head(&wq_head);
	
	���ߣ���û�����ݵ�ʱ��---��read�����У���copy_to_user֮ǰ
		//����1���ȴ�����ͷ
		//����2���������������Ϊ�٣��ú��������ߣ����Ϊ��Ͳ�����
		// �ڲ��У��ڲ�����һ���ȴ���������ҽ����̹������ȴ�������
		// ��󽫵ȴ���������뵽�ȴ�����ͷ
		//���ֻ��Ҫ�ṩһ��������һ���ȴ�����ͷ
		wait_event_interruptible(wait_queue_head_t q,  int condtion)
		
		
		
		
	���ѣ��������ݵ�ʱ��--�жϲ������������
		wake_up_interruptible(wait_queue_head_t *q);
		
		
		
���������������õ�ʱ�������ݾͶ�д��û���ݾ�������

	�����Ҫ��������
		open("/dev/xx", O_RDWR|O_NONBLOCK);
		
		���ߣ�
			fd = socket(AF_INET, SOCK_STREAM, 0);
		
			int flags = fcntl(fd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			fcntl(fd, F_SETFL, flags);
	
	======================================================
		1,�ж����������Ƿ�����ģʽ

		if((filp->f_flags & O_NONBLOCK) &&  !key_dev->key_state)
			return -EAGAIN;


======================================================

��·���ã�ͬʱ��ض���ļ���io(��д)����

	Ӧ��	select����poll
	
	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	
	����1��������е�fd�ļ���
		 struct pollfd {
               int   fd;       //��Ҫ����ص�fd
               short events;    // ϣ�����fd��ʲô��POLLIN, POLLOUT, POLLERR
               short revents;   //���ڼ�¼�Ƿ����¼�����ֵ����ϵͳ�Զ�����
           };
	����2����صģ�����
	������������ض�ã�����Ϊ��λ
			-1: ��Զ��
			
	����ֵ�� ��ȷ���ش���0�����󷵻ظ���	
			
		struct pollfd pfds[2];

		pfds[0].fd = 0;
		pfds[0].events = POLLIN;

		pfds[1].fd = fd;
		pfds[1].events = POLLIN;
		

		ret = poll(pfds,  2,  -1);
		
		
		//�жϵ������ĸ�������
		if(pfds[0].revents & POLLIN)
		{
			//��������
			fgets(inbuf, 128, stdin);
			printf("inbuf: %s\n", inbuf);
		}
		
		if(pfds[1].revents & POLLIN)
		{
		}
	=========================================
	��������xxx_poll()
	
	unsigned int key_drv_poll (struct file *filp, struct poll_table_struct *pts)
	{
		 // �������ݵ�ʱ�򷵻�һ��POLLIN
		 //û�����ݵ�ʱ�򷵻�һ��0

		unsigned int mask = 0;

		// ����ǰ�ĵȴ�����ͷע�ᵽvfs��
		poll_wait(filp, &key_dev->wq_head, pts);

		if(key_dev->key_state){
			mask |= POLLIN;
		}


		 return mask;
	}
	

=============================================================
�ж��°벿��
	�ϰ벿��request_irq()��������������ĺ��������е����ݶ����ϰ벿
	�°벿�� �Ӻ��������

tasklet:
	struct tasklet_struct
	{
		struct tasklet_struct *next;
		unsigned long state;
		atomic_t count;
		void (*func)(unsigned long);//�°벿Ҫִ�еĴ���
		unsigned long data;
	};
	
	
	1����ʼ��tasklet
		a����̬---�ڱ����ʱ��---����һ��ȫ�ֱ���
			DECLARE_TASKLET(name,func,data)
			
			
			#define DECLARE_TASKLET(name, func, data) \
				struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
		b����̬---�����е�ʱ��
			tasklet_init(struct tasklet_struct * t,void(* func)(unsigned long),unsigned long data)
	
	2�����ϰ벿��ʱ����뵽�ں��߳���ȥ�������
		tasklet_schedule(struct tasklet_struct * t)
	

�������к͹�����
	struct workqueue_struct����ʾһ���������У���ʵ�ʹ���һ���ں��߳�

	struct work_struct����ʾ�°벿�Ĺ���
	typedef void (*work_func_t)(struct work_struct *work);
	struct work_struct {
		atomic_long_t data;
		struct list_head entry;
		work_func_t func; //�°벿Ҫִ�еĴ���
	};
	
	a, ʹ���Լ���work���Լ��Ĺ�������
	
		1��������������
			struct workqueue_struct *create_workqueue(char *name) //������һ���ں˵��߳�
			void destroy_workqueue(struct workqueue_struct * wq)
 		
		2����ʼ������
			a����̬---�ڱ����ʱ��---����һ��ȫ�ֱ���
				DECLARE_WORK(n,f)
			b����̬---�����е�ʱ��
				INIT_WORK(_work,_func)
			
		
		3�����������뵽��������(���ϰ벿��ʱ����뵽�ں��߳���ȥ�������)
			queue_work(struct workqueue_struct * wq,struct work_struct * work)
	
	
	b, ʹ���Լ���work��ϵͳ�Ĺ�������
		1����ʼ������
			a����̬---�ڱ����ʱ��---����һ��ȫ�ֱ���
				DECLARE_WORK(n,f)
			b����̬---�����е�ʱ��
				INIT_WORK(_work,_func)
			
		
		2�����������뵽��������(���ϰ벿��ʱ����뵽�ں��߳���ȥ�������)
			schedule_work(struct work_struct * work)
	

���ں˴��룺
	���ɣ�
		1��ֻ�����Ķ��Ĵ���
		2��������
		3���º�����
		4�������жϺͱ���������
		5���������ʱ�����ʼǺ�ע�ͣ������ܽ�
		6�������Ҷ���
	===================================================

	open()  read()  write() exit()
		2      3     4         5
	------------------------------------------
	glibc
		 int open()
		 {
			 swi NR_open  // NR_open==2
			 //�����쳣--�ں˴���--���뵽�ں�
		 }
	--------------------------------------------------
	VFS:  fs/open.c
		sys_open         sys_read()            sys_write()
		|
		SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, int, mode)
		==>  sys_open(const char __user * filename, int  flags, int mode);
			|
			do_sys_open(AT_FDCWD, filename, flags, mode);
				|
				fd = get_unused_fd_flags(flags);//��ȡ��û�б�ʹ�õ��ļ�fd
				
				struct file *f = do_filp_open(dfd, tmp, &op, lookup);//����һ��struct file
								|
								filp = path_openat(dfd, pathname, &nd, op, flags | LOOKUP_RCU);
										|
										filp = get_empty_filp();
										error = path_init(dfd, pathname, flags | LOOKUP_PARENT, nd, &base);
										filp = do_last(nd, &path, op, pathname);
												|
												 walk_component(nd, path, &nd->last, LAST_NORM,!symlink_ok);
												complete_walk(nd);
												audit_inode(pathname, nd->path.dentry);
												dentry = lookup_hash(nd);
												path_to_nameidata(path, nd);
												error = may_open(&nd->path, acc_mode, open_flag);
												filp = nameidata_to_filp(nd);
													|
													filp = __dentry_open(nd->path.dentry, nd->path.mnt, filp, NULL, cred);
															|
															f->f_mapping = inode->i_mapping;
															f->f_path.dentry = dentry;
															f->f_path.mnt = mnt;
															f->f_pos = 0;
															f->f_op = fops_get(inode->i_fop);//�õ�inode�е�fop
															if (!open && f->f_op)
																open = f->f_op->open;//��inode��fop�е�open��ֵ��open
															 open(inode, f);//����open---�� chrdev_open
															 
															//  ���е��豸�ڵ�inode�е�fop��˭��---def_chr_fops
													return filp;
												
										return filp;
									
								return filp;
				
				fsnotify_open(f);//֪ͨ�ļ����� ��������
				fd_install(fd, f);// ��struct file��fd���й���

				
				return fd; //����һ��fd
				
				
				
				���յ�����
				const struct file_operations def_chr_fops = {
					.open = chrdev_open,
							|
							struct cdev *new = NULL;
							kobj = kobj_lookup(cdev_map, inode->i_rdev, &idx);//����inode���豸��ȥ�ҵ�cdev
							new = container_of(kobj, struct cdev, kobj);//���Լ������б�д��cdev
							inode->i_cdev = p = new;
							filp->f_op = fops_get(p->ops);//��cdev�е�ops����file->f_op
							if (filp->f_op->open) {
								ret = filp->f_op->open(inode,filp);// ������cdev��ops�е�open����
					.llseek = noop_llseek,
				};
			
	--------------------------------------------------------
	�豸������Ŀ��---��Ҫ����Ӧ��open��ε��õ��豸�����е�open����
		key_dev->cdev = cdev_alloc();
		cdev_init(key_dev->cdev, &key_fops);
		cdev_add(key_dev->cdev,key_dev->devno,1);

	const struct file_operations key_fops = {
		.open = key_drv_open,
		.release = key_drv_close,
		.write = key_drv_write,
		.read = key_drv_read,
		.poll = key_drv_poll,
	
	};





���
	read()                    ioctl
	------------------------------------------
	glibc	
		int  read()
		{
			swi  NR_read
		}
	-------------------------------------
	VFS: fs/read_write.c
	sys_read(unsigned int fd, char __user *buf, size_t count);
	SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
		|
		file = fget_light(fd, &fput_needed);//ͨ��fd��ȡ����Ӧ��struct file
		
		ret = vfs_read(file, buf, count, &pos);
				|
				if (file->f_op->read)
					ret = file->f_op->read(file, buf, count, pos);
		

	    fs/ioctl.c
		SYSCALL_DEFINE3(ioctl
		SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
				|
				filp = fget_light(fd, &fput_needed);
				do_vfs_ioctl(filp, fd, cmd, arg);
					|
					default:
						if (S_ISREG(inode->i_mode))
							error = file_ioctl(filp, cmd, arg);
						else
							error = vfs_ioctl(filp, cmd, arg);
									|
									 filp->f_op->unlocked_ioctl(filp, cmd, arg);
	










