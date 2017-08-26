## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller(RISC-V RVIMD ISA with costum extensions).
This [project](https://summerofcode.withgoogle.com/projects/#5599084384616448) is part of Google Summer of Code (GSoC) 2017

#

The names of files and functions are renamed from arm to riscv. For example `arm_add_q15` is changed to `riscv_add_q15`. Check  examples or  tests for more clarification.

#

To use DSP extension, add `#define USE_DSP_RISCV` in riscv_math.h

#

The library is already configured and integrated in the CMake files of PULPino in this [fork](https://github.com/misaleh/pulpino).

For example, after configuring PULPino itself and installing tools required, you can simulate ` Benchmark_BasicMathFunctions1 ` by typing.

	make Benchmark_BasicMathFunctions1.vsim

#

Note that some `Transform Fucntions` benchmarks needs more memory than the default (512kb instead of 32kb).

To increase memory size, three files should be changed:

* `rtl/core_region.sv`, where parameter `DATA_RAM_SIZE` changes the size of the RAM.

* `sw/ref/link.common.ld`, the `dataram` size  and `stack` start address need to be changed in the linker script.

* `sw/utils/s19toslm.py`, which translates the compiler output to the format the testbench reads, `tcdm_bank_size` needs to be changed.

#

***Directory Structure:***

* `inc/` : Include files for CMSIS DSP.

* `src/` : Ported Source code.

* `tests/` : some tests, used for benchmarking.

* `examples/` : Ported examples.

* `more_tests/` : More tests based on subset of the CMSISv5 test suite, it doesn't compare results with pre-known values but with reference implementation.

* `utils/` : Python scripts to convert supported fixed-point formats to floating-point numbers, used to interpret fixed-point results.

#

***Flow of development:***

For each module:

1. Port to PULPino.
2. Test it.
3. Benchmark basic implementation.
4. Optimize the implementation with PULPino DSP extension.
5. Test it.
6. Benchmark DSP implementation.
7. Benchmark ARM m4 and compare results.

#

### Ported Modules
1) Basic Math Functions

2) Fast Math Functions

3) Complex Math Functions

4) Statistics Functions

5) Matrix Functions

6) Support Functions

7) Filtering  Functions

8) Transform Functions (without  radix-2 suppressed Functions)

9) Controller Functions

10) Interpolation Functions

#

### Benchmarks

ARM M4 Benchmarks were done with  Keil simulator(CM4_FP) and CMSISv5.

ARM M4 uses its DSP Instructions by default.

The floating point arithmetic operations are slow as these results are obtained without using the FPU.

Improvements of DSP extension(Imp%) = (Old - New_DSP)*100/Old

#### BasicMath Functions

These Benchmarks are performed on vectors of size 32.

Results from PULPino were compared with results from ARM m4 and also were checked by hand.


| Function        | Puplino Cycles           | Puplino DSP Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_abs_f32      | 133 | N/A| 221|
| arm_abs_q7      | 274 |  185(32.5%) | 297|
| arm_abs_q15      | 224  |  187(16.5%)| 293|
| arm_abs_q31      | 254  |  237(6.7%) | 282|
| arm_add_f32      | 5310  | N/A | 295|
| arm_add_q7      | 373 | 340(8.8%)| 109|
| arm_add_q15      | 403 | 192(52.4%)| 168|
| arm_add_q31      | 543 | N/A | 290|
| arm_dot_prod_q15      | 336 |187(44.3%)  |144 |
| arm_dot_prod_q31      | 572 |  N/A|504 |
| arm_dot_prod_f32      | 10009 | N/A | 311|
| arm_dot_prod_q7      | 142 | 57(60%)  | 144|
| arm_mult_f32      | 5149 | N/A | 295|
| arm_mult_q31      | 751 |  N/A| 354|
| arm_mult_q15      | 302 | 203(32.8%) | 238|
| arm_mult_q7      |  302  | 235(22.2%) | 322 |
| arm_negate_f32      | 143 | N/A | 221|
| arm_negate_q7      | 124  |  N/A| 92|
| arm_negate_q15      | 192  |  187(2.6%)| 132|
| arm_negate_q31      | 237  | N/A | 216|
| arm_offset_f32      | 5258 |  N/A| 223|
| arm_offset_q7      | 339 | 233(31.3%)| 100|
| arm_offset_q15      | 369 | 153(58.5%) | 133|
| arm_offset_q31      |546  | N/A | 216|
| arm_scale_f32      | 5239 | N/A | 191|
| arm_scale_q7      | 377 | 207(45.1%) | 295|
| arm_scale_q15      | 406 | 207(49%) | 246|
| arm_scale_q31      | 626 | N/A | 379|
| arm_shift_q7 (left)     | 341 |  190(44.3%) |300 |
| arm_shift_q15 (left)     | 373 |  185(50.4%) | 236|
| arm_shift_q31 (left)      | 642|  N/A | 369|
| arm_shift_q7 (right)      | 143 | 62(56.6%) |246 |
| arm_shift_q15 (right)      | 141 |  91(35.5%)| 203|
| arm_shift_q31 (right)      | 145|  N/A |236 |
| arm_sub_f32      | 5350 | N/A |295 |
| arm_sub_q7      | 398 |  340(14.6%)| 109|
| arm_sub_q15      | 428 | 192(55.1%) | 168|
| arm_sub_q31      | 622 | N/A | 290|

