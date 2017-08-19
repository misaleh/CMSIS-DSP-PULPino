/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015 
* $Revision: 	V.1.4.5  
*    
* Project: 	    CMSIS DSP Library    
* Title:	    arm_cfft_radix4_q15.c    
*    
* Description:	This file has function definition of Radix-4 FFT & IFFT function and    
*				In-place bit reversal using bit reversal table    
*    
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*  
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the 
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.     

 Modifications 2017  Mostafa Saleh       (Ported to RISC-V PULPino)
* -------------------------------------------------------------------- */

#include "riscv_math.h"


void riscv_radix4_butterfly_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier);

void riscv_radix4_butterfly_inverse_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier);

void riscv_bitreversal_q15(
  q15_t * pSrc,
  uint32_t fftLen,
  uint16_t bitRevFactor,
  uint16_t * pBitRevTab);

/**    
 * @ingroup groupTransforms    
 */

/**    
 * @addtogroup ComplexFFT    
 * @{    
 */


/**    
 * @details    
 * @brief Processing function for the Q15 CFFT/CIFFT.   
 * @deprecated Do not use this function.  It has been superseded by \ref riscv_cfft_q15 and will be removed
 * @param[in]      *S    points to an instance of the Q15 CFFT/CIFFT structure.   
 * @param[in, out] *pSrc points to the complex data buffer. Processing occurs in-place.   
 * @return none.   
 *     
 * \par Input and output formats:    
 * \par    
 * Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.   
 * Hence the output format is different for different FFT sizes.    
 * The input and output formats for different FFT sizes and number of bits to upscale are mentioned in the tables below for CFFT and CIFFT:   
 * \par   
 * \image html CFFTQ15.gif "Input and Output Formats for Q15 CFFT"    
 * \image html CIFFTQ15.gif "Input and Output Formats for Q15 CIFFT"    
 */

