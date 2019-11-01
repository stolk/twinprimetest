# Twin Prime Test

Test the twin primes like Thomas Nicely did, but on AVX.

Scratching an intellectual curiosity, sparked by 
[This Tweet](https://twitter.com/fermatslibrary/status/1189900586365464588?s=20).


## Dependencies

* Python3
* Sympy

## Results

I find that it converges very slowly:

* At 8192 pairs (from primes below 1M or so,) I would get 1.710823
* At 813368 pairs (from primes below 200M,) I would get 1.761476

## Future Work

* Find a public dataset of the first 
