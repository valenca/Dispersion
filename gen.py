#!/usr/bin/python

import sys
import random

def help():
	print("## Random coordinate generator ##")
	print("\nCall with 2 integer parameters:\n\tNumber of coordinates to generate\n\tNumber of dimensions minimum 2")
	print("\nExample:\n\tpython gen.py 4 2")
	print("\nOutput:\n\tNumber of coordinates\n\tDimension number\n\tList of coordinates")
	exit(0)
	
def error(st):
	print(st)
	exit(0)

def gen(n,d):
	limit = 1000
	print(n)
	print(d)
	for i in range(n):
		for j in range(d):
			r = (random.random()-0.5)*2*limit
			print(round(r,4))
	


def main(argv=None):
	if(argv==None):
		argv=sys.argv

	if(isinstance(argv, list)==False):
		error("Argv must be a list")

	argc=len(argv)

	if("help" in argv or argc!=3):
		help()
	
	try:
		n = int(argv[1])
		d = int(argv[2])
	except:
		help()

	if(d<2):
		help()

	gen(n,d)

if __name__ == "__main__":
	main();