#

#### FastMath Functions

Fast Math Functions doesn't operate on vectors.

Test cases are the same in the two benchmarks.

All Sin and Cosine functions uses tables but with interpolation.

sqrt for float use the built in sqrt function while sqrt for fixed point use  Newton-Raphson algorithm.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_sqrt_f32      | 1199 |N/A  | 26|
| arm_sqrt_q15      | 628 |  N/A| 83|
| arm_sqrt_q31      | 581 | N/A | 131|
| arm_cos_f32      | 1853 |  N/A| 53|
| arm_cos_q15      | 37|N/A  | 36|
| arm_cos_q31      | 44 |  N/A| 46|
| arm_sin_f32      | 1779 |  N/A| 55|
| arm_sin_q15      | 28 |  N/A| 30|
| arm_sin_q31      | 37 |  N/A| 43|


#

#### Complex Math Functions

These Benchmarks are performed on vectors of size 32 (16 complex numbers).

Most of the optimizations are in q15 data type, as float is not supported, and also q31 can't use many from the extended instructions as SIMD instructions and clip.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_cmplx_conj_f32      |  129|  N/A|181 |
| arm_cmplx_conj_q31      |  175|  N/A| 173|
| arm_cmplx_conj_q15      |  185|142(23.2%) | 142|
| arm_cmplx_dot_prod_f32      | 20203| N/A |379 |
| arm_cmplx_dot_prod_q31      | 1013| N/A| 796|
| arm_cmplx_dot_prod_q15      | 499 |N/A  |294 |
| arm_cmplx_mag_f32      |  27036| N/A | 512|
| arm_cmplx_mag_q31      |  9997|  N/A| 2367|
| arm_cmplx_mag_q15      | 10396 | 10217(1.7%)| 1430|
| arm_cmplx_mag_squared_f32      | 7509 |N/A |181 |
| arm_cmplx_mag_squared_q31      | 267 |  N/A| 324|
| arm_cmplx_mag_squared_q15      |  235| 90(61.7%)| 134|
| arm_cmplx_mult_cmplx_f32      | 15257|  N/A| 364|
| arm_cmplx_mult_cmplx_q31      |565 | N/A| 366|
| arm_cmplx_mult_cmplx_q15      |  270|206 (23.7%)  | 366|
| arm_cmplx_mult_real_f32      | 5116 |  N/A| 236|
| arm_cmplx_mult_real_q31      |  720|  N/A| 302|
| arm_cmplx_mult_real_q15      |  305| 206(32.5%) | 211|

#

#### Statistics Functions

These Benchmarks are performed on vectors of size 32.

