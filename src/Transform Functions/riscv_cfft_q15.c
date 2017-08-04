/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015 
* $Revision: 	V.1.4.5  
*    
* Project: 	    CMSIS DSP Library    
* Title:	    arm_cfft_q15.c   
*    
* Description:	Combined Radix Decimation in Q15 Frequency CFFT processing function
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

extern void riscv_radix4_butterfly_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pCoef,
    uint32_t twidCoefModifier);

extern void riscv_radix4_butterfly_inverse_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    q15_t * pCoef,
    uint32_t twidCoefModifier);

extern void riscv_bitreversal_16(
    uint16_t * pSrc,
    const uint16_t bitRevLen,
    const uint16_t * pBitRevTable);
    
void riscv_cfft_radix4by2_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    const q15_t * pCoef);
    
void riscv_cfft_radix4by2_inverse_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    const q15_t * pCoef);

/**   
* @ingroup groupTransforms   
*/

/**
* @addtogroup ComplexFFT   
* @{   
*/

/**   
* @details   
* @brief       Processing function for the Q15 complex FFT.
* @param[in]      *S    points to an instance of the Q15 CFFT structure.  
* @param[in, out] *p1   points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place.  
* @param[in]     ifftFlag       flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform.  
* @param[in]     bitReverseFlag flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output.  
* @return none.  
*/

void riscv_cfft_q15( 
    const riscv_cfft_instance_q15 * S, 
    q15_t * p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag)
{
    uint32_t L = S->fftLen;

    if(ifftFlag == 1u)
    {
        switch (L) 
        {
        case 16: 
        case 64:
        case 256:
        case 1024:
        case 4096:
            riscv_radix4_butterfly_inverse_q15  ( p1, L, (q15_t*)S->pTwiddle, 1 );
            break;
            
        case 32:
        case 128:
        case 512:
        case 2048:
            riscv_cfft_radix4by2_inverse_q15  ( p1, L, S->pTwiddle );
            break;
        }  
    }
    else
    {
        switch (L) 
        {
        case 16: 
        case 64:
        case 256:
        case 1024:
        case 4096:
            riscv_radix4_butterfly_q15  ( p1, L, (q15_t*)S->pTwiddle, 1 );
            break;
            
        case 32:
        case 128:
        case 512:
        case 2048:
            riscv_cfft_radix4by2_q15  ( p1, L, S->pTwiddle );
            break;
        }  
    }
    
    if( bitReverseFlag )
        riscv_bitreversal_16((uint16_t*)p1,S->bitRevLength,S->pBitRevTable);    
}

/**    
* @} end of ComplexFFT group    
*/

void riscv_cfft_radix4by2_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    const q15_t * pCoef) 
{    
    uint32_t i;
    uint32_t n2;
    q15_t p0, p1, p2, p3;

    uint32_t ia, l;
    q15_t xt, yt, cosVal, sinVal;

    shortV VectInA;
    shortV VectInB;
    shortV *VectInC;
    n2 = fftLen >> 1; 

    
    ia = 0;
    for (i = 0; i < n2; i++)
    {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia++;
        
        l = i + n2;        
        
        xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
        pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
        
        yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
        pSrc[2 * i + 1] =
        ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;

        pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) +
                  ((int16_t) (((q31_t) yt * sinVal) >> 16)));

        pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) -
                       ((int16_t) (((q31_t) xt * sinVal) >> 16)));
    } 
    

    
    // first col
    riscv_radix4_butterfly_q15( pSrc, n2, (q15_t*)pCoef, 2u);
    // second col
    riscv_radix4_butterfly_q15( pSrc + fftLen, n2, (q15_t*)pCoef, 2u);
    VectInB = pack2(1,1);		
    for (i = 0; i < fftLen >> 1; i++)
    {
#if defined (USE_DSP_RISCV)
     VectInC = (shortV*)(pSrc + 4*i);
     *VectInC = sll2(*VectInC,VectInB);
     VectInC = (shortV*)(pSrc + 4*i + 2);
     *VectInC = sll2(*VectInC,VectInB);
#else
        p0 = pSrc[4*i+0];
        p1 = pSrc[4*i+1];
        p2 = pSrc[4*i+2];
        p3 = pSrc[4*i+3];
        
        p0 <<= 1;
        p1 <<= 1;
        p2 <<= 1;
        p3 <<= 1;
        
        pSrc[4*i+0] = p0;
        pSrc[4*i+1] = p1;
        pSrc[4*i+2] = p2;
        pSrc[4*i+3] = p3;
#endif

    }
}

void riscv_cfft_radix4by2_inverse_q15(
    q15_t * pSrc,
    uint32_t fftLen,
    const q15_t * pCoef) 
{    
    uint32_t i;
    uint32_t n2;
    q15_t p0, p1, p2, p3;

    uint32_t ia, l;
    q15_t xt, yt, cosVal, sinVal;

    
    n2 = fftLen >> 1; 

    ia = 0;
    for (i = 0; i < n2; i++)
    {
        cosVal = pCoef[ia * 2];
        sinVal = pCoef[(ia * 2) + 1];
        ia++;
        
        l = i + n2;
        xt = (pSrc[2 * i] >> 1u) - (pSrc[2 * l] >> 1u);
        pSrc[2 * i] = ((pSrc[2 * i] >> 1u) + (pSrc[2 * l] >> 1u)) >> 1u;
        
        yt = (pSrc[2 * i + 1] >> 1u) - (pSrc[2 * l + 1] >> 1u);
        pSrc[2 * i + 1] =
          ((pSrc[2 * l + 1] >> 1u) + (pSrc[2 * i + 1] >> 1u)) >> 1u;
        
        pSrc[2u * l] = (((int16_t) (((q31_t) xt * cosVal) >> 16)) -
                        ((int16_t) (((q31_t) yt * sinVal) >> 16)));
        
        pSrc[2u * l + 1u] = (((int16_t) (((q31_t) yt * cosVal) >> 16)) +
                           ((int16_t) (((q31_t) xt * sinVal) >> 16)));
    } 
    

    // first col
    riscv_radix4_butterfly_inverse_q15( pSrc, n2, (q15_t*)pCoef, 2u);
    // second col
    riscv_radix4_butterfly_inverse_q15( pSrc + fftLen, n2, (q15_t*)pCoef, 2u);
			
    for (i = 0; i < fftLen >> 1; i++)
    {
        p0 = pSrc[4*i+0];
        p1 = pSrc[4*i+1];
        p2 = pSrc[4*i+2];
        p3 = pSrc[4*i+3];
        
        p0 <<= 1;
        p1 <<= 1;
        p2 <<= 1;
        p3 <<= 1;
        
        pSrc[4*i+0] = p0;
        pSrc[4*i+1] = p1;
        pSrc[4*i+2] = p2;
        pSrc[4*i+3] = p3;
    }
}

