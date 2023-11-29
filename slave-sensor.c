#include <16f877a.h>
#device ADC=10
#use delay(clock = 20000000)
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT,NOLVP, NOCPD, NOWRT
#use RS232(baud=56000, parity = N,xmit=PIN_C6,rcv=pin_c7)
#include <stdlib.h>

#byte SSPBUF = 0x13     //Register address for 16F877

unsigned int16 s1;                 
unsigned int16 s2;
unsigned int16 s3;
unsigned int16 s4;              
unsigned int16 s5;          
unsigned int16 s6;   
unsigned int16 s7;         
float s1_Calib;                              
float s2_Calib;
float s3_Calib;      
float s4_Calib; 
float s5_Calib;         
float s6_Calib; 
float s7_Calib;
int8 e2;   
const unsigned int16 s1_Max = 773;
const unsigned int16 s2_Max = 779;         
const unsigned int16 s3_Max = 827;       
const unsigned int16 s4_Max = 802;
const unsigned int16 s5_Max = 817;
const unsigned int16 s6_Max = 866;
const unsigned int16 s7_Max = 899;     
const unsigned int16 s1_Min = 45;
const unsigned int16 s2_Min = 45;
const unsigned int16 s3_Min = 45;
const unsigned int16 s4_Min = 45;
const unsigned int16 s5_Min = 45;
const unsigned int16 s6_Min = 45;
const unsigned int16 s7_Min = 45;
const unsigned int16 s_Calib_Max = 900;
const unsigned int16 s_Calib_Min = 60;    

void readSensor()    //Read Sensor
{               
   set_ADC_channel(0);   
   delay_us(5);
   s1 = read_ADC();  
   
   set_ADC_channel(1);                    
   delay_us(5);
   s2 = read_ADC();
   
   set_ADC_channel(2);
   delay_us(5);
   s3 = read_ADC();
   
   set_ADC_channel(3);
   delay_us(5);
   s4 = read_ADC();
   
   set_ADC_channel(5);
   delay_us(5);
   s5 = read_ADC();
   
   set_ADC_channel(6);
   delay_us(5);
   s6 = read_ADC();
   
   set_ADC_channel(7);
   delay_us(5);
   s7 = read_ADC(); 
}
      

void calibSensor() //Calibrate sensors
{   
   s1_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s1*1.0 - s1_Min))/(s1_Max - s1_Min);
   s2_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s2*1.0 - s2_Min))/(s2_Max - s2_Min);
   s3_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s3*1.0 - s3_Min))/(s3_Max - s3_Min);
   s4_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s4*1.0 - s4_Min))/(s4_Max - s4_Min);
   s5_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s5*1.0 - s5_Min))/(s5_Max - s5_Min);
   s6_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s6*1.0 - s6_Min))/(s6_Max - s6_Min);
   s7_Calib = s_Calib_Min + ((s_Calib_Max - s_Calib_Min)*(s7*1.0 - s7_Min))/(s7_Max - s7_Min);
}

void e2_Calculation() //return actual tracking error e2
{
   float e2Cal = 0;    
   //e2_Cal = (s1_Calib*(-45)+s2_Calib*(-30)+s3_Calib*(-15)+s4_Calib*0+s5_Calib*15+s6_Calib*30+s7_Calib*45)/(s1_Calib+s2_Calib+s3_Calib+s4_Calib+s5_Calib+s6_Calib+s7_Calib);
   e2Cal = (s1_Calib*(-45)+s2_Calib*(-30)+s3_Calib*(-15)+s4_Calib*0+s5_Calib*15+s6_Calib*30+s7_Calib*45)/(s1_Calib+s2_Calib+s3_Calib+s4_Calib+s5_Calib+s6_Calib+s7_Calib);
   e2 =(int8) ((e2Cal - 0.699)/0.8038);  
}


int value;


void setup()
{  
   //Setup spi
   setup_spi(SPI_SLAVE|SPI_L_TO_H);
   //Setup ADC
   setup_ADC(ADC_CLOCK_INTERNAL);         
   setup_ADC_ports(ALL_ANALOG);
   //Enable interrupts for slave.
   clear_interrupt(INT_SSP);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   delay_ms(1000);

}
#int_ssp
void ssp_isr() 
{
    if(spi_data_is_in()) 
    {    //write collision test typically not performed here\
          readSensor();   
         calibSensor();
         e2_Calculation();
        spi_write(e2);         //perform a SPI writing
    }                         
    clear_interrupt(INT_SSP);
}
//-----------------------------------------------------------------------------

//===============================================================
void main() 
{
    setup();

    while(1) 
    {
    }
}

