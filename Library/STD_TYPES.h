#ifndef STD_TYPES_A_H_
#define STD_TYPES_A_H_

		//8-bits
typedef unsigned char uint8 ; // typedef Old Data type NewName
typedef signed char sint8 ;

		//16-bits
typedef unsigned short int uint16 ;
typedef signed short int sint16 ;

		//32-bits
typedef unsigned long int uint32;
typedef signed long int sint32;

		//floating data type
typedef float f32 ;
typedef double f64 ;
typedef long double f128 ;



#define NULL 			((void*)0)
		//error states
#define OK 				0u
#define NOK 			1u
#define NULL_PTR_ERR 	2u


#endif
