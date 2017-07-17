## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/?sp-search=Mostafa#5599084384616448) is part of GSoC 2017 

This Repo is still under development.

The names of files and functions are renamed from arm to riscv, Check  examples or the tests for more clarification.

To use DSP extension, add `#define USE_DSP_RISCV` in riscv_math.h


### Ported Modules
1) Basic Math Functions

2) Fast Math Functions

3) Complex Math Functions

4) Statistics Functions

5) Matrix Functions

6) Support Function

### Benchmarks

ARM M4 Benchmarks were done with  Keil simulator(CM4_FP) and CMSISv5.

ARM M4 uses its DSP intsturctions by default.

#### BasicMath Functions

These Benchmarks are performed on vectors of size 32.

It can be seen that floating point arithmetic operations are very slow, the reason is that pulpino does not have a FPU unlike ARM M4.

Results from pulpino were compared with results from ARM m4 and also were checked by hand.

Improvements of DSP extension(Imp) = (Old - New_DSP)*100/Old


| Function        | Puplino Cycles           | Puplino DSP Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_abs_f32      | 142 | N/A| 221| 
| arm_abs_q7      | 261 |  172 (51.7%)| 297| 
| arm_abs_q15      | 220  |  183(16.8%)| 293| 
| arm_abs_q31      | 252  |  235 (6.7%)| 282| 
| arm_add_f32      | 5922  | N/A | 295| 
| arm_add_q7      | 370 | 337 (8.9%)| 109| 
| arm_add_q15      | 399 | 188 (52.9%)| 168| 
| arm_add_q31      | 539 | N/A | 290| 
| arm_dot_prod_q15      | 334 |185 (44.6%)  |144 | 
| arm_dot_prod_q31      | 589 |  N/A|504 | 
| arm_dot_prod_f32      | 11193 | N/A | 311| 
| arm_dot_prod_q7      | 140 | 55 (60.7%) | 144| 
| arm_mult_f32      | 5720 | N/A | 295| 
| arm_mult_q31      | 746 |  N/A| 354| 
| arm_mult_q15      | 302 | 203 (32.8%)| 238| 
| arm_mult_q7      | 300 | 234(22%) | 322 | 
| arm_negate_f32      | 139 | N/A | 221| 
| arm_negate_q7      | 122  |  N/A| 92| 
| arm_negate_q15      | 188  |  184(2.1%)| 132| 
| arm_negate_q31      | 236  | N/A | 216| 
| arm_offset_f32      | 5959 |  N/A| 223| 
| arm_offset_q7      | 336 | 230(31.5%)| 100| 
| arm_offset_q15      | 364 | 150(58.8%) | 133| 
| arm_offset_q31      | 545 | N/A | 216| 
| arm_scale_f32      | 5810 | N/A | 191| 
| arm_scale_q7      | 375 | 205(45.3%) | 295| 
| arm_scale_q15      | 404 | 205(49.3%) | 246| 
| arm_scale_q31      | 621 | N/A | 379| 
| arm_shift_q7 (left)     | 338 |  189(44.1%) |300 | 
| arm_shift_q15 (left)     | 370 |  184(50.3%) | 236| 
| arm_shift_q31 (left)      | 640|  N/A | 369| 
| arm_shift_q7 (right)      | 140 |  120(14.3%) |246 | 
| arm_shift_q15 (right)      | 138 |  135(2.2%)| 203| 
| arm_shift_q31 (right)      | 143|  N/A |236 | 
| arm_sub_f32      | 6033 | N/A |295 | 
| arm_sub_q7      | 395 |  337(14.7%)| 109| 
| arm_sub_q15      | 425 | 189(55.5%) | 168| 
| arm_sub_q31      | 619 | N/A | 290| 

####FastMath Functions

Fast Math Functions doesn't operate on vectors.

Test cases are the same in the two benchmarks.

All Sin and Cosine functions uses tables but with interpolation.

