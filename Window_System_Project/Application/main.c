#include "../ECUAL/motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdlib.h>
#include "task.h"
#include "semphr.h"
#include "../MCAL/interrupt.h"


#define UP 0
#define DOWN 1

#define DRIVER_PORT       PORT_D
#define DRIVER_UP_PIN     PIN0
#define DRIVER_DOWN_PIN   PIN1

#define PASSENGER_PORT     PORT_D
#define PASSENGER_UP_PIN   PIN2 
#define PASSENGER_DOWN_PIN PIN3

#define LOCK_PORT          PORT_B
#define LOCK_PIN           PIN6

#define JAMMING_PORT       PORT_B
#define JAMMING_PIN        PIN7

#define MOTOR_PORT         PORT_F
#define MOTOR_FIRST_PIN    PIN1
#define MOTOR_SECOND_PIN   PIN2

#define LIMIT_UP_PORT      PORT_B
#define LIMIT_UP_PIN       PIN0

#define LIMIT_DOWN_PORT    PORT_B
#define LIMIT_DOWN_PIN     PIN1

QueueHandle_t driverQueue;
QueueHandle_t passengerQueue;
xSemaphoreHandle xMutex;
xSemaphoreHandle xSemaphore;
uint8 motorRunning = 0;
uint8 limit_UP_flag = 0;
uint8 limit_DOWN_flag = 0;


void driverTask(void *params){
    int command;
    while(1){
        xSemaphoreTake(xMutex,portMAX_DELAY); //check if there is a jamming emergency
        xSemaphoreGive(xMutex);
        xQueueReceive(driverQueue,&command,portMAX_DELAY);  //Wait For command
        if(command == UP && !limit_UP_flag){ 
            MOTOR_rotate(PORT_F,PIN1,PORT_F,PIN2);
            vTaskDelay(200);
                if (DIO_ReadPin(PORT_D,PIN0)==0){
                    while(DIO_ReadPin(PORT_D,PIN0)==0);
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
                
                }   
                else if(motorRunning){ //Second press in auto mode turns the motor off
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
                    motorRunning = 0;
                }
                else{
					motorRunning = 1;
                }					
        }

        else if(command == DOWN && !limit_DOWN_flag){
            MOTOR_rotateOpposite(PORT_F,PIN1,PORT_F,PIN2);
            vTaskDelay(200);
                if (DIO_ReadPin(PORT_D,PIN1)==0){
                    while(DIO_ReadPin(PORT_D,PIN1)==0);
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
                }
				else if(motorRunning){ //Second press in auto mode turns the motor off
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
                    motorRunning = 0;
                }
                 else{
					motorRunning = 1;
				 }					 
        }


    }
}

void passengerTask(void *params){
    int command;
    while(1){
        xSemaphoreTake(xMutex,portMAX_DELAY);
        xSemaphoreGive(xMutex);
        xQueueReceive(passengerQueue,&command,portMAX_DELAY); 
        if(command == UP && !limit_UP_flag){
			MOTOR_rotate(PORT_F,PIN1,PORT_F,PIN2);
			vTaskDelay(200);
            if (DIO_ReadPin(PORT_D,PIN0)==0){
                while(DIO_ReadPin(PORT_D,PIN0)==0);
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
            
            }
            else if(motorRunning){ //Second press in auto mode turns the motor off
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
                motorRunning = 0;
            }
            else{
			    motorRunning = 1;
			}		
        }

        else if(command == DOWN && !limit_DOWN_flag){
			MOTOR_rotateOpposite(PORT_F,PIN1,PORT_F,PIN2);
			vTaskDelay(200);
            if (DIO_ReadPin(PORT_D,PIN1)==0){
                while(DIO_ReadPin(PORT_D,PIN1)==0);
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
            
            }
			else if(motorRunning){ //Second press in auto mode turns the motor off
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
                motorRunning = 0;
            }
            else{
				motorRunning = 1;
			}		  
        }
    }
}

