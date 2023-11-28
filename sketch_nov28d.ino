#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
int flag=0;


void USART_Init(unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)));
/* Put data into buffer, sends the data */
UDR0 = data;
}

void USART_puts(unsigned char *str)
{
    do
      {USART_Transmit(*str);}
while (*++str!=0);
}

/*
USART_putsln(unsigned char *str)
{
    do
    {USART_puts(str);
     USART_Transmit(10);
     USART_Transmit(13);}
}*/

unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
return UDR0;
}


ISR(USART_RX_vect) // https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
{
  char status=UCSR0A;
if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
      {flag=1;
      }
    else
    {UDR0;
     USART_puts("error");}
}

int main()
{
USART_Init(MYUBRR);
sei();
  while(1){
    if(flag)
    {
    USART_Transmit(UDR0+1);
    flag=0;
    }
  _delay_ms(1);
  }
}