#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

#define DEVICE_NAME "kbd_intr_hook"

irqreturn_t hook_handler(int irq, void *dev_id)
{
  char data;
  data = inb(0x60);
  printk("kbd_hook: intr! data %x\n", data);
  return IRQ_HANDLED;
}

static int hook_init(void)
{
  int retval;
  retval = request_irq(1,
                       hook_handler,
                       IRQF_SHARED | IRQF_DISABLED,
                       "kbd_hook",
                       (int*)0x55aa00ff);
  printk("kbd_hook: retval=%d\n", retval);
  return retval;
}

static void hook_exit(void)
{
  free_irq(1, (int*)0x55aa00ff);
  printk("kbd_hook: exit\n");
}

module_init(hook_init);
module_exit(hook_exit);
