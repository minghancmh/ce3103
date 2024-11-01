#include <linux/gpio.h> /* you may want to look at the contents of this file, to understand
the various elements of the data structure shown in the code below */
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd0 = open("/dev/gpiochip0", O_RDWR);
  if (fd0 == -1) {
    perror("Failed to open /dev/gpiochip0");
    exit(EXIT_FAILURE);
  }
  struct gpiochip_info cinfo;

  if (ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo) == -1) {
    perror("Failed to get GPIO chip info");
    close(fd0);
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n", cinfo.name,
          cinfo.label, cinfo.lines);

  // Configure the OUTPUT LEDs
  struct gpiohandle_request req_GYR = {0};
  struct gpiohandle_data data_GYR = {0};
  req_GYR.lines = 3;                         // 2 pins in this handler
  req_GYR.lineoffsets[0] = 4;                // pin 4 - Green LED
  req_GYR.lineoffsets[1] = 17;               // pin 17 - Yellow LED
  req_GYR.lineoffsets[2] = 27;               // pin 27 - Red LED
  req_GYR.flags = GPIOHANDLE_REQUEST_OUTPUT; // set them to be output
  data_GYR.values[0] = 1; // set initial value of Green LED to High (ON)
  data_GYR.values[1] = 0; // set initial value of Yellow LED to Low (OFF)
  data_GYR.values[2] = 0; // set initial value of Red LED to Low (OFF)

  if (ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GYR) == -1) {
    perror("Failed to get line handle");
    close(fd0);
    exit(EXIT_FAILURE);
  }

  // Congirue the input push button
  struct gpiohandle_request req_pb = {0};
  struct gpiohandle_data data_pb = {0};

  req_pb.lines = 1;
  req_pb.lineoffsets[0] = 11; // pin 11 - Push Button
  req_pb.flags = GPIOHANDLE_REQUEST_INPUT;
  if (ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_pb) == -1) {
    perror("Failed to get line handle for input push button");
    close(req_GYR.fd);
    close(fd0);
    exit(EXIT_FAILURE);
  };

  for (int i = 0; i < 5000; ++i) {
    if (ioctl(req_pb.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_pb) == -1) {
      perror("Failed to get line value of Push Button");
      close(req_pb.fd);
      close(req_GYR.fd);
      close(fd0);
      exit(EXIT_FAILURE);
    }

    if (data_pb.values[0] == 1) { // Button is pressed
      data_GYR.values[2] = 1; // Turn on the Red LED
    } else {
        data_GYR.values[2] = 0; // Else if the butotn not pressed, we turn off the Red LED
    }

    if (ioctl(req_GYR.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GYR) == -1) {
      perror("Failed to set line values");
      close(req_pb.fd);
      close(req_GYR.fd);
      close(fd0);
      exit(EXIT_FAILURE);
    }

    usleep(1000000); // sleep for 1 second
  }                  
  close(req_pb.fd);
  close(req_GYR.fd);
  close(fd0);
  exit(EXIT_SUCCESS);
} // main
