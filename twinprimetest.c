#include <inttypes.h>
#include <stdio.h>
#include <immintrin.h>

#include "table.h"


#undef  USERECIP


int main( int argc, char* argv[] )
{
	const __m256* reader = (const __m256*) twins;

	__m256 accum = _mm256_setzero_ps();

	const int numblocks = NUMP/8;

	for ( int idx=numblocks-1; idx>=0; idx-- )
	{
		const __m256 p8 = reader[idx];
#if defined( USERECIP )
		// Use 8-way SIMD reciprocal, which is faster.
		const __m256 r8 = _mm256_rcp_ps( p8 );
#else
		// Use slower 8-way SIMD floating point division.
		const __m256 one8 = _mm256_set1_ps( 1.0f );
		const __m256 r8 = _mm256_div_ps( one8, p8 );
#endif
		accum = _mm256_add_ps( accum, r8 );
	}

	float res[8] __attribute__ ((aligned(32)));
	_mm256_store_ps( res, accum );

	const float total = res[0]+res[1]+res[2]+res[3]+res[4]+res[5]+res[6]+res[7];

	printf( "Sum: %f\n", total );

	return 0;
}

