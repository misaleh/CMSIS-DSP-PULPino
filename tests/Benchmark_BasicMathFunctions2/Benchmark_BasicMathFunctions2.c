#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "bench.h"

#define PRINT_F32(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(X[i]*100)); \
printf("\n\n")
#define PRINT_Q(X,Y) printf("\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")


#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define MAX_BLOCKSIZE     32
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
*Also the correct results are printed for the current vectors which are calculated from the orignal library 
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


/* ouput variables */
float32_t result_f32[MAX_BLOCKSIZE];  
q7_t result_q7[MAX_BLOCKSIZE];
q15_t result_q15[MAX_BLOCKSIZE];
q31_t result_q31[MAX_BLOCKSIZE];
q63_t result_q63[MAX_BLOCKSIZE];
int i = 0 ;
int32_t main(void)
{
/*Tests*/
/*negate*/
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_negate_f32( srcA_buf_f32,result_f32,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_negate_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_negate_q7(srcA_buf_q7,result_q7,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_negate_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x8B 0xED 0xEB 0xBC 0xEF 0xE4 0xAE 0xF5 0x7D 0x6F 0xCD 0xDB 0xE6 0xB3 0x91 0xDA 0xFF 0x0F 0x54 0x9A 0x8A 0xAC 0x79 0xCA 0xDE 0xCD 0x1F 0x9F 0xAC 0xCB 0x6F 0xB7");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_negate_q15(srcA_buf_q15,result_q15,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_negate_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x8AEE 0xEC8B 0xEA9B 0xBB3D 0xEE78 0xE35F 0xAD9C 0xF4E0 0x7CFB 0x6EEE 0xCC67 0xDAE8 0xE54E 0xB2FF 0x90DD 0xD901 0xFEDF 0x0EDD 0x53B9 0x9978 0x895E 0xAB8A 0x78AA 0xC94D 0xDDBB 0xCC8D 0x1E57 0x9EF6 0xABE7 0xCAFF 0x6F00 0xBB97");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_negate_q31(srcA_buf_q31,result_q31,MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_negate_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x8AEDDABE 0xEC8A5EC8 0xEA9ABB8D 0xBB3CBB9D 0xEE77AB3D 0xE35E7D6F 0xAD9B1DCA 0xF4DFFA70 0x7CFA6589 0x6EED68ED 0xCC662EDD 0xDAE7442F 0xE54DFC86 0xB2FE2E0F 0x90DC8ACF 0xD900AA97 0xFEDECEA7 0x0EDC3289 0x53B8ABAA 0x99775ED2 0x895D006A 0xAB89ED97 0x78A92BC8 0xC94C4969 0xDDBA69BD 0xCC8C3C98 0x1E56FE2D 0x9EF5FAC0 0xABE669BD 0xCAFEED4B 0x6EFFFBCA 0xBB9655EB ");
  printf("\n");
#endif

/*offset*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_offset_f32(srcA_buf_f32, 1.2, result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_offset_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_offset_q7(srcA_buf_q7, 0x15, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_offset_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7F 0x28 0x2A 0x59 0x26 0x31 0x67 0x20 0x98 0xA6 0x48 0x3A 0x2F 0x62 0x7F 0x3B 0x16 0x06 0xC1 0x7B 0x7F 0x69 0x9C 0x4B 0x37 0x48 0xF6 0x76 0x69 0x4A 0xA6 0x5E");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_offset_q15(srcA_buf_q15, 0x225A, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_offset_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));


#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFF 0x35CF 0x37BF 0x671D 0x33E2 0x3EFB 0x74BE 0x2D7A 0xA55F 0xB36C 0x55F3 0x4772 0x3D0C 0x6F5B 0x7FFF 0x4959 0x237B 0x137D 0xCEA1 0x7FFF 0x7FFF 0x76D0 0xA9B0 0x590D 0x449F 0x55CD 0x0403 0x7FFF 0x7673 0x575B 0xB35A 0x66C3 ");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_offset_q31(srcA_buf_q31, 0x33457193, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_offset_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFFFFFF 0x46BB12CB 0x48AAB606 0x7808B5F6 0x44CDC656 0x4FE6F424 0x7FFFFFFF 0x3E657723 0xB64B0C0A 0xC45808A6 0x66DF42B6 0x585E2D64 0x4DF7750D 0x7FFFFFFF 0x7FFFFFFF 0x5A44C6FC 0x3466A2EC 0x24693F0A 0xDF8CC5E9 0x7FFFFFFF 0x7FFFFFFF 0x7FFFFFFF 0xBA9C45CB 0x69F9282A 0x558B07D6 0x66B934FB 0x14EE7366 0x7FFFFFFF 0x7FFFFFFF 0x68468448 0xC44575C9 0x77AF1BA8");
  printf("\n");
#endif

/*Scale*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_scale_f32(srcA_buf_f32, 1.2, result_f32, MAX_BLOCKSIZE);
perf_stop();
  printf("riscv_scale_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_scale_q7(srcA_buf_q7, 0x15,1, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_scale_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x26 0x06 0x06 0x16 0x05 0x09 0x1A 0x03 0xD6 0xDB 0x10 0x0C 0x08 0x19 0x24 0x0C 0x00 0xFB 0xE4 0x21 0x26 0x1B 0xD8 0x11 0x0B 0x10 0xF5 0x1F 0x1B 0x11 0xDB 0x17");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_scale_q15(srcA_buf_q15, 0x225A,1, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_scale_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));


#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x3ED6 0x0A71 0x0B7B 0x24E8 0x0968 0x0F5D 0x2C38 0x05F8 0xBCEA 0xC475 0x1BB1 0x13E8 0x0E54 0x2954 0x3BA6 0x14EE 0x09B 0xF805 0xD30F 0x3708 0x3FAC 0x2D55 0xBF3C 0x1D5C 0x1264 0x1B9D 0xEFB7 0x3415 0x2D23 0x1C73 0xC46B 0x24B7");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_scale_q31(srcA_buf_q31, 0x33457193,1, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_scale_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x5DC97D00 0x0F96D9D0 0x1123E9B0 0x37163708 0x0E0BA9A0 0x16EFC4F8 0x4201CB3C 0x08E996B0 0x9BE0DB58 0xA7227394 0x2956900C 0x1DB7F628 0x1562D210 0x3DB101E8 0x5908D8D4 0x1F3DC88C 0xE7AD1C 0xF41867F4 0xBCEDFF88 0x522421B0 0x5F0A9DEC 0x43A9BBD8 0x9F564498 0x2BD29054 0x1B74A394 0x293813D4 0xE7B1C1D0 0x4DBD4730 0x435FA490 0x2A765E20 0xA7139264 0x36CE6ED8");
  printf("\n");
#endif

/*Shift Left*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q7(srcA_buf_q7, 3, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q7(left): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7F 0x7F 0x7F 0x7F 0x7F 0x7F 0x7F 0x58 0x80 0x80 0x7F 0x7F 0x7F 0x7F 0x7F 0x7F 0x08 0x88 0x80 0x7F 0x7F 0x7F 0x80 0x7F 0x7F 0x7F 0x80 0x7F 0x7F 0x7F 0x80 0x7F");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q15(srcA_buf_q15, 2, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q15(left): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));


#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFF 0x4DD4 0x5594 0x7FFF 0x4620 0x7284 0x7FFF 0x2C80 0x8000 0x8000 0x7FFF 0x7FFF 0x6AC8 0x7FFF 0x7FFF 0x7FFF 0x0484 0xC48C 0x8000 0x7FFF 0x7FFF 0x7FFF 0x8000 0x7FFF 0x7FFF 0x7FFF 0x86A4 0x7FFF 0x7FFF 0x7FFF 0x8000 0x7FFF");
  printf("\n");
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q31(srcA_buf_q31, 1, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q31(left): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
  printf("\nCorrect answer:\n");
  printf("0x7FFFFFFF 0x26EB4270 0x2ACA88E6 0x7FFFFFFF 0x2310A986 0x39430522 0x7FFFFFFF 0x16400B20 0x80000000 0x80000000 0x6733A246 0x4A3177A2 0x356406F4 0x7FFFFFFF 0x7FFFFFFF 0x4DFEAAD2 0x24262B2 0xE2479AEE 0x80000000 0x7FFFFFFF 0x7FFFFFFF 0x7FFFFFFF 0x80000000 0x6D676D2E 0x448B2C86 0x66E786D0 0xC35203A6 0x7FFFFFFF 0x7FFFFFFF 0x6A02256A 0x80000000 0x7FFFFFFF ");
  printf("\n");

#endif

/*Shift Right*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q7(srcA_buf_q7, -3, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q7(right): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q15(srcA_buf_q15, -2, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q15(right): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));


#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_shift_q31(srcA_buf_q31, -1, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_shift_q31(right): %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif

/*sub*/

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sub_f32(srcA_buf_f32, srcB_buf_f32, result_f32, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_sub_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_F32(result_f32,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sub_q7(srcA_buf_q7, srcB_buf_q7, result_q7, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_sub_q7: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q7,MAX_BLOCKSIZE);
#endif
  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sub_q15(srcA_buf_q15, srcB_buf_q15, result_q15, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_sub_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q15,MAX_BLOCKSIZE);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_sub_q31(srcA_buf_q31, srcB_buf_q31, result_q31, MAX_BLOCKSIZE);
  perf_stop();
  printf("riscv_sub_q31: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));

#ifdef PRINT_OUTPUT
  PRINT_Q(result_q31,MAX_BLOCKSIZE);
#endif

  printf("\nEnd\n");
  return 0 ;
}

