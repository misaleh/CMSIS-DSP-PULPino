## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/?sp-search=Mostafa#5599084384616448) is part of GSOC 2017 

This Repo is still under development.

#### Ported Modules
BasicMathFunctions
#### Benchmarks
**BasicMathFunctions**
These Benchmarks are performed on vectors of size 32.

It can be seen that floating point functions are very slow, the reason is that pulpino does not have a FPU unlike ARM M4 for example.

| Function        | Puplino Cycles           | Puplino DSP  Cycles|  ARM Cycles|
| ------------- |:-------------:| -----:| -----:|
| arm_abs_f32      | 170 |  | | 
| arm_abs_q7      | 270 |  | | 
| arm_abs_q15      | 253  |  | | 
| arm_abs_q31      | 283  |  | | 
| arm_add_f32      | 5942  |  | | 
| arm_add_q7      | 404 |  | | 
| arm_add_q15      | 434 |  | | 
| arm_add_q31      | 570 |  | | 
| arm_dot_prod_q15      | 368 |  | | 
| arm_dot_prod_q31      | 170 |  | | 
| arm_abs_f32      | 624 |  | | 
| arm_dot_prod_f32      | 11222 |  | | 
| arm_dot_prod_q7      | 173 |  | | 
| arm_mult_f32      | 5712 |  | | 
| arm_mult_q31      | 776 |  | | 
| arm_mult_q15      | 331 |  | | 
| arm_mult_q7      | 334 |  | | 
| arm_abs_f32      | 170 |  | | 
| arm_negate_f32      | 167 |  | | 
| arm_negate_q7      | 154  |  | | 
| arm_negate_q15      | 222  |  | | 
| arm_negate_q31      | 267  |  | | 
| arm_offset_f32      | 5995 |  | | 
| arm_offset_q7      | 371 |  | | 
| arm_offset_q15      | 400 |  | | 
| arm_offset_q31      | 577 |  | | 
| arm_scale_f32      | 5903 |  | | 
| arm_scale_q7      | 409 |  | | 
| arm_scale_q15      | 438 |  | | 
| arm_scale_q31      | 657 |  | | 
| arm_shift_q7      | 373 |  | | 
| arm_shift_q15      | 405 |  | | 
| arm_shift_q31      | 6068 |  | | 
| arm_sub_f32      | 373 |  | | 
| arm_sub_q7      | 430 |  | | 
| arm_sub_q15      | 463 |  | | 