void riscv_cfft_radix4_q15(
  const riscv_cfft_radix4_instance_q15 * S,
  q15_t * pSrc)
{
  if(S->ifftFlag == 1u)
  {
    /*  Complex IFFT radix-4  */
    riscv_radix4_butterfly_inverse_q15(pSrc, S->fftLen, S->pTwiddle,
                                     S->twidCoefModifier);
  }
  else
  {
    /*  Complex FFT radix-4  */
    riscv_radix4_butterfly_q15(pSrc, S->fftLen, S->pTwiddle,
                             S->twidCoefModifier);
  }

  if(S->bitReverseFlag == 1u)
  {
    /*  Bit Reversal */
    riscv_bitreversal_q15(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
  }

}

/**    
 * @} end of ComplexFFT group    
 */

/*    
* Radix-4 FFT algorithm used is :    
*    
* Input real and imaginary data:    
* x(n) = xa + j * ya    
* x(n+N/4 ) = xb + j * yb    
* x(n+N/2 ) = xc + j * yc    
* x(n+3N 4) = xd + j * yd    
*    
*    
* Output real and imaginary data:    
* x(4r) = xa'+ j * ya'    
* x(4r+1) = xb'+ j * yb'    
* x(4r+2) = xc'+ j * yc'    
* x(4r+3) = xd'+ j * yd'    
*    
*    
* Twiddle factors for radix-4 FFT:    
* Wn = co1 + j * (- si1)    
* W2n = co2 + j * (- si2)    
* W3n = co3 + j * (- si3)    
    
* The real and imaginary output values for the radix-4 butterfly are    
* xa' = xa + xb + xc + xd    
* ya' = ya + yb + yc + yd    
* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1)    
* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1)    
* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2)    
* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2)    
* xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (si3)    
* yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (si3)    
*    
*/

/**    
 * @brief  Core function for the Q15 CFFT butterfly process.   
 * @param[in, out] *pSrc16          points to the in-place buffer of Q15 data type.   
 * @param[in]      fftLen           length of the FFT.   
 * @param[in]      *pCoef16         points to twiddle coefficient buffer.   
 * @param[in]      twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.   
 * @return none.   
 */

void riscv_radix4_butterfly_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier)
{
#if defined (USE_DSP_RISCV)

  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;
  shortV VectInA;
  shortV VectInB;
  shortV VectInC;
  shortV VectInD;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;
  j = n2;

  /* Input is in 1.15(q15) format */

  /*  start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */

    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;

    /* R0 = (ya + yc) */
    R0 = clip(T0 + S0, -32768,32767);
    /* R1 = (xa + xc) */
    R1 = clip(T1 + S1, -32768,32767);

    /* S0 = (ya - yc) */
    S0 = clip(T0 - S0, -32768,32767);
    /* S1 = (xa - xc) */
    S1 = clip(T1 - S1,-32768,32767);;

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2u;

    /* T0 = (yb + yd) */
    T0 = clip(T0 + U0, -32768,32767);
    /* T1 = (xb + xd) */
    T1 = clip(T1 + U1, -32768,32767);

    /*  writing the butterfly processed i0 sample */
    /* ya' = ya + yb + yc + yd */
    /* xa' = xa + xb + xc + xd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd) */
    /* R1 = (xa + xc) - (xb + xd) */
    R0 = clip(R0 - T0, -32768,32767);
    R1 = clip(R1 - T1, -32768,32767);

    /* co2 & si2 are read from Coefficient pointer */
   // Co2 = pCoef16[2u * ic * 2u];
   // Si2 = pCoef16[(2u * ic * 2u) + 1];
    VectInA = *(shortV*)(pCoef16 + (2u * ic * 2u));
    VectInB[0] = R0;
    VectInB[1] = R1;
    /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
   // out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
   // out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = R1;
    VectInB[1] = -R0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 =  xb */
    T0 = pSrc16[i1 * 2u] >> 2;
    T1 = pSrc16[(i1 * 2u) + 1] >> 2;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd */
    U0 = pSrc16[i3 * 2u] >> 2;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2;
    /* T0 = yb-yd */
    T0 = clip(T0 - U0, -32768,32767);
    /* T1 = xb-xd */
    T1 = clip(T1 - U1, -32768,32767);

    /* R1 = (ya-yc) + (xb- xd),  R0 = (xa-xc) - (yb-yd)) */
    R0 = (q15_t) clip((q31_t) (S0 - T1), -32768,32767);
    R1 = (q15_t) clip((q31_t) (S1 + T0), -32768,32767);

    /* S1 = (ya-yc) - (xb- xd), S0 = (xa-xc) + (yb-yd)) */
    S0 = (q15_t) clip(((q31_t) S0 + T1), -32768,32767);
    S1 = (q15_t) clip(((q31_t) S1 - T0), -32768,32767);

    /* co1 & si1 are read from Coefficient pointer */
  //  Co1 = pCoef16[ic * 2u];
 //   Si1 = pCoef16[(ic * 2u) + 1];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
   // out1 = (q15_t) ((Si1 * S1 + Co1 * S0) >> 16);
    /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
   // out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16);



    VectInA = *(shortV*)(pCoef16 + (ic * 2u));
    VectInB[0] = S0;
    VectInB[1] = S1;
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = S1;
    VectInB[1] = -S0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
   // Co3 = pCoef16[3u * (ic * 2u)];
   // Si3 = pCoef16[(3u * (ic * 2u)) + 1];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
  //  out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);
    /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
 //   out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);
    /* writing output(xd', yd') in little endian format */

    VectInA = *(shortV*)(pCoef16 + (ic * 2u * 3u));
    VectInB[0] = R0;
    VectInB[1] = R1;
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = R1;
    VectInB[1] = -R0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);
  /* data is in 4.11(q11) format */

  /* end of first stage process */


  /* start of middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
    //  Co1 = pCoef16[ic * 2u];
    //  Si1 = pCoef16[(ic * 2u) + 1u];
    //  Co2 = pCoef16[2u * (ic * 2u)];
    //  Si2 = pCoef16[(2u * (ic * 2u)) + 1u];
      VectInC = *(shortV*)(pCoef16 + (ic * 2u));
      VectInA = *(shortV*)(pCoef16 + (ic * 2u *2u));
      VectInD = *(shortV*)(pCoef16 + (ic * 2u *3u));
    //  Co3 = pCoef16[3u * (ic * 2u)];
    //  Si3 = pCoef16[(3u * (ic * 2u)) + 1u];

      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];

        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = clip(T0 + S0, -32768,32767);
        R1 = clip(T1 + S1, -32768,32767);

        /* S0 = (ya - yc), S1 =(xa - xc) */
        S0 = clip(T0 - S0,-32768,32767);
        S1 = clip(T1 - S1, -32768,32767);;

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];


        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = clip(T0 + U0,-32768,32767);
        T1 = clip(T1 + U1, -32768,32767);

        /*  writing the butterfly processed i0 sample */

        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        out1 = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        out2 = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        pSrc16[i0 * 2u] = out1;
        pSrc16[(2u * i0) + 1u] = out2;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) + (xa-xb+xc-xd)* co2 */
      //  out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);

        /* (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
     //   out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);



        VectInB[0] = R0;
        VectInB[1] = R1;
        out1 = dotpv2(VectInA,VectInB)>>16;
        VectInB[0] = R1;
        VectInB[1] = -R0;
        out2 = dotpv2(VectInA,VectInB)>>16;

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd */
        T0 = clip(T0 - U0, -32768,32767);
        T1 = clip(T1 - U1, -32768,32767);

        /* R0 = (ya-yc) + (xb- xd), R1 = (xa-xc) - (yb-yd)) */
        R0 = (S0 >> 1u) - (T1 >> 1u);
        R1 = (S1 >> 1u) + (T0 >> 1u);

        /* S0 = (ya-yc) - (xb- xd), S1 = (xa-xc) + (yb-yd)) */
        S0 = (S0 >> 1u) + (T1 >> 1u);
        S1 = (S1 >> 1u) - (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
      //  out1 = (q15_t) ((Co1 * S0 + Si1 * S1) >> 16u);
      //  out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16u);
        VectInB[0] = S0;
        VectInB[1] = S1;
        out1 = dotpv2(VectInC,VectInB)>>16;
        VectInB[0] = S1;
        VectInB[1] = -S0;
        out2 = dotpv2(VectInC,VectInB)>>16;
        /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
        /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
        //out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);
        //out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);

        VectInB[0] = R0;
        VectInB[1] = R1;
        out1 = dotpv2(VectInD,VectInB)>>16;
        VectInB[0] = R1;
        VectInB[1] = -R0;
        out2 = dotpv2(VectInD,VectInB)>>16;
        /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
        /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;
      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /* end of middle stage process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point */
  /* data is in 6.10(q10) format for the 64 point */
  /* data is in 4.12(q12) format for the 16 point */

  /*  Initializations for the last stage */
  n1 = n2;
  n2 >>= 2u;

  /* start of last stage process */

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];

    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = clip(T0 + S0, -32768,32767);
    R1 = clip(T1 + S1, -32768,32767);

    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = clip(T0 - S0, -32768,32767);
    S1 = clip(T1 - S1, -32768,32767);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd)) */
    T0 = clip(T0 + U0, -32768,32767);
    T1 = clip(T1 + U1, -32768,32767);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i1 * 2u] = R0;
    pSrc16[(i1 * 2u) + 1u] = R1;

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd)  */
    T0 = clip(T0 - U0, -32768,32767);
    T1 = clip(T1 - U1, -32768,32767);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xb' = (xa+yb-xc-yd) */
    /* yb' = (ya-xb-yc+xd) */
    pSrc16[i2 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i2 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);

    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd) */
    /* yd' = (ya+xb-yc-xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);

  }

  /* end of last stage process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */


