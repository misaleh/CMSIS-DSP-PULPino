#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "more_basic_math_data.h"
#include "ref/ref.h"
#include "math_helper.h"
float32_t result_f32[BLOCK_SIZE_F32];  /* Final ouput */
q7_t result_q7[BLOCK_SIZE_Q7];
q15_t result_q15[BLOCK_SIZE_Q15];
q31_t result_q31[BLOCK_SIZE_Q31];

float32_t ref_result_f32[BLOCK_SIZE_F32];  /* Final ouput */
q7_t ref_result_q7[BLOCK_SIZE_Q7];
q15_t ref_result_q15[BLOCK_SIZE_Q15];
q31_t ref_result_q31[BLOCK_SIZE_Q31];

q63_t result_q63_1;
q31_t result_q31_1;
float32_t result_f32_1;

int is_equal = 0 ;
int snr_val = 0;

void main(void)
{
 riscv_float_to_q7(Arr_f32_2,Arr_q7, 15);
 riscv_float_to_q15(Arr_f32_2,Arr_q15, 15);
 riscv_float_to_q31(Arr_f32_2,Arr_q31, 15);

/*abs*/
  printf("Testing ABS...\n");
  riscv_abs_f32( Arr_f32_1,result_f32,BLOCK_SIZE_F32);
  ref_abs_f32( Arr_f32_1,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_abs_f32( Arr_f32_2,result_f32,15);
  ref_abs_f32( Arr_f32_2,ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_abs_q7(Arr_q7,result_q7,BLOCK_SIZE_Q7);
  ref_abs_q7(Arr_q7,ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_abs_q15(Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_abs_q15(Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_abs_q31(Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_abs_q31(Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

/*neg*/
  printf("Testing NEGATE...\n");
  riscv_negate_f32( Arr_f32_1,result_f32,BLOCK_SIZE_F32);
  ref_negate_f32( Arr_f32_1,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_negate_f32( Arr_f32_2,result_f32,15);
  ref_negate_f32( Arr_f32_2,ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_abs_q7(Arr_q7,result_q7,BLOCK_SIZE_Q7);
  ref_abs_q7(Arr_q7,ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");



  riscv_abs_q15(Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_abs_q15(Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_negate_q31(Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_negate_q31(Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
/*add*/
  printf("Testing ADD...\n");
  riscv_add_f32( Arr_f32_1,Arr_f32_1,result_f32,BLOCK_SIZE_F32);
  ref_add_f32( Arr_f32_1,Arr_f32_1,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_add_f32( Arr_f32_2,Arr_f32_2,result_f32,15);
  ref_add_f32( Arr_f32_2,Arr_f32_2,ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_add_q7(Arr_q7,Arr_q7,result_q7,BLOCK_SIZE_Q7);
  ref_add_q7(Arr_q7,Arr_q7,ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_add_q15(Arr_q15,Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_add_q15(Arr_q15,Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_add_q31(Arr_q31,Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_add_q31(Arr_q31,Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
/*mult*/
  printf("Testing MULT...\n");
  riscv_mult_f32( Arr_f32_1,Arr_f32_1,result_f32,BLOCK_SIZE_F32);
  ref_mult_f32( Arr_f32_1,Arr_f32_1,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_mult_f32( Arr_f32_2,Arr_f32_2,result_f32,15);
  ref_mult_f32( Arr_f32_2,Arr_f32_2,ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_mult_q7(Arr_q7,Arr_q7,result_q7,BLOCK_SIZE_Q7);
  ref_mult_q7(Arr_q7,Arr_q7,ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_mult_q15(Arr_q15,Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_mult_q15(Arr_q15,Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_mult_q31(Arr_q31,Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_mult_q31(Arr_q31,Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  riscv_q31_to_float(result_q31,result_f32, BLOCK_SIZE_Q31);
  riscv_q31_to_float(ref_result_q31,ref_result_f32, BLOCK_SIZE_Q31);

  snr_val = riscv_snr_f32(result_f32,ref_result_f32,BLOCK_SIZE_Q31);
  if(snr_val >= BASIC_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");


/*sub*/
  printf("Testing SUB...\n");
  riscv_sub_f32( Arr_f32_1,Arr_f32_1,result_f32,BLOCK_SIZE_F32);
  ref_sub_f32( Arr_f32_1,Arr_f32_1,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_sub_f32( Arr_f32_2,Arr_f32_2,result_f32,15);
  ref_sub_f32( Arr_f32_2,Arr_f32_2,ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_sub_q7(Arr_q7,Arr_q7,result_q7,BLOCK_SIZE_Q7);
  ref_sub_q7(Arr_q7,Arr_q7,ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_sub_q15(Arr_q15,Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_sub_q15(Arr_q15,Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_sub_q31(Arr_q31,Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_sub_q31(Arr_q31,Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

printf("Testing Basic Math 1 Done");
}
