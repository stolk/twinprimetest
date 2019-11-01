#include <inttypes.h>
#include <stdio.h>
#include <immintrin.h>

#include "sieve.h"

#undef	DPVEC
#define	DPSCALAR


#undef  USERECIP

void double_prec_scalar(int nump, double* reader)
{
	double accum = 0.0;
	for ( int idx=nump-1; idx>=0; idx-- )
	{
		accum += (1.0 / reader[idx]);
	}
	printf( "Sum: %lf\n", accum );
}

void double_prec_vector(int nump, const __m256* reader)
{
	__m256 accum = _mm256_setzero_ps();

	const int numblocks = nump/4;

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


int main( int argc, char* argv[] )
{
	const uint64_t bufsz = 30UL * 2 * 4000000;
	const uint64_t maxp  = 30UL * 1024*1024*1024;

	uint64_t* twinprimes = (uint64_t*) malloc( sizeof(uint64_t) * bufsz );
	const uint64_t numpairs = sieve( maxp, bufsz, twinprimes );

	fprintf( stderr, "Largest twin prime nr: %lu\n", twinprimes[ numpairs*2-1 ] );

	const uint64_t numpr = numpairs*2;
	double* twinprimes_dp = (double*) malloc( sizeof(double) * numpr );

	for ( uint64_t i=0; i<numpr; ++i )
		twinprimes_dp[i] = (double) twinprimes[i];

	free( twinprimes );
	twinprimes = 0;

#if defined(DPVEC)
	const __m256* reader = (const __m256*) twinprimes_dp;
	double_prec_vector( numpr, reader );
#elif defined(SPVEC)
	// TODO
#elif defined(DPSCALAR)
	double_prec_scalar( numpr, twinprimes_dp );
#endif
	return 0;
}

