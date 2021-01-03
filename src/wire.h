#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#ifndef BASICI2C_H_INCLUDE
#define BASICI2C_H_INCLUDE
#define I2C0 "/dev/i2c-0"
#define I2C1 "/dev/i2c-1"
#define I2C2 "/dev/i2c-2"
#define I2C3 "/dev/i2c-3"
#define I2C4 "/dev/i2c-4"
#define I2C5 "/dev/i2c-5"
#define I2C6 "/dev/i2c-6"

class wire {
 private:
  int fd, address;
  const char *device;

  struct i2c_smbus_ioctl_data {
    unsigned char read_write;
    unsigned char command;
    unsigned int size;
    void *data;
  };

 public:
  wire(void);
  wire(const char *device, char address);
  int Setup(void);
  int Setup(const char *device, char address);

  int Write(const void *data, int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int WriteReg(const char reg, const void *data, int length = 1);

  int Read(void *data, const int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int ReadReg(const char reg, void *data, const int length = 1);

  int Close(void);
};
#endif
