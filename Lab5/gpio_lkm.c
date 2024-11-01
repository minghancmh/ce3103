#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h> // Include for module-related macros
#include <stdbool.h>

static unsigned int gpioLED = 4;     // GPIO for LED
static unsigned int gpioButton = 11; // GPIO for Button
static unsigned int irqNumber;       // IRQ number for the button
static bool ledOn = true;            // LED state
static int result;                   // Store the result of request_irq

// Interrupt handler function prototype
// static irqreturn_t rpi_gpio_isr(int irq, void *dev_id);

static int __init rpi_gpio_init(void) {
  // Request and set up GPIO for LED
  result = gpio_request(gpioLED, "LED-Green");
  if (result) {
    printk(KERN_ALERT "GPIO_TEST: Failed to request LED GPIO\n");
    return result;
  }
  gpio_direction_output(gpioLED, ledOn); // Set as output and turn on
  gpio_export(gpioLED, false);           // Export to /sys/class/gpio

  // Request and set up GPIO for button
  result = gpio_request(gpioButton, "PushButton");
  if (result) {
    printk(KERN_ALERT "GPIO_TEST: Failed to request Button GPIO\n");
    gpio_free(gpioLED); // Free the LED GPIO before exiting
    return result;
  }
  gpio_direction_input(gpioButton); // Set as input
  gpio_export(gpioButton, false);   // Export to /sys/class/gpio

  // Map GPIO to IRQ
  irqNumber = gpio_to_irq(gpioButton);
  if (irqNumber < 0) {
    printk(KERN_ALERT "GPIO_TEST: Failed to map GPIO to IRQ\n");
    gpio_free(gpioButton);
    gpio_free(gpioLED);
    return irqNumber;
  }
  printk(KERN_INFO "GPIO_TEST: button mapped to IRQ: %d\n", irqNumber);

  // Request interrupt line
  result = request_irq(irqNumber,                   // Interrupt number
                       (irq_handler_t)rpi_gpio_isr, // ISR handler function
                       IRQF_TRIGGER_RISING,         // Rising edge trigger
                       "rpi_gpio_handler",          // Name in /proc/interrupts
                       NULL);                       // Dev_id (not shared)
  if (result) {
    printk(KERN_ALERT "GPIO_TEST: Failed to request IRQ\n");
    gpio_free(gpioButton);
    gpio_free(gpioLED);
    return result;
  }
  printk(KERN_INFO "GPIO Successfully initialised.\n");

  return 0; // Initialization successful
}

// Interrupt Service Routine (ISR) TODO: change return type to irqreturn_t ?
static irq_handler_t rpi_gpio_isr(int irq, void *dev_id) {
  ledOn = !ledOn;                 // Toggle LED state
  gpio_set_value(gpioLED, ledOn); // Update LED state
  printk(KERN_INFO "GPIO_TEST: Interrupt! LED is %s\n", ledOn ? "ON" : "OFF");
  return (irq_handler_t)IRQ_HANDLED;
}

// Exit function to release resources
static void __exit rpi_gpio_exit(void) {
  gpio_set_value(gpioLED, 0); // Turn off the LED
  gpio_unexport(gpioLED);     // Unexport the LED GPIO
  free_irq(irqNumber, NULL);  // Free the IRQ
  gpio_unexport(gpioButton);  // Unexport the Button GPIO
  gpio_free(gpioLED);         // Free the LED GPIO
  gpio_free(gpioButton);      // Free the Button GPIO
  printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}

module_init(rpi_gpio_init);
module_exit(rpi_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux GPIO Interrupt Handler Module");
MODULE_VERSION("0.1");
