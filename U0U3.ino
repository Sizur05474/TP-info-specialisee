#define FOSC 16000000 // Fréquence du quartz
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
int flag0=0;
int flag3=0;

void USART_Init(unsigned int ubrr)
{
  /*Set baud rate */
  UBRR3H = (unsigned char)(ubrr>>8);
  UBRR3L = (unsigned char)ubrr;
  /*Enable receiver and transmitter */
  UCSR3B = (1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3); // RXCIE3 pour activer l'interruption RX
  /* Set frame format: 8data, 1stop bit */
  UCSR3C = (1<<UCSZ30)|(1<<UCSZ31);

    /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /*Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); // RXCIE0 pour activer l'interruption RX
  /* Set frame format: 8data, 1stop bit */
  UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

void USART3_Transmit(unsigned char data)
{
  /* Wait for empty transmit buffer */
  while (!(UCSR3A & (1<<UDRE3)));
  /* Put data into buffer, sends the data */
  UDR3 = data;
}

void USART0_Transmit(unsigned char data)
{
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1<<UDRE0)));
  /* Put data into buffer, sends the data */
  UDR0 = data;
}
void USART_puts(unsigned char *str)
{
  do
  {
    USART0_Transmit(*str);
  } while (*++str!=0);
}
unsigned char USART3_Receive(void)
{
  /* Wait for data to be received */
  while (!(UCSR3A & (1<<RXC3)));
  /* Get and return received data from buffer */
  return UDR3;
}
unsigned char USART0_Receive(void)
{
  /* Wait for data to be received */
  while (!(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}

ISR(USART3_RX_vect)
{flag3 = 1;}
ISR(USART0_RX_vect)
{flag0 = 1;}


int main()
{
  USART_Init(MYUBRR);
  sei();
  while(1)
  {
    if(flag3)
    {
    USART0_Transmit(UDR3);
    flag3 = 0;
    }
    if(flag0)
    {
      USART3_Transmit(UDR0);
      flag0=0;
    }
    _delay_ms(1);
  }
}
