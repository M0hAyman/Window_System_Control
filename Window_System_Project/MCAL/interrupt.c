#include "interrupt.h"


void interrupt_init(GPIOA_Type* port,uint8 mode,uint8 pinNumber){
  
	  if(mode == FALLING_EDGE){
      Clear_Bit(port->IS , pinNumber);
      
      Clear_Bit(port->IBE, pinNumber);
      
      Clear_Bit(port->IEV, pinNumber);
	  }
	  else if(mode == RISING_EDGE){
      Clear_Bit(port->IS , pinNumber);
      
      Clear_Bit(port->IBE, pinNumber);
      
      Set_Bit(port->IEV, pinNumber);
      }
	  else if(mode == HIGH_LEVEL){
      Set_Bit(port->IS , pinNumber);
      
      Clear_Bit(port->IBE, pinNumber);
      
      Set_Bit(port->IEV, pinNumber);
	  }
	  
	  else if(mode == LOW_LEVEL){
      Set_Bit(port->IS , pinNumber);
      
      Clear_Bit(port->IBE, pinNumber);
      
      Clear_Bit(port->IEV, pinNumber);
	 }
	  
	 else if(mode == BOTH_EDGES){
      Clear_Bit(port->IS , pinNumber);		 
		 
      Set_Bit(port->IBE, pinNumber);	  
	 }

    
    //clear prior interrupts
	Set_Bit(port->ICR,pinNumber);
    
	if(port == GPIOA)
	{
	//set priority
	NVIC_PRI0_R |= 5 << 5;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,0);
		
	}
	if(port == GPIOB)
	{
	//set priority
	NVIC_PRI0_R |=  5 << 13;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,1);
		
	}
	if(port == GPIOC)
	{
	//set priority
	NVIC_PRI0_R |= 5 << 21;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,2);
		
	}
	if(port == GPIOD)
	{
	//set priority
	NVIC_PRI0_R |= 5 << 29;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,3);
		
	}
	
	if(port == GPIOE)
	{
	//set priority
	NVIC_PRI1_R |= 5 << 5;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,4);
		
	}
	if(port == GPIOF)
	{
	//set priority
	NVIC_PRI1_R |= 5 << 21;
    
    //enable interrupt
    Set_Bit(NVIC_EN0_R,30);	
	}
  
} //end of function

void interrupt_enable_pin(GPIOA_Type* port, uint8 pin_num){
  if(pin_num > 7){
  }
	Set_Bit(port->IM,pin_num);

}

void interrupt_disable_pin(GPIOA_Type* port, uint8 pin_num){
	
	Clear_Bit(port->IM,pin_num);	
}



