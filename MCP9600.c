/*	TO DO
- Update get temp in float
- Alert pin config

#include "MCP9600.h"

/*	I2C configtion zone	*/
static void I2C_Start()
{
    while(i2c1_driver_isBuferFull());
    i2c1_driver_start();
    Delay_Us(10);
    while(!i2c1_driver_isStart());
}
static void I2C_Stop()
{
    while(i2c1_driver_isBuferFull());
    i2c1_driver_stop();
    while(!i2c1_driver_isStop());
}
static void I2C_Move_Pointer(MCP9600_Reg_Add Addr)
{    //Use before Read
    Delay_Us(Tstr);
    I2C_Start();
    
    i2c1_driver_TXData(MCP9600_Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    Delay_Us(Tstr); 
    
    i2c1_driver_TXData(Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    I2C_Stop();
    Delay_Us(Tstr); 
}
static char I2C_Read_Byte()
{
    I2C_Start();
    //Send Slave addr to Read
    i2c1_driver_TXData((char) MCP9600_Addr|0x01);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    Delay_Us(Tstr);
    
    i2c1_driver_startRX();
    while(!I2C1STATbits.RBF);
    i2c1_driver_sendNACK();
    char x = i2c1_driver_getRXData();
    Delay_Us(Tstr);
    
    I2C_Stop();
    return x;
}
static void I2C_Read_Buffer(unsigned char *buffer, unsigned int numbyte)
{
    int i;
    I2C_Start();
    //Send Slave Addr to Read
    i2c1_driver_TXData((char) MCP9600_Addr|0x01);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    Delay_Us(Tstr);
    
    for(i = numbyte;i>1;i--){
        i2c1_driver_startRX();
        while(!I2C1STATbits.RBF);
        *buffer++ = i2c1_driver_getRXData();
        i2c1_driver_sendACK();
        Delay_Us(Tstr);
    }
    i2c1_driver_startRX();
    while(!I2C1STATbits.RBF);
    *buffer++ = i2c1_driver_getRXData();
    i2c1_driver_sendNACK();
    Delay_Us(Tstr);
    
    I2C_Stop();
}
static void I2C_Write_Byte(MCP9600_Reg_Add Addr, unsigned char data)
{
    I2C_Start();
    
    i2c1_driver_TXData(MCP9600_Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    
    i2c1_driver_TXData(Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    
    i2c1_driver_TXData(data);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    
    I2C_Stop();
}
static void I2C_Write_Buffer(MCP9600_Reg_Add Addr, unsigned char *buff, unsigned int numbyte)
{
    int i;
    I2C_Start();
    
    i2c1_driver_TXData(MCP9600_Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    
    i2c1_driver_TXData(Addr);
    while(i2c1_driver_isBuferFull());
    while(i2c1_driver_isNACK());
    while(I2C1STATbits.TRSTAT);
    
    for(i = 0;i<numbyte;i++){
    	i2c1_driver_TXData(buff[i]);
        while(i2c1_driver_isBuferFull());
        while(i2c1_driver_isNACK());
        while(I2C1STATbits.TRSTAT);
        Delay_Us(Tstr);
    }
    I2C_Stop();
}
/*------------------------------------------------------------------------*/
void MCP9600_Sensor_Cfg(Ther_type Type){
    I2C_Write_Byte(Sensor_Cfg_Reg, Type);
}
void MCP9600_Device_Cfg(unsigned char device_mode){
    I2C_Write_Byte(Device_Cfg_Reg, device_mode);
}
bool MCP9600_Check_Status(unsigned char status_mode){
    I2C_Move_Pointer(Status_Reg);
    char byte = I2C_Read_Byte();
    if((byte&status_mode)==status_mode)
        return 1;
    else
        return 0;
}
void MCP9600_Check_Reg(MCP9600_Reg_Add Addr){
    I2C_Move_Pointer(Addr);
    char byte = I2C_Read_Byte();
    UART2_Write(byte);
}
void MCP9600_Reset_Status(){
    I2C_Move_Pointer(Status_Reg);
    char byte = I2C_Read_Byte();
    Delay_Us(Tstr);
    byte &= 0x3F;
    Delay_Us(Tstr);
    I2C_Write_Byte(Status_Reg, byte);
}
void MCP9600_Read_Temp(MCP9600_Reg_Add Addr, unsigned char status_mode, unsigned char *buff){
    MCP9600_Device_Cfg(Device_Mode);
    while(!MCP9600_Check_Status(status_mode));
    
    I2C_Move_Pointer(Addr);
    I2C_Read_Buffer(buff,2);
    MCP9600_Reset_Status();
}
void MCP9600_Initialize(){
    while(!i2c1_driver_driver_open());
    Delay_Ms(1000);
    
    MCP9600_Sensor_Cfg(TypeK);
    Delay_Ms(500);
    MCP9600_Device_Cfg(Device_Mode);
}
