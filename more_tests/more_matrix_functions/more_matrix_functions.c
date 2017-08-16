#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "more_statistics_functions_data.h"
#include "ref/ref.h"
#include "math_helper.h"


int is_equal = 0 ;
int snr_val = 0;
float32_t result_f32;
q7_t result_q7;
q15_t result_q15;
q31_t result_q31;
q63_t result_q63;
uint32_t result_index;

float32_t ref_result_f32;
q7_t ref_result_q7;
q15_t ref_result_q15;
q31_t ref_result_q31;
q63_t ref_result_q63;
uint32_t ref_result_index;

void main(void)
{
 riscv_float_to_q7(Arr_f32,Arr_q7, BLOCK_SIZE_F32);
 riscv_float_to_q15(Arr_f32,Arr_q15, BLOCK_SIZE_F32);
 riscv_float_to_q31(Arr_f32,Arr_q31, BLOCK_SIZE_F32);
/*Max*/

  printf("Testing Max...\n");
  riscv_max_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32, &result_index);
  ref_max_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32, &ref_result_index);
  if((result_f32 == ref_result_f32)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");

  riscv_max_q7(Arr_q7, BLOCK_SIZE_Q7, &result_q7, &result_index);
  ref_max_q7(Arr_q7, BLOCK_SIZE_Q7, &ref_result_q7, &ref_result_index);
  if((result_q7 == ref_result_q7)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");



  riscv_max_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15, &result_index);
  ref_max_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15, &ref_result_index);
  if((result_q15 == ref_result_q15)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");


  riscv_max_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31, &result_index);
  ref_max_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31, &ref_result_index);
  if((result_q31 == ref_result_q31)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");




/*Min*/

  printf("Testing Min...\n");
  riscv_min_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32, &result_index);
  ref_min_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32, &ref_result_index);
  if((result_f32 == ref_result_f32)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");

  riscv_min_q7(Arr_q7, BLOCK_SIZE_Q7, &result_q7, &result_index);
  ref_min_q7(Arr_q7, BLOCK_SIZE_Q7, &ref_result_q7, &ref_result_index);
  if((result_q7 == ref_result_q7)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");



  riscv_min_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15, &result_index);
  ref_min_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15, &ref_result_index);
  if((result_q15 == ref_result_q15)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");


  riscv_min_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31, &result_index);
  ref_min_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31, &ref_result_index);
  if((result_q31 == ref_result_q31)&& (result_index == ref_result_index )) printf("OK\n");
  else printf("Failed\n");

/*Mean*/

  printf("Testing Mean...\n");
  riscv_mean_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32);
  ref_mean_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32);
  if(result_f32 == ref_result_f32) printf("OK\n");
  else printf("Failed\n");

  riscv_mean_q7(Arr_q7, BLOCK_SIZE_Q7, &result_q7);
  ref_mean_q7(Arr_q7, BLOCK_SIZE_Q7, &ref_result_q7);
  if(result_q7 == ref_result_q7) printf("OK\n");
  else printf("Failed\n");



  riscv_mean_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15);
  ref_mean_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15);
  if(result_q15 == ref_result_q15) printf("OK\n");
  else printf("Failed\n");


  riscv_mean_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31);
  ref_mean_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31);
  if(result_q31 == ref_result_q31) printf("OK\n");
  else printf("Failed\n");

/*power*/

  printf("Testing Power...\n");
  riscv_power_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32);
  ref_power_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32);

  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_float32_t) printf("OK\n");
  else printf("Failed\n");

  riscv_power_q7(Arr_q7, BLOCK_SIZE_Q7, &result_q31);
  ref_power_q7(Arr_q7, BLOCK_SIZE_Q7, &ref_result_q31);
  riscv_q31_to_float(&result_q31,&result_f32, 1);
  riscv_q31_to_float(&ref_result_q31,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");

  riscv_power_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q63);
  ref_power_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q63);
  if(result_q63 == ref_result_q63) printf("OK\n");
  else printf("Failed\n");

  riscv_power_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q63);
  ref_power_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q63);
  if(result_q63 == ref_result_q63) printf("OK\n");
  else printf("Failed\n");

/*RMS*/
  printf("Testing RMS...\n");
  riscv_rms_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32);
  ref_rms_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32);

  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_float32_t) printf("OK\n");
  else printf("Failed\n");


  riscv_rms_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15);
  ref_rms_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15);
  riscv_q15_to_float(&result_q15,&result_f32, 1);
  riscv_q15_to_float(&ref_result_q15,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");

  riscv_rms_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31);
  ref_rms_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31);
  riscv_q31_to_float(&result_q31,&result_f32, 1);
  riscv_q31_to_float(&ref_result_q31,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");


/*Std*/
  printf("Testing STD...\n");
  riscv_std_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32);
  ref_std_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32);

  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_float32_t) printf("OK\n");
  else printf("Failed\n");


  riscv_std_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15);
  ref_std_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15);
  riscv_q15_to_float(&result_q15,&result_f32, 1);
  riscv_q15_to_float(&ref_result_q15,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");

  riscv_std_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31);
  ref_std_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31);
  riscv_q31_to_float(&result_q31,&result_f32, 1);
  riscv_q31_to_float(&ref_result_q31,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");
/*var*/
  printf("Testing Var...\n");
  riscv_var_f32(Arr_f32, BLOCK_SIZE_F32, &result_f32);
  ref_var_f32(Arr_f32, BLOCK_SIZE_F32, &ref_result_f32);

  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_float32_t) printf("OK\n");
  else printf("Failed\n");


  riscv_var_q15(Arr_q15, BLOCK_SIZE_Q15, &result_q15);
  ref_var_q15(Arr_q15, BLOCK_SIZE_Q15, &ref_result_q15);
  riscv_q15_to_float(&result_q15,&result_f32, 1);
  riscv_q15_to_float(&ref_result_q15,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");

  riscv_var_q31(Arr_q31, BLOCK_SIZE_Q31, &result_q31);
  ref_var_q31(Arr_q31, BLOCK_SIZE_Q31, &ref_result_q31);
  riscv_q31_to_float(&result_q31,&result_f32, 1);
  riscv_q31_to_float(&ref_result_q31,&ref_result_f32, 1);
  snr_val = riscv_snr_f32(&result_f32,&ref_result_f32,1);
  if(snr_val >= STATISTICS_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");

printf("Testing Statistics Functions Done\n");
}
