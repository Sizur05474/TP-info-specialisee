/**
 * @file USART0
 * @author Lucas Borel
 */
/**
 * @brief Vérification de la bonne définition du BAUD
 */
#define FRAMING_ERROR (1<<FE0)
/**
 * @brief Définition d'une possible erreur de parité
 */
#define PARITY_ERROR (1<<UPE0)
/**
 * @brief Erreur de data définit au-delà de l'espace mémoire
 */
#define DATA_OVERRUN (1<<DOR0)
/**
 * @brief Définition de la fréquence d'oscillation à 16 MHz
 */
#define FOSC 16000000 // Clock Speed
/**
 * @brief Définition de BAUD à 9600 pour le port série
 */
#define BAUD 9600
/**
 * @brief Calcul pour le taux du port série
 */
#define MYUBRR FOSC/16/BAUD-1
int flag=0;

/**
 * @brief Initialisation des variables pour pouvoir utiliser transmit and receive et avoir un bit de stop à 2
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
 * @brief Fonction transmit
 */
void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)));
/* Put data into buffer, sends the data */
UDR0 = data;
}
/**
 * @brief Fonction permettant de sauter une ligne
 */
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
/**
 * @brief Fonction permettant de recevoir les caractères transmits
 */
unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
return UDR0;
}

/**
 *@brief S'il n'y a aucune erreur on passe le flag à 1 sinon on affiche "error" 
 * @param flag Variable pour l'interruption
 */
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
/**
 * @brief Utilisation de l'interruption dans le main pour afficher le caractère suivant sinon ne rien afficher
 */
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
