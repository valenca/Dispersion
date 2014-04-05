from scipy.spatial.distance import euclidean as dist

def closestPair(L,dp):
	try:
		return dp[tuple([tuple(i) for i in L])]
	except KeyError:
		pass

	best = [dist(L[0],L[1]),[L[0],L[1]],(0,1)]
	for i in range(len(L)):
		for j in range(i+1,len(L)):
			if dist(L[i],L[j]) < best[0]:
				best = [dist(L[i],L[j]),[L[i],L[j]],(i,j)]
	dp[tuple([tuple(i) for i in L])]=best
		
	return best

def kDispersePoints(vector,k,dp):
	if k>=len(vector):
		return (closestPair(vector,dp)[0],vector.copy())
	else:
		ip,iq=closestPair(vector,dp)[2]

		p=vector.pop(ip)
		bp=kDispersePoints(vector,k,dp)
		vector.insert(ip,p)

		q=vector.pop(iq)
		bq=kDispersePoints(vector,k,dp)
		vector.insert(iq,q)

		if bq[0]>bp[0]:
			return bq
		else :
			return bp

if __name__ == '__main__':
	def readVector():
		N=int(input())
		D=int(input())
		k=int(input())
		vector=[]
		for i in range(N):
			a=[]
			for j in range(D):
				a.append(float(input()))
			vector.append(a)
		return vector,k
			
	vector,k=readVector()
	vector.sort()
	print(kDispersePoints(vector,k,dict()))
