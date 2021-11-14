obj-m := dof.o
dof-y := ram_block.o ram_device.o partition.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


