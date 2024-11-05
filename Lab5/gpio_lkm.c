#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/init.h>

static unsigned int ledGreen = 4;         // GPIO4 (pin 7) connected to Green LED
static unsigned int pushButton = 11;      // GPIO11 (pin 23) connected to Push Button
static unsigned int irqNumber;            // Shared IRQ number within the file
static bool ledOn = 0;                    // Used to toggle the state of the LED

// The GPIO IRQ Handler function
static irq_handler_t rpi_gpio_isr(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    ledOn = !ledOn; // Toggle LED state
    gpio_set_value(ledGreen, ledOn); // Set LED accordingly
    printk(KERN_ALERT "GPIO Interrupt! LED state: %d\n", ledOn);
    return (irq_handler_t) IRQ_HANDLED; // Announce IRQ handled
}

// The LKM exit function
static void __exit rpi_gpio_exit(void) {
    gpio_set_value(ledGreen, 0); // Turn the LED off
    gpio_free(ledGreen); // Free the LED GPIO
    gpio_free(pushButton); // Free the Button GPIO
    free_irq(irqNumber, NULL); // Free the IRQ number, no *dev_id
    printk(KERN_ALERT "Goodbye from the GPIO LKM!\n");
}

// The LKM initialization function
static int __init rpi_gpio_init(void) {
    int result = 0;
    printk(KERN_ALERT "Initializing the GPIO LKM\n");

    // Initialize the LED
    ledOn = true; // Default for LED is ON
    gpio_request(ledGreen, "sysfs"); // Request the LED GPIO
    gpio_direction_output(ledGreen, ledOn); // Set in output mode and turn on LED

    // Initialize the Push Button
    gpio_request(pushButton, "sysfs"); // Request the Button GPIO
    gpio_direction_input(pushButton); // Set up as input
    gpio_set_debounce(pushButton, 1000); // Debounce delay of 1000 ms

    // Map the Push Button GPIO to an IRQ number
    irqNumber = gpio_to_irq(pushButton);
    printk(KERN_ALERT "Button mapped to IRQ: %d\n", irqNumber);

    // Request the interrupt line
    result = request_irq(irqNumber,            // Interrupt number requested
                         (irq_handler_t) rpi_gpio_isr, // ISR handler function
                         IRQF_TRIGGER_RISING,  // Trigger on rising edge
                         "rpi_gpio_handler",   // Used in /proc/interrupts
                         NULL);                // *dev_id for shared interrupt lines - NULL
    if (result) {
        printk(KERN_ALERT "GPIO LKM: Failed to request IRQ %d\n", irqNumber);
        return result; // Return an error if IRQ request fails
    }

    return result;
}

module_init(rpi_gpio_init);  // Macro to execute module's initialization routine
module_exit(rpi_gpio_exit);  // Macro to execute module's exit routine

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CE3103");
MODULE_DESCRIPTION("GPIO Device Driver for Raspberry Pi");
MODULE_VERSION("V1");