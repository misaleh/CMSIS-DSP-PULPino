;/* ----------------------------------------------------------------------
;* Copyright (C) 2010-2014 ARM Limited. All rights reserved.
;*
;* $Date:       19. March 2015 
;* $Revision: 	V.1.4.5
;*
;* Project: 	CMSIS DSP Library
;* Title:	    arm_bitreversal2.S
;*
;* Description:	This is the arm_bitreversal_32 function done in
;*              assembly for maximum speed.  This function is called
;*              after doing an fft to reorder the output.  The function
;*              is loop unrolled by 2. arm_bitreversal_16 as well.
;*
;* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
;*
;* Redistribution and use in source and binary forms, with or without
;* modification, are permitted provided that the following conditions
;* are met:
;*   - Redistributions of source code must retain the above copyright
;*     notice, this list of conditions and the following disclaimer.
;*   - Redistributions in binary form must reproduce the above copyright
;*     notice, this list of conditions and the following disclaimer in
;*     the documentation and/or other materials provided with the
;*     distribution.
;*   - Neither the name of ARM LIMITED nor the names of its contributors
;*     may be used to endorse or promote products derived from this
;*     software without specific prior written permission.
;*
;* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
;* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
;* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
;* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
;* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
;* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;* POSSIBILITY OF SUCH DAMAGE.
;*
;*  Modifications 2017  Mostafa Saleh       (Ported to RISC-V PULPino)
;* -------------------------------------------------------------------- */







;/*
;* @brief  In-place bit reversal function.
;* @param[in, out] *pSrc        points to the in-place buffer of unknown 32-bit data type.
;* @param[in]      bitRevLen    bit reversal table length
;* @param[in]      *pBitRevTab  points to bit reversal table.
;* @return none.
;*/


.text
.globl	riscv_bitreversal_32
.type	riscv_bitreversal_32, @function


riscv_bitreversal_32 :
	add      a3,a1,1  
	add      a1,a2,0  
	srl     a3,a3,1  
riscv_bitreversal_32_0 :
	lh      a2,2(a1) 
	lh      a6,0(a1) 
	add     a2,a0,a2 
	add     a6,a0,a6  
	lw      a5,0(a2)
	lw      a4,0(a6)
	sw      a5,0(a6)
	sw      a4,0(a2)
	lw      a5,4(a2)
	lw      a4,4(a6)
	sw      a5,4(a6)
	sw      a4,4(a2)
	add     a1,a1,4  
	add     a3,a3,-1    
	bnez     a3 , riscv_bitreversal_32_0
	jr	ra


.globl	riscv_bitreversal_16
.type	riscv_bitreversal_16, @function

riscv_bitreversal_16 :
	add      a3,a1,1  
	add      a1,a2,0  
	srl     a3,a3,1  
riscv_bitreversal_16_0 :
	lh      a2,2(a1) 
	lh      a6,0(a1) 
	srl     a2,a2,1  
	srl     a6,a6,1  
	add     a2,a0,a2 
	add     a6,a0,a6  
	lw      a5,0(a2)
	lw      a4,0(a6)
	sw      a5,0(a6)
	sw      a4,0(a2)
	add     a1,a1,4
	add     a3,a3,-1
	bnez     a3 , riscv_bitreversal_16_0
	jr	ra

