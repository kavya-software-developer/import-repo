/****************************************************************************
* File Name    : Alarm
* Developed by :
* Version      : V 1.00
* Description  : i)

*****************************************************************************/
#include "stdint.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include <stdio.h>
#include "LPC55S16.h"
#include "Color.h"
#include <FR_CALC.h>
#include "Keypad.h"
#include "Dynamic_Pressure.h"
#include "Shapes.h"
#include "Alarm.h"
#include "Motor.h"
#include "Driver_Timer.h"
#include "Bolus_Header.h"
#include <math.h>


uint8_t G_warn_flag=0;//28-03

extern uint8_t Door_Alm_Flg;
extern uint8_t ON_Flag;
extern volatile uint8_t normal_run;

extern uint8_t G_run_flag;
extern void main_screen_frame(uint16_t flowrate1,uint8_t mode);
extern void syringe_disp_status(uint8_t screen);

extern uint8_t G_KVO_run_flag;
extern uint16_t G_KVO_set_fr;
void Alarm_Running(uint8_t L_R_flag);
void Alarm_war_screen(uint8_t state);
void Notif_screen(uint8_t *Notif_name);
extern uint8_t G_run_count;
void Mains_symbol(uint8_t x_pos_adj,uint8_t y_pos_adj);
void Single_line_fun(void);
extern void lcd_drawline(uint16_t start_xpos, uint16_t start_ypos, uint16_t end_xpos, uint16_t end_ypos, uint32_t Color);

uint8_t G_warn_count=0;
//extern void dynamic_pressure(uint8_t pressure,uint8_t screen);





