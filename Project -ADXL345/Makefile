obj-m+=adxldriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules
	gcc -o ioctlusr adxluserapp.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean
