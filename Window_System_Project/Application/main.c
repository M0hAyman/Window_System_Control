#include "../ECUAL/motor.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "../MCAL/interrupt.h"
#include "../ECUAL/motor.h"

#define UP 0
#define DOWN 1

QueueHandle_t driverQueue;
QueueHandle_t passengerQueue;
xSemaphoreHandle xMutex;
uint8 motorRunning = 0;

void driverTask(void *params){
    int command;
    while(1){
        xQueueReceive(driverQueue,&command,portMAX_DELAY);  //Wait For command
        xSemaphoreTake(xMutex,portMAX_DELAY); //check if there is a jamming emergency
        xSemaphoreGive(xMutex);
        if(command == UP && !motorRunning){ 
            motorRunning = 1;
            MOTOR_rotate(PORT_F,PIN1,PORT_F,PIN2);
            vTaskDelay(200);
                if (DIO_ReadPin(PORT_D,PIN0)==0){
                    while(DIO_ReadPin(PORT_D,PIN0)==0);
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
                
                }        
        }

        else if(command == DOWN && !motorRunning){
            motorRunning = 1;
            MOTOR_rotateOpposite(PORT_F,PIN1,PORT_F,PIN2);
            vTaskDelay(200);
                if (DIO_ReadPin(PORT_D,PIN1)==0){
                    while(DIO_ReadPin(PORT_D,PIN1)==0);
                    MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
                }
                
        }
        else{ //Second press in auto mode turns the motor off
            MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
            motorRunning = 0;
        }

    }
}

void passengerTask(void *params){
    int command;
    while(1){
        xQueueReceive(driverQueue,&command,portMAX_DELAY); 
        xSemaphoreTake(xMutex,portMAX_DELAY);
        xSemaphoreGive(xMutex);
        if(command == 0 && !motorRunning){
        motorRunning = 1;
        MOTOR_rotate(PORT_F,PIN1,PORT_F,PIN2);
        vTaskDelay(200);
            if (DIO_ReadPin(PORT_D,PIN0)==0){
                while(DIO_ReadPin(PORT_D,PIN0)==0);
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
            
            }
            
        }

        else if(command == 1 && !motorRunning){
        motorRunning = 1;
        MOTOR_rotateOpposite(PORT_F,PIN1,PORT_F,PIN2);
        vTaskDelay(200);
            if (DIO_ReadPin(PORT_D,PIN1)==0){
                while(DIO_ReadPin(PORT_D,PIN1)==0);
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);
            
            }
                
            
        }
        else{
                MOTOR_stop(PORT_F,PIN1,PORT_F,PIN2);	
                motorRunning = 0;
        }
    }
}
void initTask(){
	DIO_Init(PORT_D);
	MOTOR_direction_init(PORT_F,PIN1,PORT_F,PIN2);
	DIO_SetupDirection(PORT_D,IN,PIN0);
	DIO_SetupDirection(PORT_D,IN,PIN1);
	interrupt_init(GPIOD,FALLING_EDGE,PIN0);
	interrupt_init(GPIOD,FALLING_EDGE,PIN1);
	interrupt_enable_pin(GPIOD,PIN0);
	interrupt_enable_pin(GPIOD,PIN1);
}

// driver isr 
void GPIOD_Handler(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int datasend =-1;
    //from driver
    if(Get_Bit(GPIO_PORTD_MIS_R,0) == 1){ //up button
        datasend = UP;
        GPIO_PORTD_ICR_R |= 0x1;
    }
    if(Get_Bit(GPIO_PORTD_MIS_R,1)==1){ //down button
        datasend = DOWN;
        GPIO_PORTD_ICR_R |= 0x2;
    }	
    xQueueSendToBackFromISR(driverQueue, &datasend, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

int main()
{	initTask();
	driverQueue = xQueueCreate(1, sizeof(long));
	xMutex = xSemaphoreCreateMutex();
	xTaskCreate(driverTask,"ELSOA2",100,0,2,0);
	xTaskCreate(passengerTask,"ELLY GNB EL SOA2",100,0,1,0);
	vTaskStartScheduler();

	for(;;);
}