Functions that need square root operations, use the functions from FastMath as rms and var.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_max_f32      | 6748 | N/A | 322|
| arm_max_q7      |  299| N/A| 283|
| arm_max_q15       |  298|  N/A|283 |
| arm_max_q31        | 267 |  N/A|283 |
| arm_mean_f32       | 5432 |   N/A|172 |
| arm_mean_q7       | 139 |  77(44.6%)| 146|
| arm_mean_q15       | 138 |  104(24.6%) |148 |
| arm_mean_q31        | 416 |  N/A|397 |
| arm_min_f32       | 6691 | N/A |322 |
| arm_min_q7        | 295 | N/A | 283|
| arm_min_q15       | 293 |N/A  | 283|
| arm_min_q31       | 262 |  N/A| 283|
| arm_power_f32      | 9698 |  N/A| 232|
| arm_power_q7        | 108 | 45(58.3%)| 109|
| arm_power_q15       | 303 | 168(44.6%) | 107|
| arm_power_q31      | 558 |N/A  | 434|
| arm_rms_f32       | 11254 |  N/A| 268|
| arm_rms_q15        | 1074 |  935(12.9%)| 386|
| arm_rms_q31      | 1282 | N/A | 765|
| arm_std_f32        | 17120 |N/A  | 345|
| arm_std_q15       | 1263 |  1108(12.3%)| 628|
| arm_std_q31      | 1749 | N/A | 1121|
| arm_var_f32       | 15770 | N/A | 455|
| arm_var_q15      | 640 | 488(23.8%) | 551|
| arm_var_q31       | 1155 | N/A | 993|

#

#### Matrix Functions

These Benchmarks are performed on matrices of size 4x4.

