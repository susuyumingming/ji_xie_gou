//////////////////////////////////////////////////////////////////////////////////	 
/////////////////δ////////////////////////κ////
////////////////STM32F407ZGT6,///////Explorer STM32F4//////,///168MHZ//////12MHZ
//QDtech-TFT/////// for STM32 IO///
//xiao//@ShenZhen QDtech co.,LTD
////////:www.qdtft.com
//////////http://qdtech.taobao.com
//wiki/////////http://www.lcdwiki.com
////////////////κμ/////////////////
/////(////) :+86 0755-23594567 
/////:15989313508////// 
//////:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
/////////QQ:3002773612  3002778157
//////////QQ/:324828016
//////////:2018/08/09
///汾//V1.0
////////У/////////
//Copyright(C) ////////////////////// 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================///////================================================//
//     LCD///                STM32/////
//      VCC          //        DC5V/3.3V      /////
//      GND          //          GND          ///////
//=======================================//////////////==========================================//
///////////////////////SPI////
//     LCD///                STM32/////    
//    SDI(MOSI)      //          PB5          ///////SPI////////д///
//    SDO(MISO)      //          PB4          ///////SPI//////////////////////////////////
//=======================================//////////////==========================================//
//     LCD/// 					      STM32///// 
//       LED         //          PB13         /////////////////////////////////5V//3.3V
//       SCK         //          PB3          ///////SPI//////////
//      DC/RS        //          PB14         //////////////////////
//       RST         //          PB12         /////////λ///////
//       CS          //          PB15         ////////////////
//=========================================////////////=========================================//
///////鲻///////////////д/////////////////////////////////д/////////
//	   LCD///                STM32///// 
//      T_IRQ        //          PB1          /////////////ж////
//      T_DO         //          PB2          ////////SPI////////
//      T_DIN        //          PF11         ////////SPI////д///
//      T_CS         //          PC5          /////////////////
//      T_CLK        //          PB0          ////////SPI//////////
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "gui.h"	    

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//////touchtype=0//////.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

//---------------------------///SPI2//////---------------------------
/*****************************************************************************
 * @name       :void SPI2_Init(void)
 * @date       :2024-05-20 
 * @function   ://///SPI2//////////SPI//////
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void SPI2_Init(void)
{

}

/*****************************************************************************
 * @name       :u8 SPI2_ReadWriteByte(u8 txData)
 * @date       :2024-05-20 
 * @function   :///SPI2/////////1//////////////
 * @parameters :txData: //////////
 * @retvalue   ://///////
******************************************************************************/  
u8 SPI2_ReadWriteByte(u8 txData)
{
  uint8_t rx_data;
  HAL_SPI_TransmitReceive(&hspi2, &txData, &rx_data, 1, 100);  
  return rx_data;
}

///*****************************************************************************
// * @name       :void TP_Write_Byte(u8 num)   
// * @date       :2018-08-09 
// * @function   :Write a byte data to the touch screen IC with SPI bus
// * @parameters :num:Data to be written
// * @retvalue   :None
//******************************************************************************/  	 			    					   
//void TP_Write_Byte(u8 num)    
//{  
//	u8 count=0;   
//	for(count=0;count<8;count++)  
//	{ 	  
//		if(num&0x80)TDIN=1;  
//		else TDIN=0;   
//		num<<=1;    
//		TCLK=0; 	 
//		delay_us(1);
//		TCLK=1;		//////////Ч	        
//	}		 			    
//}


/*****************************************************************************
 * @name       :u16 TP_Read_AD(u8 CMD)	  
 * @date       :2018-08-09 
 * @function   :Reading adc values from touch screen IC with SPI bus
 * @parameters :CMD:Read command,0xD0 for x,0x90 for y
 * @retvalue   :Read data
******************************************************************************/    
//u16 TP_Read_AD(u8 CMD)	  
//{ 	 
//	u8 count=0; 	  
//	u16 Num=0; 
//	TCLK=0;		/////////// 	 
//	TDIN=0; 	////////////
//	TCS=0; 		////д/////IC
//	TP_Write_Byte(CMD);////////////
//	delay_us(6);//ADS7846///////////6us
//	TCLK=0; 	     	    
//	delay_us(1);    	   
//	TCLK=1;		////1/////////BUSY	    	    
//	delay_us(1);    
//	TCLK=0; 	     	    
//	for(count=0;count<16;count++)//////16λ////,//и/12λ//Ч 
//	{ 				  
//		Num<<=1; 	 
//		TCLK=0;	/////////Ч  	    	   
//		delay_us(1);    
//		TCLK=1;
//		if(DOUT)Num++; 		 
//	}  	
//	Num>>=4;   	////и/12λ//Ч.
//	TCS=1;		///////	 
//	return(Num);  
////#endif
//}
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u16 Num = 0; 
	u8 i, temp;
	
	// 1. //д/////IC////////
	TCS_CLR;        // /////////У/
	delay_us(2);    // //////
	
	// 2. ////////////////SPI//
	SPI2_ReadWriteByte(CMD);
	delay_us(6);    // ///ADC///////ADS7846/////////6us//
	
	// 3. ///16λ////////SPI///////////////////////
	Num = 0;
	for(i = 0; i < 2; i++)  // //2/ζ//8λ////16λ
	{
		temp = SPI2_ReadWriteByte(0xFF);  // //// dummy ///////////1///
		Num = (Num << 8) | temp;          // ///16λ////
	}
	
	// 4. /////////////////12λ//Ч///////////4λ//
	Num >>= 4;
	
	// 5. /////
	TCS_SET;
	return Num;
}


