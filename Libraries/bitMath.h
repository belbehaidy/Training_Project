/*bit operations macros Header File*/

#ifndef _BIT_MATH_H
#define _BIT_MATH_H

/*Set bit operation*/
#define SET_BIT(reg,bit) reg|=(1<<bit)

/*Clear bit operation*/
#define CLR_BIT(reg,bit) reg&=~(1<<bit)

/*Get bit value operation*/
#define GET_BIT(reg,bit) (1&(reg>>bit))

/*Toggle bit operation*/
#define TOGGLE_BIT(reg,bit)	reg^=(1<<bit)

#endif
