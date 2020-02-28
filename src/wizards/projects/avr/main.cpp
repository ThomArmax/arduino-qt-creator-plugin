/**
 * @file main.cpp
 * @brief
 * @details
 * @author
 *
 * Project:	%{ProFileName}
 * MCU:		%{atpMCU}
 * FRENQUENCY:	%{atpSpeed}
 *
 * Created using QtCreator
 */

#ifndef F_CPU
#define F_CPU %{atpSpeed}UL
#endif

//-- Includes
#include <avr/io.h>
#include <avr/delay.h>

//-- Definitions
#define SETBIT  (ADDRESS, BIT)  (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS, BIT)  (ADDRESS &= ~(1<<BIT))
#define FLIPBIT (ADDRESS, BIT)  (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS, BIT)  (ADDRESS & (1<<BIT))

#define SETBITMASK  (x,y)       (x |= (y))
#define CLEARBITMASK(x,y)       (x &= (~y))
#define FLIPBITMASK (x,y)       (x ^= (y))
#define CHECKBITMASK(x,y)       (x & (y))

#define VARFROMCOMB(x, y)       x
#define BITFROMCOMB(x, y)       y

#define C_SETBIT(comb)          SETBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CLEARBIT(comb)        CLEARBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_FLIPBIT(comb)         FLIPBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CHECKBIT(comb)        CHECKBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))

#define Status_LED              PORTB, PB7 //use pin PB7 from port B as status led

//-- Functions

int main(void)
{
    SETBIT(DDRB, PB7); //set PB7 as output

    while(true)
    {
        C_SETBIT(Status_LED); //led on
        _delay_ms(1000);
        C_CLEARBIT(Status_LED); //led off
        _delay_ms(1000);
    }

    return 0;
}
