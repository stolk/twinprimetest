#include <inttypes.h>
#include <stdio.h>
#include <immintrin.h>

#define DP

#if defined(DP)
#	include "table_dp.h"
#else
#	include "table_sp.h"
#endif


#undef  USERECIP

void doubleprec(const __m256* reader)
{
	__m256 accum = _mm256_setzero_ps();

	const int numblocks = NUMP/4;

	for ( int idx=numblocks-1; idx>=0; idx-- )
	{
		const __m256 p4 = reader[idx];
#if defined(USERECIP)
		// Use 4-way SIMD reciprocal, which is faster.
		const __m256 r4 = _mm256_rcp_pd( p4 );
#else
		// Use slower 4-way SIMD floating point division.
		const __m256 one4 = _mm256_set1_pd( 1.0 );
		const __m256 r4 = _mm256_div_pd( one4, p4 );
#endif
		accum = _mm256_add_pd( accum, r4 );
	}

	double res[4] __attribute__ ((aligned(32)));
	_mm256_store_pd( res, accum );

	const double total = res[0]+res[1]+res[2]+res[3];

	printf( "Sum: %lf\n", total );
}


void singleprec(const __m256* reader)
{
	__m256 accum = _mm256_setzero_ps();

	const int numblocks = NUMP/8;

	for ( int idx=numblocks-1; idx>=0; idx-- )
	{
		const __m256 p8 = reader[idx];
#if defined(USERECIP)
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

}


int main( int argc, char* argv[] )
{
#if defined(DP)
	const __m256* reader = (const __m256*) twins_dp;
	doubleprec( reader );
#else
	const __m256* reader = (const __m256*) twins_sp;
	singleprec( reader );
#endif
	return 0;
}