/****************************************************************************
* Function Name : void Alarm_screen(uint8_t *alarm_name)
* Arguments     : Name of the alarm
* Return type   : void
* Description   : i)   Display the alarm name
*                 ii)  Alarm instruction

*****************************************************************************/
void Alarm_screen(uint8_t *alarm_name)
{
	uint16_t YPOS=0;
	Rectangle(0,0,320,240,ALARM_COLOR);
	TFT_ST7789V__rectangle_curve(10,65-10,60+20,300,0,LIGHT_GRAY);  //Rectangle Curve Option 1
	Rectangle(10,203,301,1,BLACK);

	Font_Select(ARIAL_20);
	YPOS=Letters_Alignments(0,0,alarm_name,2);
	TFT_String(YPOS,80,alarm_name,LIGHT_GRAY,BLACK);
	Frame_Footer(ALARM_COLOR,WHITE,(uint8_t*)"        Confirm Alarm",(uint8_t*)"OK",1);
}
/****************************************************************************
* Function Name : void Error_screen(uint8_t *Error_name)
* Arguments     : Name of the alarm
* Return type   : void
* Description   : i)   Display the Error code
*                 ii)  Alarm instruction

*****************************************************************************/
void Error_screen(uint8_t *Error_name)
{
	uint16_t YPOS=0;
	Rectangle(0,0,320,240,WHITE);
//	TFT_ST7789V__rectangle_curve(80,65-10,60+20,160,0,LIGHT_GRAY);  //Rectangle Curve Option 1
//	Rectangle(10,203,301,1,BLACK);

	Font_Select(ARIAL_20);
	YPOS=Letters_Alignments(0,0,Error_name,2);
	TFT_String(YPOS,80,Error_name,WHITE,BLACK);
	Frame_Footer(LIGHT_GRAY_4,WHITE,(uint8_t*)"Contact BME",(uint8_t*)"",0);
}
void Alarm_string(uint8_t alarm_no)
{
	switch(alarm_no)
		{
		case OCCL:

					Alarm_screen((uint8_t*)"Occlusion Alarm");
					event_no_update(5);

				break;

				case UP_OCCL:


					Alarm_screen((uint8_t*)"Upstream Occlusion Alarm");

				break;

				case AIR:

					Alarm_screen((uint8_t*)"Air Bubble Detected");

				break;
				case ACC_AIR:

					Alarm_screen((uint8_t*)"Accumulated Air Detected");

				break;
				case DOOR_OPEN:
					event_no_update(7);
					Alarm_screen((uint8_t*)"Door Opened");

				break;
				case BATTERY_ALM:

					Alarm_screen((uint8_t*)"Battery Critical");
					break;

				case TARGET:
					if(!G_KVO_run_flag)
						{
						event_no_update(8);
							Alarm_screen((uint8_t*)"Target Reached");
						}
					else
						{
							Alarm_war_screen(0);
						}
					break;
	          case MOT_ERR_1:
	        	  event_no_update(6);
				Error_screen((uint8_t*)"Err.1001"); //Motor unable to run
				break;
			case MOT_ERR_2:
				event_no_update(6);
				Error_screen((uint8_t*)"Err.1005"); //Motor reverse
				break;
			case MOT_ERR_3:
				Error_screen((uint8_t*)"Err.1006"); //Less cuts
				break;
			case MOT_ERR_4:
				Error_screen((uint8_t*)"Err.1007"); //Excess cuts
				break;
				}
}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Alarm(uint8_t alarm_no)
{
	uint8_t alarm_loop=1;


	/*switch(alarm_no)
	{
	case OCCL:

				Alarm_screen((uint8_t*)"Occlusion Alarm");

			break;

			case UP_OCCL:


				Alarm_screen((uint8_t*)"Upstream Occlusion Alarm");

			break;

			case AIR:

				Alarm_screen((uint8_t*)"Air Bubble Detected");

			break;
			case ACC_AIR:

				Alarm_screen((uint8_t*)"Accumulated Air Detected");

			break;
			case DOOR_OPEN:

				Alarm_screen((uint8_t*)"Door Opened");

			break;
			case BATTERY:

				Alarm_screen((uint8_t*)"Battery Critical");
				break;

			case TARGET:
				if(!G_KVO_run_flag)
					{
						Alarm_screen((uint8_t*)"Target Reached");
					}
				else
					{
						Alarm_war_screen(0);
					}
				break;
          case MOT_ERR_1:
			Error_screen((uint8_t*)"Err.1001"); //Motor unable to run
			break;
		case MOT_ERR_2:
			Error_screen((uint8_t*)"Err.1005"); //Motor reverse
			break;
		case MOT_ERR_3:
			Error_screen((uint8_t*)"Err.1006"); //Less cuts
			break;
		case MOT_ERR_4:
			Error_screen((uint8_t*)"Err.1007"); //Excess cuts
			break;
			}*/
	if(!G_KVO_run_flag)        // kvo RUNNING CONCEPT
    {
//		NVIC_DisableIRQ(CTIMER1_IRQn);
//		DRV8825_DISABLE();
		Timer_disable();//29-03
    }
	POW.power_status_flag=1;	//28-03
	while(alarm_loop)
	{

		alarm_sound_tone();
				Power_off();	//26-03
				if(!ON_Flag)
				{
					break;
				}		//26-03

				else if(POW.power_status_flag)
				{
					POW.power_status_flag=0;
					Alarm_string(alarm_no);
					while(KEY.value);//28-03
				}


		if(G_KVO_run_flag)
		{
			Alarm_Running(0);
		}
		if(KEY.value==OK_KEY)
		{
			if(G_KVO_run_flag)
			{
				G_KVO_run_flag=0;
//				NVIC_DisableIRQ(CTIMER1_IRQn);
//				DRV8825_DISABLE();
				Timer_disable();//29-03
			}
			MFB_count_reset(0);
			MFB_count_reset(1);
			MFB_count_reset(2);
			MFB.Motor_struck=0;
			alarm_loop=0;
			buzzer_pwm_stop();
			normal_run=0;
			BOL.run_mode=0;
			POW.power_status_flag=1;
			RAMP.up_flag=0;
			Door_Alm_Flg=1;
			while(KEY.value);
			break;
		}


	}
	 /*main_screen_frame(MOT.flowrate,1);
	 dynamic_pressure(0,1);
	 Rectangle(10,10,40,40,MAIN_FRAME_COL);*/
//	 syringe_disp_status(1);
}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Warning_screen(uint8_t *Warning_name)
{
	uint16_t YPOS=0;

	TFT_ST7789V__rectangle_curve(86,92,62,229,0,WARNING_COLOR);
	Font_Select(ARIAL_12);
	YPOS=Letters_Alignments(86,315-30,Warning_name,0);
	TFT_String(YPOS,97+2,Warning_name,WARNING_COLOR,BLACK);

	TFT_ST7789V_Warning_symbol_18pt(98,280,0,WARNING_COLOR,BLACK);
	TFT_ST7789V_Exclamatory_9pt_arial(98+6,280+9,0,WARNING_COLOR,BLACK);
//	TFT_ST7789V_Triangle_24pt(97-3,275-5,0,WARNING_COLOR,WHITE);


	YPOS=Letters_Alignments(86,315-15,(uint8_t*)"        Mute Alarm",0);
	TFT_String(YPOS,131-2,(uint8_t*)"        Mute Alarm",WARNING_COLOR,BLACK);

	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,14,WARNING_COLOR,BLACK);
	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,12,WARNING_COLOR,LIGHT_GRAY);

	TFT_ST7789V_Mute_Symbol_12pt(138-7,YPOS+15-8,0,LIGHT_GRAY,BLACK);
	Rectangle(91,122,219,0,BLACK);
	G_warn_flag=1;//28-03
	//beep_sound();

}
void warn_tone(uint8_t warn_dura_sec)
{
 if(G_warn_flag)
 {
	 if(!G_warn_count)
	 {
		 WARN_SOUND(3,1,5);
		 G_warn_count=warn_dura_sec;
	 }

 }

}

