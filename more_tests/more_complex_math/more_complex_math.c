#include <math.h>
#include <stdio.h>
#include "riscv_math.h"
#include "more_complex_math_data.h"
#include "ref/ref.h"
#include "math_helper.h"
float32_t result_f32[2*BLOCK_SIZE_F32];  /* Final ouput */
q7_t result_q7[2*BLOCK_SIZE_Q7];
q15_t result_q15[2*BLOCK_SIZE_Q15];
q31_t result_q31[2*BLOCK_SIZE_Q31];

float32_t ref_result_f32[2*BLOCK_SIZE_F32];  /* Final ouput */
q7_t ref_result_q7[2*BLOCK_SIZE_Q7];
q15_t ref_result_q15[2*BLOCK_SIZE_Q15];
q31_t ref_result_q31[2*BLOCK_SIZE_Q31];

q15_t real_q15,img_q15;
q31_t real_q31,img_q31;
q63_t real_q63,img_q63;
float32_t real_f32,img_f32;

q15_t ref_real_q15,ref_img_q15;
q31_t ref_real_q31,ref_img_q31;
q63_t ref_real_q63,ref_img_q63;
float32_t ref_real_f32,ref_img_f32;

int is_equal = 0 ;
int snr_val = 0;

void main(void)
{
 riscv_float_to_q7(Arr_f32,Arr_q7, 30);
 riscv_float_to_q15(Arr_f32,Arr_q15, 30);
 riscv_float_to_q31(Arr_f32,Arr_q31, 30);

/*Conjugate*/

  printf("Testing Conjugate...\n");

  riscv_cmplx_conj_f32( Arr_f32,result_f32,BLOCK_SIZE_F32);
  ref_cmplx_conj_f32( Arr_f32,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,2*BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_conj_q15( Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_cmplx_conj_q15( Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,2*BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_cmplx_conj_q31( Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_cmplx_conj_q31( Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,2*BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

/*Magnitude*/

  printf("Testing Magnitude...\n");

  riscv_cmplx_mag_f32( Arr_f32,result_f32,BLOCK_SIZE_F32);
  ref_cmplx_mag_f32( Arr_f32,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,2*BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_mag_q15( Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_cmplx_mag_q15( Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);
 
  riscv_q15_to_float(result_q15,result_f32,2*BLOCK_SIZE_Q15);
  riscv_q15_to_float(ref_result_q15,ref_result_f32, 2*BLOCK_SIZE_Q15);
  snr_val = riscv_snr_f32(result_f32,ref_result_f32,2*BLOCK_SIZE_Q15);
  if(snr_val >= COMPLEX_MATH_SNR_THRESHOLD_q15_t) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_mag_q31( Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_cmplx_mag_q31( Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  riscv_q31_to_float(result_q31,result_f32, BLOCK_SIZE_Q31);
  riscv_q31_to_float(ref_result_q31,ref_result_f32, BLOCK_SIZE_Q31);
  snr_val = riscv_snr_f32(result_f32,ref_result_f32,BLOCK_SIZE_Q31);
  if(snr_val >= COMPLEX_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");


/*Magnitude Squared*/

  printf("Testing Squared...\n");

  riscv_cmplx_mag_squared_f32( Arr_f32,result_f32,BLOCK_SIZE_F32);
  ref_cmplx_mag_squared_f32( Arr_f32,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,2*BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_mag_squared_q15( Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_cmplx_mag_squared_q15( Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,2*BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_cmplx_mag_squared_q31( Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_cmplx_mag_squared_q31( Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,2*BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
/*Complex Mult*/
  printf("Testing Complex Mult...\n");

  riscv_cmplx_mult_cmplx_f32( Arr_f32,Arr_f32,result_f32,BLOCK_SIZE_F32);
  ref_cmplx_mult_cmplx_f32( Arr_f32,Arr_f32,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,2*BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_mult_cmplx_q15( Arr_q15,Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_cmplx_mult_cmplx_q15( Arr_q15,Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,2*BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_cmplx_mult_cmplx_q31( Arr_q31,Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_cmplx_mult_cmplx_q31( Arr_q31,Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  CHECK_EQ(result_q31,ref_result_q31,2*BLOCK_SIZE_Q31,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");
/*Real Mult*/
  printf("Testing Real Mult...\n");

  riscv_cmplx_mult_real_f32( Arr_f32,Arr_f32,result_f32,BLOCK_SIZE_F32);
  ref_cmplx_mult_real_f32( Arr_f32,Arr_f32,ref_result_f32,BLOCK_SIZE_F32);

  CHECK_EQ(result_f32,ref_result_f32,2*BLOCK_SIZE_F32,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_mult_real_q15( Arr_q15,Arr_q15,result_q15,BLOCK_SIZE_Q15);
  ref_cmplx_mult_real_q15( Arr_q15,Arr_q15,ref_result_q15,BLOCK_SIZE_Q15);

  CHECK_EQ(result_q15,ref_result_q15,2*BLOCK_SIZE_Q15,is_equal);
  if(is_equal) printf("OK\n");
  else printf("Failed\n");


  riscv_cmplx_mult_real_q31( Arr_q31,Arr_q31,result_q31,BLOCK_SIZE_Q31);
  ref_cmplx_mult_real_q31( Arr_q31,Arr_q31,ref_result_q31,BLOCK_SIZE_Q31);

  riscv_q31_to_float(result_q31,result_f32, BLOCK_SIZE_Q31);
  riscv_q31_to_float(ref_result_q31,ref_result_f32, BLOCK_SIZE_Q31);
  snr_val = riscv_snr_f32(result_f32,ref_result_f32,BLOCK_SIZE_Q31);
  if(snr_val >= COMPLEX_MATH_SNR_THRESHOLD_q31_t) printf("OK\n");
  else printf("Failed\n");
/*Dot Product*/
  printf("Testing Complex Dot Product...\n");
  riscv_cmplx_dot_prod_f32( Arr_f32,Arr_f32,BLOCK_SIZE_F32,&real_f32,&img_f32);
  ref_cmplx_dot_prod_f32( Arr_f32,Arr_f32,BLOCK_SIZE_F32,&ref_real_f32,&ref_img_f32);

  if ( (abs(real_f32 - ref_real_f32) < DELTA_F) && (abs(img_f32 - ref_img_f32) < DELTA_F))printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_dot_prod_q15( Arr_q15,Arr_q15,BLOCK_SIZE_Q15,&real_q31,&img_q31);
  ref_cmplx_dot_prod_q15( Arr_q15,Arr_q15,BLOCK_SIZE_Q15,&ref_real_q31,&ref_img_q31);

  if ( (real_q15 == ref_real_q15) && (img_q15 == ref_img_q15))printf("OK\n");
  else printf("Failed\n");

  riscv_cmplx_dot_prod_q31( Arr_q31,Arr_q31,BLOCK_SIZE_Q31,&real_q63,&img_q63);
  ref_cmplx_dot_prod_q31( Arr_q31,Arr_q31,BLOCK_SIZE_Q31,&ref_real_q63,&ref_img_q63);

  if ( (real_q15 == ref_real_q15) && (img_q15 == ref_img_q15))printf("OK\n");
  else printf("Failed\n");
//  for(int j =0 ; j <30;j++)
//   printf("%X   %X\n",result_q31[j],ref_result_q31[j]);


  printf("Testing Complex Math Done\n");
}