#else
  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;
  j = n2;

  /* Input is in 1.15(q15) format */

  /*  start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */

    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;

    /* R0 = (ya + yc) */
    R0 = __SSAT(T0 + S0, 16u);
    /* R1 = (xa + xc) */
    R1 = __SSAT(T1 + S1, 16u);

    /* S0 = (ya - yc) */
    S0 = __SSAT(T0 - S0, 16);
    /* S1 = (xa - xc) */
    S1 = __SSAT(T1 - S1, 16);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* input is down scale by 4 to avoid overflow */
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2u;

    /* T0 = (yb + yd) */
    T0 = __SSAT(T0 + U0, 16u);
    /* T1 = (xb + xd) */
    T1 = __SSAT(T1 + U1, 16u);

    /*  writing the butterfly processed i0 sample */
    /* ya' = ya + yb + yc + yd */
    /* xa' = xa + xb + xc + xd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd) */
    /* R1 = (xa + xc) - (xb + xd) */
    R0 = __SSAT(R0 - T0, 16u);
    R1 = __SSAT(R1 - T1, 16u);

    /* co2 & si2 are read from Coefficient pointer */
    Co2 = pCoef16[2u * ic * 2u];
    Si2 = pCoef16[(2u * ic * 2u) + 1];

    /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
    out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
    out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 =  xb */
    T0 = pSrc16[i1 * 2u] >> 2;
    T1 = pSrc16[(i1 * 2u) + 1] >> 2;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd */
    U0 = pSrc16[i3 * 2u] >> 2;
    U1 = pSrc16[(i3 * 2u) + 1] >> 2;
    /* T0 = yb-yd */
    T0 = __SSAT(T0 - U0, 16);
    /* T1 = xb-xd */
    T1 = __SSAT(T1 - U1, 16);

    /* R1 = (ya-yc) + (xb- xd),  R0 = (xa-xc) - (yb-yd)) */
    R0 = (q15_t) __SSAT((q31_t) (S0 - T1), 16);
    R1 = (q15_t) __SSAT((q31_t) (S1 + T0), 16);

    /* S1 = (ya-yc) - (xb- xd), S0 = (xa-xc) + (yb-yd)) */
    S0 = (q15_t) __SSAT(((q31_t) S0 + T1), 16u);
    S1 = (q15_t) __SSAT(((q31_t) S1 - T0), 16u);

    /* co1 & si1 are read from Coefficient pointer */
    Co1 = pCoef16[ic * 2u];
    Si1 = pCoef16[(ic * 2u) + 1];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
    out1 = (q15_t) ((Si1 * S1 + Co1 * S0) >> 16);
    /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
    out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16);

    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
    Co3 = pCoef16[3u * (ic * 2u)];
    Si3 = pCoef16[(3u * (ic * 2u)) + 1];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
    out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);
    /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
    out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);
    /* writing output(xd', yd') in little endian format */
    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);
  /* data is in 4.11(q11) format */

  /* end of first stage process */


  /* start of middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
      Co1 = pCoef16[ic * 2u];
      Si1 = pCoef16[(ic * 2u) + 1u];
      Co2 = pCoef16[2u * (ic * 2u)];
      Si2 = pCoef16[(2u * (ic * 2u)) + 1u];
      Co3 = pCoef16[3u * (ic * 2u)];
      Si3 = pCoef16[(3u * (ic * 2u)) + 1u];

      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];

        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = __SSAT(T0 + S0, 16);
        R1 = __SSAT(T1 + S1, 16);

        /* S0 = (ya - yc), S1 =(xa - xc) */
        S0 = __SSAT(T0 - S0, 16);
        S1 = __SSAT(T1 - S1, 16);

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];


        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = __SSAT(T0 + U0, 16);
        T1 = __SSAT(T1 + U1, 16);

        /*  writing the butterfly processed i0 sample */

        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        out1 = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        out2 = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        pSrc16[i0 * 2u] = out1;
        pSrc16[(2u * i0) + 1u] = out2;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) + (xa-xb+xc-xd)* co2 */
        out1 = (q15_t) ((Co2 * R0 + Si2 * R1) >> 16u);

        /* (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
        out2 = (q15_t) ((-Si2 * R0 + Co2 * R1) >> 16u);

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 + (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 - (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd */
        T0 = __SSAT(T0 - U0, 16);
        T1 = __SSAT(T1 - U1, 16);

        /* R0 = (ya-yc) + (xb- xd), R1 = (xa-xc) - (yb-yd)) */
        R0 = (S0 >> 1u) - (T1 >> 1u);
        R1 = (S1 >> 1u) + (T0 >> 1u);

        /* S0 = (ya-yc) - (xb- xd), S1 = (xa-xc) + (yb-yd)) */
        S0 = (S0 >> 1u) + (T1 >> 1u);
        S1 = (S1 >> 1u) - (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
        out1 = (q15_t) ((Co1 * S0 + Si1 * S1) >> 16u);

        out2 = (q15_t) ((-Si1 * S0 + Co1 * S1) >> 16u);

        /* xb' = (xa+yb-xc-yd)* co1 + (ya-xb-yc+xd)* (si1) */
        /* yb' = (ya-xb-yc+xd)* co1 - (xa+yb-xc-yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
        out1 = (q15_t) ((Si3 * R1 + Co3 * R0) >> 16u);

        out2 = (q15_t) ((-Si3 * R0 + Co3 * R1) >> 16u);
        /* xd' = (xa-yb-xc+yd)* Co3 + (ya+xb-yc-xd)* (si3) */
        /* yd' = (ya+xb-yc-xd)* Co3 - (xa-yb-xc+yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;
      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /* end of middle stage process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point */
  /* data is in 6.10(q10) format for the 64 point */
  /* data is in 4.12(q12) format for the 16 point */

  /*  Initializations for the last stage */
  n1 = n2;
  n2 >>= 2u;

  /* start of last stage process */

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];

    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = __SSAT(T0 + S0, 16u);
    R1 = __SSAT(T1 + S1, 16u);

    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = __SSAT(T0 - S0, 16u);
    S1 = __SSAT(T1 - S1, 16u);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd)) */
    T0 = __SSAT(T0 + U0, 16u);
    T1 = __SSAT(T1 + U1, 16u);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i1 * 2u] = R0;
    pSrc16[(i1 * 2u) + 1u] = R1;

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd)  */
    T0 = __SSAT(T0 - U0, 16u);
    T1 = __SSAT(T1 - U1, 16u);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xb' = (xa+yb-xc-yd) */
    /* yb' = (ya-xb-yc+xd) */
    pSrc16[i2 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i2 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);

    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa-yb-xc+yd) */
    /* yd' = (ya+xb-yc-xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);

  }

  /* end of last stage process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */
