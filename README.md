## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/?sp-search=Mostafa#5599084384616448) is part of GSOC 2017 

This Repo is still under development.

#### Ported Modules
BasicMathFunctions

FastMathFunctions
#### Benchmarks
**BasicMathFunctions**

These Benchmarks are performed on vectors of size 32.

It can be seen that floating point arithmetic operations are very slow, the reason is that pulpino does not have a FPU unlike ARM M4.

ARM M4 Benchmarks were done with  Keil simulator(CM4_FP) and CMSISv5.


| Function        | Puplino Cycles           | Puplino DSP  Cycles|  ARM M4 Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_abs_f32      | 170 |  | 221| 
| arm_abs_q7      | 270 |  | 297| 
| arm_abs_q15      | 253  |  | 293| 
| arm_abs_q31      | 283  |  | 282| 
| arm_add_f32      | 5942  |  | 295| 
| arm_add_q7      | 404 |  | 109| 
| arm_add_q15      | 434 |  | 168| 
| arm_add_q31      | 570 |  | 290| 
| arm_dot_prod_q15      | 368 |  |144 | 
| arm_dot_prod_q31      | 624 |  |504 | 
| arm_dot_prod_f32      | 11222 |  | 311| 
| arm_dot_prod_q7      | 173 |  | 144| 
| arm_mult_f32      | 5712 |  | 295| 
| arm_mult_q31      | 776 |  | 354| 
| arm_mult_q15      | 331 |  | 238| 
| arm_mult_q7      | 334 |  | 322| 
| arm_negate_f32      | 167 |  | 221| 
| arm_negate_q7      | 154  |  | 92| 
| arm_negate_q15      | 222  |  | 132| 
| arm_negate_q31      | 267  |  | 216| 
| arm_offset_f32      | 5995 |  | 223| 
| arm_offset_q7      | 371 |  | 100| 
| arm_offset_q15      | 400 |  | 133| 
| arm_offset_q31      | 577 |  | 216| 
| arm_scale_f32      | 5903 |  | 191| 
| arm_scale_q7      | 409 |  | 295| 
| arm_scale_q15      | 438 |  | 246| 
| arm_scale_q31      | 657 |  | 379| 
| arm_shift_q7      | 373 |  |300 | 
| arm_shift_q15      | 405 |  | 236| 
| arm_shift_q31      | 674|  | 369| 
| arm_sub_f32      | 6068 |  |295 | 
| arm_sub_q7      | 430 |  | 109| 
| arm_sub_q15      | 460 |  | 168| 
| arm_sub_q31      | 654 |  | 290| 

**FastMathFunctions**

Fast Math Functions doesn't operate on vectors.

Test cases are the same in the two benchmarks.

All Sin and Cosine functions uses tables but float functions are slower as they use the fixed point tables and interpolate the results from it using float data type.

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
