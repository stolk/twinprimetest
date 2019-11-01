#!/usr/bin/python3

import sys

import sympy

l = [ n for n in sympy.primerange(1,1002750) if sympy.isprime(n+2) ]
sz = len( l )

print( "Generated %d twins." % ( sz, ) )

oname = sys.argv[1]
f = open( oname, "w" )
assert f

f.write( "#define NUMP %d\n" % ( sz*2, ) )
f.write( "static float twins[NUMP] __attribute__ ((aligned(32))) = {\n" )
rows = int( sz / 8 )

for r in range(rows) :
	f.write( "  " )
	for c in range(8) :
		p = l[r*8+c]
		f.write( "%d,%d, " % ( p, p+2 ) )
	f.write( "\n" )

f.write( "};\n" )

f.close()