sqrt for float use the built in sqrt function while sqrt for fixed point use  Newton-Raphson algorithm.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_sqrt_f32      | 1404 |N/A  | 26| 
| arm_sqrt_q15      | 700 |  N/A| 83| 
| arm_sqrt_q31      | 655 | N/A | 131| 
| arm_cos_f32      | 2102 |  N/A| 53| 
| arm_cos_q15      | 37|N/A  | 36| 
| arm_cos_q31      | 42 |  N/A| 46| 
| arm_sin_f32      | 2015 |  N/A| 55| 
| arm_sin_q15      | 29 |  N/A| 30| 
| arm_sin_q31      | 38 |  N/A| 43| 

####Complex Math Functions

These Benchmarks are performed on vectors of size 32 (16 complex numbers).

Most of the optimizations are in q15 data type, as float is not supported, and also q31 can't use many from the extended instructions as SIMD instructions and clip.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_cmplx_conj_f32      |  125|  N/A|181 | 
| arm_cmplx_conj_q31      |  171|  N/A| 173| 
| arm_cmplx_conj_q15      |  184|141(23.4%) | 142| 
| arm_cmplx_dot_prod_f32      | 22757| N/A |379 | 
| arm_cmplx_dot_prod_q31      | 1013| N/A| 796| 
| arm_cmplx_dot_prod_q15      | 492 |N/A  |294 | 
| arm_cmplx_mag_f32      |  31288| N/A | 512| 
| arm_cmplx_mag_q31      |  11147|  N/A| 2367| 
| arm_cmplx_mag_q15      |  11514| 11335(1.6%)| 1430| 
| arm_cmplx_mag_squared_f32      | 8440 |N/A |181 | 
| arm_cmplx_mag_squared_q31      | 265 |  N/A| 324| 
| arm_cmplx_mag_squared_q15      |  233| 88(62.2%)| 134| 
| arm_cmplx_mult_cmplx_f32      | 17102|  N/A| 364| 
| arm_cmplx_mult_cmplx_q31      |562 | N/A| 366| 
| arm_cmplx_mult_cmplx_q15      |  268|204 (23.9%)  | 366| 
| arm_cmplx_mult_real_f32      | 5689 |  N/A| 236| 
| arm_cmplx_mult_real_q31      |  717|  N/A| 302| 
| arm_cmplx_mult_real_q15      |  302| 203(32.8%) | 211| 

####Statistics Functions

These Benchmarks are performed on vectors of size 32.

Functions that need square root operations, use the functions from FastMath as rms and var.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_max_f32      | 7361 | N/A | 322| 
| arm_max_q7      |  296| N/A| 283| 
| arm_max_q15       |  294|  N/A|283 | 
| arm_max_q31        | 266 |  N/A|283 | 
| arm_mean_f32       | 6323 |   N/A|172 | 
| arm_mean_q7       | 136 |  74(45.6%)| 146| 
| arm_mean_q15       | 136 |  102(25%) |148 | 
| arm_mean_q31        | 403 |  N/A|397 | 
| arm_min_f32       | 7246 | N/A |322 | 
| arm_min_q7        | 291 | N/A | 283| 
| arm_min_q15       | 291 |N/A  | 283| 
| arm_min_q31       | 259 |  N/A| 283| 
| arm_power_f32      | 11005 |  N/A| 232| 
| arm_power_q7        | 106 | 43(59.4%)| 109| 
| arm_power_q15       | 300 | 165(45%) | 107| 
| arm_power_q31      | 556 |N/A  | 434| 
| arm_rms_f32       | 12923 |  N/A| 268| 
| arm_rms_q15        | 1132 |  993(12.3%)| 386| 
| arm_rms_q31      | 1345 | N/A | 765| 
| arm_std_f32        | 19715 |N/A  | 345| 
| arm_std_q15       | 1309 |  1154(11.8%)| 628| 
| arm_std_q31      | 1807 | N/A | 1121| 
| arm_var_f32       | 18177 | N/A | 455| 
| arm_var_q15      | 641 | 489(23.7%) | 551| 
| arm_var_q31       | 1142 | N/A | 993| 

