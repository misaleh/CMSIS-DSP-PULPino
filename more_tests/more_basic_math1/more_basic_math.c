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

q63_t ref_result_q63_1;
q31_t ref_result_q31_1;
float32_t ref_result_f32_1;

int is_equal = 0 ;
int snr_val = 0;

void main(void)
{
 riscv_float_to_q7(Arr_f32_2,Arr_q7, 15);
 riscv_float_to_q15(Arr_f32_2,Arr_q15, 15);
 riscv_float_to_q31(Arr_f32_2,Arr_q31, 15);
/*offset*/
  printf("Testing Offset...\n");

for(int j =0;j<4;j++){
  riscv_offset_f32( Arr_f32_2,eltsf[j],result_f32,15);
  ref_offset_f32( Arr_f32_2,eltsf[j],ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_offset_q7(Arr_q7,elts[j],result_q7,BLOCK_SIZE_Q7);
  ref_offset_q7(Arr_q7,elts[j],ref_result_q7,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_offset_q15(Arr_q15,elts[j],result_q15,BLOCK_SIZE_Q15);
  ref_offset_q15(Arr_q15,elts[j],ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_offset_q31(Arr_q31,elts2[j],result_q31,BLOCK_SIZE_Q31);
  ref_offset_q31(Arr_q31,elts2[j],ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
}
/*scale*/
/*offset*/
  printf("Testing Scale...\n");

for(int j =0;j<4;j++){
  riscv_scale_f32( Arr_f32_2,eltsf[j],result_f32,15);
  ref_scale_f32( Arr_f32_2,eltsf[j],ref_result_f32,15);

  CHECK_EQ(result_f32,ref_result_f32,15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_scale_q7(Arr_q7,elts[j],result_q7,1,BLOCK_SIZE_Q7);
  ref_scale_q7(Arr_q7,elts[j],ref_result_q7,1,BLOCK_SIZE_Q7);

  CHECK_EQ(result_q7,ref_result_q7,BLOCK_SIZE_Q7,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_scale_q15(Arr_q15,elts[j],1,result_q15,BLOCK_SIZE_Q15);
  ref_scale_q15(Arr_q15,elts[j],1,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_scale_q31(Arr_q31,elts2[j],1,result_q31,BLOCK_SIZE_Q31);
  ref_scale_q31(Arr_q31,elts2[j],1,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
}
/*Do product*/
  printf("Testing Dot Product...\n");
  riscv_dot_prod_f32(Arr_f32_2,Arr_f32_2,15,&result_f32_1);
  ref_dot_prod_f32(Arr_f32_2,Arr_f32_2,15,&ref_result_f32_1);
  if(ref_result_f32_1 == result_f32_1) printf("OK\n");
  else printf("Failed\n");

  riscv_dot_prod_q7(Arr_q7,Arr_q7,BLOCK_SIZE_Q7,&result_q31_1);
  ref_dot_prod_q7(Arr_q7,Arr_q7,BLOCK_SIZE_Q7,&ref_result_q31_1);
  if(ref_result_q31_1 == result_q31_1) printf("OK\n");
  else printf("Failed\n");


  riscv_dot_prod_q15(Arr_q15,Arr_q15,BLOCK_SIZE_Q15,&result_q63_1);
  ref_dot_prod_q15(Arr_q15,Arr_q15,BLOCK_SIZE_Q15,&ref_result_q63_1);
  if(ref_result_q63_1 == result_q63_1) printf("OK\n");
  else printf("Failed\n");


  riscv_dot_prod_q31(Arr_q31,Arr_q31,BLOCK_SIZE_Q31,&result_q63_1);
  ref_dot_prod_q31(Arr_q31,Arr_q31,BLOCK_SIZE_Q31,&ref_result_q63_1);
  if(ref_result_q63_1 == result_q63_1) printf("OK\n");
  else printf("Failed\n");

  printf("Testing Basic Math 1 Done\n");
}
