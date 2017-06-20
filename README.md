## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/?sp-search=Mostafa#5599084384616448) is part of GSOC 2017 

This Repo is still under development.

To use DSP extension, add `#define USE_DSP_RISCV` in arm_math.h
#### Ported Modules
BasicMathFunctions

FastMathFunctions
#### Benchmarks
**BasicMathFunctions**

These Benchmarks are performed on vectors of size 32.

It can be seen that floating point arithmetic operations are very slow, the reason is that pulpino does not have a FPU unlike ARM M4.

ARM M4 Benchmarks were done with  Keil simulator(CM4_FP) and CMSISv5.

Results from pulpino were compared with results from ARM m4 and also were checked by hand.

Improvements of DSP extension(Imp) = (Old - New_DSP)*100/Old


| Function        | Puplino Cycles           | Puplino DSP Cycles (Imp%)|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_abs_f32      | 170 | N/A| 221| 
| arm_abs_q7      | 270 |  205 (24.1%)| 297| 
| arm_abs_q15      | 253  |  219 (13.4%)| 293| 
| arm_abs_q31      | 283  |  267 (5.7%)| 282| 
| arm_add_f32      | 5942  | N/A | 295| 
| arm_add_q7      | 404 | 367 (9.2%)| 109| 
| arm_add_q15      | 434 | 217 (50%)| 168| 
| arm_add_q31      | 570 | N/A | 290| 
| arm_dot_prod_q15      | 368 |219 (40.5%)  |144 | 
| arm_dot_prod_q31      | 624 |  N/A|504 | 
| arm_dot_prod_f32      | 11222 | N/A | 311| 
| arm_dot_prod_q7      | 173 | 89 (48.6%) | 144| 
| arm_mult_f32      | 5712 | N/A | 295| 
| arm_mult_q31      | 776 |  N/A| 354| 
| arm_mult_q15      | 331 | 238 (28.1%)| 238| 
| arm_mult_q7      | 334 | 270(19.2%) | 322 | 
| arm_negate_f32      | 167 | N/A | 221| 
| arm_negate_q7      | 154  |  199(-29%)| 92| 
| arm_negate_q15      | 222  |  217(2.3%)| 132| 
| arm_negate_q31      | 267  | N/A | 216| 
| arm_offset_f32      | 5995 |  N/A| 223| 
| arm_offset_q7      | 371 |  263(29.1%)| 100| 
| arm_offset_q15      | 400 | 184(54%) | 133| 
| arm_offset_q31      | 577 | N/A | 216| 
| arm_scale_f32      | 5903 | N/A | 191| 
| arm_scale_q7      | 409 | 239(41.6%) | 295| 
| arm_scale_q15      | 438 | 240(45.2%) | 246| 
| arm_scale_q31      | 657 | N/A | 379| 
| arm_shift_q7      | 373 |  N/A |300 | 
| arm_shift_q15      | 405 |  N/A | 236| 
| arm_shift_q31      | 674|  N/A | 369| 
| arm_sub_f32      | 6068 | N/A |295 | 
| arm_sub_q7      | 430 |  372(13.5%)| 109| 
| arm_sub_q15      | 460 | 224(51.3%) | 168| 
| arm_sub_q31      | 654 | N/A | 290| 

**FastMathFunctions**

Fast Math Functions doesn't operate on vectors.

Test cases are the same in the two benchmarks.

All Sin and Cosine functions uses tables but with interpolation.

sqrt for float use the built in functions sqrt function while sqrt for fixed point use  Newton-Raphson algorithm.

| Function        | Puplino Cycles           | Puplino DSP  Cycles|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_sqrt_f32      | 38 |N/A  | 26| 
| arm_sqrt_q15      | 723 |  N/A| 83| 
| arm_sqrt_q31      | 683 | N/A | 131| 
| arm_cos_f32      | 2134 |  N/A| 53| 
| arm_cos_q15      | 64|N/A  | 36| 
| arm_cos_q31      | 68 |  N/A| 46| 
| arm_sin_f32      | 2047 |  N/A| 55| 
| arm_sin_q15      | 56 |  N/A| 30| 
| arm_sin_q31      | 62 |  N/A| 43| 
