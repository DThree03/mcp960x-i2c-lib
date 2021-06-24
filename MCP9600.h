/********MCP9600 library Version 1.00********************************************************
 * Library for communicate with only ONE MCP9600 IC in I2c bus.
 * Note: This library using driver I2C1 [Foundation Services Library by Microchip Technology, Inc.]
 * And Delay_Ms function in "Tick_Timer.h"
 * You can use another suitable driver, which have same function.
 * Modify every STATIC FUNCTION in MCP9600.c (which start with i2c1_driver_xxx or I2C1STATbits)
*********************************************************************************************/
#ifndef _MCP9600_H 
#define _MCP9600_H

/* Include your MCU system lib
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#include "../i2c1_driver.h"
#include "../../Tick_Timer.h"
/*****************************************/

#define Tstr 60                 //I2C bus timing delay ms 

#define MCP9600_Addr    0xC0    //Pin ADDR -> GND

#define Status_Burst_Mode   0xC0
#define Status_Nomarl_Mode  0x40

#define Device_Mode 0xAE    //0.25-16bit-16sample Burst mode


typedef enum
{
    T_HotJ = 0x00,
    T_Delta = 0x01,
    T_ColdJ = 0x02,
    Device_Cfg_Reg = 0x06,
    Sensor_Cfg_Reg = 0x05,
    Status_Reg = 0x04,
}MCP9600_Reg_Add;
typedef enum
{
    TypeK = 0x04,
    TypeJ = 0x14,
    TypeT = 0x24,
    TypeN = 0x34,
    TypeS = 0x44,
    TypeE = 0x54,
    TypeB = 0x64,
    TypeR = 0X74,       
}Ther_type; //Use mid filter

static void I2C_Move_Pointer(MCP9600_Reg_Add Addr);
static char I2C_Read_Byte();
static void I2C_Read_Buffer(unsigned char *buffer, unsigned int numbyte);
static void I2C_Write_Byte(MCP9600_Reg_Add Addr, unsigned char data);
static void I2C_Write_Buffer(MCP9600_Reg_Add Addr, unsigned char *buff, unsigned int numbyte);

void MCP9600_Sensor_Cfg(Ther_type Type);
void MCP9600_Device_Cfg(unsigned char device_mode);
bool MCP9600_Check_Status(unsigned char status_mode);
void MCP9600_Check_Reg(MCP9600_Reg_Add Addr);
/********  MCP9600_Read_Temp********
 * Return 2 byte in temp Rigister
 * If you want to show temp value in int format. Follow:
 * char buff[2];
 * MCP9600_Read_Temp(T_HotJ, Status_Burst_Mode,buff);
 * int BufftoTemp(buff);
 * *****************************
 * int BufftoTemp(char *buff){
        int temp;
        if((buff[0]&0x80)==0x80){
            //buff[0]&=0x7F;
            temp = (buff[0]*16+(buff[1]&0xF0)/16) - 4096;
        }
        else{
            temp = (buff[0]*16+(buff[1]&0xF0)/16);
        }
        return temp;
    }
 */
void MCP9600_Read_Temp(MCP9600_Reg_Add Addr, unsigned char status_mode, unsigned char *buff);
void MCP9600_Initialize();
#endif
