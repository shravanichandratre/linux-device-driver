#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>  // for copy_from_user
#include <linux/version.h>

MODULE_LICENSE("GPL");

#define BUFFER_SIZE 128

static struct proc_dir_entry *custom_proc_node;
static char kernel_buffer[BUFFER_SIZE];  // Buffer to store data from user space
static size_t data_size = 0;  // Track data size in buffer

// Read function for proc file
static ssize_t my_read(struct file *file_pointer, char *user_space_buffer, size_t count, loff_t *offset) {
    size_t len = data_size;  // Use actual data size

    printk("my_read\n");

    if (*offset >= len) {
        return 0;  // End of file
    }

    if (count > len - *offset) {
        count = len - *offset;  // Adjust count to not exceed data size
    }

    if (copy_to_user(user_space_buffer, kernel_buffer + *offset, count) != 0) {
        return -EFAULT;  // Return an error if copy fails
    }

    *offset += count;
    return count;
}

// Write function for proc file
static ssize_t my_write(struct file *file_pointer, const char *user_space_buffer, size_t count, loff_t *offset) {
    printk("my_write\n");

    if (count > BUFFER_SIZE - 1) {
        count = BUFFER_SIZE - 1;  // Limit count to allow for null terminator
    }

    if (copy_from_user(kernel_buffer, user_space_buffer, count) != 0) {
        return -EFAULT;  // Return an error if copy fails
    }

    kernel_buffer[count] = '\0';  // Null-terminate the string
    data_size = count;  // Update data size

    printk("Data received from user space: %s\n", kernel_buffer);
    return count;
}

// Define the file operations for kernel versions 5.6 and above
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static struct proc_ops driver_proc_ops = {
    .proc_read = my_read,
    .proc_write = my_write
};
#else
// Define the file operations for older kernel versions
static struct file_operations driver_proc_ops = {
    .read = my_read,
    .write = my_write
};
#endif

static int my_init(void) {
    printk("my_init: Entry\n");

    custom_proc_node = proc_create("my_driver", 0666, NULL, &driver_proc_ops);  // Set permissions to allow writing
    if (!custom_proc_node) {
        printk("Failed to create /proc/my_driver\n");
        return -ENOMEM;
    }

    printk("my_init: Exit\n");
    return 0;
}

static void my_exit(void) {
    printk("my_exit: Entry\n");
    proc_remove(custom_proc_node);
    printk("my_exit: Exit\n");
}

module_init(my_init);
module_exit(my_exit);
