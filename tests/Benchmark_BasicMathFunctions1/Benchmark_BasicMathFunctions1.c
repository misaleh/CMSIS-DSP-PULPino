
#include <math.h>
#include <stdio.h>
#include "arm_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"


#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32


 float32_t srcA_buf_f32[MAX_BLOCKSIZE] =
{
  -0.4325648115282207,  -1.6655843782380970,  0.1253323064748307,
   0.2876764203585489,  -1.1464713506814637,  1.1909154656429988,
   1.1891642016521031,  -0.0376332765933176,  0.3272923614086541,
   0.1746391428209245,  -0.1867085776814394,  0.7257905482933027,
  -0.5883165430141887,   2.1831858181971011, -0.1363958830865957,
   0.1139313135208096,   1.0667682113591888,  0.0592814605236053,
  -0.0956484054836690,  -0.8323494636500225,  0.2944108163926404,
  -1.3361818579378040,   0.7143245518189522,  1.6235620644462707,
  -0.6917757017022868,   0.8579966728282626,  1.2540014216025324,
  -1.5937295764474768,  -1.4409644319010200,  0.5711476236581780,
  -0.3998855777153632,   0.6899973754643451
};


 float32_t srcB_buf_f32[MAX_BLOCKSIZE] =
{
   1.7491401329284098,  0.1325982188803279,   0.3252281811989881,
  -0.7938091410349637,  0.3149236145048914,  -0.5272704888029532,
   0.9322666565031119,  1.1646643544607362,  -2.0456694357357357,
  -0.6443728590041911,  1.7410657940825480,   0.4867684246821860,
   1.0488288293660140,  1.4885752747099299,   1.2705014969484090,
  -1.8561241921210170,  2.1343209047321410,  1.4358467535865909,
  -0.9173023332875400, -1.1060770780029008,   0.8105708062681296,
   0.6985430696369063, -0.4015827425012831,   1.2687512030669628,
  -0.7836083053674872,  0.2132664971465569,   0.7878984786088954,
   0.8966819356782295, -0.1869172943544062,   1.0131816724341454,
   0.2484350696132857,  0.0596083377937976
};
 q7_t srcA_buf_q7[MAX_BLOCKSIZE] =
{
   0x75,  0x13,   0x15,
   0x44,  0x11,   0x1C,
   0x52,  0x0B,   0x83,
   0x91,  0x33,   0x25,
   0x1A,  0x4D,   0x6F,
   0x26,  0x01,   0xF1,
   0xAC,  0x66,   0x76,
   0x54,  0x87,   0x36,
   0x22,  0x33,   0xE1,
   0x61,  0x54,   0x35,
   0x91,  0x49
};

 q7_t srcB_buf_q7[MAX_BLOCKSIZE] =
{
   0x75,  0x13,   0x15,
   0x44,  0x11,   0x1C,
   0x52,  0x0B,   0x83,
   0x91,  0x33,   0x25,
   0x1A,  0x4D,   0x6F,
   0x26,  0x01,   0xF1,
   0xAC,  0x66,   0x76,
   0x54,  0x87,   0x36,
   0x22,  0x33,   0xE1,
   0x61,  0x54,   0x35,
   0x91,  0x49
};

 q15_t srcA_buf_q15[MAX_BLOCKSIZE] =
{
   0x7512,  0x1375,   0x1565,
   0x44C3,  0x1188,   0x1CA1,
   0x5264,  0x0B20,   0x8305,
   0x9112,  0x3399,   0x2518,
   0x1AB2,  0x4D01,   0x6F23,
   0x26FF,  0x0121,   0xF123,
   0xAC47,  0x6688,   0x76A2,
   0x5476,  0x8756,   0x36B3,
   0x2245,  0x3373,   0xE1A9,
   0x610A,  0x5419,   0x3501,
   0x9100,  0x4469
};
 q15_t srcB_buf_q15[MAX_BLOCKSIZE] =
{
   0x7512,  0x1375,   0x1565,
   0x44C3,  0x1188,   0x1CA1,
   0x5264,  0x0B20,   0x8305,
   0x9112,  0x3399,   0x2518,
   0x1AB2,  0x4D01,   0x6F23,
   0x26FF,  0x0121,   0xF123,
   0xAC47,  0x6688,   0x76A2,
   0x5476,  0x8756,   0x36B3,
   0x2245,  0x3373,   0xE1A9,
   0x610A,  0x5419,   0x3501,
   0x9100,  0x4469
};
 q31_t srcA_buf_q31[MAX_BLOCKSIZE] =
{
   0x75122542,  0x1375A138,   0x15654473,
   0x44C34463,  0x118854C3,   0x1CA18291,
   0x5264E236,  0x0B200590,   0x83059A77,
   0x91129713,  0x3399D123,   0x2518BBD1,
   0x1AB2037A,  0x4D01D1F1,   0x6F237531,
   0x26FF5569,  0x01213159,   0xF123CD77,
   0xAC475456,  0x6688A12E,   0x76A2FF96,
   0x54761269,  0x8756D438,   0x36B3B697,
   0x22459643,  0x3373C368,   0xE1A901D3,
   0x610A0540,  0x54199643,   0x350112B5,
   0x91000436,  0x4469AA15
};
 q31_t srcB_buf_q31[MAX_BLOCKSIZE] =
{
   0x75122542,  0x1375A138,   0x15654473,
   0x44C34463,  0x118854C3,   0x1CA18291,
   0x5264E236,  0x0B200590,   0x83059A77,
   0x91129713,  0x3399D123,   0x2518BBD1,
   0x1AB2037A,  0x4D01D1F1,   0x6F237531,
   0x26FF5569,  0x01213159,   0xF123CD77,
   0xAC475456,  0x6688A12E,   0x76A2FF96,
   0x54761269,  0x8756D438,   0x36B3B697,
   0x22459643,  0x3373C368,   0xE1A901D3,
   0x610A0540,  0x54199643,   0x350112B5,
   0x91000436,  0x4469AA15
};



