/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015
* $Revision: 	V.1.4.5
*    
* Project: 	    CMSIS DSP Library    
* Title:        arm_fir_q7.c    
*    
* Description:  Q7 FIR filter processing function.    
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
 * @param[in]   *S points to an instance of the Q7 FIR filter structure.    
 * @param[in]   *pSrc points to the block of input data.    
 * @param[out]  *pDst points to the block of output data.    
 * @param[in]   blockSize number of samples to process per call.    
 * @return 	none.    
 *    
 * <b>Scaling and Overflow Behavior:</b>    
 * \par    
 * The function is implemented using a 32-bit internal accumulator.    
 * Both coefficients and state variables are represented in 1.7 format and multiplications yield a 2.14 result.    
 * The 2.14 intermediate results are accumulated in a 32-bit accumulator in 18.14 format.    
 * There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.    
 * The accumulator is converted to 18.7 format by discarding the low 7 bits.    
 * Finally, the result is truncated to 1.7 format.    
 */

void riscv_fir_q7(
  const riscv_fir_instance_q7 * S,
  q7_t * pSrc,
  q7_t * pDst,
  uint32_t blockSize)
{

  uint32_t numTaps = S->numTaps;                 /* Number of taps in the filter */
  uint32_t i, blkCnt;                            /* Loop counters */
  q7_t *pState = S->pState;                      /* State pointer */
  q7_t *pCoeffs = S->pCoeffs;                    /* Coefficient pointer */
  q7_t *px, *pb;                                 /* Temporary pointers to state and coeff */
  q31_t acc = 0;                                 /* Accumlator */
  q7_t *pStateCurnt;                             /* Points to the current sample of the state */


  /* S->pState points to state array which contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = S->pState + (numTaps - 1u);

  /* Initialize blkCnt with blockSize */
  blkCnt = blockSize;

  /* Perform filtering upto BlockSize - BlockSize%4  */
  while(blkCnt > 0u)
  {
    /* Copy one sample at a time into state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Set accumulator to zero */
    acc = 0;

    /* Initialize state pointer of type q7 */
    px = pState;

    /* Initialize coeff pointer of type q7 */
    pb = pCoeffs;


    i = numTaps;

    while(i > 0u)
    {
      /* acc =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0] */
      acc += (q15_t) * px++ * *pb++;
      i--;
    }

    /* Store the 1.7 format filter output in destination buffer */
    *pDst++ = (q7_t) __SSAT((acc >> 7), 8);

    /* Advance the state pointer by 1 to process the next sample */
    pState = pState + 1;

    /* Decrement the loop counter */
    blkCnt--;
  }

  /* Processing is complete.         
   ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.       
   ** This prepares the state buffer for the next function call. */


  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;


  /* Copy numTaps number of values */
  i = (numTaps - 1u);

  /* Copy q7_t data */
  while(i > 0u)
  {
    *pStateCurnt++ = *pState++;
    i--;
  }

}

/**    
 * @} end of FIR group    
 */
