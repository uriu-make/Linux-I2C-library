#include "i2clib.h"

i2c::i2c(const char *device, char address) {
  this->device = device;
  this->address = address;
}

int i2c::Setup(void) {
  fd = open(device, O_RDWR);
  if (ioctl(fd, I2C_SLAVE, address) < 0)
    return -1;
  else
    return fd;
}

int i2c::Write(const void *data, int length) {
  return write(fd, data, length);
}

int i2c::WriteReg(unsigned char reg, const void *data, int length) {
  unsigned char t_data[length + 1];
  unsigned char *d = (unsigned char *)data;
  t_data[0] = reg;
  for (int i = 0; i < length; i++)
    t_data[i + 1] = d[i];
  return write(fd, t_data, length + 1);
}

int i2c::Read(void *data, int length) {
  return read(fd, data, length);
}

int i2c::ReadReg(unsigned char reg, void *data, int length) {
    struct i2c_msg args[2];
    struct i2c_rdwr_ioctl_data msgset;
    args[0].addr = address;
    args[0].flags = 0;
    args[0].len = 1;
    args[0].buf = &reg;
    args[1].addr = address;
    args[1].flags = I2C_M_RD;
    args[1].len = length;
    args[1].buf = (unsigned char *)data;
    msgset.msgs = args;
    msgset.nmsgs = 2;
    return ioctl(fd, I2C_RDWR, &msgset);
}

int i2c::Close(void) {
  return close(fd);
}
