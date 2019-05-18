#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
struct {
  
  volatile unsigned int FLAG_ISR1:1;
  
}FLAG_BIT;
volatile uint16_t us_value=0;
volatile uint16_t *pus_value;
typedef void (*funpointer)(void);
funpointer array_fp[2]={extint1,adc_init};
int main()
  
{  pus_value=&us_value;
    SET_BIT(DDRD,PD7); //RED LED       
    SET_BIT(DDRD,PD6); //BUZZER
    SET_BIT(DDRD,PD5);//GREEN LED
    CLR_BIT(PORTD,PD7);        
    CLR_BIT(PORTD,PD6); 
    CLR_BIT(PORTD,PD5);
    Serial.begin(9600);
  	array_fp[0]();
    
  while(1)
  {   
  if(FLAG_BIT.FLAG_ISR1==1)
  {
    
   cli();
   array_fp[1]();
   
  }
   else
   {
     CLR_BIT(PORTD,PD7);
      CLR_BIT(PORTD,PD6);
      CLR_BIT(PORTD,PD5);
   }
   sei();
  }
  
}
void extint1()
{
  EICRA|=(1<<ISC10);
    EICRA&=~(1<<ISC11);
    EIMSK|=(1<<INT1);
  sei();
}
void adc_init()
{
  ADMUX=0x00;
  ADMUX |=(1<<REFS0);
  ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIE);
  ADCSRA|=(1<<ADSC);
  while(ADCSRA & (1<<ADSC));
 
  
}
ISR(INT1_vect)
            {
   			cli();
            FLAG_BIT.FLAG_ISR1=!FLAG_BIT.FLAG_ISR1;
            sei(); 
            }
ISR(ADC_vect)
{ 
   
      if((ADC<100 || ADC>200))
  {
    
      SET_BIT(PORTD,PD7);
      SET_BIT(PORTD,PD6);
      CLR_BIT(PORTD,PD5);
   }
    else 
    {
      
      CLR_BIT(PORTD,PD7);
      CLR_BIT(PORTD,PD6);
      SET_BIT(PORTD,PD5);
    }
}
