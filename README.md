# Linux-basic-I2C
Basic Linux i2c library for C++.

To insrall library:
```sh
make install
```

To build example/st7032.cpp:
```sh
make st7032
```
To build example/bme280.cpp:
```sh
make bme280
```
--------
## Reference
```
wire::wire(const char *device, char address);
```
Creating an object.

**device** :Name of the I2C to be used (example "/dev/I2C-1").Can be omitted.\
**address** :I2C device address.Can be omitted.\
/dev/i2c-0~/dev/i2c-6 are defined as I2C0~I2C6
```
int wire::Setup(const char *device, char address);
```
Open device.

If defined in the constructor, the argument can be omitted.
```
int wire::Write(const void *data, int length = 1);
```
Data sending.

**data** :Pointer to the data to be sent.\
**length** :Size of the data to be sent (bytes). Default is 1 byte.Can be omitted.
```
int wire::WriteReg(const char reg, const void *data, int length = 1, bool resterted = false);
```
Send data by specifying a register.

**reg** :Register to be accessed\
**data** :Pointer to the data to be sent.\
**length** :Size of the data to be sent (bytes). Default is 1 byte.Can be omitted.\
"length" does not include the number of bytes in the "reg"\
**resterted** :Select whether to perform repeated start condition.  Valid with true.
```
int wire::Read(void *data, const int length = 1);
```
Read out the data.

**data** :A pointer to a variable that stores the data to read.\
**length** :Size of the data to be read (bytes). Default is 1 byte.Can be omitted.
```
int wire::ReadReg(const char reg, void *data, const int length = 1, bool resterted = false);
```
Read data by specifying a register.

**reg** :Register to be accessed\
**data** :A pointer to a variable that stores the data to read.\
**length** :Size of the data to be read (bytes). Default is 1 byte.Can be omitted.\
"length" does not include the number of bytes in the "reg"\
**resterted** :Select whether to perform repeated start condition.  Valid with true.
```
int wire::Close(void);
```
Terminate the use of I2C devices.
