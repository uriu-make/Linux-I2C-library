#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#ifndef I2CLIB_H_INCLUDE
#define I2CLIB_H_INCLUDE
#define I2C0 "/dev/i2c-0"
#define I2C1 "/dev/i2c-1"
#define I2C2 "/dev/i2c-2"
#define I2C3 "/dev/i2c-3"
#define I2C4 "/dev/i2c-4"
#define I2C5 "/dev/i2c-5"
#define I2C6 "/dev/i2c-6"

class i2c {
 private:
  int fd;
  unsigned short address;
  const char *device;

 public:
  i2c(const char *device, char address);
  int Setup(void);

  int Write(const void *data, int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int WriteReg(unsigned char reg, const void *data, int length = 1);

  int Read(void *data, const int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int ReadReg(unsigned char reg, void *data, const int length = 1);

  int Close(void);
};
#endif
