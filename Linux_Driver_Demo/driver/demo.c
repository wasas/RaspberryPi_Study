#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#define DRIVER_NAME "Demo"
#define DEVICE_NAME "Demo"

static dev_t demo_devno; //设备号
static struct class *demo_class;
static struct cdev demo_dev;


static struct gpio_config{
	int button_num;
	int led_num;
}config;

static int flag = 0;

static irqreturn_t irq_handler(int irq,void *dev)
{
	printk(KERN_ERR"Enter irq\n");
	if(flag)
		gpio_set_value(config.led_num,!gpio_get_value(config.led_num));
	return IRQ_HANDLED;
}

//open 函数，应用程序调用open系统调用时会调用本函数
static int demo_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO"Demo open\n");
	return 0;

}

//release 函数，应用程序调用close系统调用时会调用本函数
static int demo_release(struct inode *inode, struct file *filp)
{	
	if(flag){
		free_irq(gpio_to_irq(config.button_num),NULL);
		gpio_free(config.button_num);
		gpio_free(config.led_num);
		flag = 0;
	}

	printk(KERN_INFO"Demo release\n");
	return 0;
}

//ioctl 控制函数，应用程序调用ioctl系统调用时会调用本函数
static long demo_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	int err = 0;
	switch(cmd){
		case 0://0 表示命令号，一般都用宏定义来控制
			{
				if(copy_from_user(&config,(void *)arg,sizeof(struct gpio_config))){ //从用户程序中获取配置数据
					printk(KERN_ERR"[%s %d] : copy_from user failed !\n",__func__,__LINE__);
					return -EFAULT;
				}
				printk(KERN_INFO"[%s %d]: Get button gpio num: %d and led gpio num: %d\n ",__func__,__LINE__,config.button_num,config.led_num);
				err = gpio_request_one(config.button_num,GPIOF_IN,"Light Button");
				if(err){
					printk(KERN_ERR"[%s %d]:Request button gpio failed\n",__func__,__LINE__);
					return -EFAULT;
				}
				err = gpio_request_one(config.led_num,GPIOF_OUT_INIT_LOW,"LED light");
				if(err){
					printk(KERN_ERR"[%s %d]:Request led gpio failed\n",__func__,__LINE__);
					gpio_free(config.button_num);
				}
				enable_irq(gpio_to_irq(config.button_num));
				err = request_irq(gpio_to_irq(config.button_num),irq_handler,IRQF_TRIGGER_FALLING,"Led Test",NULL);
				if(err < 0)
				{
					printk(KERN_ERR"[%s %d]:Request button gpio irq failed\n",__func__,__LINE__);
					gpio_free(config.led_num);
					gpio_free(config.button_num);
				}
				flag = 1;
				break;
			}
		default:
			printk(KERN_INFO"[%s %d]:Invalid cmd", __func__,__LINE__);
			break;
	}
	return 0;
}



static struct file_operations demo_fops = {
	.owner = THIS_MODULE,
	.open  = demo_open,
	.release  = demo_release,
	.unlocked_ioctl = demo_ioctl,
};


static int __init demo_init(void)
{
	int err;

	printk(KERN_INFO"Demo Init \n");

	err = alloc_chrdev_region(&demo_devno,0,1,DRIVER_NAME);   
	if(err < 0){
		goto err;
	}
	cdev_init(&demo_dev,&demo_fops);

	err = cdev_add(&demo_dev,demo_devno,1);

	if(err < 0)
	{
		printk(KERN_ERR"[%s,%d]add cdev failed\n",__func__,__LINE__);
		goto FREE_DEVNO;
	}
	//自动生成设备文件 在/dev目录下，文件名为DEVICE_NAME
	demo_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(demo_class))
	{
		printk(KERN_ERR"[%s,%d]class create failed\n",__func__,__LINE__);
		goto DEV_FREE;
	}
	device_create(demo_class,NULL,demo_devno,NULL,DEVICE_NAME);

	return 0;
DEV_FREE:
	cdev_del(&demo_dev);
FREE_DEVNO:
	unregister_chrdev_region(demo_devno, 1);
err:
	return err;
}


static void demo_exit(void)
{
	if(flag){
		free_irq(gpio_to_irq(config.button_num),NULL);
		gpio_free(config.button_num);
		gpio_free(config.led_num);
	}
	device_destroy(demo_class,demo_devno);
	class_destroy(demo_class);
	cdev_del(&demo_dev);
	unregister_chrdev_region(demo_devno, 1);
	printk(KERN_INFO"Demo exit\n");
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_AUTHOR("hyg");
MODULE_DESCRIPTION("BUTTON LED Driver");
MODULE_LICENSE("GPL");