####Matrix Functions

These Benchmarks are performed on matrices of size 4x4.

Functions with Fast version, use results or accumulators of size 32 bits instead of 64 bits. For more information refer to [
CMSIS DSP Software Library
]( http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_mat_add_f32      | 2843 | N/A|190  | 
| arm_mat_add_q15      | 214 | 112(47.7%)|124|
| arm_mat_add_q31       |  318|N/A | 172|
| arm_mat_cmplx_mult_f32        | 78646 | N/A|1992  | 
| arm_mat_cmplx_mult_q15       | 2981 | 2402(19.4%) |1606 | 
| arm_mat_cmplx_mult_q31      | 3543 | N/A|3516 | 
| arm_mat_inverse_f32      | 35145 |N/A |2994 |
| arm_mat_inverse_f64       | 62500 | N/A |20765 | 
| arm_mat_mult_f32      | 16701 |  N/A |1067 | 
| arm_mat_mult_q15      | 1106 | 858(22.4%) |946|
| arm_mat_mult_q31       | 1278 | N/A |1531 | 
| arm_mat_mult_fast_q15      | 844 |635(24.8%) |743 | 
| arm_mat_mult_fast_q31      | 1094 |  N/A|643| 
| arm_mat_scale_f32       | 2138 | N/A |144 | 
| arm_mat_scale_q15      | 216 |  117(45.8%)|156 | 
| arm_mat_scale_q31      | 312 | N/A |227 | 
| arm_mat_sub_f32       | 2410 | N/A |190| 
| arm_mat_sub_q15     | 216 | 112(48.1%)  |122 | 
| arm_mat_sub_q31     |  334| N/A  |172|
| arm_mat_trans_f32       |  113| N/A |268 | 
| arm_mat_trans_q15       |  113| N/A |202|
| arm_mat_trans_q31       |  114| N/A | 262 |


####Filtering Functions

These Benchmarks are performed on vectors of size 32.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|   arm_biquad_cas_df1_32x64_q31    |  3855|  |  | 
|    arm_biquad_cascade_df1_f32   | 50713 |  |  | 
|  arm_biquad_cascade_df1_q15    |  1778|  |  | 
|   arm_biquad_cascade_df1_q31   |  2194|  |  | 
|    arm_biquad_cascade_df1_fast_q15   | N/A |  |  | 
|   arm_biquad_cascade_df1_fast_q31    | 1818 |  |  | 
|     arm_biquad_cascade_df2T_f32  |  51629|  |  | 
|  arm_biquad_cascade_df2T_f64    | 77806 |  |  | 
|    arm_biquad_cascade_stereo_df2T_f32   |113130 |  |  | 
|    arm_conv_f32   |  375567|  |  | 
|     arm_conv_q7  |  18594|  |  | 
|  arm_conv_q15     | 25044 |  |  | 
|   arm_conv_q31   |  27919|  |  | 
|     arm_conv_opt_q7  | N/A |  |  | 
|     arm_conv_fast_q15  | N/A |  |  | 
|    arm_conv_opt_q15    | N/A |  |  | 
|    arm_conv_fast_opt_q15   | N/A |  |  | 
|    arm_conv_fast_q31   |  10084|  |  | 
|   arm_conv_partial_f32    | 374605 |  |  | 
|    arm_conv_partial_q7   |  18114|  |  | 
|     arm_conv_partial_q15  |  24554|  |  | 
|     arm_conv_partial_q31  | 27428 |  |  | 
|   arm_conv_partial_opt_q7   |  N/A|  |  | 
|   arm_conv_partial_fast_q15    | N/A |  |  | 
|    arm_conv_partial_opt_q15   | N/A |  |  | 
|  arm_conv_partial_fast_opt_q15   |  N/A|  |  | 
|    arm_conv_partial_fast_q31   | 9896 |  |  | 
