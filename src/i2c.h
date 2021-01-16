#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#ifndef I2C_H_INCLUDE
#define I2C_H_INCLUDE
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
  struct t_data{
	unsigned char read_write;
	unsigned char command;
	unsigned int size;
	unsigned char *data;
  };
 public:
  i2c(void);
  i2c(const char *device, char address);
  int Setup(void);
  int Setup(const char *device, char address);

  int Write(const int data, int length = 1);
  int Write(const void *data, int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int WriteReg(unsigned char reg, const void *data, int length);

  int Read(void *data, const int length = 1);
  /*"length" does not include the number of bytes in the "reg".*/
  int ReadReg(unsigned char reg, void *data, const int length, bool repeated = false);

  int Close(void);
};
#endif