void jammingTask(void* params){
	xSemaphoreTake(xSemaphore,0);
	int dummy;
	for(;;){
		xSemaphoreTake(xSemaphore,portMAX_DELAY);
		MOTOR_rotateOpposite(MOTOR_PORT,MOTOR_FIRST_PIN,MOTOR_PORT,MOTOR_SECOND_PIN);
        xSemaphoreTake(xMutex,portMAX_DELAY); //check if there is a jamming emergency
		vTaskDelay(pdMS_TO_TICKS(500));
		MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
		xSemaphoreGive(xMutex);
		xQueueReceive(driverQueue,&dummy,0);
		xQueueReceive(passengerQueue,&dummy,0);	
	}
	
}

void initTask(){
	DIO_Init(PORT_D);
	DIO_Init(LOCK_PORT);
	MOTOR_direction_init(PORT_F,PIN1,PORT_F,PIN2);
	
	DIO_SetupDirection(PORT_D,IN,DRIVER_UP_PIN);
	DIO_SetupDirection(PORT_D,IN,DRIVER_DOWN_PIN);
	DIO_SetupDirection(PORT_D,IN,PASSENGER_UP_PIN);
	DIO_SetupDirection(PORT_D,IN,PASSENGER_DOWN_PIN);
	
	interrupt_init(GPIOD,FALLING_EDGE,DRIVER_UP_PIN);
	interrupt_init(GPIOD,FALLING_EDGE,DRIVER_DOWN_PIN);
	interrupt_init(GPIOD,FALLING_EDGE,PASSENGER_UP_PIN);
	interrupt_init(GPIOD,FALLING_EDGE,PASSENGER_DOWN_PIN);
	
	interrupt_enable_pin(GPIOD,DRIVER_UP_PIN);
	interrupt_enable_pin(GPIOD,DRIVER_DOWN_PIN);
	interrupt_enable_pin(GPIOD,PASSENGER_UP_PIN);
	interrupt_enable_pin(GPIOD,PASSENGER_DOWN_PIN);
	
	DIO_SetupDirection(PORT_B,IN,LOCK_PIN);
	DIO_SetupDirection(PORT_B,IN,JAMMING_PIN);
	DIO_SetupDirection(PORT_B,IN,LIMIT_UP_PIN);
	DIO_SetupDirection(PORT_B,IN,LIMIT_DOWN_PIN);	
	
	interrupt_init(GPIOB,BOTH_EDGES,LOCK_PIN);
	interrupt_init(GPIOB,FALLING_EDGE,JAMMING_PIN);
	interrupt_init(GPIOB,BOTH_EDGES,LIMIT_UP_PIN);
	interrupt_init(GPIOB,BOTH_EDGES,LIMIT_DOWN_PIN);
	
	interrupt_enable_pin(GPIOB,LOCK_PIN);
	interrupt_enable_pin(GPIOB,JAMMING_PIN);
	interrupt_enable_pin(GPIOB,LIMIT_UP_PIN);
	interrupt_enable_pin(GPIOB,LIMIT_DOWN_PIN);
}

