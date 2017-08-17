#include "riscv_math.h"
#include "gpio.h" //for indication for benchmarking
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include <stdio.h>
#include "riscv_const_structs.h"
#include "bench.h"

#define EVENT_ID 0x00  /*number of cycles ID for benchmarking*/


#define PRINT_F32(X,Y) printf(Z"\n"); for(int i =0 ; i < (Y); i++) printf("%d  ",(int)(X[i]*100)); \
printf("\n\n")
#define PRINT_Q(X,Y) printf(Z"\n"); for(int i =0 ; i < (Y); i++) printf("0x%X  ",X[i]); \
printf("\n\n")
//#define PRINT_OUTPUT  /*for testing functionality for each function, removed while benchmarking*/
#define TEST_LENGTH_SAMPLES 128
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
#define CLR_GPIO_5() *(volatile int*) (GPIO_REG_PADOUT) = 0x00;


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

float32_t testInput_f32[TEST_LENGTH_SAMPLES] = 
{   
-0.865129623056441, 	0.000000000000000, 	-2.655020678073846, 	0.000000000000000, 	0.600664612949661, 	0.000000000000000, 	0.080378093886515, 	0.000000000000000, 	
-2.899160484012034, 	0.000000000000000, 	2.563004262857762, 	0.000000000000000, 	3.078328403304206, 	0.000000000000000, 	0.105906778385130, 	0.000000000000000, 	
0.048366940168201, 	0.000000000000000, 	-0.145696461188734, 	0.000000000000000, 	-0.023417155362879, 	0.000000000000000, 	2.127729174988954, 	0.000000000000000, 	
-1.176633086028377, 	0.000000000000000, 	3.690223557991855, 	0.000000000000000, 	-0.622791766173194, 	0.000000000000000, 	0.722837373872203, 	0.000000000000000, 	
2.739754205367484, 	0.000000000000000, 	-0.062610410524552, 	0.000000000000000, 	-0.891296810967338, 	0.000000000000000, 	-1.845872258871811, 	0.000000000000000, 	
1.195039415434387, 	0.000000000000000, 	-2.177388969045026, 	0.000000000000000, 	1.078649103637905, 	0.000000000000000, 	2.570976050490193, 	0.000000000000000, 	
-1.383551403404574, 	0.000000000000000, 	2.392141424058873, 	0.000000000000000, 	2.858002843205065, 	0.000000000000000, 	-3.682433899725536, 	0.000000000000000, 	
-3.488146646451150, 	0.000000000000000, 	1.323468578888120, 	0.000000000000000, 	-0.099771155430726, 	0.000000000000000, 	1.561168082500454, 	0.000000000000000, 	
1.025026795103179, 	0.000000000000000, 	0.928841900171200, 	0.000000000000000, 	2.930499509864950, 	0.000000000000000, 	2.013349089766430, 	0.000000000000000, 	
2.381676148486737, 	0.000000000000000, 	-3.081062307950236, 	0.000000000000000, 	-0.389579115537544, 	0.000000000000000, 	0.181540149166620, 	0.000000000000000, 	
-2.601953341353208, 	0.000000000000000, 	0.333435137783218, 	0.000000000000000, 	-2.812945856162965, 	0.000000000000000, 	2.649109640172910, 	0.000000000000000, 	
-1.003963025744654, 	0.000000000000000, 	1.552460768755035, 	0.000000000000000, 	0.088641345335247, 	0.000000000000000, 	-2.519951327113426, 	0.000000000000000, 	
-4.341348988610527, 	0.000000000000000, 	0.557772429359965, 	0.000000000000000, 	-1.671267412948494, 	0.000000000000000, 	0.733951350960387, 	0.000000000000000, 	
0.409263788034864, 	0.000000000000000, 	3.566033071952806, 	0.000000000000000, 	1.882565173848352, 	0.000000000000000, 	-1.106017073793287, 	0.000000000000000, 	
0.154456720778718, 	0.000000000000000, 	-2.513205795512153, 	0.000000000000000, 	0.310978660939421, 	0.000000000000000, 	0.579706500111723, 	0.000000000000000, 	
0.000086383683251, 	0.000000000000000, 	-1.311866980897721, 	0.000000000000000, 	1.840007477574986, 	0.000000000000000, 	-3.253005768451345, 	0.000000000000000, 
};

