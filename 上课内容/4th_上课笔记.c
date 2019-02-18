主要内容：
1,中断在linux驱动的中处理
2,文件io模型--阻塞和非阻塞, 多路复用
3，中断下半部--tasklet和work
4，应用open调用到设备驱动的open过程代码
=================================================
外设和cpu进行异步交互的一种方式
	
	
	外设--------中断控制器vic-------fiq---------
							--------irq------cpu(A8)
	
	
	注册/申请中断
	int request_irq(unsigned int irq,irq_handler_t handler,unsigned long flags,const char * name,void * dev)
	//参数1--中断号码
			号码的定义：
				a, IRQ_EINT(x);---定义外部中断
				
				b,	GPH0_3---EINT3 //gpio是有中断功能的
						int irqno = gpio_to_irq(S5PV210_GPH0(3));
				
				c,内部中断--irqs.h
					#define IRQ_TIMER0_VIC		S5P_IRQ_VIC0(21)
					#define IRQ_TIMER1_VIC		S5P_IRQ_VIC0(22)
					#define IRQ_TIMER2_VIC		S5P_IRQ_VIC0(23)
					#define IRQ_TIMER3_VIC		S5P_IRQ_VIC0(24)
					#define IRQ_TIMER4_VIC		S5P_IRQ_VIC0(25)
					#define IRQ_SYSTIMER		S5P_IRQ_VIC0(26)
					#define IRQ_WDT			S5P_IRQ_VIC0(27)
				
	//参数2--中断处理方法
			irqreturn_t (*irq_handler_t)(int, void *);
			//处理方法的参数：
			//1,当前发生中断的号码
			// 2, request_irq第五个参数
			irqreturn_t key_irq_handle(int irqno, void *dev_id)
			{
				printk("---------*_*- %s----------\n", __FUNCTION__);

				return IRQ_HANDLED;

			}
	//参数3--触发方式
			#define IRQF_TRIGGER_NONE	0x00000000 //内部中断的触发方法
			#define IRQF_TRIGGER_RISING	0x00000001
			#define IRQF_TRIGGER_FALLING	0x00000002
			#define IRQF_TRIGGER_HIGH	0x00000004
			#define IRQF_TRIGGER_LOW	0x00000008
	//参数4--自定义字符串--/proc/interrupts
	//参数5--传递给参数2的值--可以填NULL
	//返回值--正确为0
	
	
	注销中断：
	void free_irq(unsigned int irq,void * dev_id)
	//参数1--中断号码
	//参数2--request_irq中断第五个参数是一样
	
	
========================================================
阻塞： 就是资源不可达的时候，进程要休眠(驱动中去休眠)
	linux大部分的函数默认都是阻塞：
		scanf(), read(), accept(); fgets()...

	三步：
		add_wait_queue(wait_queue_head_t * q,wait_queue_t * wait)
		set_current_state(state_value)
		schedule(void)

	
	现在用一个：
	初始化一个等待队列头：
		wait_queue_head_t wq_head;

			init_waitqueue_head(wait_queue_head_t *q)
		
		用法：
			init_waitqueue_head(&wq_head);
	
	休眠：在没有数据的时候---在read函数中，在copy_to_user之前
		//参数1：等待队列头
		//参数2：条件：如果条件为假，该函数会休眠，如果为真就不休眠
		// 内部中：内部构建一个等待队列项，并且将进程关联到等待队列项
		// 最后将等待队列项加入到等待队列头
		//编程只需要提供一个条件和一个等待队列头
		wait_event_interruptible(wait_queue_head_t q,  int condtion)
		
		
		
		
	唤醒：在有数据的时候--中断产生后就有数据
		wake_up_interruptible(wait_queue_head_t *q);
		
		
		
