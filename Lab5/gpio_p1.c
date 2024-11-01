#include <linux/gpio.h> /* you may want to look at the contents of this file, to understand
the various elements of the data structure shown in the code below */
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd0 = open("/dev/gpiochip0", O_RDWR); // open the file descriptor
  if (fd0 == -1) {
    perror("Failed to open /dev/gpiochip0");
    exit(EXIT_FAILURE);
  }
  struct gpiochip_info cinfo;
  // ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo); // get the chip information
  if (ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo) == -1) {
    perror("Failed to get GPIO chip info");
    close(fd0);
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n", cinfo.name,
          cinfo.label, cinfo.lines);
  // struct gpiohandle_request req_GY; // Green and Yellow
  // struct gpiohandle_data data_GY;   // for data bit
  struct gpiohandle_request req_GY = {0};
  struct gpiohandle_data data_GY = {0};
  req_GY.lines = 2;                         // 2 pins in this handler
  req_GY.lineoffsets[0] = 4;                // pin 4 - Green LED
  req_GY.lineoffsets[1] = 17;               // pin 17 - Yellow LED
  req_GY.flags = GPIOHANDLE_REQUEST_OUTPUT; // set them to be output
  data_GY.values[0] = 1; // set initial value of Green LED to High (ON)
  data_GY.values[1] = 0; // set initial value of Yellow LED to Low (OFF)
  // ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL,
  // &req_GY); // now get the line handler req_GY
  if (ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GY) == -1) {
    perror("Failed to get line handle");
    close(fd0);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < 5; ++i) {
    // ioctl(req_GY.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL,
    //       &data_GY);                        // output data bits
    if (ioctl(req_GY.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GY) == -1) {
      perror("Failed to set line values");
      close(req_GY.fd);
      close(fd0);
      exit(EXIT_FAILURE);
    }
    usleep(1000000);                        // sleep for 1 second
    data_GY.values[0] = !data_GY.values[0]; // toggle
    data_GY.values[1] = !data_GY.values[1];
  }                 // for
  close(req_GY.fd); // release line
  close(fd0);       // close the file
  exit(EXIT_SUCCESS);
} // main
