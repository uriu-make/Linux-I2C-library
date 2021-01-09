#include "wire.h"

wire::wire(void) {}

wire::wire(const char *device, char address) {
  this->device = device;
  this->address = address;
}

int wire::Setup(void) {
  fd = open(device, O_RDWR);
  if (ioctl(fd, I2C_SLAVE, address) < 0)
    return -1;
  else
    return fd;
}

int wire::Setup(const char *device, char address) {
  this->device = device;
  this->address = address;
  fd = open(device, O_RDWR);
  if (ioctl(fd, I2C_SLAVE, this->address) < 0)
    return -1;
  else
    return fd;
}

int wire::Write(const int data, int length) {
  return write(fd, &data, length);
}

int wire::Write(const void *data, int length) {
  return write(fd, data, length);
}

int wire::WriteReg(unsigned char reg, const void *data, int length, bool repeated) {
  if (repeated == true) {
    struct i2c_msg args[2];
    struct i2c_rdwr_ioctl_data msgset;
    args[0].addr = address;
    args[0].flags = 0;
    args[0].len = 1;
    args[0].buf = &reg;
    args[1].addr = address;
    args[1].flags = 0;
    args[1].len = length;
    args[1].buf = (unsigned char *)data;
    msgset.msgs = args;
    msgset.nmsgs = 2;
    return ioctl(fd, I2C_RDWR, &msgset) >= 0;
  } else {
    struct t_data args;
    args.read_write = 0;
    args.command = reg;
    args.size = length + 1;
    args.data = (unsigned char *)data;
    return ioctl(fd, I2C_SMBUS, &args);
  }
}

int wire::Read(void *data, int length) {
  return read(fd, data, length);
}

int wire::ReadReg(unsigned char reg, void *data, int length, bool repeated) {
  if (repeated == true) {
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
    return ioctl(fd, I2C_RDWR, &msgset) >= 0;
  } else {
    write(fd, &reg, 1);
    return read(fd, data, length);
  }
}

int wire::Close(void) {
  return close(fd);
}
