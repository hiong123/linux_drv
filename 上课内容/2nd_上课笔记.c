��Ҫ���ݣ�linux��led�����ı�д
1���豸��ע��
2���豸�ļ�
3��ӳ��
4���û����ں����ݽ���
5��������
6��gpio�⺯����ʹ��
==================================================
�豸�ļ���
farsight@ubuntu:~$ ls /dev/input/event0 -l
crw-r----- 1 root root 13, 64 Aug 17 19:30 /dev/input/event0

�豸��(32bit����)== ���豸��+���豸��
					 12bit    20bit
					 
	���豸�ţ�ĳһ���豸---����ͷ
	���豸�ţ�ĳһ����ĳ���豸--ǰ�úͺ�������ͷ
	
	
�豸�� ==> dev_t == (250<<20)|0 <=== 250 + 0
	֪���豸�ţ���Ҫ��ȡ���豸/���豸��

	#define MAJOR(dev)	((unsigned int) ((dev) >> 20))
	#define MINOR(dev)	((unsigned int) ((dev) & 0xfffff))
	#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi)) //��ϳ��豸��

�������豸�ţ�
	int register_chrdev(unsigned int major,const char * name,const struct file_operations * fops)
	����1������ָ������Ҫ�����豸��--250���ϣ���̬��
			�˴���0����ϵͳ�Զ����䣬����֮������豸��ͨ������ֵ�õ�
			
	����2���豸���������Զ��壭��/proc/devices
		���ȥ�ж��������豸�ţ��Ƿ�����ɹ����鿴/proc/devices�ļ�
	
	����3���ļ��������϶���
	
	
	����ֵ����ȷ���أ������ߴ��ڣ�����̬���䣩
			���󣺷��ظ���

�ͷ����豸�ţ�
	void unregister_chrdev(unsigned int major,const char * name)
	����1����Ҫ�ͷŵ����豸��
	����2���豸������
	
	
=============================================================
�����豸�ļ�(�豸�ڵ�)��Ӧ�������Ҫȥ����������ֻ��ͨ���ļ�ȥ����
	1,�ֶ�����
	
		mknod �ļ��� �豸���� ���豸�� ���豸��
		
		
		���ӣ�
			mknod /dev/cat  c  250 0   //���豸��һ��Ҫ������������ı���һ�£����豸�ſ����Լ����壬һ���0��ʼ
		
		
		��ȱ�ݣ�/dev/Ŀ¼���ļ��������ڴ��У�ÿ�ο���֮����Ҫ�ֶ��������Ƚ��鷳
	
	
	2���ڴ������Զ�����--����һ��װ�غ󣬾Ϳ����Զ��Ĵ����豸�ڵ�
		
		//����һ����
		struct  class *class_create(owner,name)
		
		�÷���
			//�Զ������豸�ڵ�  /dev/xxx  ��  ��
			//������
			//����1--THIS_MODULE
			//����2--�ַ������Զ���
			//����ֵ: ����һ��classָ��
			simple_cls = class_create(THIS_MODULE, "simple_cls");
			
			ɾ���ࣺ
				class_destroy(struct class * cls)
		
		
		device_create(struct class * class,struct device * parent,dev_t devt,void * drvdata,const char * fmt,...)

		�÷���
			// �����豸�ڵ�
			// ����1--class_create����֮�����
			// ����2--�豸�ĸ���--NULL
			// ����3--�豸��--���豸��+���豸��
			//����4---˽������--NULL
			//����5--�ɱ����--��ʾ�豸�ڵ������
			//����ֵ: ����һ��deviceָ��
			simple_dev = device_create(simple_cls,NULL, MKDEV(dev_major, 0), NULL, "simple%d", 0);
	
		//����Ч��--/dev/simple0,ͬʱ������/sys/class/simple_cls/simple0
	
		ɾ���豸�ڵ㣺
			// ����1--class_create����֮�����
			// ����2--�豸��--���豸��+���豸��
			device_destroy(struct class * class,dev_t devt)
=================================================================================
��������ȥ����Ӳ���ķ���---���ǲ���Ӳ���������ַ
	�����ַ�� �������ַ��mmu��Э���£�ת�����ɵ�
	
	�������ֲ��ʱ�򣺿������ǼĴ����������ַ
	
	
	
	
	
	gpio: �๦�ܵ����ţ� ���룬������жϣ�i2c��spi, vsync�ȵ�
		1, ����gpio�Ĺ���--���
				GPC0CON, R/W, Address = 0xE0200060
				
				��GPC0_3/4 ���ó����---0001
				
				GPC0CON &= ~(0xff<<12); 
				GPC0CON |= (0x11<<12);
		
		2�����һ���ߵ͵�ƽ
				GPC0DAT, R/W, Address = 0xE0200064
				
				������
					GPC0DAT |= 0x3<<3;
				������
					GPC0DAT &= ~(0x3<<3);	
					
	
	�ڴ�������Ҫ�������ַת���������ַ��Ȼ���ٲ�����
		void *ioremap(cookie,size)
		����1�������ַ
		����2��ӳ���С
		����ֵ�������ַ
		
		//ȡ��ӳ��
		void iounmap(void *addr);
		����1�������ַ
		

	���ӣ�
		volatile unsigned long *gpc0conf = ioremap(0xE0200060, 8);
		volatile unsigned long *gpc0data = gpc0conf + 1;
		 
		*gpc0conf &= ~(0xff<<12); 
		*gpc0conf |= (0x11<<12);


		*gpc0data |= 0x3<<3;


	
	
	// 0������һ��ȫ���豸����--�������
	// 1�� �������豸��
	// 2, �����豸�ڵ�
	// 3,Ӳ���ĳ�ʼ��---��ַӳ��
	// 4,ʵ��fops�и��� �ӿ�


	==========================================================
	Ӧ�ó�����������ݽ�����
	1�����û��ռ��п������ݵ�����(��������xxx_write)
		 long copy_from_user(void *to,const void __user * from, unsigned long n)
		//����1��Ŀ�ĵ�,�û��ռ䴫�ݹ����ĵ�ַ
		//����2��Դͷ
		//����3������
		����ֵ����ʣ�¶���û�п����ɹ� 
				����0��ʾ����
				����0��ʾ��ȷ

	2�������������ݿ������û��ռ�(��������xxx_read)
		long copy_to_user(void __user *to,const void *from, unsigned long n)






���񣺱�дled����
1����Ϥ�ַ��豸������̵Ĳ���
2����Ϥ����������ʹ��
3����������(a, �Ȳ��ܳ��� b��дled������Ҫ������)


samba���ã�
[farsight]
path = /home/farsight/
available  = yes
browseable = yes
public     = yes
writable   = yes

�ĳɣ�
[farsight]
path = /home/farsight/
available  = yes
browseable = yes
;public     = yes
valid users = farsight
writable   = yes


//Ϊsamba��½��������
farsight@ubuntu:~$ sudo smbpasswd -a farsight
New SMB password:
Retype new SMB password:

//����samba
farsight@ubuntu:~$ sudo service smbd restart
smbd stop/waiting
smbd start/running, process 12920






	
	