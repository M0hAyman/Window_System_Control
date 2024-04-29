#include "motor.h"

void MOTOR_direction_init(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
                          uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2)
{
    if (motorPortDirectionControl1 == motorPortDirectionControl2)
        DIO_Init(motorPortDirectionControl1);
    else
    {
        DIO_Init(motorPortDirectionControl1);
        DIO_Init(motorPortDirectionControl2);
    }
    DIO_SetupDirection(motorPortDirectionControl1, OUT, motorPinDirectionControl1);
    DIO_SetupDirection(motorPortDirectionControl2, OUT, motorPinDirectionControl2);
}

void MOTOR_rotate(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
                  uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2)
{
    DIO_WritePin(motorPortDirectionControl1, LOGIC_HIGH, motorPinDirectionControl1);
    DIO_WritePin(motorPortDirectionControl2, LOGIC_LOW, motorPinDirectionControl2);
}

void MOTOR_rotateOpposite(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
                          uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2)
{
    DIO_WritePin(motorPortDirectionControl1, LOGIC_LOW, motorPinDirectionControl1);
    DIO_WritePin(motorPortDirectionControl2, LOGIC_HIGH, motorPinDirectionControl2);
}

/*This*/
void MOTOR_rotateToggle(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1, 
													uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2)
{
			DIO_TogglePin(motorPortDirectionControl1,motorPinDirectionControl1);
			DIO_TogglePin(motorPortDirectionControl2,motorPinDirectionControl2);
}

void MOTOR_stop(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
                uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2)
{
    DIO_WritePin(motorPortDirectionControl1, LOGIC_LOW, motorPinDirectionControl1);
    DIO_WritePin(motorPortDirectionControl2, LOGIC_LOW, motorPinDirectionControl2);
}