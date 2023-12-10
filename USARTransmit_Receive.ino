/**
*@file USARTransmit_Receive
*@brief Programme permettant d'envoyer et recevoir des caractères
*@author Lucas Borel
*@version 1.0
 */
 
 /**
*@brief Déclaration de la fréquence d'oscillation à 16 MHz
*/
#define FOSC 16000000 // Clock Speed
/**
 * @brief Déclaration de BAUD à 9600 pour la valeur du port série
 */
#define BAUD 9600
/**
 * @brief Calcul pour initialiser le taux du BAUD
 */
#define MYUBRR FOSC/16/BAUD-1
/**
 * @brief Initialisation du flag à 0 permettant de ne pas rentrer dans l'interruption au démarrage
 */
int flag=0;

/**
*@brief Déclaration d'USART0 pour utiliser la fonction receive et transmit avec un bit de stop à 2
*/
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

/**
*@brief Fonction permettant de transmettre un caractère
*/
void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)));
/* Put data into buffer, sends the data */
UDR0 = data;
}

/**
*@brief Fonction permettant de recevoir ce que l'utilisateur envoie et de le stocker dans une variable
*/
unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
return UDR0;
}

ISR(USART_RX_vect) // https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
{
  flag=1;
_delay_ms(1);
}

/**
*@brief Permet d'utiliser USART_Init et une interruption qui sera active si le flag est à 1 sinon elle renvoit le caractère 'A'
*/
int main()
{
USART_Init(MYUBRR);
sei();

while(1){
    if(flag)
  { 
    USART_Transmit(10);
    USART_Transmit(UDR0+1);
          flag=0;
          _delay_ms(1);}
    else 
    { USART_Transmit('A');
      flag=0;
      _delay_ms(10);}
}
}