q15_t testInput_q15[TEST_LENGTH_SAMPLES] = 
{   
0x4212, 	0, 	0xA973, 	0,      0xDA07, 	0, 	0x3030, 	0, 	
0x557E, 	0, 	0xFF00, 	0, 	0x31B2, 	0, 	0x5643, 	0, 	
0xBD07, 	0, 	0x66E2, 	0, 	0xA110, 	0, 	0xB005, 	0, 	
0x8812, 	0, 	0x3127, 	0, 	0x6227, 	0, 	0x724A, 	0, 	
0x1982, 	0, 	0x456A, 	0, 	0x0017, 	0, 	0xC90C,  	0, 	
0xE000, 	0, 	0x221C, 	0, 	0x1044, 	0, 	0x2705, 	0, 	
0x1380, 	0, 	0x2F98, 	0, 	0x4190, 	0, 	0xF111, 	0, 	
0xBDDB, 	0, 	0x1000, 	0, 	0x0099, 	0, 	0x7442, 	0, 	
0xA1F1, 	0, 	0x3189, 	0, 	0x6987, 	0, 	0x5050, 	0, 	
0x5A42, 	0,      0x3109, 	0, 	0x2222, 	0, 	0x1815, 	0, 	
0x34A8, 	0, 	0x3334, 	0, 	0x5423, 	0, 	0x6971, 	0, 	
0x7779, 	0, 	0xA111, 	0, 	0x8297, 	0, 	0x5119, 	0, 	
0x1010, 	0, 	0x43A9, 	0,      0x55AA, 	0, 	0x0010, 	0, 	
0x4039, 	0, 	0x0020, 	0, 	0x3001, 	0, 	0x0010, 	0, 	
0x1523, 	0, 	0x3014, 	0, 	0x9797, 	0, 	0x5797, 	0, 	
0x0008, 	0, 	0x110C, 	0, 	0x3A42, 	0, 	0xA212, 	0, 
};
q31_t testInput_q31[TEST_LENGTH_SAMPLES] = 
{   
0x42120000, 	0, 	0xA9730000, 	0,      0xDA070000, 	0, 	0x30300000, 	0, 	
0x557E0000, 	0, 	0xFF000000, 	0, 	0x31B20000, 	0, 	0x56430000, 	0, 	
0xBD070000, 	0, 	0x66E20000, 	0, 	0xA1100000, 	0, 	0xB0050000, 	0, 	
0x88120000, 	0, 	0x31270000, 	0, 	0x62270000, 	0, 	0x724A0000, 	0, 	
0x19820000, 	0, 	0x456A0000, 	0, 	0x00170000, 	0, 	0xC90C0000,  	0, 	
0xE0000000, 	0, 	0x221C0000, 	0, 	0x10440000, 	0, 	0x27050000, 	0, 	
0x13800000, 	0, 	0x2F980000, 	0, 	0x41900000, 	0, 	0xF1110000, 	0, 	
0xBDDB0000, 	0, 	0x10000000, 	0, 	0x00990000, 	0, 	0x74420000, 	0, 	
0xA1F10000, 	0, 	0x31890000, 	0, 	0x69870000, 	0, 	0x50500000, 	0, 	
0x5A420000, 	0,      0x31090000, 	0, 	0x22220000, 	0, 	0x18150000, 	0, 	
0x34A80000, 	0, 	0x33340000, 	0, 	0x54230000, 	0, 	0x69710000, 	0, 	
0x77790000, 	0, 	0xA1110000, 	0, 	0x82970000, 	0, 	0x51190000, 	0, 	
0x10100000, 	0, 	0x43A90000, 	0,      0x55AA0000, 	0, 	0x00100000, 	0, 	
0x40390000, 	0, 	0x00200000, 	0, 	0x30010000, 	0, 	0x00100000, 	0, 	
0x15230000, 	0, 	0x30140000, 	0, 	0x97970000, 	0, 	0x57970000, 	0, 	
0x00080000, 	0, 	0x110C0000, 	0, 	0x3A420000, 	0, 	0xA2120000, 	0, 
};

uint32_t fftSize = 64;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;



int32_t main(void)
{
  //printf("bitrev =%d\n",doBitReverse);

  perf_reset();
  perf_enable_id(EVENT_ID);
  riscv_cfft_f32(&riscv_cfft_sR_f32_len64, testInput_f32, ifftFlag, doBitReverse);
  perf_stop();
  printf("riscv_cfft_f32: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_F32(testInput_f32,fftSize);
#endif

  perf_reset();
  perf_enable_id(EVENT_ID);	
  riscv_cfft_q15(&riscv_cfft_sR_q15_len64, testInput_q15, ifftFlag, doBitReverse);
  perf_stop();
  printf("riscv_cfft_q15: %s: %d\n", SPR_PCER_NAME(EVENT_ID),  cpu_perf_get(EVENT_ID));
#ifdef PRINT_OUTPUT
  PRINT_Q(testInput_q15,fftSize);
#endif


  printf("End\n");

 return 0;
}