Functions with Fast version, use results or accumulators of size 32 bits instead of 64 bits. For more information refer to [
CMSIS DSP Software Library
]( http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_mat_add_f32      | 2492 | N/A|190  |
| arm_mat_add_q15      | 216 | 114(47.2%)|124|
| arm_mat_add_q31       |  319|N/A | 172|
| arm_mat_cmplx_mult_f32        | 68431 | N/A|1992  |
| arm_mat_cmplx_mult_q15       | 2984 | 2055(31.1%) |1606 | 
| arm_mat_cmplx_mult_q31      | 3544 | N/A|3516 |
| arm_mat_inverse_f32      | 29482 |N/A |2994 |
| arm_mat_inverse_f64       | 53268 | N/A |20765 |
| arm_mat_mult_f32      | 14510 |  N/A |1067 |
| arm_mat_mult_q15      | 1110 | 862(22.3%) |946|
| arm_mat_mult_q31       | 1279 | N/A |1531 |
| arm_mat_mult_fast_q15      | 847 |637(24.8%) |743 |
| arm_mat_mult_fast_q31      | 1093 |  N/A|643|
| arm_mat_scale_f32       | 1875 | N/A |144 |
| arm_mat_scale_q15      | 215 |  116(46%)|156 |
| arm_mat_scale_q31      | 315 | N/A |227 |
| arm_mat_sub_f32       | 2099 | N/A |190|
| arm_mat_sub_q15     | 217 | 113(47.9%)  |122 |
| arm_mat_sub_q31     |  334| N/A  |172|
| arm_mat_trans_f32       |  114| N/A |268 |
| arm_mat_trans_q15       |  114| N/A |202|
| arm_mat_trans_q31       |  114| N/A | 262 |

#


#### Support Functions

These Benchmarks are performed on vectors of size 32.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_copy_f32      | 110 | N/A|189  |
| arm_copy_q7      | 70 | 45(35.7%)|94|
| arm_copy_q15       | 92 |71(22.8%) | 126|
| arm_copy_q31       | 108 | N/A|182  |
| arm_fill_f32      | 75 | N/A |135 |
| arm_fill_q7     |  63| 40(36.5%)|89 |
| arm_fill_q15    | 62 |35(43.5%) |98 |
| arm_fill_q31    | 75 | N/A |118 |
| arm_float_to_q7      | 7178 |  7036(2%) |559 |
| arm_float_to_q15      | 7242 | 7036(2.8%) |534|
| arm_float_to_q31       | 45287 | N/A |1566 |
| arm_q7_to_float      | 7295 |N/A |328 |
| arm_q7_to_q15     | 140 |  N/A|156|
| arm_q7_to_q31       | 140 | N/A |182 |
| arm_q15_to_float    | 7327 |  N/A|182 |
| arm_q15_to_q7    | 140| 133(5%) |270 |
| arm_q15_to_q31      | 138 | N/A |206|
| arm_q31_to_float   | 7305 | N/A |182 |
| arm_q31_to_q7    |  140| 115(17.6%) |156|
| arm_q31_to_q15     |  138| 119(13.8%) |182 |


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

>Opt versions are supported for Q15 and Q7. Design uses internal scratch buffer for getting good optimization. These versions are optimized in cycles and consumes more memory(Scratch memory) compared to Q15 and Q7 versions

**Opt fast functions** use scratch buffers and 32 bit accumulator.


Most of Fast and opt functions are available only for DSP extension(as in ARM).

For more refer to [
CMSIS DSP Software Library
]( http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

Init functions are not considered in benchmarking as they are just copying a few pointers and will only be called once in real applications or can be bypassed by initializing the structure directly.



| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|   arm_biquad_cas_df1_32x64_q31    |  3859| N/A |4200  |
|    arm_biquad_cascade_df1_f32   | 44970 |N/A  |  661|
|  arm_biquad_cascade_df1_q15    |  1781| 1169(34.4%) |  574|
|   arm_biquad_cascade_df1_q31   |  2195| N/A | 1493 |
|    arm_biquad_cascade_df1_fast_q15   | N/A | 616 |  472|
|   arm_biquad_cascade_df1_fast_q31    | 1819 | N/A |  506|
|     arm_biquad_cascade_df2T_f32  |  45881|  N/A|  537|
|  arm_biquad_cascade_df2T_f64    | 70296 | N/A | 36912 |
|    arm_biquad_cascade_stereo_df2T_f32   |102567 | N/A | 1007 |
|    arm_conv_f32   |  334063|N/A  | 10844 |
|     arm_conv_q7  |  18601| 4417(76.3%)  |9555  |
|  arm_conv_q15     | 25048 | 10771(57%) |7749  |
|   arm_conv_q31   |  27925| N/A | 15322 |
|     arm_conv_opt_q7  | N/A |  4483| 4803 |
|     arm_conv_fast_q15  | N/A | 4736 |7087  |
|    arm_conv_opt_q15    | N/A |11255  |5942  |
|    arm_conv_fast_opt_q15   | N/A |  4704| 4648 |
|    arm_conv_fast_q31   |  4744|  N/A|  10083|
|   arm_conv_partial_f32    | 333142 |N/A  |  11038|
|    arm_conv_partial_q7   |  18120| 4258(76.5%) | 9594 |
|     arm_conv_partial_q15  |  24558| 10861(55.8%) |7999  |
|     arm_conv_partial_q31  | 27433 | N/A | 15301 |
|   arm_conv_partial_opt_q7   |  N/A| 4330  |  4410|
|   arm_conv_partial_fast_q15    | N/A | 4780 |  7263|
|    arm_conv_partial_opt_q15   | N/A |  10999|  5591|
|  arm_conv_partial_fast_opt_q15   |  N/A| 4721 |  4295|
|    arm_conv_partial_fast_q31   | 9897 | N/A | 9000 |
|    arm_correlate_f32   | 333911 |N/A |  11155|
|   arm_correlate_q7    | 18791|2914(84.5%) |  9153|
|     arm_correlate_q15  | 25239 |10502(58.4%) |  6476|
|     arm_correlate_q31  | 27944 | N/A|  15479|
|    arm_correlate_opt_q7   | N/A |4579 |  4969|
|     arm_correlate_fast_q15  | N/A |4911 | 5770 |
|    arm_correlate_opt_q15   |  N/A| 11273| 5988 |
|     arm_correlate_fast_opt_q15  | N/A | 4642| 4707 |
|     arm_correlate_fast_q31  | 10032 |N/A | 8719 |
|    arm_fir_decimate_f32   | 14339 |N/A | 753 |
|     arm_fir_decimate_q15  |  1021| 820(19.7%)| 762 |
|     arm_fir_decimate_q31  |  1017|N/A | 1487 |
|    arm_fir_decimate_fast_q15   | N/A | 571| 675 |
|    arm_fir_decimate_fast_q31   | 887 | N/A| 791 |
|    arm_fir_f32   | 57512 |N/A | 1267 |
|    arm_fir_q7   |  1383|932(32.6%) |  1327|
|    arm_fir_q15   | 2551 |1675(34.3%) | 1185 |
|    arm_fir_q31   | 2961 |N/A | 2722 |
|   arm_fir_fast_q15    | N/A | 853| 920 |
|  arm_fir_fast_q31     |  3056| N/A|  1323|
|     arm_fir_lattice_f32  |  120999| N/A|  2195|
|    arm_fir_lattice_q15   | 5483 |2234(59.3%) | 2280 |
|    arm_fir_lattice_q31   |  5106| N/A| 2185 |
|    arm_fir_interpolate_f32   |  58641|N/A | 2560|
| arm_fir_interpolate_q15 | 3967| N/A | 3924|
| arm_fir_interpolate_q31 | 4288| N/A | 4667|
|  arm_iir_lattice_f32|189904 | N/A |5241 |
| arm_iir_lattice_q15 | 7668| 5082(33.7%) | 4854|
|  arm_iir_lattice_q31| 13796| N/A | 7756|
| arm_lms_norm_f32 | 159993| N/A | 7057|
| arm_lms_norm_q15 | 11185| 9558(14.5%) |8835 |
| arm_lms_norm_q31|14634 |  N/A| 13127|
| arm_lms_f32 |127573   |N/A | 6167|
|  arm_lms_q15| 4325| N/A |7347 |
| arm_lms_q31 | 8707|N/A  | 9017|
| arm_fir_sparse_f32 | 55605| N/A | 5447|
| arm_fir_sparse_q7 | 4113| N/A | 5351|
| arm_fir_sparse_q15 | 4284| N/A | 5326|
|arm_fir_sparse_q31  | 4773| N/A | 6007|

#

#### Transform Functions

For CFFT, Transform size was 64.

RFFT uses Transform of size 16.

DCT uses Transform of size 128.

To use most of theses tests, the RAM size has to be changed to 512kb.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|  arm_cfft_f32(without bit reversal)  | 174295 | N/A |3110 |
|   arm_cfft_f32(with bit reversal) |  174783|  N/A| 3823|
| arm_cfft_q15(without bit reversal)   | 6117 | 4484(26.6%) | 2827|
|  arm_cfft_q15(with bit reversal)  |  6541| 4908(25%) | 3318|
|   arm_cfft_q31(without bit reversal) | 4948 |N/A  |5322 |
|  arm_cfft_q31(with bit reversal)  | 5428 | N/A |6038 |
|  arm_rfft_fast_f32| 48371 | N/A  | 1930|
| arm_rfft_q15 | 1466 | N/A  |1228 |
|  arm_rfft_q31|2466 |N/A   |2727 |
| arm_dct4_f32 |584790  | N/A  |16037|
| arm_dct4_q15 |  17773| N/A  | |
|arm_dct4_q31  | 29773 | N/A  | |

#### Controller Functions

For PID functions, it operates on single sample at a time, this results are obtained by iterating over a vector of size 32.

Init functions are not considered in benchmarking.

| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
|  arm_pid_f32 |  29913 | N/A| 2158 |
|  arm_pid_q15 |  1428 | 1347(5.7%)|  2318 |
| arm_pid_q31  |   1327| N/A|  2536 |
|  arm_clarke_f32 | 485  |N/A |  25 |
| arm_clarke_q31  |  104 |N/A |  36 |
| arm_inv_clarke_f32  | 956  | N/A|  24 |
|  arm_inv_clarke_q31 |  92 |N/A |   34|
|   arm_sin_cos_f32| 6690  | N/A|   129|
| arm_sin_cos_q31  | 284  | N/A|  309 |
| arm_park_f32  |  922 |N/A |  34 |
|  arm_park_q31 |  185 | N/A|   54|
|  arm_inv_park_f32 |  932 |N/A | 33|
|  arm_inv_park_q31 |  185 |N/A |  54 |

#### Interpolation Functions


| Function        | Puplino Cycles           | Puplino DSP  Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_linear_interp_f32 |525 | N/A| 55 |
|  arm_linear_interp_q7  |22| N/A| 80 |
|  arm_linear_interp_q15 | 36|N/A| 92 |
| arm_linear_interp_q31  |29| N/A| 84 |
| arm_bilinear_interp_f32 |1505| N/A| 189 |
|  arm_bilinear_interp_q7 |81| N/A| 219 |
| arm_bilinear_interp_q15  |132| N/A| 240 |
|  arm_bilinear_interp_q31 |104| N/A| 226 |

#

### Credits

The initial port was performerd by Mostafa Saleh, for the lowRISC project in Google Summer of Code (GSoC) 2017.

#
### License

This project in under The 3-Clause BSD [License](LICENSE).
