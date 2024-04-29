#include "../ECUAL/motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdlib.h>

int main()
{
	//This is for testing the motor //Delete this and continue in the project!
	MOTOR_direction_init(PORT_F, PIN1, PORT_F, PIN3);

	// Enabling switch (note: PORTF already enabled in DIO_Init in MOTOR_direction_init)
	DIO_SetupDirection(PORT_F, IN, PIN0);

	// Initially motor is rotating
	MOTOR_rotate(PORT_F, PIN1, PORT_F, PIN3);

	while (1)
	{
		if (DIO_ReadPin(PORT_F, PIN0) == 0)
		{
			MOTOR_rotateToggle(PORT_F, PIN1, PORT_F, PIN3);
			while (DIO_ReadPin(PORT_F, PIN0) == 0);
		}
	}
}
