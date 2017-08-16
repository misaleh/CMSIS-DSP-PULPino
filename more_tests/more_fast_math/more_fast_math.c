#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "more_fast_math_data.h"
#include "ref/ref.h"
#include "math_helper.h"


int is_equal = 0 ;
int snr_val = 0;

void main(void)
{
/*sqrt*/
  printf("Testing Square Root...\n");
  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  riscv_sqrt_q15(fast_math_q15_inputs[j],&fast_math_output_q15_fut[j]);
  ref_sqrt_q15(fast_math_q15_inputs[j],&fast_math_output_q15_ref[j]);
}
  riscv_q15_to_float(fast_math_output_q15_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);
  riscv_q15_to_float(fast_math_output_q15_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);

  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);
  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");

  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  riscv_sqrt_q31(fast_math_q31_inputs[j],&fast_math_output_q31_fut[j]);
  ref_sqrt_q31(fast_math_q31_inputs[j],&fast_math_output_q31_ref[j]);
}
  riscv_q31_to_float(fast_math_output_q31_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);
  riscv_q31_to_float(fast_math_output_q31_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);


  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);

  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");

  else printf("Failed\n");

/*sine*/
  printf("Testing Sine...\n");
  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  fast_math_output_f32_fut[j] = riscv_sin_f32(fast_math_f32_inputs[j]);
  fast_math_output_f32_ref[j] = ref_sin_f32(fast_math_f32_inputs[j]);
}

  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);

  if(snr_val >= FAST_MATH_SNR_THRESHOLD_float32_t) printf("OK\n");

  else printf("Failed\n");

  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  fast_math_output_q15_fut[j] = riscv_sin_q15(fast_math_q15_inputs[j]);
  fast_math_output_q15_ref[j] = ref_sin_q15(fast_math_q15_inputs[j]);
}
  riscv_q15_to_float(fast_math_output_q15_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);
  riscv_q15_to_float(fast_math_output_q15_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);


  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);

  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q15_t) printf("OK\n");

  else printf("Failed\n");


  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  fast_math_output_q31_fut[j] = riscv_sin_q31(fast_math_q31_inputs[j]);
  fast_math_output_q31_ref[j] = ref_sin_q31(fast_math_q31_inputs[j]);
}
  riscv_q31_to_float(fast_math_output_q31_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);
  riscv_q31_to_float(fast_math_output_q31_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);

  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);
  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");


/*cosine*/
  printf("Testing Cosine...\n");
  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  fast_math_output_f32_fut[j] = riscv_cos_f32(fast_math_f32_inputs[j]);
  fast_math_output_f32_ref[j] = ref_cos_f32(fast_math_f32_inputs[j]);
}
  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);
  if(snr_val >= FAST_MATH_SNR_THRESHOLD_float32_t) printf("OK\n");
  else printf("Failed\n");

  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){

  fast_math_output_q15_fut[j] = riscv_cos_q15(fast_math_q15_inputs[j]);
  fast_math_output_q15_ref[j] = ref_cos_q15(fast_math_q15_inputs[j]);
}
  riscv_q15_to_float(fast_math_output_q15_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);

  riscv_q15_to_float(fast_math_output_q15_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);


  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);
  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");



  for(int j = 0 ; j < FAST_MATH_MAX_LEN ; j++){
  fast_math_output_q31_fut[j] = riscv_cos_q31(fast_math_q31_inputs[j]);
  fast_math_output_q31_ref[j] = ref_cos_q31(fast_math_q31_inputs[j]);
}
  riscv_q31_to_float(fast_math_output_q31_fut,fast_math_output_f32_fut, FAST_MATH_MAX_LEN);
  riscv_q31_to_float(fast_math_output_q31_ref,fast_math_output_f32_ref, FAST_MATH_MAX_LEN);

  snr_val = riscv_snr_f32(fast_math_output_f32_fut,fast_math_output_f32_ref,FAST_MATH_MAX_LEN);
  if(snr_val >= FAST_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");

printf("Testing Fast Math Done\n");
}
