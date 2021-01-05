install:
	gcc -shared -fPIC src/wire.cpp -o src/libwire.so
	sudo cp src/wire.h /usr/include/wire.h
	sudo cp src/libwire.so /usr/lib/libwire.so

st7032:
	gcc example/st7032.cpp -o example/st7032.o -lwire

bme280:
	gcc example/bme280.cpp -o example/bme280.o -lwire

all: install st7032 bme280