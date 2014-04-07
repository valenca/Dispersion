from scipy.spatial.distance import euclidean as dist
    
def closestPair(L,dp):
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
        L = list(merge(recur(L[:split],ind), recur(L[split:],ind+split)))

        E = [p for p in L if abs(p[0]-splitx) < best[0]]
        for i in range(len(E)):
            for j in range(1,threshold):
                if i+j < len(E):
                    testPair(E[i],E[i+j],ind+i,ind+i+j)
        return L
	
    L.sort()
    try:
        return dp[tuple([tuple(i) for i in L])]
    except KeyError:
        pass
    recur(L,0)
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
                a.append(int(float(input())))
            vector.append(a)
        return vector,k
            
    vector,k=readVector()
    vector.sort()
    print(kDispersePoints(vector,k,dict()))