float32_t result_f32[MAX_BLOCKSIZE];  /* Final ouput */
q7_t result_q7[MAX_BLOCKSIZE];
q15_t result_q15[MAX_BLOCKSIZE];
q31_t result_q31[MAX_BLOCKSIZE];

q63_t result_q63_1;
q31_t result_q31_1;
float32_t result_f32_1;
int * ptr = &result_q63_1;

int i = 0 ;
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
/*abs*/
  set_gpio_pin_value(6, 1);	
  arm_abs_f32( srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_abs_f32:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",(int)(result_f32[i]*100));  
    }
#endif
  set_gpio_pin_value(5, 1);
  arm_abs_q7(srcA_buf_q7,result_q7,MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_abs_q7:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q7[i]);  
    }
#endif
  set_gpio_pin_value(6, 1);	
  arm_abs_q15(srcA_buf_q15,result_q15,MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_abs_q15:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q15[i]);  
    }
#endif
  set_gpio_pin_value(5, 1);	
  arm_abs_q31(srcA_buf_q31,result_q31,MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_abs_q31:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q31[i]);  
    }
#endif
/*add*/

  set_gpio_pin_value(6, 1);	
  arm_add_f32(srcA_buf_f32, srcB_buf_f32, result_f32, MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_add_f32:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",(int)(result_f32[i]*100));  
    }
#endif
  set_gpio_pin_value(5, 1);	
  arm_add_q7(srcA_buf_q7, srcB_buf_q7, result_q7, MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_add_q7:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q7[i]);  
    }
#endif
  set_gpio_pin_value(6, 1);	
  arm_add_q15(srcA_buf_q15, srcB_buf_q15, result_q15, MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);

#ifdef PRINT_OUTPUT
  printf("\narm_add_q15:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q15[i]);  
    }
#endif
  set_gpio_pin_value(5, 1);	
  arm_add_q31(srcA_buf_q31, srcB_buf_q31, result_q31, MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_add_q31:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q31[i]);  
    }
#endif
/*dot product*/
  set_gpio_pin_value(6, 1);	
  arm_dot_prod_q15(srcA_buf_q15,srcB_buf_q15,MAX_BLOCKSIZE,&result_q63_1);/*output format Q34.30*/
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_dot_prod_q15:\n");  
  printf("%X ",*(ptr++));
  printf("%X ",*(ptr++));

#endif
  set_gpio_pin_value(5, 1);	
  arm_dot_prod_q31(srcA_buf_q31,srcB_buf_q31,MAX_BLOCKSIZE,&result_q63_1); /*output format Q16.48*/
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_dot_prod_q31:\n");  
  printf("%X ",*(ptr++));
  printf("%X ",*(ptr++));

#endif
  set_gpio_pin_value(6, 1);	
  arm_dot_prod_f32(srcA_buf_f32,srcB_buf_f32,MAX_BLOCKSIZE,&result_f32_1);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_dot_prod_f32:\n");  
  printf("%d ",(int)(result_f32_1*100));
#endif
  set_gpio_pin_value(5, 1);	
  arm_dot_prod_q7(srcA_buf_q7,srcB_buf_q7,MAX_BLOCKSIZE,&result_q31_1);/*output format Q18.14*/
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_dot_prod_q7:\n");  
  printf("%X ",result_q31_1);  

#endif
/*Mult*/
  set_gpio_pin_value(6, 1);
  arm_mult_f32(srcA_buf_f32, srcB_buf_f32, result_f32, MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_mult_f32:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",(int)(result_f32[i]*100));  
    }
#endif
  set_gpio_pin_value(5, 1);
  arm_mult_q31(srcA_buf_q31, srcB_buf_q31, result_q31, MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_mult_q31:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q31[i]);  
    }
#endif
  set_gpio_pin_value(6, 1);
  arm_mult_q15(srcA_buf_q15, srcB_buf_q15, result_q15, MAX_BLOCKSIZE);
  set_gpio_pin_value(6, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_mult_q15:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q15[i]);  
    }
#endif
  set_gpio_pin_value(5, 1);
  arm_mult_q7(srcA_buf_q7, srcB_buf_q7, result_q7, MAX_BLOCKSIZE);
  set_gpio_pin_value(5, 0);
#ifdef PRINT_OUTPUT
  printf("\narm_mult_q7:\n");  
  for(i = 0 ; i < MAX_BLOCKSIZE ; i++)
    {
      printf("%d ",result_q7[i]);  
    }
#endif

  printf("End\n");

  return 0 ;
}


