
#include "USER_PROGRAM.H"  
#include "../../LIB/BS83B08A/TIMER.H"    
#include "../../LIB/BS83B08A/EX_INT.H"         
#include "../../LIB/STRUCT.H"
#include "../../LIB/FUN/GPIO.H"
#include "../../LIB/FUN/TOUCH.H"
#include "../../LIB/USER_SETING.H"

struct ST_jaroo  jaroo;
struct ST_touch  touch;

unsigned char i=0;


#define vector_multy_fun0_interupt 0x0C
#define vector_ex_interupt 0x04


#define timer_100us_reload 249  
#define timer_number_load_for_10ms 100


#define drive _pa1
#define drive_port _pac1


int timer_load=0;
void __attribute((interrupt(vector_multy_fun0_interupt)))  timer()
{ 

	_tmr = timer_100us_reload;
	_ton=1;			
					
	if( jaroo.SOFT_SLOW == SOFT_START_TIME ){ jaroo.SOFT_SLOW=0;
		if( jaroo.SOFT < (100-jaroo.POWER) )jaroo.SOFT++;
		else if( jaroo.SOFT > (100-jaroo.POWER) )jaroo.SOFT--; 
	}
	else jaroo.SOFT_SLOW++;

	timer_load++;
	
	if( timer_load < jaroo.SOFT ){
		drive=0;
	}
	else if( timer_load == jaroo.SOFT ) drive=1;
	else { drive=0;
	
		timer_load=0;
		_ton=0;	
	}
}

void __attribute((interrupt(vector_ex_interupt)))  ex_interupt()
{ 

	_tmr = timer_100us_reload;
	_ton=1;	
		
}


void ini_main();



void USER_PROGRAM_INITIAL()
{
	ini_main();	
}



void USER_PROGRAM()
{
										
	GET_KEY_BITMAP();
	
	touch_led_manage();
		
}



void ini_main(){

	jaroo.LEVEL = START_LEVEL;
	
	drive_port=0;
	drive=0;
		
	timer_ini();
	timer_interupt_enable();
	_ton=0;
	
	EX_INT_init();
	 
	GPIO_ini();
	
	jaroo.SOFT = 90;
	
}


/*
void touch_change(unsigned char loc){
	
	unsigned char temp = 0x01<<loc;
    if( touch & temp )touch &= ~ temp;
    else touch |= temp;	
	
}*/