void Single_line_fun(void)
{
	/*************************mains unplug line*******************************/

uint16_t angle=0;
float theta=0;
uint8_t radius=46;
int  start_x,start_y;

		for(angle=20;angle>0;angle--)
		{
			theta=angle*(3.14/90);
			start_x=cos(theta)*radius;
			start_y=sin(theta)*radius;



			//if (angle>=15 && angle<=16)
			if(angle==15)
			{
			lcd_drawline(90+start_x,135-start_y,90,135,BLACK);
			lcd_drawline(90+start_x,135-start_y,90,135,BLACK);

			}


		}




	/**************************************************************************/
}
void Notification(uint8_t notif)
{
	//beep_sound();
	WARN_SOUND(2,1,2);
	switch(notif)
	{
		case KEY_LOCK:
						Notif_screen((uint8_t *)"Keypad locked");
						TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
						delay_secs(2);
						Warning_Scr_Erase();
						break;
		case KEY_UNLOCK:
						Notif_screen((uint8_t *)"Keypad unlocked");
						TFT_ST7789V_keyunlock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
						delay_secs(2);
						Warning_Scr_Erase();
						break;
		case MAINS_PLUGIN:
		             	Notif_screen((uint8_t *)"Mains Plugin");
		             	Mains_symbol(90,118);

		             	delay_secs(2);
		             	Warning_Scr_Erase();
					    break;
		case MAINS_PLUGOUT:
						Notif_screen((uint8_t *)"Mains Plugout");
						Mains_symbol(90,118);
						 Single_line_fun();
						delay_secs(2);

		case SET_FLOWRATE:
						Notif_screen((uint8_t *)"SET FLOWRATE");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
						TFT_ST7789V_Warning_symbol_18pt(110,100,0,WARNING_COLOR,BLACK);
						TFT_ST7789V_Exclamatory_9pt_arial(110+6,100+9,0,WARNING_COLOR,BLACK);
						delay_secs(2);
						Warning_Scr_Erase();
						break;
		case DOOR:
						Notif_screen((uint8_t *)"DOOR OPEN");
						TFT_ST7789V_Warning_symbol_18pt(110,100,0,WARNING_COLOR,BLACK);
						TFT_ST7789V_Exclamatory_9pt_arial(110+6,100+9,0,WARNING_COLOR,BLACK);
						delay_secs(2);
						Warning_Scr_Erase();

						break;
		case DOOR_ACC:
						//Notif_screen((uint8_t *)"DOOR OPEN");
						TFT_ST7789V__rectangle_curve(50,130,62,229,0,WARNING_COLOR);
						Font_Select(ARIAL_12);
						TFT_String(120,153,(uint8_t* )"DOOR OPENED",WARNING_COLOR,BLACK);
						Rectangle(105,138,1,45,BLACK);
						delay_secs(2);
						//Warning_Scr_Erase();

						break;
						
		case SET_FLOWRATE_1:
						Notif_screen_1((uint8_t *)"SET FLOWRATE");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
						TFT_ST7789V_Warning_symbol_18pt(110,70,0,WARNING_COLOR,BLACK);
						TFT_ST7789V_Exclamatory_9pt_arial(110+6,70+9,0,WARNING_COLOR,BLACK);
						delay_secs(2);
						Warning_Scr_Erase();
						break;
		case SET_TARGET:
						Notif_screen_1((uint8_t *)"SET TARGET");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
						TFT_ST7789V_Warning_symbol_18pt(110,70,0,WARNING_COLOR,BLACK);
						TFT_ST7789V_Exclamatory_9pt_arial(110+6,70+9,0,WARNING_COLOR,BLACK);
						delay_secs(2);
						Warning_Scr_Erase();
						break;						
	}
}
void Notif_screen(uint8_t *Notif_name)
{
	uint16_t YPOS=0;
	TFT_ST7789V__rectangle_curve(86,92,62,229,0,WARNING_COLOR);
	Font_Select(ARIAL_12);
	YPOS=Letters_Alignments(86,315+30,Notif_name,0);
	TFT_String(YPOS,115,Notif_name,WARNING_COLOR,BLACK);
	Rectangle(135,100,1,45,BLACK);
}

