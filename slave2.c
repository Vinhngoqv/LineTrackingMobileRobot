#include <16f877a.h>
#use delay(clock=20000000)
#include <stdlib.h>
#include <stdio.h>
#use RS232(baud=9600, parity = N,xmit=PIN_C6,rcv=pin_c7)
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT,NOLVP, NOCPD, NOWRT



int  c = 0;
double d = 0;
signed int8 i = 0;
int32 xung = 0, tongxung = 0;
float tocdo = 0;

#byte SSPCON = 0x34  
#bit SSPM0 = SSPCON.0
#bit SSPM1 = SSPCON.1
#bit SSPM2 = SSPCON.2
#bit SSPM3 = SSPCON.3
#bit CKP = SSPCON.4
#bit SSPEN = SSPCON.5



float integral = 0, ref = 120 , error = 0, error_last= 0, pwm= 0,
      kp = 0.3, ki = 0, kd = 0,pwwm= 0;
int8 PID(float tocdo, float bv)  //bv la gia tri them vao
{
    error = 50 + bv - tocdo; //50+bv là gia tri ref
    integral = integral + error;
    pwm += kp*error+ki*integral+kd*(error-error_last);
    if (pwm >= 255)
      pwm = 255;
    if (pwm <= 0)
      pwm = 0;      
    error_last = error;
    //printf (" %f \n",pwm);
    return (int8)(pwm*(255)/600);
}


void setup()
{
   setup_ccp1(ccp_pwm);                   //pwm
   setup_timer_2(T2_div_by_1,255,1);      //timer2 20Hz
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_8); // Internal clock and pr89escaler 8
   set_timer1(53035);  // Lay mau 0.02s
   enable_interrupts(global);
   ext_int_edge(H_TO_L);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_SSP);            //ngat spi
   enable_interrupts(INT_TIMER1);
   setup_spi(spi_slave | spi_l_to_h );
   set_tris_d(0x00);
   set_tris_b(0xff);
   set_pwm1_duty(0);
   delay_ms(2000);
}



#INT_EXT
void int_handler(void)
{
   xung = xung + 1;
   tongxung = tongxung + 1;
   clear_interrupt(INT_EXT);
   
}

#INT_TIMER1
void tansolaymau()
{
   set_timer1(53035);
   tocdo = xung*60/(11*0.02*21);
   xung = 0;
   clear_interrupt(INT_TIMER1);
}



#INT_SSP
void nhanspi()
{
   if (spi_data_is_in()==1)
   {
      i = spi_read();
      c = c + 1;
      //printf (" %d  ",e);
   }
}
signed int aa;
void main()
{
   setup();
   //ref = 191;
   while (1)
   {
      //set_pwm1_duty(255);
      printf (" %f \n",tocdo);
      printf (" %d  ",i);
      //printf (" %li  ",tongxung);
      //aa =  PID(tocdo,0);
      //printf (" %d \n",aa);
      //set_pwm1_duty((aa));
      
      if ( i > 25)
      {
         aa =  PID(tocdo,-19);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
      }
      else if ( i < -25)
      {
         aa =  PID(tocdo,19);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
      }
      
      
      else if ( i >= 20)
      {
         aa =  PID(tocdo,-17);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
      }
      else if ( i <= -20)
      {
         aa =  PID(tocdo,17);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
      }      
      else if ( (i)<=(-10))
      {  
         aa =  PID(tocdo,5);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
         //printf (" %f \n",tocdo);
         //printf (" 1  ",);
      }
      else if ( i>=10)
      {
         aa =  PID(tocdo,-5);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
         //printf (" %f \n",tocdo);
         //printf (" 2  ",);
      }
      else if ( (i<10) && (i > -10) )
      {
         aa =  PID(tocdo,0);
         //printf (" %li \n",aa);
         set_pwm1_duty((aa));
         //printf (" %f \n",tocdo);
         //printf (" 3  ",);
      }
      
      
     
      
      //delay_ms(100);
      if( c%2 == 1 )//nháy den cho vui
      {
         output_low (pin_d0);
        //set_pwm1_duty((e));
        //printf (" %f ",tocdo);
      }
      else 
      {
      output_high (pin_d0);
      //set_pwm1_duty(512);
      }
    
    
   }
}
