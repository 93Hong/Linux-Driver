#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

// module attributes
MODULE_LICENSE("GPL"); // avoid kerner taint warning
MODULE_DESCRIPTION("Echo devier driver");
MODULE_AUTHOR("Giwook Hong");

#define DEV_NAME "echo"

static int __open(struct inode *, struct file *);
static ssize_t __read(struct file *, char *, size_t, loff_t *);
static ssize_t __write(struct file *, const char *, size_t, loff_t *);
static int __release_(struct inode *, struct file *);

static int _major = 0;
static char _buf[1024];
static int _open = 0;
static int _pos = 0;

static struct file_operations fops = {
	.open = __open,
	.read = __read,
	.write = __write,
	.release = __release_,
};

// called when module is loaded
int init_module(void) {
	_major = register_chrdev(0, DEV_NAME, &fops);

	if (_major < 0) {
		printk(KERN_ALERT "%s device registration failed.\n", DEV_NAME);
		return _major;
	}

	printk(KERN_INFO DEV_NAME " device registered.\n");
	printk(KERN_INFO "Mafor: %d\n", _major);

	return 0;
}

// called when module is unloaded
void cleanup_module(void) {
	unregister_chrdev(_major, DEV_NAME);
}

static int __open(struct inode *inode, struct file *file) {
	_open++;
	printk(KERN_INFO DEV_NAME " opened %d times\n", _open);
	return 0;
}

static ssize_t __read(struct file *file, char *buf, size_t len, loff_t *off) {
	int count = 0;
	while (len && (_buf[_pos] != 0)) {
		put_user(_buf[_pos], buf++);
		count++;
		len--;
		_pos++;
	}
	return count;
}

static ssize_t __write(struct file *file, const char *buf, size_t len, loff_t *off) {
	int i;
	for (i = _pos = 0; i<len; i++) {
		_buf[i] = buf[i];
	}

	_buf[i] = '\0';
	return i;
}

static int __release_(struct inode *inode, struct file *file) {
	printk(KERN_INFO DEV_NAME " device closed\n");
	return 0;
}