#endif
}


/**    
 * @brief  Core function for the Q15 CIFFT butterfly process.   
 * @param[in, out] *pSrc16          points to the in-place buffer of Q15 data type.   
 * @param[in]      fftLen           length of the FFT.   
 * @param[in]      *pCoef16         points to twiddle coefficient buffer.   
 * @param[in]      twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.   
 * @return none.   
 */

/*    
* Radix-4 IFFT algorithm used is :    
*    
* CIFFT uses same twiddle coefficients as CFFT function    
*  x[k] = x[n] + (j)k * x[n + fftLen/4] + (-1)k * x[n+fftLen/2] + (-j)k * x[n+3*fftLen/4]    
*    
*    
* IFFT is implemented with following changes in equations from FFT    
*    
* Input real and imaginary data:    
* x(n) = xa + j * ya    
* x(n+N/4 ) = xb + j * yb    
* x(n+N/2 ) = xc + j * yc    
* x(n+3N 4) = xd + j * yd    
*    
*    
* Output real and imaginary data:    
* x(4r) = xa'+ j * ya'    
* x(4r+1) = xb'+ j * yb'    
* x(4r+2) = xc'+ j * yc'    
* x(4r+3) = xd'+ j * yd'    
*    
*    
* Twiddle factors for radix-4 IFFT:    
* Wn = co1 + j * (si1)    
* W2n = co2 + j * (si2)    
* W3n = co3 + j * (si3)    
    
* The real and imaginary output values for the radix-4 butterfly are    
* xa' = xa + xb + xc + xd    
* ya' = ya + yb + yc + yd    
* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1)    
* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1)    
* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2)    
* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2)    
* xd' = (xa+yb-xc-yd)* co3 - (ya-xb-yc+xd)* (si3)    
* yd' = (ya-xb-yc+xd)* co3 + (xa+yb-xc-yd)* (si3)    
*    
*/

