// File Name: ultrasonic interface on 8051
// Author: R.venkatesan
// lcd 16X2 8bit method 
 // lcd pin connection port0 -data pin (D0-D7)
 // control pin lcd conection  RS=P2^7, R/W=P2^6, E=P2^5
 

# include <reg51.h>
# include <stdio.h>
#include <intrins.h>
# include "lcd.h"  // lcd library 
/* if want change the lcd pin connection open lcd file library change the pin as per your aspect 
 And also remain you change busy flag= bf(used name in lcdlibrary ) also for inlcd pin data  lcd file libary  */
 
 sfr16 DPTR=0x82;
  sbit echo=P3^2;
	sbit triger=P3^3;
	
	int get;
  void number(int a);
	void basic_step();
	void send_pulse();
	int ultrasonic();
	
 void main()
 {
  port(); // pulldown the all pin in micro controller
	lcd_initial(); // lcd basic initialization cammand line 
	 basic_step(); // ultrasonic basic initialization
	 echo=1; // pull up pin 
	 while(1)
	 {
	   lcd_com(0x01);  // lcd cammand dispaly frist line
		 lcd_string("ultrasonic range"); // write your own word in double quotes
	
		 lcd_com (0xc0);  // lcd cammand dispaly second line
		
		 number(ultrasonic());
		  delay(100); // delay for seen the lcd lines display
	 }
		 
 }
	 void number(int a)
	 {
	   lcd_data(a/10000+0x30);
		 lcd_data((a%10000)/1000+0x30);
		 lcd_data((a%1000)/100+0x30);
		 lcd_data((a%100)/10+0x30);
		 lcd_data(a%10+0x30);
	 }
	  void basic_step()
		{
		TMOD=0x09;   // using frist interrupt0 on ultrasonic  timer mode 1
			/* if wannt use interruprt  TMOD=0x90
			And also use both  interrupt timern TMOD=0x99 */
	TH0=0x00;TL0=0x00;  // enable timer mode start on zero
	TR0=1;
	TH1=0x00;TL1=0x00;
	TR1=1;
		}
		
		void send_pulse() 
{
	 triger=1;
	 _nop_();_nop_();_nop_();_nop_();_nop_();
	 _nop_();_nop_();_nop_();_nop_();_nop_();
	 triger=0;
}
int ultrasonic()
{	
	send_pulse();
	while(!echo);
	while(echo);
	DPH=TH0;
	DPL=TL0;
	TH0=0xff;
	TL0=0xff;
	if(DPTR<36000)
	{
		get=DPTR/58;
	} 		                        // Object distance in inches = (time/148); Distance in centimeters = (Time/58)
	else
	{
		get=0;
	}
	return get;
}