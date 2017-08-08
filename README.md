## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/#5599084384616448) is part of Google Summer of Code (GSoC) 2017 

This Repo is still under development.

The names of files and functions are renamed from arm to riscv, Check  examples or the tests for more clarification.

To use DSP extension, add `#define USE_DSP_RISCV` in riscv_math.h

### Ported Modules
1) Basic Math Functions

2) Fast Math Functions

3) Complex Math Functions

4) Statistics Functions

5) Matrix Functions

6) Support Functions

7) Filtering  Functions

8) Transform Functions (CFFT only without suppressed Functions)

9) Controller Functions

#

### Benchmarks

ARM M4 Benchmarks were done with  Keil simulator(CM4_FP) and CMSISv5.

ARM M4 uses its DSP Instructions by default.

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
| arm_shift_q7 (right)      | 140 | 62(55.7%) |246 | 
| arm_shift_q15 (right)      | 138 |  135(2.2%)| 203| 
| arm_shift_q31 (right)      | 143|  N/A |236 | 
| arm_sub_f32      | 6033 | N/A |295 | 
| arm_sub_q7      | 395 |  337(14.7%)| 109| 
| arm_sub_q15      | 425 | 189(55.5%) | 168| 
| arm_sub_q31      | 619 | N/A | 290| 

#

#### FastMath Functions

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


#

#### Complex Math Functions

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

#

#### Statistics Functions

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

#

#### Matrix Functions

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

#

#### Filtering Functions

Operations are performed on vectors of size 32.

There is some functions with opt and/or fast keywords , for example there is 4 funcions for convolution for q15 data type, which are:

* Riscv_conv_q15

* Riscv_conv_fast_q15

* Riscv_conv_opt_q15

* Riscv_conv_fast_opt_q15


**Normal Functions**  without opt or fast keywords have slower implementation, use 64-bit accumulation to avoid wrap around.

**Fast functions** uses a 32-bit accumulator (may overflow).

From arm documentation:

>Fast versions are supported for Q31 and Q15. Cycles for Fast versions are less compared to Q31 and Q15 of conv and the design requires the input signals should be scaled down to avoid intermediate overflows.





**Opt** functions have faster implementation using scratch buffers  for optimization(but uses more memory), but with 64 bit accumulator.

From arm documentation:
 
>Opt versions are supported for Q15 and Q7. Design uses internal scratch buffer for getting good optimisation. These versions are optimised in cycles and consumes more memory(Scratch memory) compared to Q15 and Q7 versions 

**Opt fast functions** use scratch buffers and 32 bit accumulator. 


Most of Fast and opt functions are available only for DSP extension(as in ARM).