void riscv_radix4_butterfly_inverse_q15(
  q15_t * pSrc16,
  uint32_t fftLen,
  q15_t * pCoef16,
  uint32_t twidCoefModifier)
{

#if defined (USE_DSP_RISCV)
  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;
  shortV VectInA;
  shortV VectInB;
  shortV VectInC;
  shortV VectInD;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;

  j = n2;

  /* Input is in 1.15(q15) format */


  /*  Start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;
    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;


    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = clip(T0 + S0, -32768,32767);;
    R1 = clip(T1 + S1,  -32768,32767);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = clip(T0 - S0,  -32768,32767);
    S1 = clip(T1 - S1, -32768,32767);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;
    /* Read yd (real), xd(imag) input */
    /* input is down scale by 4 to avoid overflow */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = clip(T0 + U0,  -32768,32767);
    T1 = clip(T1 + U1, -32768,32767);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc)- (xb + xd) */
    R0 = clip(R0 - T0,  -32768,32767);
    R1 = clip(R1 - T1,  -32768,32767);
    /* co2 & si2 are read from Coefficient pointer */
  //  Co2 = pCoef16[2u * ic * 2u];
  //  Si2 = pCoef16[(2u * ic * 2u) + 1u];
    /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
  //  out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
   // out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16u);

    VectInA = *(shortV*)(pCoef16 + (ic * 2u * 2u));
    VectInB[0] = R0;
    VectInB[1] = -R1;
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = R1;
    VectInB[1] = R0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 = xb */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1u] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd) */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = yb-yd, T1 = xb-xd) */
    T0 = clip(T0 - U0,  -32768,32767);
    T1 = clip(T1 - U1,  -32768,32767);
    /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
    R0 = (q15_t) clip((q31_t) (S0 + T1), -32768,32767);
    R1 = (q15_t) clip((q31_t) (S1 - T0),  -32768,32767);
    /* S = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
    S0 = (q15_t) clip((q31_t) (S0 - T1),  -32768,32767);
    S1 = (q15_t) clip((q31_t) (S1 + T0),  -32768,32767);

    /* co1 & si1 are read from Coefficient pointer */
   // Co1 = pCoef16[ic * 2u];
   // Si1 = pCoef16[(ic * 2u) + 1u];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
  //  out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
    /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
  //  out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);

    VectInA = *(shortV*)(pCoef16 + (ic * 2u ));
    VectInB[0] = S0;
    VectInB[1] = -S1;
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = S1;
    VectInB[1] = S0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1u] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
    //Co3 = pCoef16[3u * ic * 2u];
    //Si3 = pCoef16[(3u * ic * 2u) + 1u];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
    //out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);
    /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
    //out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);

    VectInA = *(shortV*)(pCoef16 + (ic * 2u * 3u ));
    VectInB[0] = R0;
    VectInB[1] = -R1;
    out1 = dotpv2(VectInA,VectInB)>>16;
    VectInB[0] = R1;
    VectInB[1] = R0;
    out2 = dotpv2(VectInA,VectInB)>>16;

    /* writing output(xd', yd') in little endian format */
    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1u] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);

  /*  End of first stage process */

  /* data is in 4.11(q11) format */


  /*  Start of Middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
     /* Co1 = pCoef16[ic * 2u];
      Si1 = pCoef16[(ic * 2u) + 1u];
      Co2 = pCoef16[2u * ic * 2u];
      Si2 = pCoef16[2u * ic * 2u + 1u];
      Co3 = pCoef16[3u * ic * 2u];
      Si3 = pCoef16[(3u * ic * 2u) + 1u];*/
      VectInC = *(shortV*)(pCoef16 + (ic * 2u));
      VectInA = *(shortV*)(pCoef16 + (ic * 2u *2u));
      VectInD = *(shortV*)(pCoef16 + (ic * 2u *3u));
      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];


        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = clip(T0 + S0,  -32768,32767);
        R1 = clip(T1 + S1, -32768,32767);
        /* S0 = (ya - yc), S1 = (xa - xc) */
        S0 = clip(T0 - S0,  -32768,32767);
        S1 = clip(T1 - S1,  -32768,32767);

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = clip(T0 + U0,  -32768,32767);
        T1 = clip(T1 + U1,  -32768,32767);

        /*  writing the butterfly processed i0 sample */
        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        pSrc16[i0 * 2u] = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        pSrc16[(i0 * 2u) + 1u] = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) - (xa-xb+xc-xd)* co2 */
       // out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16);
        /* (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
       // out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16);
        VectInB[0] = R0;
        VectInB[1] = -R1;
        out1 = dotpv2(VectInA,VectInB)>>16;
        VectInB[0] = R1;
        VectInB[1] = R0;
        out2 = dotpv2(VectInA,VectInB)>>16;

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */
        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd) */
        T0 = clip(T0 - U0, -32768,32767);
        T1 = clip(T1 - U1,  -32768,32767);

        /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
        R0 = (S0 >> 1u) + (T1 >> 1u);
        R1 = (S1 >> 1u) - (T0 >> 1u);

        /* S1 = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
        S0 = (S0 >> 1u) - (T1 >> 1u);
        S1 = (S1 >> 1u) + (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
       // out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
       // out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);

        VectInB[0] = S0;
        VectInB[1] = -S1;
        out1 = dotpv2(VectInC,VectInB)>>16;
        VectInB[0] = S1;
        VectInB[1] = S0;
        out2 = dotpv2(VectInC,VectInB)>>16;

        /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
        /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
      //  out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);

      //  out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);

        VectInB[0] = R0;
        VectInB[1] = -R1;
        out1 = dotpv2(VectInD,VectInB)>>16;
        VectInB[0] = R1;
        VectInB[1] = R0;
        out2 = dotpv2(VectInD,VectInB)>>16;
        /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
        /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;


      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /*  End of Middle stages process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point   */
  /* data is in 6.10(q10) format for the 64 point  */
  /* data is in 4.12(q12) format for the 16 point  */

  /* start of last stage process */


  /*  Initializations for the last stage */
  n1 = n2;

  n2 >>= 2u;

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = clip(T0 + S0,  -32768,32767);
    R1 = clip(T1 + S1, -32768,32767);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = clip(T0 - S0,  -32768,32767);
    S1 = clip(T1 - S1,  -32768,32767);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = clip(T0 + U0, -32768,32767);
    T1 = clip(T1 + U1,  -32768,32767);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);

    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i1 * 2u] = R0;
    pSrc16[(i1 * 2u) + 1u] = R1;

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd) */
    T0 = clip(T0 - U0,  -32768,32767);
    T1 = clip(T1 - U1,  -32768,32767);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xb' = (xa-yb-xc+yd) */
    /* yb' = (ya+xb-yc-xd) */
    pSrc16[i2 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i2 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);


    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd) */
    /* yd' = (ya-xb-yc+xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);
  }
  /* end of last stage  process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */


