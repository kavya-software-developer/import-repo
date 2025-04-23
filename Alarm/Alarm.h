/*
 * Alarm.h
 *
 *  Created on: 27-Nov-2023
 *      Author: Software3
 */

#ifndef ALARM_H_
#define ALARM_H_

#define OCCL       1
#define UP_OCCL    2
#define AIR   3
#define ACC_AIR 4
#define DOOR_OPEN 5
#define BATTERY_ALM 6
#define TARGET   7
#define MOT_ERR_1  8
#define MOT_ERR_2  9
#define MOT_ERR_3  10
#define MOT_ERR_4  11
/************Notification Macros**************************/
#define KEY_LOCK       1
#define KEY_UNLOCK     2
#define MAINS_PLUGIN   3
#define MAINS_PLUGOUT  4
#define SET_FLOWRATE   5
#define  DOOR		6
#define DOOR_ACC 	7
#define SET_FLOWRATE_1  8
#define SET_TARGET 9

void Alarm_screen(uint8_t *alarm_name);
void Alarm(uint8_t alarm_no);
void Warning_screen(uint8_t *Warning_name);
void Warning_Scr_Erase();
void Notification(uint8_t notif);
void Cursor_info(uint8_t * Info_name);

void warn_tone(uint8_t warn_dura_sec);
#endif /* ALARM_H_ */
