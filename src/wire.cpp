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

int wire::Write(const void *data, int length) {
  return write(fd, data, length);
}

int wire::WriteReg(const char reg, const void *data, int length) {
  struct i2c_smbus_ioctl_data args;
  args.read_write = I2C_SMBUS_WRITE;
  args.command = reg;
  args.size = length + 1;
  args.data = (void *)data;
  return ioctl(fd, I2C_SMBUS, &args);
}

int wire::Read(void *data, int length) {
  return read(fd, data, length);
}

int wire::ReadReg(char reg, void *data, int length) {
  struct i2c_smbus_ioctl_data args;
  args.read_write = I2C_SMBUS_READ;
  args.command = reg;
  args.size = length + 1;
  args.data = data;
  return ioctl(fd, I2C_SMBUS, &args);
}

int wire::Close(void) {
  return close(fd);
}