#else
  q15_t R0, R1, S0, S1, T0, T1, U0, U1;
  q15_t Co1, Si1, Co2, Si2, Co3, Si3, out1, out2;
  uint32_t n1, n2, ic, i0, i1, i2, i3, j, k;

  /* Total process is divided into three stages */

  /* process first stage, middle stages, & last stage */

  /*  Initializations for the first stage */
  n2 = fftLen;
  n1 = n2;

  /* n2 = fftLen/4 */
  n2 >>= 2u;

  /* Index for twiddle coefficient */
  ic = 0u;

  /* Index for input read and output write */
  i0 = 0u;

  j = n2;

  /* Input is in 1.15(q15) format */

  /*  Start of first stage process */
  do
  {
    /*  Butterfly implementation */

    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u] >> 2u;
    T1 = pSrc16[(i0 * 2u) + 1u] >> 2u;
    /* input is down scale by 4 to avoid overflow */
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u] >> 2u;
    S1 = pSrc16[(i2 * 2u) + 1u] >> 2u;

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = __SSAT(T0 + S0, 16u);
    R1 = __SSAT(T1 + S1, 16u);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = __SSAT(T0 - S0, 16u);
    S1 = __SSAT(T1 - S1, 16u);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* input is down scale by 4 to avoid overflow */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;
    /* Read yd (real), xd(imag) input */
    /* input is down scale by 4 to avoid overflow */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = __SSAT(T0 + U0, 16u);
    T1 = __SSAT(T1 + U1, 16u);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc)- (xb + xd) */
    R0 = __SSAT(R0 - T0, 16u);
    R1 = __SSAT(R1 - T1, 16u);
    /* co2 & si2 are read from Coefficient pointer */
    Co2 = pCoef16[2u * ic * 2u];
    Si2 = pCoef16[(2u * ic * 2u) + 1u];
    /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
    out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16u);
    /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
    out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16u);

    /*  Reading i0+fftLen/4 */
    /* input is down scale by 4 to avoid overflow */
    /* T0 = yb, T1 = xb */
    T0 = pSrc16[i1 * 2u] >> 2u;
    T1 = pSrc16[(i1 * 2u) + 1u] >> 2u;

    /* writing the butterfly processed i0 + fftLen/4 sample */
    /* writing output(xc', yc') in little endian format */
    pSrc16[i1 * 2u] = out1;
    pSrc16[(i1 * 2u) + 1u] = out2;

    /*  Butterfly calculations */
    /* input is down scale by 4 to avoid overflow */
    /* U0 = yd, U1 = xd) */
    U0 = pSrc16[i3 * 2u] >> 2u;
    U1 = pSrc16[(i3 * 2u) + 1u] >> 2u;

    /* T0 = yb-yd, T1 = xb-xd) */
    T0 = __SSAT(T0 - U0, 16u);
    T1 = __SSAT(T1 - U1, 16u);
    /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
    R0 = (q15_t) __SSAT((q31_t) (S0 + T1), 16);
    R1 = (q15_t) __SSAT((q31_t) (S1 - T0), 16);
    /* S = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
    S0 = (q15_t) __SSAT((q31_t) (S0 - T1), 16);
    S1 = (q15_t) __SSAT((q31_t) (S1 + T0), 16);

    /* co1 & si1 are read from Coefficient pointer */
    Co1 = pCoef16[ic * 2u];
    Si1 = pCoef16[(ic * 2u) + 1u];
    /*  Butterfly process for the i0+fftLen/2 sample */
    /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
    out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
    /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
    out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);
    /* writing output(xb', yb') in little endian format */
    pSrc16[i2 * 2u] = out1;
    pSrc16[(i2 * 2u) + 1u] = out2;

    /* Co3 & si3 are read from Coefficient pointer */
    Co3 = pCoef16[3u * ic * 2u];
    Si3 = pCoef16[(3u * ic * 2u) + 1u];
    /*  Butterfly process for the i0+3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
    out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);
    /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
    out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);
    /* writing output(xd', yd') in little endian format */
    pSrc16[i3 * 2u] = out1;
    pSrc16[(i3 * 2u) + 1u] = out2;

    /*  Twiddle coefficients index modifier */
    ic = ic + twidCoefModifier;

    /*  Updating input index */
    i0 = i0 + 1u;

  } while(--j);

  /*  End of first stage process */

  /* data is in 4.11(q11) format */


  /*  Start of Middle stage process */

  /*  Twiddle coefficients index modifier */
  twidCoefModifier <<= 2u;

  /*  Calculation of Middle stage */
  for (k = fftLen / 4u; k > 4u; k >>= 2u)
  {
    /*  Initializations for the middle stage */
    n1 = n2;
    n2 >>= 2u;
    ic = 0u;

    for (j = 0u; j <= (n2 - 1u); j++)
    {
      /*  index calculation for the coefficients */
      Co1 = pCoef16[ic * 2u];
      Si1 = pCoef16[(ic * 2u) + 1u];
      Co2 = pCoef16[2u * ic * 2u];
      Si2 = pCoef16[2u * ic * 2u + 1u];
      Co3 = pCoef16[3u * ic * 2u];
      Si3 = pCoef16[(3u * ic * 2u) + 1u];

      /*  Twiddle coefficients index modifier */
      ic = ic + twidCoefModifier;

      /*  Butterfly implementation */
      for (i0 = j; i0 < fftLen; i0 += n1)
      {
        /*  index calculation for the input as, */
        /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
        i1 = i0 + n2;
        i2 = i1 + n2;
        i3 = i2 + n2;

        /*  Reading i0, i0+fftLen/2 inputs */
        /* Read ya (real), xa(imag) input */
        T0 = pSrc16[i0 * 2u];
        T1 = pSrc16[(i0 * 2u) + 1u];

        /* Read yc (real), xc(imag) input */
        S0 = pSrc16[i2 * 2u];
        S1 = pSrc16[(i2 * 2u) + 1u];


        /* R0 = (ya + yc), R1 = (xa + xc) */
        R0 = __SSAT(T0 + S0, 16u);
        R1 = __SSAT(T1 + S1, 16u);
        /* S0 = (ya - yc), S1 = (xa - xc) */
        S0 = __SSAT(T0 - S0, 16u);
        S1 = __SSAT(T1 - S1, 16u);

        /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = (yb + yd), T1 = (xb + xd) */
        T0 = __SSAT(T0 + U0, 16u);
        T1 = __SSAT(T1 + U1, 16u);

        /*  writing the butterfly processed i0 sample */
        /* xa' = xa + xb + xc + xd */
        /* ya' = ya + yb + yc + yd */
        pSrc16[i0 * 2u] = ((R0 >> 1u) + (T0 >> 1u)) >> 1u;
        pSrc16[(i0 * 2u) + 1u] = ((R1 >> 1u) + (T1 >> 1u)) >> 1u;

        /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
        R0 = (R0 >> 1u) - (T0 >> 1u);
        R1 = (R1 >> 1u) - (T1 >> 1u);

        /* (ya-yb+yc-yd)* (si2) - (xa-xb+xc-xd)* co2 */
        out1 = (q15_t) ((Co2 * R0 - Si2 * R1) >> 16);
        /* (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
        out2 = (q15_t) ((Si2 * R0 + Co2 * R1) >> 16);

        /*  Reading i0+3fftLen/4 */
        /* Read yb (real), xb(imag) input */
        T0 = pSrc16[i1 * 2u];
        T1 = pSrc16[(i1 * 2u) + 1u];

        /*  writing the butterfly processed i0 + fftLen/4 sample */
        /* xc' = (xa-xb+xc-xd)* co2 - (ya-yb+yc-yd)* (si2) */
        /* yc' = (ya-yb+yc-yd)* co2 + (xa-xb+xc-xd)* (si2) */
        pSrc16[i1 * 2u] = out1;
        pSrc16[(i1 * 2u) + 1u] = out2;

        /*  Butterfly calculations */
        /* Read yd (real), xd(imag) input */
        U0 = pSrc16[i3 * 2u];
        U1 = pSrc16[(i3 * 2u) + 1u];

        /* T0 = yb-yd, T1 = xb-xd) */
        T0 = __SSAT(T0 - U0, 16u);
        T1 = __SSAT(T1 - U1, 16u);

        /* R0 = (ya-yc) - (xb- xd) , R1 = (xa-xc) + (yb-yd) */
        R0 = (S0 >> 1u) + (T1 >> 1u);
        R1 = (S1 >> 1u) - (T0 >> 1u);

        /* S1 = (ya-yc) + (xb- xd), S1 = (xa-xc) - (yb-yd) */
        S0 = (S0 >> 1u) - (T1 >> 1u);
        S1 = (S1 >> 1u) + (T0 >> 1u);

        /*  Butterfly process for the i0+fftLen/2 sample */
        out1 = (q15_t) ((Co1 * S0 - Si1 * S1) >> 16u);
        out2 = (q15_t) ((Si1 * S0 + Co1 * S1) >> 16u);
        /* xb' = (xa-yb-xc+yd)* co1 - (ya+xb-yc-xd)* (si1) */
        /* yb' = (ya+xb-yc-xd)* co1 + (xa-yb-xc+yd)* (si1) */
        pSrc16[i2 * 2u] = out1;
        pSrc16[(i2 * 2u) + 1u] = out2;

        /*  Butterfly process for the i0+3fftLen/4 sample */
        out1 = (q15_t) ((Co3 * R0 - Si3 * R1) >> 16u);

        out2 = (q15_t) ((Si3 * R0 + Co3 * R1) >> 16u);
        /* xd' = (xa+yb-xc-yd)* Co3 - (ya-xb-yc+xd)* (si3) */
        /* yd' = (ya-xb-yc+xd)* Co3 + (xa+yb-xc-yd)* (si3) */
        pSrc16[i3 * 2u] = out1;
        pSrc16[(i3 * 2u) + 1u] = out2;


      }
    }
    /*  Twiddle coefficients index modifier */
    twidCoefModifier <<= 2u;
  }
  /*  End of Middle stages process */


  /* data is in 10.6(q6) format for the 1024 point */
  /* data is in 8.8(q8) format for the 256 point   */
  /* data is in 6.10(q10) format for the 64 point  */
  /* data is in 4.12(q12) format for the 16 point  */

  /* start of last stage process */


  /*  Initializations for the last stage */
  n1 = n2;
  n2 >>= 2u;

  /*  Butterfly implementation */
  for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
  {
    /*  index calculation for the input as, */
    /*  pSrc16[i0 + 0], pSrc16[i0 + fftLen/4], pSrc16[i0 + fftLen/2], pSrc16[i0 + 3fftLen/4] */
    i1 = i0 + n2;
    i2 = i1 + n2;
    i3 = i2 + n2;

    /*  Reading i0, i0+fftLen/2 inputs */
    /* Read ya (real), xa(imag) input */
    T0 = pSrc16[i0 * 2u];
    T1 = pSrc16[(i0 * 2u) + 1u];
    /* Read yc (real), xc(imag) input */
    S0 = pSrc16[i2 * 2u];
    S1 = pSrc16[(i2 * 2u) + 1u];

    /* R0 = (ya + yc), R1 = (xa + xc) */
    R0 = __SSAT(T0 + S0, 16u);
    R1 = __SSAT(T1 + S1, 16u);
    /* S0 = (ya - yc), S1 = (xa - xc) */
    S0 = __SSAT(T0 - S0, 16u);
    S1 = __SSAT(T1 - S1, 16u);

    /*  Reading i0+fftLen/4 , i0+3fftLen/4 inputs */
    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];
    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];

    /* T0 = (yb + yd), T1 = (xb + xd) */
    T0 = __SSAT(T0 + U0, 16u);
    T1 = __SSAT(T1 + U1, 16u);

    /*  writing the butterfly processed i0 sample */
    /* xa' = xa + xb + xc + xd */
    /* ya' = ya + yb + yc + yd */
    pSrc16[i0 * 2u] = (R0 >> 1u) + (T0 >> 1u);
    pSrc16[(i0 * 2u) + 1u] = (R1 >> 1u) + (T1 >> 1u);

    /* R0 = (ya + yc) - (yb + yd), R1 = (xa + xc) - (xb + xd) */
    R0 = (R0 >> 1u) - (T0 >> 1u);
    R1 = (R1 >> 1u) - (T1 >> 1u);

    /* Read yb (real), xb(imag) input */
    T0 = pSrc16[i1 * 2u];
    T1 = pSrc16[(i1 * 2u) + 1u];

    /*  writing the butterfly processed i0 + fftLen/4 sample */
    /* xc' = (xa-xb+xc-xd) */
    /* yc' = (ya-yb+yc-yd) */
    pSrc16[i1 * 2u] = R0;
    pSrc16[(i1 * 2u) + 1u] = R1;

    /* Read yd (real), xd(imag) input */
    U0 = pSrc16[i3 * 2u];
    U1 = pSrc16[(i3 * 2u) + 1u];
    /* T0 = (yb - yd), T1 = (xb - xd) */
    T0 = __SSAT(T0 - U0, 16u);
    T1 = __SSAT(T1 - U1, 16u);

    /*  writing the butterfly processed i0 + fftLen/2 sample */
    /* xb' = (xa-yb-xc+yd) */
    /* yb' = (ya+xb-yc-xd) */
    pSrc16[i2 * 2u] = (S0 >> 1u) - (T1 >> 1u);
    pSrc16[(i2 * 2u) + 1u] = (S1 >> 1u) + (T0 >> 1u);


    /*  writing the butterfly processed i0 + 3fftLen/4 sample */
    /* xd' = (xa+yb-xc-yd) */
    /* yd' = (ya-xb-yc+xd) */
    pSrc16[i3 * 2u] = (S0 >> 1u) + (T1 >> 1u);
    pSrc16[(i3 * 2u) + 1u] = (S1 >> 1u) - (T0 >> 1u);
  }
  /* end of last stage  process */

  /* output is in 11.5(q5) format for the 1024 point */
  /* output is in 9.7(q7) format for the 256 point   */
  /* output is in 7.9(q9) format for the 64 point  */
  /* output is in 5.11(q11) format for the 16 point  */

#endif
}
