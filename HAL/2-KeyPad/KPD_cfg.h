/**************************************************************/
/**************************************************************/
/*********		Author: Mahmoud Arafat		*******************/
/*********		File: 	KPD_cfg.h			*******************/
/*********		Version: 1.00				*******************/
/**************************************************************/
/**************************************************************/

#ifndef KPD_CFG_H_
#define KPD_CFG_H_

#define KPD_u8COL_NUM	4u
#define KPD_u8ROW_NUM	4u

/*rows pins must be input*/
/*the initial value rows pins is pulled up*/

#define KPD_u8RowPORT	DIO_u8PORTC
#define KPD_u8ROW1_PIN	DIO_u8PIN0
#define KPD_u8ROW2_PIN	DIO_u8PIN1
#define KPD_u8ROW3_PIN	DIO_u8PIN2
#define KPD_u8ROW4_PIN	DIO_u8PIN3


/*columns pins must be output*/
/*the initial value columns pins is Output high up*/

#define KPD_u8Col_PORT	DIO_u8PORTC
#define KPD_u8COL1_PIN	DIO_u8PIN4
#define KPD_u8COL2_PIN	DIO_u8PIN5
#define KPD_u8COL3_PIN	DIO_u8PIN6
#define KPD_u8COL4_PIN	DIO_u8PIN7


#define KPD_u8BUTTON_ARR	{{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}}

#define KPD_u8NO_PRESSED_KEY_VAL	0xff


#endif
