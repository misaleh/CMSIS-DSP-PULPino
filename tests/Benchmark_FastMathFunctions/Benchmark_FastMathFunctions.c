
#include <math.h>
#include <stdio.h>
#include "arm_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"

#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
float32_t result_f32;  /* Final ouput */
q15_t result_q15;
q31_t result_q31;
int i = 0 ;
q15_t angle_q15 = 0x7FFF ; /*360*/
q31_t angle_q31 = 0x7FFFFFFF ; /*360*/
int angle =360;
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
  set_gpio_pin_value(6, 1);	
  arm_sqrt_f32(4.1,&result_f32);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sqrt_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
#endif
  set_gpio_pin_value(5, 1);
  arm_sqrt_q15(0x1245,&result_q15);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sqrt_q15:\n");  
  printf("%d ",result_q15);  
#endif
  set_gpio_pin_value(6, 1);	
  arm_sqrt_q31(0x48A3617B,&result_q31);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sqrt_q31:\n");  
  printf("%d ",result_q31);  
#endif*/
/*cos*/
  set_gpio_pin_value(5, 1);	
  result_f32=arm_cos_f32(0.9);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_cos_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
#endif
  set_gpio_pin_value(6, 1);	
  result_q15=arm_cos_q15(0x0FD6);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_cos_q15:\n");  
  printf("%d ",result_q15);  
#endif
  set_gpio_pin_value(5, 1);
  result_q31=arm_cos_q31(0x07FFFFFF);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_cos_q31:\n");  
  printf("%d ",result_q31);  
#endif*/
/*sin*/
  set_gpio_pin_value(6, 1);	
  result_f32=arm_sin_f32(0.9);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sin_f32:\n");  
  printf("%d ",(int)(result_f32*100));  
#endif
  set_gpio_pin_value(5, 1);	
  result_q15=arm_sin_q15(0);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sin_q15:\n");  
  printf("%d ",result_q15);  
#endif
  set_gpio_pin_value(6, 1);
  result_q31=arm_sin_q31(0x07FFFFFF);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_sin_q31:\n");  
  printf("%d ",result_q31);  
#endif

/*More tests*/

angle_q15 = 0x7FFF ;
printf("arm_cos_q15\n"); 
for(int i = 0 ; i < 10  ; i++)
{
  result_q15=arm_cos_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}

angle = 360;
angle_q31 = 0x7FFFFFFF ; 
printf("arm_cos_q31\n"); 

for(int i = 0 ; i < 10 ; i++)
{
  result_q31=arm_cos_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q31,result_q31,(result_q31/21474836));
  angle = angle /2;
}

angle = 360;
angle_q15 = 0x7FFF ;
printf("arm_sin_q15\n"); 
/*input from range 0 to 0.999, scaled to [0 2*pi]*/
for(int i = 0 ; i < 10 ; i++)
{
  result_q15=arm_sin_q15(angle_q15);
  angle_q15 = angle_q15 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",angle,angle_q15,result_q15,(result_q15/327));
  angle = angle /2;
}


angle = 360;
angle_q31 = 0x7FFFFFFF ; /*360*/
printf("arm_sin_q31\n"); 
/*input from range 0 to 0.999, scaled to [0 2*pi]*/
for(int i = 0 ; i < 10 ; i++)
{
  result_q31=arm_sin_q31(angle_q31);
  angle_q31 = angle_q31 >> 1; //divide by 2
  printf("Angle = %d,Fixed =  %d, result in fixed point is %d, equivalent(*100) = %d\n",(int)angle,(q31_t)angle_q31,(q31_t)result_q31,(q31_t)(result_q31/21474836));
  angle = angle /2;
}


  printf("End\n");
  return 0 ;
}

 /** \endlink */
