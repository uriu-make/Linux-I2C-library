install:
	gcc -shared -fPIC src/i2clib.cpp -o src/libi2clib.so
	sudo cp src/i2clib.h /usr/include/i2clib.h
	sudo cp src/libi2clib.so /usr/lib/libi2clib.so

st7032:
	gcc example/st7032.cpp -o example/st7032.o -li2clib

bme280:
	gcc example/bme280.cpp -o example/bme280.o -li2clib

all: install st7032 bme280
examples: st7032 bme280