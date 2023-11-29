#include <16f877a.h>
#use delay(clock = 20000000)
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT,NOLVP, NOCPD, NOWRT
#use RS232(baud=9600, parity = N,xmit=PIN_C6,rcv=pin_c7)
#include <stdlib.h>

#define SPI_SS1 PIN_D1
#define SPI_SS2 PIN_D2
#define SPI_Sensor PIN_D0

/*union convert
{
 int data[4];
 float number;
} val;

void SPI_Write_Float(float gttruyen) 
{
   val.number = gttruyen;
   for (int i=0;i<4;i++)
   {
      SPI_Write(val.data[i]);
   }            
} */      


void setup()
{
   set_tris_b(0xff);
   set_tris_d(0x00);
   setup_spi (spi_master | spi_l_to_h | spi_clk_div_4);
   //clear_interrupt(INT_SSP);   
   //enable_interrupts(INT_SSP); 
   enable_interrupts(global);      //ngat spi
   output_high(pin_d1);
   output_high(pin_d2);
   output_high(pin_d0);
   output_high(pin_d3);   
   delay_ms(2000);
}
int a = 0 ,value = 0, c= 0, d= 0 , temp;
unsigned  e = 127;



void main()
{
   setup();
   
  while(1)
   {
    
        {
      
         delay_ms(10);
         
         output_low(SPI_Sensor);// slave sensor
         value = spi_read(0);
         //spi_write(1);
         //delay_ms(100);
         output_high(SPI_Sensor);
         //delay_ms(1000);r);
         delay_us(10);
         temp = value;
         //delay_ms(100); 
         output_low(SPI_SS1); //slave motor 1
         output_low(SPI_SS2);
         spi_write(temp);
         output_high(SPI_SS1);//Read value from the transmitting Slave1);*/
                    //Give slave some time to respond
        // output_low(SPI_SS2);
         //spi_write(temp);           //slave motor 2
         output_high(SPI_SS2);//Send response to the receiving Slave
         delay_us(10);   
         
         
            
      }   
   }
 
}


// This program demonstrates an SPI Master that can
// read bytes from a SPI slave(transmitter) and writes
// them to another SPI slave(receiver). The byte values
// are then displayed on a 7-segm LED display.

//SPI Master; program this code into U1 in spi_ex4.pdsprj

/*#include <16f877a.h>
#use delay(clock = 20000000) 
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT,NOLVP, NOCPD, NOWRT
#use RS232(baud=56000, parity = N,xmit=PIN_C6,rcv=pin_c7)
#include <stdlib.h>


#define SPI_SS1 PIN_D1
#define SPI_SS2 PIN_D2

//SPI mode definitions
#define SPI_MODE_0 (SPI_L_TO_H | SPI_XMIT_L_TO_H)
#define SPI_MODE_1 (SPI_L_TO_H)
#define SPI_MODE_2 (SPI_H_TO_L)
#define SPI_MODE_3 (SPI_H_TO_L | SPI_XMIT_L_TO_H)




void main() {
    int8 value;

    output_high(SPI_SS1);        //Initial Slave Transmit Select to a high level
    output_high(SPI_SS2);        //Initial Slave Receive Select to a high level

    //Initialize the hardware SPI for SPI master mode.
    setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_CLK_DIV_4);
    delay_ms(500);

    while(1) {
        output_low(SPI_SS1);
        value=spi_read(0);          //Read value from the transmitting Slave
        output_high(SPI_SS1);

        delay_us(100);              //Give slave some time to respond

        output_low(SPI_SS2);
        spi_write(value);           //Send response to the receiving Slave
        output_high(SPI_SS2);
    }
}*/

