#include "ref.h"

void ref_abs_f32(
  float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
	uint32_t i;
	
	for(i=0;i<blockSize;i++)
	{
		pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
	}
}

void ref_abs_q31(
  q31_t * pSrc,
  q31_t * pDst,
  uint32_t blockSize)
{
	uint32_t i;
	
	for(i=0;i<blockSize;i++)
	{
		pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
                if(pSrc[i] == INT32_MIN) 
                     pDst[i] = 0x7fffffff;

	}
}

void ref_abs_q15(
  q15_t * pSrc,
  q15_t * pDst,
  uint32_t blockSize)
{
	uint32_t i;
	
	for(i=0;i<blockSize;i++)
	{
		pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
                if(pSrc[i] == -32768) pDst[i] = 32767;
	}
}

void ref_abs_q7(
  q7_t * pSrc,
  q7_t * pDst,
  uint32_t blockSize)
{
	uint32_t i;
	
	for(i=0;i<blockSize;i++)
	{
		pDst[i] = pSrc[i] < 0 ? -pSrc[i] : pSrc[i];
                if(pSrc[i] == -128) pDst[i] = 127;
	}
}