非阻塞：函数调用的时候，有数据就读写，没数据就立马返回

	如果需要非阻塞：
		open("/dev/xx", O_RDWR|O_NONBLOCK);
		
		或者：
			fd = socket(AF_INET, SOCK_STREAM, 0);
		
			int flags = fcntl(fd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			fcntl(fd, F_SETFL, flags);
	
	======================================================
		1,判断是阻塞还是非阻塞模式

		if((filp->f_flags & O_NONBLOCK) &&  !key_dev->key_state)
			return -EAGAIN;


======================================================

多路复用：同时监控多个文件的io(读写)操作

	应用	select或者poll
	
	int poll(struct pollfd *fds, nfds_t nfds, int timeout);
	
	参数1：监控所有的fd的集合
		 struct pollfd {
               int   fd;       //需要被监控的fd
               short events;    // 希望监控fd的什么：POLLIN, POLLOUT, POLLERR
               short revents;   //用于记录是否有事件，该值是由系统自动设置
           };
	参数2：监控的ｆｄ个数
	参数３：　监控多久：毫秒为单位
			-1: 永远等
			
	返回值； 正确返回大于0，错误返回负数	
			
		struct pollfd pfds[2];

		pfds[0].fd = 0;
		pfds[0].events = POLLIN;

		pfds[1].fd = fd;
		pfds[1].events = POLLIN;
		

		ret = poll(pfds,  2,  -1);
		
		
		//判断到底是哪个有数据
		if(pfds[0].revents & POLLIN)
		{
			//处理数据
			fgets(inbuf, 128, stdin);
			printf("inbuf: %s\n", inbuf);
		}
		
		if(pfds[1].revents & POLLIN)
		{
		}
	=========================================
	驱动：　xxx_poll()
	
	unsigned int key_drv_poll (struct file *filp, struct poll_table_struct *pts)
	{
		 // 在有数据的时候返回一个POLLIN
		 //没有数据的时候返回一个0

		unsigned int mask = 0;

		// 将当前的等待队列头注册到vfs层
		poll_wait(filp, &key_dev->wq_head, pts);

		if(key_dev->key_state){
			mask |= POLLIN;
		}


		 return mask;
	}
	

=============================================================
中断下半部：
	上半部：request_irq()参数二中所定义的函数中所有的内容都是上半部
	下半部： 延后操作代码

tasklet:
	struct tasklet_struct
	{
		struct tasklet_struct *next;
		unsigned long state;
		atomic_t count;
		void (*func)(unsigned long);//下半部要执行的代码
		unsigned long data;
	};
	
	
	1，初始化tasklet
		a，静态---在编译的时候---定义一个全局变量
			DECLARE_TASKLET(name,func,data)
			
			
			#define DECLARE_TASKLET(name, func, data) \
				struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
		b，动态---在运行的时候
			tasklet_init(struct tasklet_struct * t,void(* func)(unsigned long),unsigned long data)
	
	2，在上半部的时候放入到内核线程中去参与调度
		tasklet_schedule(struct tasklet_struct * t)
	

工作队列和工作：
	struct workqueue_struct：表示一个工作队列，其实就代表一个内核线程

	struct work_struct：表示下半部的工作
	typedef void (*work_func_t)(struct work_struct *work);
	struct work_struct {
		atomic_long_t data;
		struct list_head entry;
		work_func_t func; //下半部要执行的代码
	};
	
	a, 使用自己的work和自己的工作队列
	
		1，创建工作队列
			struct workqueue_struct *create_workqueue(char *name) //创建了一个内核的线程
			void destroy_workqueue(struct workqueue_struct * wq)
 		
		2，初始化工作
			a，静态---在编译的时候---定义一个全局变量
				DECLARE_WORK(n,f)
			b，动态---在运行的时候
				INIT_WORK(_work,_func)
			
		
		3，将工作放入到工作队列(在上半部的时候放入到内核线程中去参与调度)
			queue_work(struct workqueue_struct * wq,struct work_struct * work)
	
	
	b, 使用自己的work和系统的工作队列
		1，初始化工作
			a，静态---在编译的时候---定义一个全局变量
				DECLARE_WORK(n,f)
			b，动态---在运行的时候
				INIT_WORK(_work,_func)
			
		
		2，将工作放入到工作队列(在上半部的时候放入到内核线程中去参与调度)
			schedule_work(struct work_struct * work)
	

读内核代码：
	技巧：
		1，只看看的懂的代码
		2，看主线
		3，猜函数名
		4，出错判断和变量名不看
		5，看代码的时候做笔记和注释，还有总结
		6，不懂找度娘
	===================================================

	open()  read()  write() exit()
		2      3     4         5
	------------------------------------------
	glibc
		 int open()
		 {
			 swi NR_open  // NR_open==2
			 //产生异常--内核处理--进入到内核
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
				fd = get_unused_fd_flags(flags);//获取到没有被使用的文件fd
				
				struct file *f = do_filp_open(dfd, tmp, &op, lookup);//创建一个struct file
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
															f->f_op = fops_get(inode->i_fop);//得到inode中的fop
															if (!open && f->f_op)
																open = f->f_op->open;//将inode中fop中的open赋值给open
															 open(inode, f);//调用open---》 chrdev_open
															 
															//  所有的设备节点inode中的fop是谁？---def_chr_fops
													return filp;
												
										return filp;
									
								return filp;
				
				fsnotify_open(f);//通知文件打开了 ？？？？
				fd_install(fd, f);// 将struct file和fd进行关联

				
				return fd; //返回一个fd
				
				
				
				最终调用了
				const struct file_operations def_chr_fops = {
					.open = chrdev_open,
							|
							struct cdev *new = NULL;
							kobj = kobj_lookup(cdev_map, inode->i_rdev, &idx);//根据inode中设备号去找到cdev
							new = container_of(kobj, struct cdev, kobj);//你自己驱动中编写的cdev
							inode->i_cdev = p = new;
							filp->f_op = fops_get(p->ops);//将cdev中的ops给了file->f_op
							if (filp->f_op->open) {
								ret = filp->f_op->open(inode,filp);// 调用了cdev中ops中的open方法
					.llseek = noop_llseek,
				};
			
	--------------------------------------------------------
	设备驱动：目标---需要看到应用open如何调用到设备驱动中的open函数
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





理解
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
		file = fget_light(fd, &fput_needed);//通过fd获取到对应的struct file
		
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
	










