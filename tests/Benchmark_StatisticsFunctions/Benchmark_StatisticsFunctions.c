#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"

//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32
#define NUM_SAMPLES  16  /*size of array 32. num of elements are 16 as each 2 elements represnt a complex number */
/*
  this macros used for benchmarking, they are not friendly as they affect other GPIOs, but the main purpose here is to minimize the overhead,
a more fierndly version will be
#define SET_GPIO_6()     *(volatile int*) (GPIO_REG_PADOUT) |=  0x20;
#define CLR_GPIO_6()    *(volatile int*) (GPIO_REG_PADOUT) &=  0xDF;
#define SET_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) |=  0x10;
#define CLR_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) &=  0xEF;
*/

#define SET_GPIO_6()     *(volatile int*) (GPIO_REG_PADOUT) =  0x20;
#define CLR_GPIO_6()    *(volatile int*) (GPIO_REG_PADOUT) =  0x00;
#define SET_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) =  0x10;
#define CLR_GPIO_5()     *(volatile int*) (GPIO_REG_PADOUT) =  0x00;


/*
*Each function has a GPIO pin (5 and 6 alternatively ) set before it runs and is cleared after it finish running
to measure the time of execution of each function.
*Define PRINT_OUTPUT to print the results to check for the functionality of the functions(may be slow)
*Also the correct results are printed for the current values which are calculated from the orignal library 
and also were checked by hand
*/
 float32_t src_buf_f32[MAX_BLOCKSIZE] =
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
/*another as the other one overflow in rms q31*/
q31_t srcB_buf_q31[MAX_BLOCKSIZE] =
{
   0x75122542,  0,   0x15654473,
   0,  0x118854C3,   0,
   0x5264E236,  0x0B200590,   0x83059A77,
   0x91129713,  0x3399D123,   0x2518BBD1,
   0x1AB2037A,  0,   0x6F237531,
   0,  0x01213159,   0xF123CD77,
   0xAC475456,  0x6688A12E,   0x76A2FF96,
   0x54761269,  0x8756D438,   0x36B3B697,
   0x22459643,  0x3373C368,   0,
   0x610A0540,  0,   0x350112B5,
   0,  0x4469AA15
};
 q7_t src_buf_q7[MAX_BLOCKSIZE] =
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

 q15_t src_buf_q15[MAX_BLOCKSIZE] =
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

 q31_t src_buf_q31[MAX_BLOCKSIZE] =
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




/* Final ouput */
float32_t result_f32;  
q7_t result_q7;
q15_t result_q15;
q31_t result_q31;
q63_t result_q63;
uint32_t result_index = 0 ;

int32_t main(void)
{
 /*Init*/ 
  set_pin_function(5, FUNC_GPIO);
  set_gpio_pin_direction(5, DIR_OUT);
  set_pin_function(6, FUNC_GPIO);
  set_gpio_pin_direction(6, DIR_OUT);
  CLR_GPIO_5();
  CLR_GPIO_6();
/*Tests*/

/*Max*/
  SET_GPIO_6();
  riscv_max_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32, &result_index);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_max_f32\nvalue = %d index = %d\n",(int)(result_f32*100),result_index);
#endif
  SET_GPIO_5();
  riscv_max_q7(src_buf_q7, MAX_BLOCKSIZE, &result_q7, &result_index);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_max_q7\nvalue = 0x%X index = %d\n",result_q7,result_index);
#endif  
  SET_GPIO_6();
  riscv_max_q15(src_buf_q15, MAX_BLOCKSIZE, &result_q15, &result_index);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_max_q15\nvalue = 0x%X index = %d\n",result_q15,result_index);
#endif
  SET_GPIO_5();
  riscv_max_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q31, &result_index);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_max_q31\nvalue = 0x%X index = %d\n",result_q31,result_index);
#endif

/*Mean*/
  SET_GPIO_6();
  riscv_mean_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_mean_f32\nvalue = %d\n",(int)(result_f32*100));
#endif
  SET_GPIO_5();
  riscv_mean_q7(src_buf_q7, MAX_BLOCKSIZE, &result_q7);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_mean_q7\nvalue = 0x%X\n",result_q7);
