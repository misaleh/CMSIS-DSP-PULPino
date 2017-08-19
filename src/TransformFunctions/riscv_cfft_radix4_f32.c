/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015 
* $Revision: 	V.1.4.5  
*    
* Project: 	    CMSIS DSP Library    
* Title:	    arm_cfft_radix4_f32.c    
*    
* Description:	Radix-4 Decimation in Frequency CFFT & CIFFT Floating point processing function    
*    
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

extern void riscv_bitreversal_f32(
float32_t * pSrc,
uint16_t fftSize,
uint16_t bitRevFactor,
uint16_t * pBitRevTab);

/**    
* @ingroup groupTransforms    
*/

/* ----------------------------------------------------------------------    
** Internal helper function used by the FFTs    
** ------------------------------------------------------------------- */

/*    
* @brief  Core function for the floating-point CFFT butterfly process.   
* @param[in, out] *pSrc            points to the in-place buffer of floating-point data type.   
* @param[in]      fftLen           length of the FFT.   
* @param[in]      *pCoef           points to the twiddle coefficient buffer.   
* @param[in]      twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.   
* @return none.   
*/

void riscv_radix4_butterfly_f32(
float32_t * pSrc,
uint16_t fftLen,
float32_t * pCoef,
uint16_t twidCoefModifier)
{

   float32_t co1, co2, co3, si1, si2, si3;
   uint32_t ia1, ia2, ia3;
   uint32_t i0, i1, i2, i3;
   uint32_t n1, n2, j, k;

   float32_t t1, t2, r1, r2, s1, s2;

   /* Run the below code for Cortex-M0 */

   /*  Initializations for the fft calculation */
   n2 = fftLen;
   n1 = n2;
   for (k = fftLen; k > 1u; k >>= 2u)
   {
      /*  Initializations for the fft calculation */
      n1 = n2;
      n2 >>= 2u;
      ia1 = 0u;

      /*  FFT Calculation */
      j = 0;
      do
      {
         /*  index calculation for the coefficients */
         ia2 = ia1 + ia1;
         ia3 = ia2 + ia1;
         co1 = pCoef[ia1 * 2u];
         si1 = pCoef[(ia1 * 2u) + 1u];
         co2 = pCoef[ia2 * 2u];
         si2 = pCoef[(ia2 * 2u) + 1u];
         co3 = pCoef[ia3 * 2u];
         si3 = pCoef[(ia3 * 2u) + 1u];

         /*  Twiddle coefficients index modifier */
         ia1 = ia1 + twidCoefModifier;

         i0 = j;
         do
         {
            /*  index calculation for the input as, */
            /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
            i1 = i0 + n2;
            i2 = i1 + n2;
            i3 = i2 + n2;

            /* xa + xc */
            r1 = pSrc[(2u * i0)] + pSrc[(2u * i2)];

            /* xa - xc */
            r2 = pSrc[(2u * i0)] - pSrc[(2u * i2)];

            /* ya + yc */
            s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];

            /* ya - yc */
            s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

            /* xb + xd */
            t1 = pSrc[2u * i1] + pSrc[2u * i3];

            /* xa' = xa + xb + xc + xd */
            pSrc[2u * i0] = r1 + t1;

            /* xa + xc -(xb + xd) */
            r1 = r1 - t1;

            /* yb + yd */
            t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];

            /* ya' = ya + yb + yc + yd */
            pSrc[(2u * i0) + 1u] = s1 + t2;

            /* (ya + yc) - (yb + yd) */
            s1 = s1 - t2;

            /* (yb - yd) */
            t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];

            /* (xb - xd) */
            t2 = pSrc[2u * i1] - pSrc[2u * i3];

            /* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
            pSrc[2u * i1] = (r1 * co2) + (s1 * si2);

            /* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
            pSrc[(2u * i1) + 1u] = (s1 * co2) - (r1 * si2);

            /* (xa - xc) + (yb - yd) */
            r1 = r2 + t1;

            /* (xa - xc) - (yb - yd) */
            r2 = r2 - t1;

            /* (ya - yc) -  (xb - xd) */
            s1 = s2 - t2;

            /* (ya - yc) +  (xb - xd) */
            s2 = s2 + t2;

            /* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
            pSrc[2u * i2] = (r1 * co1) + (s1 * si1);

            /* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
            pSrc[(2u * i2) + 1u] = (s1 * co1) - (r1 * si1);

            /* xd' = (xa-yb-xc+yd)co3 + (ya+xb-yc-xd)(si3) */
            pSrc[2u * i3] = (r2 * co3) + (s2 * si3);

            /* yd' = (ya+xb-yc-xd)co3 - (xa-yb-xc+yd)(si3) */
            pSrc[(2u * i3) + 1u] = (s2 * co3) - (r2 * si3);
         
            i0 += n1;
         } while( i0 < fftLen);
         j++;
      } while(j <= (n2 - 1u));
      twidCoefModifier <<= 2u;
   }

}

