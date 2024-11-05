#include <linux/init.h>      // needed by the macros module_init and module_exit
#include <linux/module.h>    // needed by all modules
#include <linux/kernel.h>    // needed by KERN_ definitions

// Initialization function
static int __init hello_init(void) {
    printk(KERN_ALERT "Hello from kernel world\n");
    return 0;  // 0 for success, negative for failure
}

// Exit function
static void __exit hello_exit(void) {
    printk(KERN_ALERT "Goodbye from kernel world\n");
}

// Register the initialization and exit functions
module_init(hello_init);  // Macro to execute module's initialization routine
module_exit(hello_exit);  // Macro to execute module's exit routine

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CE3103");
MODULE_DESCRIPTION("Simple Hello module");
MODULE_VERSION("V1");