#endif  
  SET_GPIO_6();
  riscv_mean_q15(src_buf_q15, MAX_BLOCKSIZE,&result_q15);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_mean_q15\nvalue = 0x%X\n",result_q15);
#endif
  SET_GPIO_5();
  riscv_mean_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q31);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_mean_q31\nvalue = 0x%X\n",result_q31);
#endif

/*Min*/
  SET_GPIO_6();
  riscv_min_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32, &result_index);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("arm_min_f32\nvalue = %d index = %d\n",(int)(result_f32*100),result_index);
#endif
  SET_GPIO_5();
  riscv_min_q7(src_buf_q7, MAX_BLOCKSIZE, &result_q7, &result_index);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_min_q7\nvalue = 0x%X index = %d\n",result_q7,result_index);
#endif  
  SET_GPIO_6();
  riscv_min_q15(src_buf_q15, MAX_BLOCKSIZE, &result_q15, &result_index);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_min_q15\nvalue = 0x%X index = %d\n",result_q15,result_index);
#endif
  SET_GPIO_5();
  riscv_min_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q31, &result_index);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_min_q31\nvalue = 0x%X index = %d\n",result_q31,result_index);
#endif

/*Power*/
  SET_GPIO_6();
  riscv_power_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_power_f32\nvalue = %d\n",(int)(result_f32*100));
#endif
  SET_GPIO_5();
  riscv_power_q7(src_buf_q7, MAX_BLOCKSIZE, &result_q31);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_power_q7\nvalue = 0x%X\n",result_q31);
#endif  
  SET_GPIO_6();
  riscv_power_q15(src_buf_q15, MAX_BLOCKSIZE,&result_q63);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_power_q15\n");
  int * ptr = &result_q63;
  ptr++;
  printf("0x%X",*(ptr--));
  printf("%X\n",*(ptr));
 
#endif
  SET_GPIO_5();
  riscv_power_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q63);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_power_q31\n");
  ptr = &result_q63;
  ptr++;
  printf("0x%X",*(ptr--));
  printf("%X\n",*(ptr));
#endif

/*RMS*/
  SET_GPIO_6();
  riscv_rms_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_rms_f32\nvalue = %d\n",(int)(result_f32*100));
#endif
  SET_GPIO_5();
  riscv_rms_q15(src_buf_q15, MAX_BLOCKSIZE,&result_q15);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_rms_q15\nvalue = 0x%X\n",result_q15);
#endif
  SET_GPIO_6();
  riscv_rms_q31(srcB_buf_q31, MAX_BLOCKSIZE, &result_q31);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_rms_q31\nvalue = 0x%X\n",result_q31);
#endif

/*Standard deviation*/
  SET_GPIO_5();
  riscv_std_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_std_f32\nvalue = %d\n",(int)(result_f32*100));
#endif
  SET_GPIO_6();
  riscv_std_q15(src_buf_q15, MAX_BLOCKSIZE,&result_q15);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_std_q15\nvalue = 0x%X\n",result_q15);
#endif
  SET_GPIO_5();
  riscv_std_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q31);
  CLR_GPIO_5();
#ifdef PRINT_OUTPUT
  printf("riscv_std_q31\nvalue = 0x%X\n",result_q31);
#endif

/*Variance*/
  SET_GPIO_6();
  riscv_var_f32(src_buf_f32, MAX_BLOCKSIZE, &result_f32);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_var_f32\nvalue = %d\n",(int)(result_f32*100));
#endif
  SET_GPIO_5();
  riscv_var_q15(src_buf_q15, MAX_BLOCKSIZE,&result_q15);
  set_gpio_pin_value(4, 0);
#ifdef PRINT_OUTPUT
  printf("riscv_var_q15\nvalue = 0x%X\n",result_q15);
#endif
  SET_GPIO_6();
  riscv_var_q31(src_buf_q31, MAX_BLOCKSIZE, &result_q31);
  CLR_GPIO_6();
#ifdef PRINT_OUTPUT
  printf("riscv_var_q31\nvalue = 0x%X\n",result_q31);
#endif
  printf("End\n");
  return 0 ;
}

 

