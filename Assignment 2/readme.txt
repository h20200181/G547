The files partition.c and partition.h are for partitions. 
The files ram_device.c and ram_device.h are for functions to read and write from the memory.
The file ram_block.c is the block device driver. 

1. Insert the kernel module using sudo insmod dof.ko
2. The number of partitions displayed using sudo fdisk -l
3. Change the access permissions for dof in dev using sudo chmod 777 /dev/dof.
4. write into disk using echo 'text' >/dev/dof
5. check the write request data using dmesg.
6. read the disk details using cat /dev/dof