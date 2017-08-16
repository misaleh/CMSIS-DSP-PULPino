#ifndef MORE_BASIC_MATH_DATA_H
#define MORE_BASIC_MATH_DATA_H
#define BLOCK_SIZE_F32 32
#define BLOCK_SIZE_Q7 15
#define BLOCK_SIZE_Q15 15
#define BLOCK_SIZE_Q31 15
#define BASIC_MATH_SNR_THRESHOLD_float32_t 120
#define BASIC_MATH_SNR_THRESHOLD_q31_t 100
#define BASIC_MATH_SNR_THRESHOLD_q15_t 75
#define BASIC_MATH_SNR_THRESHOLD_q7_t 25
uint32_t elts[4] = {0, 1, 0x80000000, 0x7fffffff};
int8_t elts2[4] = {0, 3, -3, -7, };
float32_t eltsf[4] = { 0.0f, -1.665584, -127.435646, 245.34634267};

float32_t Arr_f32_1[32] = {
                      -0.432565, -1.665584,  0.125332,  0.287676, -1.146471,
                       1.190915,  1.189164, -0.037633,  0.327292,  0.174639,
                      -0.186709,  0.725791, -0.588317,  2.183186, -0.136396,
                       0.113931,  1.066768,  0.059281, -0.095648, -0.832349,
                       0.294411, -1.336182,  0.714325,  1.623562, -0.691776,
                       0.857997,  1.254001, -1.593730, -1.440964,  0.571148,
                      -0.399886,  0.689997
};
float32_t Arr_f32_2[15] = {
                      -0.432565, -1.665584,  0.125332,  0.287676, -1.146471,
                       1.190915,  1.189164, -0.037633,  0.327292,  0.174639,
                      -0.186709,  0.725791, -0.588317,  2.183186, -0.136396,
};

q31_t Arr_q31[15];
q15_t Arr_q15[15];
q7_t Arr_q7[15];
#endif
