#include "chardev.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int ret_val;
u_int32_t res;
long result;
int ioctl_MPU9255_accelerometer(int file_desc,int index)
{
	switch(index){
		case 0:ret_val =ioctl(file_desc,IOCTL_Accx,Reg_Add_Acc_X);
			if(ret_val < 0){
				printf("ioctl_accx failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Accelerometer x axis readings: %i\n",res);
			break;
		case 1:ret_val =ioctl(file_desc,IOCTL_Accy,Reg_Add_Acc_Y);
			if(ret_val < 0){
				printf("ioctl_accy failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Accelerometer y axis readings: %i\n",res);
			break;
		case 2:ret_val =ioctl(file_desc,IOCTL_Accz,Reg_Add_Acc_Z);
			
			if(ret_val < 0){
				printf("ioctl_accz failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Accelerometer z axis readings: %i\n",res);
			break;
		default: printf("Incorrect index number");
	}
	return 0;
}
int ioctl_MPU9255_gyroscope(int file_desc,int index)
{
	switch(index){
		case 0:ret_val =ioctl(file_desc,IOCTL_Gyrx,Reg_Add_Gyr_X);
			if(ret_val < 0){
				printf("ioctl_gyrx failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Gyroscope x axis readings: %i\n",res);
			break;
		case 1:ret_val =ioctl(file_desc,IOCTL_Gyry,Reg_Add_Gyr_Y);
			if(ret_val < 0){
				printf("ioctl_gyry failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Gyroscope y axis readings: %i\n",res);
			break;
		case 2:ret_val =ioctl(file_desc,IOCTL_Gyrz,Reg_Add_Gyr_Z);
			if(ret_val < 0){
				printf("ioctl_gyrz failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Gyroscope z axis readings: %i\n",res);
			break;
		default: printf("Incorrect index number");
	}
	return 0;
}
int ioctl_MPU9255_magnetometer(int file_desc,int index)
{
	switch(index){
		case 0:ret_val =ioctl(file_desc,IOCTL_Magx,Reg_Add_Mag_X);
			if(ret_val < 0){
				printf("ioctl_magx failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Magnetometer x axis readings: %i\n",res);
			break;
		case 1:ret_val =ioctl(file_desc,IOCTL_Magy,Reg_Add_Mag_Y);
			if(ret_val < 0){
				printf("ioctl_magy failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Magnetometer y axis readings: %i\n",res);
			break;
		case 2:ret_val =ioctl(file_desc,IOCTL_Magz,Reg_Add_Mag_Z);
			if(ret_val < 0){
				printf("ioctl_magz failed: %i\n",ret_val);
				exit(-1);
			}
			ret_val = read(file_desc,&res,2); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
			printf("Magnetometer z axis readings: %i\n",res);
			break;
		default: printf("Incorrect index number");
	}
	return 0;
}

int ioctl_BMP280(int file_desc)
{
	int ret_val;
	u_int32_t res;
	ret_val =ioctl(file_desc,IOCTL_Bar,Reg_Add_Bar_Pressure);
	if(ret_val < 0){
		printf("ioctl_accx failed: %i\n",ret_val);
		exit(-1);
	}
	ret_val = read(file_desc,&res,3); 
			if(ret_val > 0){
				printf("Not able to read %i bytes\n",ret_val);
			}
	printf("Presure readings: %i\n",res);
	return 0;
}

int main(){

	int file_desc;

	file_desc = open(DEVICE_FILE_NAME,0);
	
	if(file_desc <0 ){
		printf("cant open\n");
		exit(-1);
	}
	
	//Accelerometer readings
	for(int i =0;i<3;i++){
		result =ioctl_MPU9255_accelerometer(file_desc,i);
	}
	//Gyroscope readings
	for(int i =0;i<3;i++){
		result =ioctl_MPU9255_gyroscope(file_desc,i);
	}
	//Magnetometer readings
	for(int i =0;i<3;i++){
		result =ioctl_MPU9255_magnetometer(file_desc,i);
	}
	//Barometer readings
	result = ioctl_BMP280(file_desc);
	return 0;
	
	close(file_desc);

}
