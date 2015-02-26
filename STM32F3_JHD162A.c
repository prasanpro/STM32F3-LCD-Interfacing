// THis code is with reference to EE herald
//http://www.eeherald.com/section/design-guide/sample_lcd_c_programs.html

#include <stdio.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>

void    Delay(unsigned int nTime);
void Timingdelay(void);

int  timingdelay = 0;
unsigned  int   count = 0;
RCC_ClocksTypeDef RCC_Clocks;

#define RS GPIO_Pin_13 // RS is named as Port 13
#define RW GPIO_Pin_14 // RW is named as Port 14
#define EN GPIO_Pin_15 // EN is named as Port 15

//------------------------------------------------------------------------------
// Function Name : Delay
// Description : delay for some time in ms unit(accurate)
// Input : n_ms is how many ms of time to delay
//------------------------------------------------------------------------------
void SysTick_Handler(void) {
	Timingdelay();
}

void Delay(uint32_t nTime)
{ 
  timingdelay = nTime;

  while(timingdelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void Timingdelay(void)
{
  if (timingdelay != 0x00)
  { 
    timingdelay--;
  }
  
  count++; 
}
//------------------------------------------------------------------------------
// Function Name : Init GPIO 
// Description : pins ,port clock & mode initialization.
//------------------------------------------------------------------------------
void initgpio()
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
RCC_GetClocksFreq(&RCC_Clocks);
SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
GPIO_Init(GPIOA, &GPIO_InitStructure);


}
//------------------------------------------------------------------------------
// Function Name : s_init
// Description : Send Instruction Function (RS=0 & RW=0)
//------------------------------------------------------------------------------

void s_init() 
{
GPIOC->BRR=RS;
GPIOC->BRR=RW;
}
//------------------------------------------------------------------------------
// Function Name : s_data
// Description : Send Data Select routine(RS=1 & RW=0)
//------------------------------------------------------------------------------

void s_data() 
{
GPIOC->BSRR=RS;
GPIOC->BRR=RW;
}
//------------------------------------------------------------------------------
// Function Name : s_latch
// Description : Latch Data/Instruction on LCD Databus.
//------------------------------------------------------------------------------

void s_latch() 
{
GPIOC->BSRR=EN;
Delay(10);
GPIOC->BRR=EN;
Delay(10);
}

/*******************************************************************************
* Function Name : main
* Description : Main program.
*******************************************************************************/
int main(void) //Main function
{ 

initgpio();

int k=0; 
char a[]=" PRASANNDUTT.COM";
char b[]="EMBEDDED SYSTEMS";

GPIOC->BRR=RS; //Initialize RS=0 for selecting instruction Send
GPIOC->BRR=RW; // Select RW=0 to write Instruction/data on LCD
GPIOC->BSRR=EN; // EN=1 for unlatch. (used at initial condition)

Delay(10);

s_init(); //Call Instruction Select routine
GPIOA->ODR=0x0001; // Clear Display, Cursor to Home 
s_latch(); //Latch the above instruction 
GPIOA->ODR=0x0038; // Display Function (2 rows for 8-bit data; small) 
s_latch(); //Latch this above instruction 4 times
s_latch(); 
s_latch(); 
s_latch(); 
GPIOA->ODR=0x000E; // Display and Cursor on, Cursor Blink off 
s_latch(); //Latch the above instruction 
GPIOA->ODR=0x0010; // Cursor shift left 
s_latch(); //Latch the above instruction 
GPIOA->ODR=0x0006; // Cursor Increment, Shift off 
s_data(); //Change the input type to Data.(before it was instruction input)
s_latch(); //Latch the above instruction

for(k=0;a[k];k++)
{
GPIOA->ODR=a[k]; //It will send a[0]='P' as = '0x0050' on Port A.
s_latch(); //Latch the above instruction only once. Or it will clone each character twice if you latch twice.
}
GPIOC->BRR=RS; //Initialize RS=0 for selecting instruction Send
GPIOC->BRR=RW; // Select RW=0 to write Instruction/data on LCD
GPIOC->BSRR=EN; // EN=1 for unlatch. (used at initial condition)

Delay(10);
GPIOA->ODR=0x00C0; // Move cursor to beginning of second row 
s_latch(); //Latch the above instruction
s_data(); //Change the input type to Data.(before it was instruction input)
for(k=0;b[k];k++)
{
GPIOA->ODR=b[k]; //It will send b[0]='E' as = '0x0044' on Port A.
s_latch();//Latch the above instruction only once. Or it will clone each character twice if you latch twice.
}
s_init(); 
}
