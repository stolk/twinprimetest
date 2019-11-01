#include <stdio.h>
#include <inttypes.h>
#include <immintrin.h>
#include <string.h>
#include <math.h>

#if 0
void print_binary(uint64_t v)
{
	for ( int i=0; i<64; ++i )
	{
		char c = ( v & (1UL<<i) ) ? '1':'0';
		printf( "%c", c );
	}
	printf("\n");
}
#endif


uint64_t sieve( uint64_t maxp, uint64_t bufsz, uint64_t* buffer )
{
	const uint64_t last    = (uint64_t) sqrt(maxp);		// we should scan up to sqrt of max prime.
	const uint64_t numoddp = maxp / 2;			// nr of odd primes we will consider.
	const uint64_t blocksz = sizeof(uint64_t);		// size of a block of packed booleans.
	const uint64_t cnt     = numoddp / 64;			// nr of 64-bit bit-sets we need.
	const size_t   sz      = blocksz * cnt;

	fprintf( stderr, "Using %ld 64-bit integers as table.\n", cnt );

	uint64_t* oddprim = (uint64_t*) aligned_alloc( 32, sz );
	memset( oddprim, -1, sz );

	for ( uint64_t factor=3; factor<=last; factor+=2 )
	{
		const uint64_t idx = (factor>>1UL);
		const uint64_t maj = (idx>>6UL);
		const uint64_t min = (idx&63UL);
		if ( ( oddprim[maj] & (1UL<<min) ) != 0 )
		{
			for ( uint64_t mult=factor*factor; mult<maxp; mult += factor )
			{
				if ( mult&1 )
				{
					const uint64_t midx = (mult>>1UL);
					const uint64_t mmaj = (midx>>6UL);
					const uint64_t mmin = (midx&63UL);
					oddprim[ mmaj ] &= ~(1UL<<mmin);
				}
			}
		}
	}

	uint64_t numprimes = 1;	// 2 is prime too!
	uint64_t numpairs  = 0;
	uint64_t written   = 0;
	for ( uint64_t cand=3; cand<maxp; cand+=2 )
	{
		const uint64_t idx0 = (cand>>1UL);
		const uint64_t maj0 = (idx0>>6UL);
		const uint64_t min0 = (idx0&63UL);
		if ( ( oddprim[maj0] & (1UL<<min0) ) != 0 )
		{
			numprimes++;
			const uint64_t idx1 = idx0+1;
			const uint64_t maj1 = (idx1>>6UL);
			const uint64_t min1 = (idx1&63UL);
			if ( ( oddprim[maj1] & (1UL<<min1) ) != 0 )
			{
				numpairs++;
#if 0
				const double primespernr = numprimes / (double) (2*cand);
				const double pairsperprime = numpairs / (double) numprimes;
				printf( "%lu\t%lf\t%lf\n", cand, primespernr, pairsperprime );
#else
				if ( written < bufsz )
				{
					buffer[ written++ ] = cand;
					buffer[ written++ ] = cand+2;
				}
#endif
			}
		}
	}

	double ratio = (numpairs*2UL) / (double) numprimes;
	fprintf( stderr, "Generated %lu primes, %lu of which are twins. (%f percent)\n", numprimes, numpairs*2, ratio*100 );

	return numpairs;
}


#if 0
int main( int argc, char* argv[] )
{
	if ( argc!=2 )
	{
		fprintf( stderr, "Usage: %s maxprime\n", argv[0] );
		return -1;
	}

	const uint64_t maxp    = atol( argv[1] );		// highest prime nr we will consider.
	sieve( maxp );
	return 0;
}
#endif

