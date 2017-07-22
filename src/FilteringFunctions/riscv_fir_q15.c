/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015
* $Revision: 	V.1.4.5
*    
* Project: 	    CMSIS DSP Library    
* Title:        arm_fir_q15.c    
*    
* Description:  Q15 FIR filter processing function.    
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

/**       
 * @ingroup groupFilters       
 */

/**       
 * @addtogroup FIR       
 * @{       
 */

/**       
 * @brief Processing function for the Q15 FIR filter.       
 * @param[in] *S points to an instance of the Q15 FIR structure.       
 * @param[in] *pSrc points to the block of input data.       
 * @param[out] *pDst points to the block of output data.       
 * @param[in]  blockSize number of samples to process per call.       
 * @return none.       
 *   
 *   
 * \par Restrictions   
 *  If the silicon does not support unaligned memory access enable the macro UNALIGNED_SUPPORT_DISABLE   
 *	In this case input, output, state buffers should be aligned by 32-bit   
 *   
 * <b>Scaling and Overflow Behavior:</b>       
 * \par       
 * The function is implemented using a 64-bit internal accumulator.       
 * Both coefficients and state variables are represented in 1.15 format and multiplications yield a 2.30 result.       
 * The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.       
 * There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.       
 * After all additions have been performed, the accumulator is truncated to 34.15 format by discarding low 15 bits.       
 * Lastly, the accumulator is saturated to yield a result in 1.15 format.       
 *       
 * \par       
 * Refer to the function <code>riscv_fir_fast_q15()</code> for a faster but less precise implementation of this function.       
 */


void riscv_fir_q15(
  const riscv_fir_instance_q15 * S,
  q15_t * pSrc,
  q15_t * pDst,
  uint32_t blockSize)
{
  q15_t *pState = S->pState;                     /* State pointer */
  q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
  q15_t *pStateCurnt;                            /* Points to the current sample of the state */



  q15_t *px;                                     /* Temporary pointer for state buffer */
  q15_t *pb;                                     /* Temporary pointer for coefficient buffer */
  q63_t acc;                                     /* Accumulator */
  uint32_t numTaps = S->numTaps;                 /* Number of nTaps in the filter */
  uint32_t tapCnt, blkCnt;                       /* Loop counters */

  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = &(S->pState[(numTaps - 1u)]);

  /* Initialize blkCnt with blockSize */
  blkCnt = blockSize;

  while(blkCnt > 0u)
  {
    /* Copy one sample at a time into state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Set the accumulator to zero */
    acc = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize Coefficient pointer */
    pb = pCoeffs;

    tapCnt = numTaps;

    /* Perform the multiply-accumulates */
    do
    {
      /* acc =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0] */
      acc += (q31_t) * px++ * *pb++;
      tapCnt--;
    } while(tapCnt > 0u);

    /* The result is in 2.30 format.  Convert to 1.15         
     ** Then store the output in the destination buffer. */
    *pDst++ = (q15_t) __SSAT((acc >> 15u), 16);

    /* Advance state pointer by 1 for the next sample */
    pState = pState + 1;

    /* Decrement the samples loop counter */
    blkCnt--;
  }

  /* Processing is complete.         
   ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.       
   ** This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;

  /* Copy numTaps number of values */
  tapCnt = (numTaps - 1u);

  /* copy data */
  while(tapCnt > 0u)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement the loop counter */
    tapCnt--;
  }

}




/**       
 * @} end of FIR group       
 */
