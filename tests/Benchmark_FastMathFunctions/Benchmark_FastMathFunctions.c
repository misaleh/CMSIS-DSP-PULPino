#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"

#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
/*
*Each function has a GPIO pin (5 and 6 alternatively ) set before it runs and is cleared after it finish running
to measure the time of execution of each function.
*Define PRINT_OUTPUT to print the results to check for the functionality of the functions(may be slow)
*Also the correct results are printed for the current values which are calculated from the orignal library 
and also were checked by hand
*/

/* ouput variables*/
float32_t result_f32;  
q15_t result_q15;
q31_t result_q31;
int i = 0 ;
q15_t angle_q15 = 0x7FFF ; /*360*/
q31_t angle_q31 = 0x7FFFFFFF ; /*360*/
int angle =360;
volatile float32_t test_f= 4.2;
volatile q15_t test_q15 = 0x1245;
volatile q31_t test_q31 = 0x48A3617B;
int32_t main(void)
{

 /*Init*/ 
  set_pin_function(5, FUNC_GPIO);
  set_gpio_pin_direction(5, DIR_OUT);
  set_pin_function(6, FUNC_GPIO);
  set_gpio_pin_direction(6, DIR_OUT);
  set_gpio_pin_value(5, 0);
  set_gpio_pin_value(6, 0);
/*Tests*/
/*sqrt*/
  test_f +=2.0;
  set_gpio_pin_value(6, 1);	
  riscv_sqrt_f32(test_f,&result_f32);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sqrt_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
  printf("\n");


#endif
  set_gpio_pin_value(5, 1);
  riscv_sqrt_q15(0x1245,&result_q15);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sqrt_q15:\n");  
  printf("0x%X ",result_q15);  
  printf("\n");
  printf("Correct answer = 0x305B \n");
#endif
  set_gpio_pin_value(6, 1);	
  riscv_sqrt_q31(0x48A3617B,&result_q31);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sqrt_q31:\n");  
  printf("0x%X ",result_q31);  
  printf("\n");
  printf("Correct answer = 0x606CAE24 \n");
#endif
/*cos*/
  set_gpio_pin_value(5, 1);	
  result_f32=riscv_cos_f32(0.9);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_cos_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
#endif
  set_gpio_pin_value(6, 1);	
  result_q15=riscv_cos_q15(0x0FD6);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_cos_q15:\n");  
  printf("0x%X ",result_q15);  
  printf("\n");
  printf("Correct answer = 0x5B3A \n");
#endif
  set_gpio_pin_value(5, 1);
  result_q31=riscv_cos_q31(0x07FFFFFF);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_cos_q31:\n");  
  printf("0x%X ",result_q31); 
  printf("\n");
  printf("Correct answer = 0x7641AF3C \n"); 
#endif
/*sin*/
  set_gpio_pin_value(6, 1);	
  result_f32=riscv_sin_f32(0.9);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sin_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
#endif
  set_gpio_pin_value(5, 1);	
  result_q15=riscv_sin_q15(0);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sin_q15:\n");  
  printf("0x%X ",result_q15); 
  printf("\n");
  printf("\nCorrect answer = 0 \n"); 
#endif
  set_gpio_pin_value(6, 1);
  result_q31=riscv_sin_q31(0x07FFFFFF);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\nriscv_sin_q31:\n");  
  printf("0x%X ",result_q31);  
  printf("\n");
  printf("\nCorrect answer = 0x30FBC546 \n");
#endif

/*More tests*/
/*
angle_q15 = 0x7FFF ;
printf("riscv_cos_q15\n"); 
for(int i = 0 ; i < 10  ; i++)
{
  result_q15=riscv_cos_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}

angle = 360;
angle_q31 = 0x7FFFFFFF ; 
printf("riscv_cos_q31\n"); 

for(int i = 0 ; i < 10 ; i++)
{
  result_q31=riscv_cos_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q31,result_q31,(result_q31/21474836));
  angle = angle /2;
}

angle = 360;
angle_q15 = 0x7FFF ;
printf("riscv_sin_q15\n"); 
//input from range 0 to 0.999, scaled to [0 2*pi]
for(int i = 0 ; i < 10 ; i++)
{
  result_q15=riscv_sin_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}


angle = 360;
angle_q31 = 0x7FFFFFFF ; //360
printf("riscv_sin_q31\n"); 
//input from range 0 to 0.999, scaled to [0 2*pi]
for(int i = 0 ; i < 10 ; i++)
{
  result_q31=riscv_sin_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",(int)angle,(q31_t)angle_q31,(q31_t)result_q31,(q31_t)(result_q31/21474836));
  angle = angle /2;
}
*/


  printf("End\n");
  return 0 ;
}
