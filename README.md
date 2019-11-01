# Twin Prime Test

Test the twin primes like Thomas Nicely did, but on AVX.

Scratching an intellectual curiosity, sparked by 
[This Tweet](https://twitter.com/fermatslibrary/status/1189900586365464588?s=20).


## Dependencies

* Python3
* Sympy

## Results

I find that it converges very slowly:

* At 8192 pairs (from primes below 1M or so): 1.710823
* At 813368 pairs (from primes below 200M): 1.761476
* At 813368 pairs, adding in reversed order: 1.764010
* At 813368 pairs, adding reversed, double prec: 1.764010
* At 813368 pairs, scalar operations, dp: 1.764010

So I switched to my own C implementation for sieving primes.

* At 79556299 pairs: 1.793023


