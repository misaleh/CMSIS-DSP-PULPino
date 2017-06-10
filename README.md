## CMSIS DSD for PULPino
This is a port of ARM CMSIS DSP [library](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) to [PULPino](https://github.com/pulp-platform/pulpino) microcontroller.
This [project](https://summerofcode.withgoogle.com/projects/?sp-search=Mostafa#5599084384616448) is part of GSOC 2017 

This Repo is still under development

#### Ported Modules
BasicMathFunctions (part1)
#### Benchmarks
**BasicMathFunctions**
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

