#ifndef ADXLDRV_H
#define ADXLDRV_H
#include<linux/ioctl.h>

#define MAJOR_NUM 340

#define IOCTL_Accx0 _IOR(MAJOR_NUM,0,u_int8_t *)
#define IOCTL_Accx1 _IOR(MAJOR_NUM,1,u_int8_t *)
#define IOCTL_Accy0 _IOR(MAJOR_NUM,2,u_int8_t *)

#define IOCTL_Accy1 _IOR(MAJOR_NUM,3,u_int8_t *)
#define IOCTL_Accz0 _IOR(MAJOR_NUM,4,u_int8_t *)
#define IOCTL_Accz1 _IOR(MAJOR_NUM,5,u_int8_t *)
#define IOCTL_standby _IO(MAJOR_NUM,6)
#define IOCTL_poweron _IO(MAJOR_NUM,7)
#define IOCTL_sleep _IO(MAJOR_NUM,8)
#define IOCTL_BW_high _IO(MAJOR_NUM,9)
#define IOCTL_BW_low _IO(MAJOR_NUM,10)

//Register Address for ADXL345
#define REG_DEVID 0x00
#define REG_DATA_FORMAT 0x31
#define REG_INT_ENABLE 0x2E
#define REG_POWER_CTL 0x2D
#define REG_INT_MAP 0x2F
#define REG_INT_SOURCE 0x30
#define REG_DATAX0 0x32
#define REG_DATAX1 0x33
#define REG_DATAY0 0x34
#define REG_DATAY1 0x35
#define REG_DATAZ0 0x36
#define REG_DATAZ1 0x37

#define DEVICE_FILE_NAME "/dev/adxl345"

#endif

