/**************************************************************/
/**************************************************************/
/*********		Author: Mahmoud Arafat		*******************/
/*********		File: 	CLCD_program.c		*******************/
/*********		Version: 1.00				*******************/
/**************************************************************/
/**************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>

#include "DIO_interface.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"


static void voidSetLCDHalfDataPort(uint8 Copy_u8Nipple)
{
	DIO_u8SetPinValue(CLCD_DATA_PORT,CLCD_D4_PIN,GET_BIT(Copy_u8Nipple,0));
	DIO_u8SetPinValue(CLCD_DATA_PORT,CLCD_D5_PIN,GET_BIT(Copy_u8Nipple,1));
	DIO_u8SetPinValue(CLCD_DATA_PORT,CLCD_D6_PIN,GET_BIT(Copy_u8Nipple,2));
	DIO_u8SetPinValue(CLCD_DATA_PORT,CLCD_D7_PIN,GET_BIT(Copy_u8Nipple,3));
}

static void voidSendEnablePulse(void)
{
	/*Send Enable pulse*/
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_HIGH);
	_delay_ms(2);

	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_LOW);
}

void CLCD_voidSendCmd(uint8 Copy_u8Cmd)
{
	/*Set RS pin to low for Command*/
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_LOW);
#if CLCD_RW_CTRL_EN == ENABLED
	/*Set RW pin to low for write operation */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_OP_MODE == EIGHT_BIT_MODE
	/*Send the command on data pins*/
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Cmd);

	voidSendEnablePulse();

#elif CLCD_OP_MODE == FOUR_BIT_MODE
	voidSetLCDHalfDataPort(Copy_u8Cmd >> 4); 	/*send the most 4-bit to the LCD*/
	voidSendEnablePulse();

	voidSetLCDHalfDataPort(Copy_u8Cmd);		/*send the least 4-bit to the LCD*/
	voidSendEnablePulse();
#endif

}
void CLCD_voidSendData(uint8 Copy_u8Data)
{
	/*Set RS pin to high for data */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_HIGH);

#if CLCD_RW_CTRL_EN == ENABLED
	/*Set RW pin to low for write operation */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);
#endif

#if CLCD_OP_MODE == EIGHT_BIT_MODE
	/*Send the command on data pins*/
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Data);

	voidSendEnablePulse();

#elif CLCD_OP_MODE == FOUR_BIT_MODE
	voidSetLCDHalfDataPort(Copy_u8Data>>4); 	/*send the most 4-bit to the LCD*/
	voidSendEnablePulse();

	voidSetLCDHalfDataPort(Copy_u8Data);		/*send the least 4-bit to the LCD*/
	voidSendEnablePulse();
#endif
}
void CLCD_voidInit(void)
{
	/*wait for more than 30ms after power on */
	_delay_ms(40);

	/*Function set command : 2 lines, 5*7 front size */
#if CLCD_OP_MODE == EIGHT_BIT_MODE
	CLCD_voidSendCmd(0b00111000);
	/* 39us delay has been done in SendCmd function in Enable pulse */
#elif CLCD_OP_MODE == FOUR_BIT_MODE
	voidSetLCDHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetLCDHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetLCDHalfDataPort(0b1000);
	voidSendEnablePulse();

#endif
	/*Display on/off control : display enable , cursor and blink disable */
	CLCD_voidSendCmd(0b00001100);
	/* 39us delay has been done in SendCmd function in Enable pulse */

	/*Display Clear*/
	CLCD_voidSendCmd(1);
	/* 1.53ms delay has been done in SendCmd function in Enable pulse */
}
uint8 CLCD_u8SendString(const char* Copy_chString)
{
	uint8 Local_u8ErrorState= OK;
	if(Copy_chString != NULL)
	{
		uint8 Local_u8Counter = 0u;
		while(Copy_chString[Local_u8Counter] != '\0')
		{
			CLCD_voidSendData(Copy_chString[Local_u8Counter]);
			Local_u8Counter++;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}
	return	 Local_u8ErrorState;
}
void CLCD_voidSendNumber(sint32 Copy_s32Number)
{
	char Local_chNumberArr[10];
	uint8 Local_u8RighDigit ;
	uint8 Local_u8Counter=0 ;
	sint8 Local_s8Counter2;

	if(Copy_s32Number == 0)
	{
		CLCD_voidSendData('0');
		return ; /* Mean go out of the function */
	}
	else if(Copy_s32Number <0 )
	{
		/*Number is negative, print -ve sign*/
		CLCD_voidSendData('-');
		Copy_s32Number *= -1 ;
	}
	while(Copy_s32Number != 0 )
	{
		Local_u8RighDigit = Copy_s32Number%10 ; 	/*Get the right most digit*/
		Copy_s32Number /=10;						/*Get rid of  the right most digit*/
		Local_chNumberArr[Local_u8Counter] = Local_u8RighDigit + '0' ; /*convert from decimal value to ascii value*/
		Local_u8Counter++;
	}
	for(Local_s8Counter2=Local_u8Counter-1 ;Local_s8Counter2>=0 ;Local_s8Counter2--)
	{
		CLCD_voidSendData(Local_chNumberArr[Local_s8Counter2]);
	}
}
void CLCD_voidGoToXY(uint8 Copty_u8XPos,uint8 Copty_u8YPos)
{
	uint8 Local_u8DDRamAdd; /*this varaible used to calculate the address in ddram*/
	if(Copty_u8YPos == 0u)
	{
		Local_u8DDRamAdd= Copty_u8XPos;
	}
	else if (Copty_u8YPos== 1u)
	{
		Local_u8DDRamAdd= 0x40 +Copty_u8XPos;
	}
	/*Set bit 7 for Set DDRAM Address command */
	Local_u8DDRamAdd |=1<<7u ;

	CLCD_voidSendCmd(Local_u8DDRamAdd);
}
void CLCD_voidSendSpecialCharacter(uint8 *Copy_pu8Pattern ,uint8 Copy_u8PatternNum,uint8 Copty_u8XPos,uint8 Copty_u8YPos)
{
	 /*calculate starting address */
	uint8 Local_u8CGRAMAdd= Copy_u8PatternNum * 8u;

	uint8 Local_u8LoopCounter ;

	/*Set bit number 6 for Set CGRAM address command*/
	Local_u8CGRAMAdd |=1<<6 ;

	/*Set CGRAM Address*/
	CLCD_voidSendCmd(Local_u8CGRAMAdd);

	/*write the pattern into CGRAM*/
	for(Local_u8LoopCounter=0;Local_u8LoopCounter<8;Local_u8LoopCounter++)
	{
		CLCD_voidSendData(Copy_pu8Pattern[Local_u8LoopCounter]);
	}

	/*Goto DDRAM to Display the pattern*/
	CLCD_voidGoToXY( Copty_u8XPos,  Copty_u8YPos);

	/*Diplay the pattern written inside CGRAM*/
	CLCD_voidSendData(Copy_u8PatternNum);
}
