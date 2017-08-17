#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/
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
void perf_enable_id( int eventid){
  cpu_perf_conf_events(SPR_PCER_EVENT_MASK(eventid));
  cpu_perf_conf(SPR_PCMR_ACTIVE | SPR_PCMR_SATURATE);
};

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
 float32_t src_real_f32[NUM_SAMPLES] =
{
   1.7491401329284098,  0.1325982188803279,   0.3252281811989881,
  -0.7938091410349637,  0.3149236145048914,  -0.5272704888029532,
   0.9322666565031119,  1.1646643544607362,  -2.0456694357357357,
  -0.6443728590041911,  1.7410657940825480,   0.4867684246821860,
   1.0488288293660140,  1.4885752747099299,   1.2705014969484090,
  -1.8561241921210170
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
 q15_t src_real_q15[NUM_SAMPLES] =
{
   0x7512,  0x1375,   0x1565,
   0x44C3,  0x1188,   0x1CA1,
   0x5264,  0x0B20,   0x8305,
   0x9112,  0x3399,   0x2518,
   0x1AB2,  0x4D01,   0x6F23,
   0x26FF
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
 q31_t src_real_q31[NUM_SAMPLES] =
{
   0x75122542,  0x1375A138,   0x15654473,
   0x44C34463,  0x118854C3,   0x1CA18291,
   0x5264E236,  0x0B200590,   0x83059A77,
   0x91129713,  0x3399D123,   0x2518BBD1,
   0x1AB2037A,  0x4D01D1F1,   0x6F237531,
   0x26FF5569
};


/* Final ouput */
float32_t result_f32[MAX_BLOCKSIZE];  
q7_t result_q7[MAX_BLOCKSIZE];
q15_t result_q15[MAX_BLOCKSIZE];
q31_t result_q31[MAX_BLOCKSIZE];
q63_t result_q63[MAX_BLOCKSIZE];
q15_t real_q15,img_q15;
q31_t real_q31,img_q31;
q63_t real_q63,img_q63;
float32_t real_f32,img_f32;
int i= 0;

int32_t main(void)
{
/*Tests*/
/*Complex Conjugate*/
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_conj_f32( srcA_buf_f32,result_f32,NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_conj_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("%d + i%d\n",(int)(result_f32[i]*100),(int)(result_f32[i+1]*100));  
    }
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_conj_q31(srcA_buf_q31,result_q31,NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_conj_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + 0xi%X\n",result_q31[i],result_q31[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x75122542 + 0xiEC8A5EC8\n\
0x15654473 + 0xiBB3CBB9D\n\
0x118854C3 + 0xiE35E7D6F\n\
0x5264E236 + 0xiF4DFFA70\n\
0x83059A77 + 0xi6EED68ED\n\
0x3399D123 + 0xiDAE7442F\n\
0x1AB2037A + 0xiB2FE2E0F\n\
0x6F237531 + 0xiD900AA97\n\
0x01213159 + 0xi0EDC3289\n\
0xAC475456 + 0xi99775ED2\n\
0x76A2FF96 + 0xiAB89ED97\n\
0x8756D438 + 0xiC94C4969\n\
0x22459643 + 0xiCC8C3C98\n\
0xE1A901D3 + 0xi9EF5FAC0\n\
0x54199643 + 0xiCAFEED4B\n\
0x91000436 + 0xiBB9655EB");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_conj_q15(srcA_buf_q15,result_q15,NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_conj_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + 0xi%X\n",result_q15[i],result_q15[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x7512 + 0xiEC8B\n\
0x1565 + 0xiBB3D\n\
0x1188 + 0xiE35F\n\
0x5264 + 0xiF4E0\n\
0x8305 + 0xi6EEE\n\
0x3399 + 0xiDAE8\n\
0x1AB2 + 0xiB2FF\n\
0x6F23 + 0xiD901\n\
0x0121 + 0xi0EDD\n\
0xAC47 + 0xi9978\n\
0x76A2 + 0xiAB8A\n\
0x8756 + 0xiC94D\n\
0x2245 + 0xiCC8D\n\
0xE1A9 + 0xi9EF6\n\
0x5419 + 0xiCAFF\n\
0x9100 + 0xiBB97");
  printf("\n");
#endif
/*Complex Dot Product*/
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_dot_prod_f32( srcA_buf_f32,srcB_buf_f32,NUM_SAMPLES,&real_f32,&img_f32);
  perf_stop();
  printf("riscv_cmplx_dot_prod_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("%d + i%d\n",(int)(real_f32*100),(int)(img_f32*100));  
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_dot_prod_q31(srcA_buf_q31,srcB_buf_q31,NUM_SAMPLES,&real_q63,&img_q63);
  perf_stop();
  printf("riscv_cmplx_dot_prod_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  int * ptr = &real_q63;
  ptr++;
  printf("0x%X",*(ptr--));
  printf("%X + ",*(ptr));
  ptr = &img_q63;
  ptr++;
  printf("i0x%X",*(ptr--));
  printf("%X \n",*(ptr));
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x2927C892B96BC + i0x22D6ABC4110A6 \n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_dot_prod_q15(srcA_buf_q15,srcB_buf_q15,NUM_SAMPLES,&real_q31,&img_q31);
  perf_stop();
  printf("riscv_cmplx_dot_prod_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  printf("0x%X + 0xi%X\n",real_q31,img_q31);  
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x2927F77 + 0xi22D64D9 \n");
#endif

/*Complex Magnitude*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_f32(srcA_buf_f32,result_f32,NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mag_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("%d\n",(int)(result_f32[i]*100));  
    }
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_q31(srcA_buf_q31,result_q31,NUM_SAMPLES); //output 2.30
  perf_stop();
  printf("riscv_cmplx_mag_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("0x%X\n",result_q31[i]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x3B56AC71\n\
0x2401DB1B\n\
0x10C94A6F\n\
0x29922505\n\
0x538D9122\n\
0x1FC69AF1\n\
0x28C061FE\n\
0x3AE413B9\n\
0x07739631\n\
0x422FA7F2\n\
0x48D0C238\n\
0x423DA14E\n\
0x1EE923FE\n\
0x32D5F4A0\n\
0x31B46A01\n\
0x4131CA9E ");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_q15(srcA_buf_q15,result_q15,NUM_SAMPLES); //output 2.14
  perf_stop();
  printf("riscv_cmplx_mag_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("0x%X\n",result_q15[i]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x3B57\n\
0x23FF\n\
0x10C7\n\
0x298F\n\
0x538C\n\
0x1FC5\n\
0x28BF\n\
0x3AE5\n\
0x0773\n\
0x4230\n\
0x48D0\n\
0x423C\n\
0x1EE6\n\
0x32D5\n\
0x31B3\n\
0x4130");
  printf("\n");
#endif

/*Complex Magnitude Squared*/
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_squared_f32(srcA_buf_f32,result_f32,NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mag_squared_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("%d\n",(int)(result_f32[i]*100));  
    }
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_squared_q31(srcA_buf_q31,result_q31,NUM_SAMPLES);//output 3.29
  perf_stop();
  printf("riscv_cmplx_mag_squared_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("0x%X\n",result_q31[i]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x1B8221A8\n\
0x0A210B46\n\
0x02338F28\n\
0x0D804693\n\
0x368A34CE\n\
0x07E36736\n\
0x0CF95E65\n\
0x1B184847\n\
0x006F0CCE\n\
0x223936F1\n\
0x296C2EF7\n\
0x2247AC03\n\
0x0776F184\n\
0x14308CC0\n\
0x134D1F75\n\
0x2134A528");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mag_squared_q15(srcA_buf_q15,result_q15,NUM_SAMPLES); //output 3.13
  perf_stop();
  printf("riscv_cmplx_mag_squared_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < NUM_SAMPLES ; i++)
    {
      printf("0x%X\n",result_q15[i]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x1B82\n\
0x0A20\n\
0x0233\n\
0x0D7F\n\
0x368A\n\
0x07E3\n\
0x0CF9\n\
0x1B18\n\
0x006F\n\
0x2239\n\
0x296B\n\
0x2247\n\
0x0776\n\
0x1430\n\
0x134C\n\
0x2134");
  printf("\n");
#endif

/*Complex-by-Complex Multiplication*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_cmplx_f32(srcA_buf_f32, srcB_buf_f32, result_f32, NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mult_cmplx_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("%d + i%d\n",(int)(result_f32[i]*100),(int)(result_f32[i+1]*100));  
    }
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_cmplx_q31(srcA_buf_q31, srcB_buf_q31, result_q31, NUM_SAMPLES); //output 3.29
  perf_stop();
  printf("riscv_cmplx_mult_cmplx_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + i0x%X\n",result_q31[i],result_q31[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x1A0775AE + i0x08E623C8\n\
0xF7A8BA02 + i0x05BF3808\n\
0xFEFFD4B4 + i0x01F5F8F2\n\
0x0D048219 + i0x0394A41E\n\
0x06795278 + i0x36277EAA\n\
0x02833E88 + i0x077A3780\n\
0xF5CF4617 + i0x0807BBA2\n\
0x15277C41 + i0x10EE1CCA\n\
0xFF9239E2 + i0xFFEF368C\n\
0xF9280D95 + i0xDE77BCC8\n\
0x0D8E7C6F + i0x27243386\n\
0x16975CD3 + i0xE6379C6A\n\
0xFD1F9D50 + i0x06E35C5A\n\
0xEF67F462 + i0xF47FD9AA\n\
0x0853ADB5 + i0x1169A65A\n\
0x0EEC5730 + i0xE2563060");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_cmplx_q15(srcA_buf_q15, srcB_buf_q15, result_q15, NUM_SAMPLES); //output 3.13
  perf_stop();
  printf("riscv_cmplx_mult_cmplx_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + i0x%X\n",result_q15[i],result_q15[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x1A07 + i0x08E4\n\
0xF7A8 + i0x05BE\n\
0xFF00 + i0x01F4\n\
0x0D05 + i0x0394\n\
0x067A + i0x3628\n\
0x0284 + i0x0778\n\
0xF5D0 + i0x0806\n\
0x1527 + i0x10EC\n\
0xFF92 + i0xFFEE\n\
0xF928 + i0xDE76\n\
0x0D8E + i0x2722\n\
0x1697 + i0xE636\n\
0xFD20 + i0x06E2\n\
0xEF68 + i0xF47E\n\
0x0854 + i0x1168\n\
0x0EED + i0xE256");
  printf("\n");
#endif

/*Complex-by-Real Multiplication*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_real_f32(srcA_buf_f32, src_real_f32, result_f32, NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mult_real_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("%d + i%d\n",(int)(result_f32[i]*100),(int)(result_f32[i+1]*100));  
    }
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_real_q31(srcA_buf_q31, src_real_q31, result_q31, NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mult_real_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + i0x%X\n",result_q31[i],result_q31[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x6B132EAF + i0x11CC4791\n\
0x0340B1AC + i0x0A742D3E\n\
0x02EE3CDB + i0x04C9262C\n\
0x2C4349E2 + i0x05F9FBAE\n\
0xEEE1A1B5 + i0xF0CE525C\n\
0x0B8AC1DB + i0x084C3809\n\
0x112F147A + i0x3191DC02\n\
0x09A8D9C3 + i0x0363B2DE\n\
0xFEE5A277 + i0x0E8262D1\n\
0x488DFC15 + i0xA7246875\n\
0x2FD38281 + i0x220C8A77\n\
0xDD07C893 + i0x0FDA84BD\n\
0x0725CC38 + i0x0ABB1212\n\
0xEDBF3CA7 + i0x3A616865\n\
0x49057C4C + i0x2E059D0B\n\
0xDE2E9537 + i0x14D7D6A5");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cmplx_mult_real_q15(srcA_buf_q15, src_real_q15, result_q15, NUM_SAMPLES);
  perf_stop();
  printf("riscv_cmplx_mult_real_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  for(i = 0 ; i < MAX_BLOCKSIZE ; i+=2)
    {
      printf("0x%X + i0x%X\n",result_q15[i],result_q15[i+1]);  
    }
  printf("\n");
  printf("\nCorrect answer:\n");
  printf("0x6B12 + i0x11CB\n\
0x0340 + i0x0A73\n\
0x02EE + i0x04C9\n\
0x2C42 + i0x05F9\n\
0xEEE1 + i0xF0CE\n\
0x0B8A + i0x084B\n\
0x112E + i0x3190\n\
0x09A8 + i0x0363\n\
0xFEE5 + i0x0E83\n\
0x488E + i0xA724\n\
0x2FD2 + i0x220B\n\
0xDD08 + i0x0FD9\n\
0x0725 + i0x0ABA\n\
0xEDBF + i0x3A60\n\
0x4904 + i0x2E05\n\
0xDE2E + i0x14D7");
  printf("\n");
#endif

  printf("End\n");
  return 0 ;
}


