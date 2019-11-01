#!/usr/bin/python3

import sys

import sympy

#limit=1002750	# This will give 8192 twins.
limit=200000000	# This will give X twins.

l = [ n for n in sympy.primerange(1,limit) if sympy.isprime(n+2) ]
sz = len( l )
rows = int( sz / 8 )
sz = rows * 8

print( "Generated %d twins." % ( sz, ) )

oname = sys.argv[1]
f = open( oname, "w" )
assert f

f.write( "#define NUMP %d\n" % ( sz*2, ) )
f.write( "static double twins[NUMP] __attribute__ ((aligned(32))) = {\n" )

for r in range(rows) :
	f.write( "  " )
	for c in range(8) :
		p = l[r*8+c]
		f.write( "%d,%d, " % ( p, p+2 ) )
	f.write( "\n" )

f.write( "};\n" )

f.close()