void Notif_screen_1(uint8_t *Notif_name)
{
	uint16_t YPOS=0;
	TFT_ST7789V__rectangle_curve(36,92,62,250,0,WARNING_COLOR);
	Font_Select(ARIAL_12);
	YPOS=Letters_Alignments(36,315+30,Notif_name,0);
	TFT_String(YPOS,115,Notif_name,WARNING_COLOR,BLACK);
////
////	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,14,WARNING_COLOR,BLACK);
////	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,12,WARNING_COLOR,LIGHT_GRAY);
//
////	TFT_ST7789V_Mute_Symbol_12pt(138-6,YPOS+15-8,0,LIGHT_GRAY,BLACK);
////	Rectangle(91,122,219,0,BLACK);
//
//
//}
}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Warning_Scr_Erase()
{
	Rectangle(86,92,229,62,MAIN_FRAME_COL);
	Rectangle(120,115,180,0,BLACK); 	//Row Lines
	G_warn_flag=0;//28-03

}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Alarm_Running(uint8_t L_R_flag)
{
	uint16_t Y_pos[2][3]={{30,47,64},{236,253,270}};
	uint32_t colour[2][4][3]={
			{{LIGHT_GRAY,LIGHT_GRAY,BLACK},{LIGHT_GRAY,BLACK,BLACK},
									{BLACK,BLACK,BLACK},{LIGHT_GRAY,LIGHT_GRAY,LIGHT_GRAY}
				},
			{{LIGHT_GRAY,LIGHT_GRAY,LIGHT_GRAY},{BLACK,LIGHT_GRAY,LIGHT_GRAY},
								{BLACK,BLACK,LIGHT_GRAY},{BLACK,BLACK,BLACK}
	}

	};
	uint8_t pos;
	uint32_t col=LIGHT_GRAY;
	uint8_t X_pos[2]={92,92};
	static uint8_t state=0;
	 if(!G_run_count)
	 {
		for (pos = 0; pos <= 2; pos++)
		 {
			TFT_ST7789V_ALARM_ARROW_20pt(X_pos[L_R_flag],Y_pos[L_R_flag][pos],0,col,colour[L_R_flag][state][pos],L_R_flag);
		 }
		 if(state<3)
		 {
			 state++;
		 }
		 else
		 {
			 state=0;
		 }
		 G_run_count=5;

	 }

}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Alarm_war_screen(uint8_t state)
{
	uint16_t war_sym_pos[2]={260,210};
	Rectangle(0,0,320,240,ALARM_COLOR);
	TFT_ST7789V__rectangle_curve(10,40,120,300,0,WARNING_COLOR);
	TFT_ST7789V__rectangle_curve(15,80,40,290,0,LIGHT_GRAY);


	Rectangle(10,193+10,301,1,BLACK);  // Footer Boarder line
//	Rectangle(10,36,301,1,BLACK);      // Footer Boarder line

	TFT_ST7789V_Warning_symbol_18pt(51,war_sym_pos[state],0,WARNING_COLOR,BLACK);
    TFT_ST7789V_Exclamatory_9pt_arial(51+6,war_sym_pos[state]+9,0,WARNING_COLOR,BLACK);

//    TFT_ST7789V_war_symbol(51,270,0,WARNING_COLOR,BLACK);

    TFT_ST7789V_lcd_drawcircle1(60+35,141,14,WARNING_COLOR,BLACK);
	TFT_ST7789V_lcd_drawcircle1(60+35,141,12,WARNING_COLOR,LIGHT_GRAY);

	TFT_ST7789V_Mute_Symbol_12pt(141-7,60+35-8,0,LIGHT_GRAY,BLACK);

	Font_Select(ARIAL_14);
	TFT_String(90+30,130,(uint8_t*)"Mute Alarm ",WARNING_COLOR,BLACK);
	if(!state)
	{

		TFT_String(70,50,(uint8_t*)"Target Reached",WARNING_COLOR,BLACK);
//		Font_Select(ARIAL_12);
		TFT_String(100,90,(uint8_t*)"KVO in progress",LIGHT_GRAY,BLACK);
	}
	else
	{
		TFT_String(95,50,(uint8_t*)"Occlusion ",WARNING_COLOR,BLACK);
		TFT_String(100,90,(uint8_t*)"Anti Bolus",LIGHT_GRAY,BLACK);
	}



	Frame_Footer(ALARM_COLOR,WHITE,(uint8_t*)"        Confirm Alarm",(uint8_t*)"OK",1);
}

