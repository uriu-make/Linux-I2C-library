# Reference
```
i2c::i2c(const char *device, char address);
```
Creating an object.

**device** :Name of the I2C to be used (example "/dev/I2C-1").Can be omitted.\
**address** :I2C device address.Can be omitted.\
/dev/i2c-0~/dev/i2c-6 are defined as I2C0~I2C6
```
int i2c::Setup(const char *device, char address);
```
Open device.

If defined in the constructor, the argument can be omitted.
```
int i2c::Write(const void *data, int length);
```
Data sending.

**data** :Pointer to the data to be sent.\
**length** :Size of the data to be sent (bytes).
```
int i2c::WriteReg(const char reg, const void *data, int length);
```
Send data by specifying a register.

**reg** :Register to be accessed\
**data** :Pointer to the data to be sent.\
**length** :Size of the data to be sent (bytes).\
"length" does not include the number of bytes in the "reg"
```
int i2c::Read(void *data, const int length);
```
Read out the data.

**data** :A pointer to a variable that stores the data to read.\
**length** :Size of the data to be read (bytes).
```
int i2c::ReadReg(const char reg, void *data, const int length, bool repeated = false);
```
Read data by specifying a register.

**reg** :Register to be accessed\
**data** :A pointer to a variable that stores the data to read.\
**length** :Size of the data to be read (bytes).\
"length" does not include the number of bytes in the "reg"\
**repeated** :Select whether to perform repeated start condition.  Valid with true.
```
int i2c::Close(void);
```
Terminate the use of I2C devices.
