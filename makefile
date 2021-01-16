install:
	gcc -shared -fPIC src/i2c.cpp -o src/libi2c.so
	sudo cp src/i2c.h /usr/include/i2c.h
	sudo cp src/libi2c.so /usr/lib/libi2c.so

st7032:
	gcc example/st7032.cpp -o example/st7032.o -li2c

bme280:
	gcc example/bme280.cpp -o example/bme280.o -li2c

all: install st7032 bme280