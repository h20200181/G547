#include "adxldrv.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

int ret_val_low, ret_val_high;
u_int8_t low,high;
u_int32_t res_address;
long result;
int ioctl_readData(int file_desc,int index)
{       
	switch(index){

		case 0:ret_val_low =ioctl(file_desc,IOCTL_Accx0, &low);
			
			if(ret_val_low < 0){
				printf("ioctl_accx0 failed: %i\n",ret_val_low);
				exit(-1);
			}
			ret_val_high =ioctl(file_desc,IOCTL_Accx1,&high);
			if(ret_val_high < 0){
				printf("ioctl_accx1 failed: %i\n",ret_val_high);
				exit(-1);
			}
			printf(" X0: %lu,X1: %lu \n ",low,high);
			break;
		case 1:ret_val_low =ioctl(file_desc,IOCTL_Accy0,&low);
			if(ret_val_low < 0){
				printf("ioctl_accy0 failed: %i\n",ret_val_low);
				exit(-1);
			}
			ret_val_high =ioctl(file_desc,IOCTL_Accy1,&high);
			if(ret_val_high < 0){
				printf("ioctl_accy1 failed: %i\n",ret_val_high);
				exit(-1);
			}
			printf(" Y0: %lu,Y1: %lu \n ",low,high);
			break;
		case 2:ret_val_low =ioctl(file_desc,IOCTL_Accz0,&low);
			if(ret_val_low < 0){
				printf("ioctl_accz0 failed: %i\n",ret_val_low);
				exit(-1);
			}
			ret_val_high =ioctl(file_desc,IOCTL_Accz1,&high);
			if(ret_val_high < 0){
				printf("ioctl_accz1 failed: %i\n",ret_val_high);
				exit(-1);
			}
			printf(" Z0: %lu,Z1: %lu \n ",low,high);
			break;
		default: printf("Incorrect index number");
	}
	
	return 0;
}

void ioctl_standby(int file_desc){
	ioctl(file_desc,IOCTL_standby);
}

void ioctl_poweron(int file_desc){
	ioctl(file_desc,IOCTL_poweron);
}
void ioctl_sleep(int file_desc){
	ioctl(file_desc,IOCTL_sleep);
}
void ioctl_BW_high(int file_desc){
	ioctl(file_desc,IOCTL_BW_high);
}
void ioctl_BW_low(int file_desc){
	ioctl(file_desc,IOCTL_BW_low);
}

int main(){

	int file_desc;

	file_desc = open(DEVICE_FILE_NAME,0);
	
	if(file_desc <0 ){
		printf("cant open\n");
		exit(-1);
	}
	
	//Accelerometer readings
	printf("Accelerometer readings\n");
	for(int j = 0; j <2 ;j++){
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	}
	ioctl_standby(file_desc);
	printf("adxl345 on standby\n");
	for(int j = 0; j <5 ;j++){
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	}
	ioctl_poweron(file_desc);
	printf("adxl345 powered on\n");
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	printf("adxl345 bandwidth changed to 3200HZ\n");
	ioctl_BW_high(file_desc);
	for(int j = 0; j <5 ;j++){
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	}
	printf("adxl345 bandwidth changed to 0.2 HZ\n");
	ioctl_BW_low(file_desc);
	for(int j = 0; j <5 ;j++){
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	}
	printf("adxl345 on sleep\n");
	ioctl_sleep(file_desc);
	for(int j = 0; j <2 ;j++){
		for(int i =0;i<3;i++){
			ioctl_readData(file_desc, i);
		}
	}
	close(file_desc);
return 0;
	
	
}

