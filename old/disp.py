from scipy.spatial.distance import euclidean as dist

v=[[0,0],[90,30],[40,30],[20,20],[11,11]]
P1=0
P2=1
min_d=dist(v[0],v[1])
THRESHOLD=8

def divc(vector):
    print(vector)
    length=len(vector)
    half=int(length/2)
    global P1,P2,min_d
    if length==3:
        for i,j in ((0,1),(1,2),(2,0)):
            tmp=dist(vector[i],vector[j])
            if (min_d>tmp):
                P1,P2,min_d=i,j,tmp
        return (P1,P2)
    elif length==2:
        tmp=dist(vector[0],vector[1])
        if (min_d>tmp):
            P1,P2,min_d=i,j,tmp
        return (P1,P2)
    else:
        divc(vector[:half])
        divc(vector[half:])
        
        for i in range(half,0,-1):
            k=0;
            for j in range(half,length,1):
                if(k>THRESHOLD or vector[i][0]-vector[j][0]>=min_d): 
                    break
                if(abs(vector[j][1]-vector[i][1])>=min_d): 
                    continue
                else:
                    tmp=dist(vector[0],vector[1])
                    if(min_d>tmp):
                        P1,P2,min_d=i,j,tmp
                    k+=1
    return min_d
                
if __name__ == '__main__':
    print(divc(v))
