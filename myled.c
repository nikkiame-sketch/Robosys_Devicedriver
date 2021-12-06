//SPDX-License-Identifier: GPL-3.0
//Copyright (c) Shunsuke Yoneyama & Ryuichi Ueda All rights reserved.

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>

MODULE_AUTHOR("Shunsuke Yoneyama & Ryuichi Ueda");
MODULE_DESCRIPTION("driver for charactor display");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

int segments[7] = {23,24,25,8,7,1,12};	//set gpio for 7  segments
char cha[8] =
{
	0b01110111,	/* a */
	0b01111100,	/* b */
	0b00111001,	/* c */
	0b01011110,	/* d */
	0b01111001,	/* e */
	0b01110001,	/* f */
	0b00111101,	/* g */
	0b01110110,	/* h */
};

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;

static volatile u32 *gpio_base = NULL;

void seg_out(int disp)
{
	int i;
	char data = cha[disp];

	for(i=0;i<7;i++)
	{
		if(data & 0x01)
			gpio_base[7] = 1 << segments[i];
		else
			gpio_base[10] = 1 << segments[i];

		data >>= 1;
	}
}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	int i;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;

	printk(KERN_INFO"receive %c\n",c);

	
	
	switch(c)
	{
		case 'a':
			seg_out(0);
			break;
		case 'b':
			seg_out(1);
			break;
		case 'c':
			seg_out(2);
			break;
		case 'd':
			seg_out(3);	
			break;
		case 'e':
			seg_out(4);
                        break;
		case 'f':
			seg_out(5);
     			break;
		case 'g':
	                seg_out(6);
	                break;
		case 'h':
			seg_out(7);
			break;
		case 'x':
			for(i=0;i<7;i++)
			{
				gpio_base[10] = 1 << segments[i];
			}
			break;
		default:
			break;
	}
	
	
	return 1;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write
};

static int __init init_mod(void)
{
	int retval;
	int i;
	gpio_base = ioremap_nocache(0x3f200000, 0xa0); //0xfe..:base address, 0xA0: region to map
	
	for(i=0;i<7;i++)
	{
		u32 led = segments[i];
		u32 index = led/10;//GPFSEL2
		u32 shift = (led%10)*3;//15bit
		u32 mask = ~(0x7 << shift);

		gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
	}

	retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
	printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	cdv.owner = THIS_MODULE;
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
	iounmap(gpio_base);
}

module_init(init_mod);
module_exit(cleanup_mod);