// driver & Passenger isr 
void GPIOD_Handler(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int datasend =-1;
    //from driver
    if(Get_Bit(GPIO_PORTD_MIS_R,DRIVER_UP_PIN) == 1){ //up button
        datasend = UP;
        Set_Bit(GPIO_PORTD_ICR_R,DRIVER_UP_PIN);
        xQueueSendToBackFromISR(driverQueue, &datasend, &xHigherPriorityTaskWoken);
    }
    else if(Get_Bit(GPIO_PORTD_MIS_R,DRIVER_DOWN_PIN)==1){ //down button
        datasend = DOWN;
        Set_Bit(GPIO_PORTD_ICR_R,DRIVER_DOWN_PIN);
        xQueueSendToBackFromISR(driverQueue, &datasend, &xHigherPriorityTaskWoken);
    }	
	//from Passenger
	else if(Get_Bit(GPIO_PORTD_MIS_R,PASSENGER_UP_PIN) == 1){ //up button
        datasend = UP;
        Set_Bit(GPIO_PORTD_ICR_R,PASSENGER_UP_PIN);
	    xQueueSendToBackFromISR(passengerQueue, &datasend, &xHigherPriorityTaskWoken);
    }
    else if(Get_Bit(GPIO_PORTD_MIS_R,PASSENGER_DOWN_PIN)==1){ //down button
        datasend = DOWN;
        Set_Bit(GPIO_PORTD_ICR_R,PASSENGER_DOWN_PIN);
		xQueueSendToBackFromISR(passengerQueue, &datasend, &xHigherPriorityTaskWoken);
    }	

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

//Driver Lock ISR
void GPIOB_Handler(){
    //Driver Lock
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(Get_Bit(GPIO_PORTB_MIS_R,LOCK_PIN)==1){ //down button
		if(DIO_ReadPin(LOCK_PORT,LOCK_PIN) == 0){ //Falling Edge
			interrupt_disable_pin(GPIOD,PASSENGER_UP_PIN);
			interrupt_disable_pin(GPIOD,PASSENGER_DOWN_PIN);			
		}
		else if(DIO_ReadPin(LOCK_PORT,LOCK_PIN) == 1){
			interrupt_enable_pin(GPIOD,PASSENGER_UP_PIN);
			interrupt_enable_pin(GPIOD,PASSENGER_DOWN_PIN);					
		}
	    Set_Bit(GPIO_PORTB_ICR_R,LOCK_PIN);
    }
	//Jamming
	else if(Get_Bit(GPIO_PORTB_MIS_R,JAMMING_PIN)==1){
		xSemaphoreGiveFromISR(xSemaphore,&xHigherPriorityTaskWoken);
	    Set_Bit(GPIO_PORTB_ICR_R,JAMMING_PIN);		
	}
	
	//UP Limit Switch
	else if(Get_Bit(GPIO_PORTB_MIS_R,LIMIT_UP_PIN)==1){
		if(DIO_ReadPin(LIMIT_UP_PORT,LIMIT_UP_PIN) == 0){ //Falling Edge
			MOTOR_stop(MOTOR_PORT,MOTOR_FIRST_PIN,MOTOR_PORT,MOTOR_SECOND_PIN);
			motorRunning = 0;
			limit_UP_flag = 1;
		}
		else if(DIO_ReadPin(LIMIT_UP_PORT,LIMIT_UP_PIN) == 1){
			limit_UP_flag = 0;
		}
	    Set_Bit(GPIO_PORTB_ICR_R,LIMIT_UP_PIN);	
	}
	
	else if(Get_Bit(GPIO_PORTB_MIS_R,LIMIT_DOWN_PIN)==1){
		if(DIO_ReadPin(LIMIT_DOWN_PORT,LIMIT_DOWN_PIN) == 0){ //Falling Edge
			MOTOR_stop(MOTOR_PORT,MOTOR_FIRST_PIN,MOTOR_PORT,MOTOR_SECOND_PIN);
			motorRunning = 0;
			limit_DOWN_flag = 1;
		}
		else if(DIO_ReadPin(LIMIT_DOWN_PORT,LIMIT_DOWN_PIN) == 1){
			limit_DOWN_flag = 0;
		}
	    Set_Bit(GPIO_PORTB_ICR_R,LIMIT_DOWN_PIN);	
	}
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);	
}





int main()
{	initTask();
	driverQueue = xQueueCreate(1, sizeof(uint8_t));
	passengerQueue = xQueueCreate(1, sizeof(uint8_t));
	xMutex = xSemaphoreCreateMutex();
	xSemaphore = xSemaphoreCreateBinary();
	xTaskCreate(driverTask,"ELSOA2",100,0,2,0);
	xTaskCreate(passengerTask,"ELLY GNB EL SOA2",100,0,1,0);
	xTaskCreate(jammingTask,"EMERGENCY!!",100,0,3,0);
	vTaskStartScheduler();

	for(;;);
}
