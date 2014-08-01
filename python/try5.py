from pprint import pprint

def dist(p1,p2):
	res = 0
	for c1,c2 in zip(p1,p2):
		res += (c1-c2)**2
	return res**0.5

def closestPair(L):
	best = [dist(L[0],L[1]), (L[0],L[1]),(0,1)]
	dim = len(vector[0])
	threshold = (3**dim)-(3**(dim-1))

	def testPair(p,q,ip,iq):
		d = dist(p,q)
		if d < best[0]:
			best[0] = d
			best[1] = p,q
			best[2] = ip,iq
			
	def merge(A,B):
		i = 0
		j = 0
		while i < len(A) or j < len(B):
			if j >= len(B) or (i < len(A) and A[i][1] <= B[j][1]):
				yield A[i]
				i += 1
			else:
				yield B[j]
				j += 1

	def recur(L,ind):
		if len(L) < 2:
			return L
		split = int(len(L)/2)
		splitx = L[split][0]
		L2 = list(merge(recur(L[:split],ind), recur(L[split:],ind+split)))

		E = [p for p in L2 if abs(p[0]-splitx) < best[0]]
		for i in range(len(E)):
			for j in range(1,threshold+2):
				if i+j < len(E):
					testPair(E[i],E[i+j],ind+L.index(E[i]),ind+L.index(E[i+j]))

		return L
	
	recur(L,0)
	return best

def kDispersePoints(vector,mask,k):
	if k>=len(vector):
		return closestPair(vector)[0],vector[:]
		
	ip,iq=closestPair(vector)[2]

	if vector[iq][0]+vector[ip][0]==2:
		return [0]
	
	lft=[0]
	rgt=[0]
		
	
	if mask[ip]==0:
		p=vector.pop(ip)
		m=mask.pop(ip)
		lft=kDispersePoints(vector,mask,k)
		vector.insert(ip,p)
		mask.insert(ip,m)

	if mask[iq]==0:
		mask[ip]=1
		q=vector.pop(iq)
		m=mask.pop(iq)
		rgt=kDispersePoints(vector,mask,k)
		vector.insert(iq,q)
		mask.insert(iq,m)
		mask[ip]=0

	if lft[0]>rgt[0]:
		return lft
	else :
		return rgt

if __name__ == '__main__':
	def readVector():
		N=int(input())
		D=int(input())
		k=int(input())
		vector=[]
		b=[]
		for i in range(N):
			a=[]
			for j in range(D):
				a.append(float(input()))
			vector.append(list(a))
			b.append(0)
		return vector,b,k

	vector,mask,k=readVector()
	vector.sort()
	print kDispersePoints(vector,mask,k)[0]
