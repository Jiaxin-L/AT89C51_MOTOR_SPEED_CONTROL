/*******************************************************************
	************************************************************
	*
	*@file     bsp_lcd1602.c
	*@author   Bin
	*@data     2017.05
	*@brief    LCD1602的驱动文件
	*
	*************************************************************
*********************************************************************/

#include "bsp_lcd1602.h"

sbit RS=P2^0; //LCD片选信号引脚位定义
sbit RW=P2^1; //LCD读写信号引脚位定义
sbit EN=P2^2; //LCD使能端引脚位定义

/****************************************************************************************
	*@brief		软件延时函数，单位为ms
	*@param		形参ms用于设置延时的长短
	*@retvel	无
*****************************************************************************************/
void delayms(uint ms)
{
	uchar i; 
	while(ms--)
	{
		for(i=0;i<120;i++); 
	}
}

/****************************************************************************************
	*@brief		LCD忙检测函数
	*@param		无
	*@retvel	返回LCD的状态值
*****************************************************************************************/
uchar busy_check()
{
	uchar status; 
	RS=0;						//片选信号，低电平表示选择的是指令寄存器
	RW=1;						//读写信号，高电平表示进行读操作
	EN=1; 					//使能信号，高电平使能
	delayms(1); 
	status=P0; 			//读取P0口的状态，忙状态下，位BF为1，空闲状态，位BF为0，(位BF在8位寄存器的位7位)
	EN=0; 
	return status; 	//返回状态值
}

/****************************************************************************************
	*@brief		LCD写命令函数
	*@param		传入的命令
	*@retvel	无
*****************************************************************************************/
void write_command(uchar cmd)
{
	while((busy_check()&0x80)==0x80); 	//检测LCD是否处于空闲状态
	RS=0; 															//片选信号，低电平选择指令寄存器
	RW=0; 															//读写信号，低电平表示进行写操作
	EN=0; 															//先对LCD不使能
	P0=cmd; 														//送入要写入的命令
	EN=1;																//对LCD使能
	delayms(1); 												//简单延时，让LCD读取命令
	EN=0;																//完成一次命令的写操作，关闭使能
}

/****************************************************************************************
	*@brief		LCD写数据函数
	*@param		需要写入的数据
	*@retvel	无
*****************************************************************************************/
void write_data(uchar dat)
{
	while((busy_check()&0x80)==0x80);  //检测LCD是否处于空闲状态
	RS=1;                              //片选信号，高电平选择数据寄存器
	RW=0;                              //读写信号，低电平表示进行写操作
	EN=0;                              //先对LCD不使能
	P0=dat;                            //送入要写入的数据
	EN=1 ;                             //对LCD使能
	delayms(1);                        //简单延时，让LCD读取数据
	EN=0;                              //完成一次数据的写操作，关闭使能
}

/****************************************************************************************
	*@brief		LCD初始化函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void lcd_init()
{
	write_command(0x38); 		//开显示
	delayms(1);            
	write_command(0x01);    //清屏
	delayms(1);             
	write_command(0x06);    //写一个字符指针加一
	delayms(1); 
	write_command(0x0c);		//开显示不显示光标
	delayms(1); 
}	

/****************************************************************************************
	*@brief		显示函数，该工程里面用于显示电机转速
	*@param		x表示在第几列开始显示，y表示在第几行开始显示，str是指向字符型的指针
	*@retvel	无
*****************************************************************************************/
void display(uchar x ,uchar y , uchar * str)
{
	uchar i=0;
	if(y==0)
		write_command(0x80|x); 								//y=0，第一行显示
	if(y==1)
		write_command(0xc0|x);								//y=1，第二行显示
	for(i=0;i<16;i++)
	{
			write_data(str[i]); 								//向LCD写入需要显示的数组值
	}
}
