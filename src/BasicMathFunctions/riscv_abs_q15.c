/* ----------------------------------------------------------------------    
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.    
*    
* $Date:        19. March 2015
* $Revision: 	V.1.4.5
*    
* Project: 	    CMSIS DSP Library    
* Title:		arm_abs_q15.c    
*    
* Description:	Q15 vector absolute value.    
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
 * @ingroup groupMath    
 */

/**    
 * @addtogroup BasicAbs    
 * @{    
 */

/**    
 * @brief Q15 vector absolute value.    
 * @param[in]       *pSrc points to the input buffer    
 * @param[out]      *pDst points to the output buffer    
 * @param[in]       blockSize number of samples in each vector    
 * @return none.    
 *    
 * <b>Scaling and Overflow Behavior:</b>    
 * \par    
 * The function uses saturating arithmetic.    
 */

void riscv_abs_q15(
  q15_t * pSrc,
  q15_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* loop counter */
  q15_t in;                                      /* Temporary input variable */
#if defined (USE_DSP_RISCV)

  shortV *VectInA;
  shortV VectInC; 

  blkCnt = blockSize >> 1u; /*divide by 2, to operate on 2 elements in the same loop*/
  while (blkCnt > 0u)
  {
    /* C = A + B */
    /* Add and then store the results in the destination buffer. */
    VectInA = (shortV*)pSrc;
    VectInC = abs2(*VectInA); /*abs of 2 elements at the same time*/
    /* check for saturation*/
    *pDst++ = ( VectInC[0] == -32768)?0x7fff:VectInC[0]; 
    *pDst++ = ( VectInC[1] == -32768)?0x7fff:VectInC[1];
    pSrc+=2; /*inc src buffer for next loop*/
    /* Decrement the loop counter */
    blkCnt--;
  }

  blkCnt = blockSize % 0x2u;
  while (blkCnt > 0u)
  {
    /* C = A + B */
    /* Add and then store the results in the destination buffer. */
    in = *pSrc++;
    *pDst++ = (in > 0) ? in :(q15_t)clip((q31_t)(-in),-32768,32767);
    /* Decrement the loop counter */
    blkCnt--;
  }

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

  while(blkCnt > 0u)
  {
    /* C = |A| */
    /* Read the input */
    in = *pSrc++;

    /* Calculate absolute value of input and then store the result in the destination buffer. */
    *pDst++ = (in > 0) ? in : ((in == (q15_t) 0x8000) ? 0x7fff : -in);

    /* Decrement the loop counter */
    blkCnt--;
  }
#endif

}

/**    
 * @} end of BasicAbs group    
 */