#define READ_TIMES 5 	/////////
#define LOST_VAL 1	  	///////
/*****************************************************************************
 * @name       :u16 TP_Read_XOY(u8 xy)  
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates (x or y),
								Read the READ_TIMES secondary data in succession 
								and sort the data in ascending order,
								Then remove the lowest and highest number of LOST_VAL 
								and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/  
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//////
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//////////
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

/*****************************************************************************
 * @name       :u8 TP_Read_XY(u16 *x,u16 *y)
 * @date       :2018-08-09 
 * @function   :Read touch screen x and y coordinates,
								The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;/////////
	*x=xtemp;
	*y=ytemp;
	return 1;/////////
}

#define ERR_RANGE 50 /////Χ 
/*****************************************************************************
 * @name       :u8 TP_Read_XY2(u16 *x,u16 *y) 
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates twice in a row, 
								and the deviation of these two times can not exceed ERR_RANGE, 
								satisfy the condition, then think the reading is correct, 
								otherwise the reading is wrong.
								This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))////////β/////+-50//
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
				if(*y==2047)
				{*y=0;}
				if(*x!=0)
				{*x=330/0.17-*x;}
        return 1;
    }else return 0;	  
} 

/*****************************************************************************
 * @name       :void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :Draw a touch point,Used to calibrate							
 * @parameters :x:Read x coordinate of the touch screen
								y:Read y coordinate of the touch screen
								color:the color value of the touch point
 * @retvalue   :None
******************************************************************************/  
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//////
	LCD_DrawLine(x,y-12,x,y+13);//////
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	gui_circle(x,y,POINT_COLOR,6,0);/////////
}	

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :Draw a big point(2*2)					
 * @parameters :x:Read x coordinate of the point
								y:Read y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
******************************************************************************/   
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);/////// 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}	

/*****************************************************************************
 * @name       :u8 TP_Scan(u8 tp)
 * @date       :2018-08-09 
 * @function   :Scanning touch event				
 * @parameters :tp:0-screen coordinate 
									 1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
								0-no touch
								1-touch
******************************************************************************/  					  
u8 TP_Scan(u8 tp)
{			   
	if(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_7) == 1)//有按键按下
	{
		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//读取屏幕坐标
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x0=tp_dev.x;//记录第一次按下时的坐标
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开	
		}else//之前就没有被按下
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}
	  
