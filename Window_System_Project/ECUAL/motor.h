#include "../MCAL/DIO.h"
void MOTOR_direction_init(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
						  uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2);

void MOTOR_rotate(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
				  uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2);

void MOTOR_rotateOpposite(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
						  uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2);

/**
 * This function toggles the rotation direction of a motor.
 * It should be used when the motor is already rotating and you want to change its direction.
 */
void MOTOR_rotateToggle(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
						uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2);
void MOTOR_stop(uint8 motorPortDirectionControl1, uint8 motorPinDirectionControl1,
				uint8 motorPortDirectionControl2, uint8 motorPinDirectionControl2);
