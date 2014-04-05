import argparse
import random

def gen(n,k,d,l):
	points = list()

	print(n)
	print(k)
	print(d)
	r = abs(l[0])+abs(l[1])

	for i in range(n):
		for j in range(d):
			print(round((random.random()*r)-l[0],4))


if __name__ == '__main__':
	parser = argparse.ArgumentParser()

	parser.add_argument('-n','--number',dest='num',default=10,type=int,help='Number of points to generate')
	parser.add_argument('-d','--dimensions',dest='dim',default=2,type=int,help='Number of dimensions per point')
	parser.add_argument('-l','--limits',dest='lim',default=[0, 4],type=int,nargs=2,help='Number of dimensions per point')
	parser.add_argument('-k','--kpoints',dest='k',default=2,type=int,help='Number of points to obtain in dispersion')

	args = parser.parse_args()
	
	gen(args.num,args.k,args.dim,args.lim)