//////////////////////////////////////////////////////////////////////////	 
////////EEPROM/////////////,///13/////(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Save calibration parameters		
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 										    
void TP_Save_Adjdata(void)
{
	// u32 temp;			 
	// //////У/////!		   							  
	// temp=tp_dev.xfac*100000000;//////xУ//////      
    // AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	// temp=tp_dev.yfac*100000000;//////yУ//////    
    // AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	// //////x/////
    // AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);		    
	// //////y/////
	// AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
	// /////津//////
	// AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	// temp=0X0A;/////У/////
	// AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Gets the calibration values stored in the EEPROM		
 * @parameters :None
 * @retvalue   :1-get the calibration values successfully
								0-get the calibration values unsuccessfully and Need to recalibrate
******************************************************************************/ 	
u8 TP_Get_Adjdata(void)
{					  
	// u32 tempfac;
	// tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//////////,/////У///// 		 
	// if(tempfac==0X0A)///////////У/////			   
	// {    												 
	// 	tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
	// 	tp_dev.xfac=(float)tempfac/100000000;/////xУ/////
	// 	tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
	// 	tp_dev.yfac=(float)tempfac/100000000;/////yУ/////
	//     /////x/////
	// 	tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	//     /////y/////
	// 	tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
 	// 	tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);///////////////
	// 	if(tp_dev.touchtype)//X,Y///////////
	// 	{
	// 		CMD_RDX=0X90;
	// 		CMD_RDY=0XD0;	 
	// 	}else				   //X,Y////////////
	// 	{
	// 		CMD_RDX=0XD0;
	// 		CMD_RDY=0X90;	 
	// 	}		 
	// 	return 1;	 
	// }
	return 0;
}	
 
//////////
const u8* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/*****************************************************************************
 * @name       :void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
 * @date       :2018-08-09 
 * @function   :Display calibration results	
 * @parameters :x0:the x coordinates of first calibration point
								y0:the y coordinates of first calibration point
								x1:the x coordinates of second calibration point
								y1:the y coordinates of second calibration point
								x2:the x coordinates of third calibration point
								y2:the y coordinates of third calibration point
								x3:the x coordinates of fourth calibration point
								y3:the y coordinates of fourth calibration point
								fac:calibration factor 
 * @retvalue   :None
******************************************************************************/ 	 					  
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,160,16,"x1:",1);
 	LCD_ShowString(40+80,160,16,"y1:",1);
 	LCD_ShowString(40,180,16,"x2:",1);
 	LCD_ShowString(40+80,180, 16,"y2:",1);
	LCD_ShowString(40,200, 16,"x3:",1);
 	LCD_ShowString(40+80,200, 16,"y3:",1);
	LCD_ShowString(40,220, 16,"x4:",1);
 	LCD_ShowString(40+80,220, 16,"y4:",1);  
 	LCD_ShowString(40,240, 16,"fac is:",1);     
	LCD_ShowNum(40+24,160,x0,4,16);		////////
	LCD_ShowNum(40+24+80,160,y0,4,16);	////////
	LCD_ShowNum(40+24,180,x1,4,16);		////////
	LCD_ShowNum(40+24+80,180,y1,4,16);	////////
	LCD_ShowNum(40+24,200,x2,4,16);		////////
	LCD_ShowNum(40+24+80,200,y2,4,16);	////////
	LCD_ShowNum(40+24,220,x3,4,16);		////////
	LCD_ShowNum(40+24+80,220,y3,4,16);	////////
 	LCD_ShowNum(40+56,lcddev.width,fac,3,16); 	////////,///////////95~105//Χ///.
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Calibration touch screen and Get 4 calibration parameters
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 		 
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];/////////
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
	u16 outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//////   
	POINT_COLOR=RED;///// 
	LCD_Clear(WHITE);////// 	   
	POINT_COLOR=BLACK;
	LCD_ShowString(10,40,16,"Please use the stylus click the",1);///////////
	LCD_ShowString(10,56,16,"cross on the screen.The cross will",1);///////////
	LCD_ShowString(10,72,16,"always move until the screen ",1);///////////
	LCD_ShowString(10,88,16,"adjustment is completed.",1);///////////

	TP_Drow_Touch_Point(20,20,RED);//////1 
	tp_dev.sta=0;///////////// 
	tp_dev.xfac=0;//xfac//////////У///,////У//////////!///////	 
	while(1)/////////10//////а///,////////
	{
		tp_dev.scan(1);/////////////
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)///////////////(////////////.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);/////////////////////.
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				///////1 
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//////2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	///////2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//////3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			///////3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//////4
					break;
				case 4:	 ////////////////
	    		    ////////
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);/////1,2/////
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);/////3,4/////
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)///////
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	///////4
   	 					TP_Drow_Touch_Point(20,20,RED);								//////1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);/////////   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);/////1,3/////
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);/////2,4/////
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)///////
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	///////4
   	 					TP_Drow_Touch_Point(20,20,RED);								//////1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);/////////   
						continue;
					}///////
								   
					//////////
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);/////1,4/////
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);/////2,3/////
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)///////
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	///////4
   	 					TP_Drow_Touch_Point(20,20,RED);								//////1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);/////////   
						continue;
					}///////
					////////
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);/////xfac		 
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;/////xoff
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);/////yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;/////yoff  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)////////////////.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	///////4
   	 					TP_Drow_Touch_Point(20,20,RED);								//////1
						LCD_ShowString(40,26, 16,"TP Need readjust!",1);
						tp_dev.touchtype=!tp_dev.touchtype;////////////.
						if(tp_dev.touchtype)//X,Y///////////
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y////////////
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//////
					LCD_ShowString(35,110, 16,"Touch Screen Adjust OK!",1);//У/////
					delay_ms(1000);
					TP_Save_Adjdata();  
 					LCD_Clear(WHITE);//////   
					return;//У/////				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}		

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
u8 TP_Init(void)
{			    		   
	// GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	
	// // //////////GPIOB/C/F + SPI2//
	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOF, ENABLE);
	// SPI2_Init();  // ////////SPI2
	
	// // ////////ж/////T_IRQ//PB1////////////
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// // //////T_CS//PC5///////////
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	// GPIO_Init(GPIOA, &GPIO_InitStructure);
	TCS_SET;  // ////////
	
	// /////24CXX///////
//	AT24CXX_Init();
//	if(TP_Get_Adjdata())return 0; // //У/
//	else
//	{ 										    
//		LCD_Clear(WHITE);
//	    TP_Adjust();  // У///////
//		TP_Save_Adjdata();	 
//	}			
//	TP_Get_Adjdata();	
	return 1; 									 
}

u8 touch_is_pressed(void)
{
	if(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_7) == 1)
	{  
		return 1;
	}
	return 0;
}