/*    
* @brief  Core function for the floating-point CIFFT butterfly process.   
* @param[in, out] *pSrc            points to the in-place buffer of floating-point data type.   
* @param[in]      fftLen           length of the FFT.   
* @param[in]      *pCoef           points to twiddle coefficient buffer.   
* @param[in]      twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.   
* @param[in]      onebyfftLen      value of 1/fftLen.   
* @return none.   
*/

void riscv_radix4_butterfly_inverse_f32(
float32_t * pSrc,
uint16_t fftLen,
float32_t * pCoef,
uint16_t twidCoefModifier,
float32_t onebyfftLen)
{
   float32_t co1, co2, co3, si1, si2, si3;
   uint32_t ia1, ia2, ia3;
   uint32_t i0, i1, i2, i3;
   uint32_t n1, n2, j, k;


   float32_t t1, t2, r1, r2, s1, s2;

   /* Run the below code for Cortex-M0 */

   /*  Initializations for the first stage */
   n2 = fftLen;
   n1 = n2;

   /*  Calculation of first stage */
   for (k = fftLen; k > 4u; k >>= 2u)
   {
      /*  Initializations for the first stage */
      n1 = n2;
      n2 >>= 2u;
      ia1 = 0u;

      /*  Calculation of first stage */
      j = 0;
      do
      {
         /*  index calculation for the coefficients */
         ia2 = ia1 + ia1;
         ia3 = ia2 + ia1;
         co1 = pCoef[ia1 * 2u];
         si1 = pCoef[(ia1 * 2u) + 1u];
         co2 = pCoef[ia2 * 2u];
         si2 = pCoef[(ia2 * 2u) + 1u];
         co3 = pCoef[ia3 * 2u];
         si3 = pCoef[(ia3 * 2u) + 1u];

         /*  Twiddle coefficients index modifier */
         ia1 = ia1 + twidCoefModifier;

         i0 = j;
         do
         {
            /*  index calculation for the input as, */
            /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
            i1 = i0 + n2;
            i2 = i1 + n2;
            i3 = i2 + n2;

            /* xa + xc */
            r1 = pSrc[(2u * i0)] + pSrc[(2u * i2)];

            /* xa - xc */
            r2 = pSrc[(2u * i0)] - pSrc[(2u * i2)];

            /* ya + yc */
            s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];

            /* ya - yc */
            s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

            /* xb + xd */
            t1 = pSrc[2u * i1] + pSrc[2u * i3];

            /* xa' = xa + xb + xc + xd */
            pSrc[2u * i0] = r1 + t1;

            /* xa + xc -(xb + xd) */
            r1 = r1 - t1;

            /* yb + yd */
            t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];

            /* ya' = ya + yb + yc + yd */
            pSrc[(2u * i0) + 1u] = s1 + t2;

            /* (ya + yc) - (yb + yd) */
            s1 = s1 - t2;

            /* (yb - yd) */
            t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];

            /* (xb - xd) */
            t2 = pSrc[2u * i1] - pSrc[2u * i3];

            /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
            pSrc[2u * i1] = (r1 * co2) - (s1 * si2);

            /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
            pSrc[(2u * i1) + 1u] = (s1 * co2) + (r1 * si2);

            /* (xa - xc) - (yb - yd) */
            r1 = r2 - t1;

            /* (xa - xc) + (yb - yd) */
            r2 = r2 + t1;

            /* (ya - yc) +  (xb - xd) */
            s1 = s2 + t2;

            /* (ya - yc) -  (xb - xd) */
            s2 = s2 - t2;

            /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
            pSrc[2u * i2] = (r1 * co1) - (s1 * si1);

            /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
            pSrc[(2u * i2) + 1u] = (s1 * co1) + (r1 * si1);

            /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
            pSrc[2u * i3] = (r2 * co3) - (s2 * si3);

            /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
            pSrc[(2u * i3) + 1u] = (s2 * co3) + (r2 * si3);
         
            i0 += n1;
         } while( i0 < fftLen);
         j++;
      } while(j <= (n2 - 1u));
      twidCoefModifier <<= 2u;
   }
   /*  Initializations of last stage */
   n1 = n2;
   n2 >>= 2u;

   /*  Calculations of last stage */
   for (i0 = 0u; i0 <= (fftLen - n1); i0 += n1)
   {
      /*  index calculation for the input as, */
      /*  pSrc[i0 + 0], pSrc[i0 + fftLen/4], pSrc[i0 + fftLen/2], pSrc[i0 + 3fftLen/4] */
      i1 = i0 + n2;
      i2 = i1 + n2;
      i3 = i2 + n2;

      /*  Butterfly implementation */
      /* xa + xc */
      r1 = pSrc[2u * i0] + pSrc[2u * i2];

      /* xa - xc */
      r2 = pSrc[2u * i0] - pSrc[2u * i2];

      /* ya + yc */
      s1 = pSrc[(2u * i0) + 1u] + pSrc[(2u * i2) + 1u];

      /* ya - yc */
      s2 = pSrc[(2u * i0) + 1u] - pSrc[(2u * i2) + 1u];

      /* xc + xd */
      t1 = pSrc[2u * i1] + pSrc[2u * i3];

      /* xa' = xa + xb + xc + xd */
      pSrc[2u * i0] = (r1 + t1) * onebyfftLen;

      /* (xa + xb) - (xc + xd) */
      r1 = r1 - t1;

      /* yb + yd */
      t2 = pSrc[(2u * i1) + 1u] + pSrc[(2u * i3) + 1u];

      /* ya' = ya + yb + yc + yd */
      pSrc[(2u * i0) + 1u] = (s1 + t2) * onebyfftLen;

      /* (ya + yc) - (yb + yd) */
      s1 = s1 - t2;

      /* (yb-yd) */
      t1 = pSrc[(2u * i1) + 1u] - pSrc[(2u * i3) + 1u];

      /* (xb-xd) */
      t2 = pSrc[2u * i1] - pSrc[2u * i3];

      /* xc' = (xa-xb+xc-xd)co2 - (ya-yb+yc-yd)(si2) */
      pSrc[2u * i1] = r1 * onebyfftLen;

      /* yc' = (ya-yb+yc-yd)co2 + (xa-xb+xc-xd)(si2) */
      pSrc[(2u * i1) + 1u] = s1 * onebyfftLen;

      /* (xa - xc) - (yb-yd) */
      r1 = r2 - t1;

      /* (xa - xc) + (yb-yd) */
      r2 = r2 + t1;

      /* (ya - yc) + (xb-xd) */
      s1 = s2 + t2;

      /* (ya - yc) - (xb-xd) */
      s2 = s2 - t2;

      /* xb' = (xa+yb-xc-yd)co1 - (ya-xb-yc+xd)(si1) */
      pSrc[2u * i2] = r1 * onebyfftLen;

      /* yb' = (ya-xb-yc+xd)co1 + (xa+yb-xc-yd)(si1) */
      pSrc[(2u * i2) + 1u] = s1 * onebyfftLen;

      /* xd' = (xa-yb-xc+yd)co3 - (ya+xb-yc-xd)(si3) */
      pSrc[2u * i3] = r2 * onebyfftLen;

      /* yd' = (ya+xb-yc-xd)co3 + (xa-yb-xc+yd)(si3) */
      pSrc[(2u * i3) + 1u] = s2 * onebyfftLen;
   }
}

/**    
* @addtogroup ComplexFFT    
* @{    
*/

/**    
* @details    
* @brief Processing function for the floating-point Radix-4 CFFT/CIFFT.   
* @deprecated Do not use this function.  It has been superseded by \ref riscv_cfft_f32 and will be removed
* in the future.
* @param[in]      *S    points to an instance of the floating-point Radix-4 CFFT/CIFFT structure.   
* @param[in, out] *pSrc points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place.   
* @return none.   
*/

void riscv_cfft_radix4_f32(
const riscv_cfft_radix4_instance_f32 * S,
float32_t * pSrc)
{

   if(S->ifftFlag == 1u)
   {
      /*  Complex IFFT radix-4  */
      riscv_radix4_butterfly_inverse_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier, S->onebyfftLen);
   }
   else
   {
      /*  Complex FFT radix-4  */
      riscv_radix4_butterfly_f32(pSrc, S->fftLen, S->pTwiddle,
      S->twidCoefModifier);
   }

   if(S->bitReverseFlag == 1u)
   {
      /*  Bit Reversal */
      riscv_bitreversal_f32(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
   }

}

/**    
* @} end of ComplexFFT group    
*/

