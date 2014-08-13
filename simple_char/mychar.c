#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>

#define debug(fmt, ...) printk(KERN_DEBUG fmt, ##__VA_ARGS__)

#define DEV_NAME "mychar"

#define DEVICE_MAJOR 0

#define DEV_COUNT 1

struct cdev mychar_cdev;
int mychar_major = DEVICE_MAJOR;
int mychar_minor = 0;
int mychar_dev_count = DEV_COUNT;
dev_t devno;
char *data;

static int mychar_open(struct inode *inode, struct file *filp)
{
  debug("in %s()\n", __func__);
  return 0;
}

static int mychar_release(struct inode *inode, struct file *filp)
{
  debug("in %s()\n", __func__);
  return 0;
}

static ssize_t mychar_read(struct file *filp, char *buf, size_t count, loff_t *f_ops)
{
  int len;
  char *tmp;

  if (count < 0)
    return -EINVAL;

  tmp = (char*)kmalloc(sizeof(char)*(count+1), GFP_KERNEL);
  sprintf(tmp, "The voice from hell:%s\n", data);
  len = strlen(tmp);
  if (len < count)
    count = len;

  if (copy_to_user(buf, tmp, count))
    return -EFAULT;

  debug("in %s() tmp: %s\n", __func__, tmp);
  debug("in %s() buf: %s\n", __func__, buf);
  debug("in %s() data: %s\n", __func__, data);

  kfree(tmp);
  return count;
}

static ssize_t mychar_write(struct file *filp, const char *buf, size_t count, loff_t *f_ops)
{
  if(count < 0)	
    return -EINVAL;

  data = (char*)kmalloc(sizeof(char)*(count+1), GFP_KERNEL);
  if(data == NULL)
    return -ENOMEM;

  if(copy_from_user(data, buf, count+1))
    return -EFAULT;

  debug("in %s() buff:%s\n", __func__, buf);
  debug("in %s() data:%s\n", __func__, data);

  return count;
}

static struct file_operations mychar_fops = {
  .owner = THIS_MODULE,
  .open = mychar_open,
  .release = mychar_release,
  .read = mychar_read,
  .write = mychar_write,
};

static int __init mychar_init(void)
{
  int ret = -1;

  cdev_init(&mychar_cdev, &mychar_fops);
  mychar_cdev.owner = THIS_MODULE;

  /* Get major device number and minor device number */
  if(mychar_major)
  {
    devno = MKDEV(mychar_major, mychar_minor);
    ret = register_chrdev_region(devno, mychar_dev_count, DEV_NAME);
  }
  else
  {
    ret = alloc_chrdev_region(&devno, mychar_minor, mychar_dev_count, DEV_NAME);
    mychar_major = MAJOR(devno);
  }

  if(ret < 0)
  {
    debug("%s can't get major %d\n", DEV_NAME, mychar_major);
    return -EINVAL;
  }

  ret = cdev_add(&mychar_cdev, devno, 1);
  if(ret < 0)
  {
    debug("%s cdev_add failure!\n", DEV_NAME);
    return -EINVAL;
  }

  debug("%s init ok!\n", DEV_NAME);
  return 0;
}

static void __exit mychar_exit(void)
{
  if(data != NULL)
    kfree(data);

  unregister_chrdev_region(devno,1);
  cdev_del(&mychar_cdev);
  debug("%s exit ok!\n", DEV_NAME);
}

module_init(mychar_init);
module_exit(mychar_exit);
