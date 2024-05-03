#include "TM4C123.h"
#include "../Utilities/tm4c123gh6pm.h"
#include "../Utilities/bitwise_operation.h"
#include "../Utilities/types.h"
#include "../MCAL/DIO.h"
#ifndef INTERRUPT_H
#define INTERRUPT_H

#define FALLING_EDGE 0
#define RISING_EDGE  1
#define HIGH_LEVEL   2
#define LOW_LEVEL    3
#define BOTH_EDGES   4

void interrupt_init(GPIOA_Type* port,uint8 mode,uint8 pinNumber);
void interrupt_enable_pin(GPIOA_Type* port, uint8 pin_num);
void interrupt_disable_pin(GPIOA_Type* port, uint8 pin_num);

#endif