void Mains_symbol(uint8_t x_pos_adj,uint8_t y_pos_adj)
{
	TFT_ST7789V_lcd_drawcircle1(x_pos_adj+20,y_pos_adj+5,17,BLACK,BLACK);
	TFT_ST7789V_lcd_drawcircle1(x_pos_adj+20,y_pos_adj+5,14,WARNING_COLOR,WARNING_COLOR);

    TFT_ST7789V_circle1(x_pos_adj+20,y_pos_adj+3,9,BLACK,BLACK);   //left quadrant circle for mains symbol
    TFT_ST7789V_circle3(x_pos_adj+20,y_pos_adj+3,9,BLACK,BLACK);   //right quadrant circle for mains symbol

	Rectangle(x_pos_adj+10,y_pos_adj,20,2,BLACK);            //Bar type line
	Rectangle(x_pos_adj+19,y_pos_adj+10,3,7,BLACK);       //mains symbol line
	Rectangle(x_pos_adj+15,y_pos_adj-5,2,4,BLACK);      //left plugin line
	Rectangle(x_pos_adj+23,y_pos_adj-5,2,4,BLACK);    //right plugin line
	//TFT_ST7789V_thunder_8_pt(y_pos_adj+2,x_pos_adj+17,0,BLACK,WHITE);          //Thunder symbol
}

void Cursor_info(uint8_t * Info_name_name) //28-03
{
	uint16_t XPOS=0;

	TFT_ST7789V__rectangle_curve(45,80,82,229-15,0,WARNING_COLOR);
	Font_Select(ARIAL_12);
	XPOS=Letters_Alignments(60,315-61,Info_name_name,0);
	TFT_String(XPOS,97+2+15,Info_name_name,WARNING_COLOR,BLACK);

	TFT_ST7789V_Warning_symbol_18pt(86+12+12,48,0,WARNING_COLOR,BLACK);
	TFT_ST7789V_Exclamatory_9pt_arial(86+12+6+12,48+9,0,WARNING_COLOR,BLACK);
}