For more refer to [
CMSIS DSP Software Library
]( http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

Init functions are not considered in benchmarking as they are just copying a few pointers and will only be called once in real applications or can be bypassed by initializing the structure directly. 



| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|   arm_biquad_cas_df1_32x64_q31    |  3855| N/A |4200  | 
|    arm_biquad_cascade_df1_f32   | 50713 |N/A  |  661| 
|  arm_biquad_cascade_df1_q15    |  1778| 1164(35.5%) |  574| 
|   arm_biquad_cascade_df1_q31   |  2194| N/A | 1493 | 
|    arm_biquad_cascade_df1_fast_q15   | N/A | 613 |  472| 
|   arm_biquad_cascade_df1_fast_q31    | 1818 | N/A |  506| 
|     arm_biquad_cascade_df2T_f32  |  51629|  N/A|  537| 
|  arm_biquad_cascade_df2T_f64    | 77806 | N/A | 36912 | 
|    arm_biquad_cascade_stereo_df2T_f32   |113130 | N/A | 1007 | 
|    arm_conv_f32   |  375567|N/A  | 10844 | 
|     arm_conv_q7  |  18594| 4413(76.3%)  |9555  | 
|  arm_conv_q15     | 25044 | 10738(57.1%) |7749  | 
|   arm_conv_q31   |  27919| N/A | 15322 | 
|     arm_conv_opt_q7  | N/A |  4478| 4803 | 
|     arm_conv_fast_q15  | N/A | 4734 |7087  | 
|    arm_conv_opt_q15    | N/A |11253  |5942  | 
|    arm_conv_fast_opt_q15   | N/A |  4702| 4648 | 
|    arm_conv_fast_q31   |  10084|  N/A|  9049| 
|   arm_conv_partial_f32    | 374605 |N/A  |  11038| 
|    arm_conv_partial_q7   |  18114| 4258(76.5%) | 9594 | 
|     arm_conv_partial_q15  |  24554| 10856(55.8%) |7999  | 
|     arm_conv_partial_q31  | 27428 | N/A | 15301 | 
|   arm_conv_partial_opt_q7   |  N/A| 4326  |  4410| 
|   arm_conv_partial_fast_q15    | N/A | 4783 |  7263| 
|    arm_conv_partial_opt_q15   | N/A |  10960|  5591| 
|  arm_conv_partial_fast_opt_q15   |  N/A| 4722 |  4295| 
|    arm_conv_partial_fast_q31   | 9896 | N/A | 9000 | 
|    arm_correlate_f32   | 375490 |N/A |  11155| 
|   arm_correlate_q7    | 18784|2907(84.5%) |  9153|
|     arm_correlate_q15  | 25234 |10499(58.4%) |  6476|
|     arm_correlate_q31  | 27943 | N/A|  15479|
|    arm_correlate_opt_q7   | N/A |4568 |  4969|
|     arm_correlate_fast_q15  | N/A |4907 | 5770 |
|    arm_correlate_opt_q15   |  N/A| 11269| 5988 |
|     arm_correlate_fast_opt_q15  | N/A | 4668| 4707 |
|     arm_correlate_fast_q31  | 10031 |N/A | 8719 |
|    arm_fir_decimate_f32   | 16201 |N/A | 753 | 
|     arm_fir_decimate_q15  |  1017| 817(24.5%)| 762 |
|     arm_fir_decimate_q31  |  1013|N/A | 1487 |
|    arm_fir_decimate_fast_q15   | N/A | 568| 675 |
|    arm_fir_decimate_fast_q31   | 887 | N/A| 791 |
|    arm_fir_f32   | 65074 |N/A | 1267 |
|    arm_fir_q7   |  1382|931(32.6%) |  1327|
|    arm_fir_q15   | 2546 |1681(34%) | 1185 |
|    arm_fir_q31   | 2956 |N/A | 2722 |
|   arm_fir_fast_q15    | N/A | 854| 920 |
|  arm_fir_fast_q31     |  3054| N/A|  1323|
|     arm_fir_lattice_f32  |  136410| N/A|  2195|
|    arm_fir_lattice_q15   | 5484 |2232(59.2%) | 2280 |
|    arm_fir_lattice_q31   |  5108| N/A| 2185 |
|    arm_fir_interpolate_f32   |  66193|	N/A | 2560|
| arm_fir_interpolate_q15 | 3966| N/A | 3924|
| arm_fir_interpolate_q31 | 4282| N/A | 4667|
|  arm_iir_lattice_f32|214182 | N/A |5241 |
| arm_iir_lattice_q15 | 7667| 5082(33.7%) | 4854|
|  arm_iir_lattice_q31| 13793| N/A | 7756|
| arm_lms_norm_f32 | 184391| N/A | 7057|
| arm_lms_norm_q15 | 11182| 9557(21%) |8835 |
| arm_lms_norm_q31|14629 |  N/A| 13127|
| arm_lms_f32 |144095   |N/A | 6167|
|  arm_lms_q15| 5944| N/A |7347 |
| arm_lms_q31 | 8702|N/A  | 9017|
| arm_fir_sparse_f32 | 62525| N/A | 5447|
| arm_fir_sparse_q7 | 4215| N/A | 5351|
| arm_fir_sparse_q15 | 4523| N/A | 5326|
|arm_fir_sparse_q31  | 4769| N/A | 6007|

#

#### Transform Functions

For CFFT, transform size was 64.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|  arm_cfft_f32(without bit reversal)  | 197447 | N/A |3110 | 
|   arm_cfft_f32(with bit reversal) |  197934|  N/A| 3823| 
| arm_cfft_q15(without bit reversal)   | 6113 | 4484(26.6%) | 2827| 
|  arm_cfft_q15(with bit reversal)  |  6541| 4908(25%) | 3318| 
|   arm_cfft_q31(without bit reversal) | 4949 |N/A  |5322 | 
|  arm_cfft_q31(with bit reversal)  | 5429 | N/A |6038 | 


#### Controller Functions

For PID functions it operates on single at a time, this results are obtained by iterating over a vector of size 32.

Init functions are not considered in benchmarking.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|  arm_pid_f32 |  33726 | | 2158 | 
|  arm_pid_q15 |  1429 | |  2318 | 
| arm_pid_q31  |   1334| |  2536 | 
|  arm_clarke_f32 | 557  | |  25 | 
| arm_clarke_q31  |  97 | |  36 | 
| arm_inv_clarke_f32  | 1093  | |  24 | 
|  arm_inv_clarke_q31 |  82 | |   34| 
|   arm_sin_cos_f32| 7529  | |   129| 
| arm_sin_cos_q31  | 284  | |  309 | 
| arm_park_f32  |  1046 | |  34 | 
|  arm_park_q31 |  177 | |   54| 
|  arm_inv_park_f32 |  1039 | | 33	  | 
|  arm_inv_park_q31 |  178 | |  54 | 

