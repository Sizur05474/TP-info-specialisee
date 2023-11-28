#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)
#define FOSC 16000000 // Fréquence du CPU en Hz
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define MAX_STRING_LENGTH 50
char status=UCSR0A
int flag = 0;

void USART_Init(unsigned int ubrr) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
}

void USART_Transmit(unsigned char data) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void USART_Transmit_String(const char *data) {
    while (*data != '\0') {
        USART_Transmit(*data++);
    }

    // Ajouter un retour à la ligne ('\n') à la fin de la chaîne
    USART_Transmit('\n');
}

unsigned char USART_Receive(void) {
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get and return received data from buffer */
    return UDR0;
}

ISR(USART_RX_vect) {
    flag = 1;
}

int main() {
    USART_Init(MYUBRR);
    sei();

    while (1) {
        if (flag) {
            // Example: Receive a string and transmit it back with a modification
            char receivedString[MAX_STRING_LENGTH];
            int index = 0;

            // Receive the string until a newline character is received or the buffer is full
            char receivedChar;
            do {
                receivedChar = USART_Receive();
                if (index < MAX_STRING_LENGTH - 1) {
                    receivedString[index++] = receivedChar;
                }
            } while (receivedChar != '\n' && index < MAX_STRING_LENGTH - 1);

            // Null-terminate the string
            receivedString[index] = '\0';

            // Transmit the modified string with a newline
            USART_Transmit_String(receivedString);

            flag = 0;
        }

        _delay_ms(1);
    }

    return 0;
}
