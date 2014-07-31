from cPickle import load
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

global it
it=0


class PolygonHandler:
	# Constructor
	def __init__(self):
		self.X = []
		self.Y = []
		self.listx = []
		self.listy = []
		self.numberPoints = 0
		self.fig , ax = plt.subplots()
		self.sc = ax.scatter(self.X,self.Y)
		ax.set_xlim(-1000,1000)
		ax.set_ylim(-1000,1000)

	# Print the polygon
	def update(self,_):
		#print len(self.listx)
		global it
		it +=1
		print it
		self.X=[]
		self.Y=[]
		for i in range(self.numberPoints):
			try:
				self.X.append(self.listx.pop(0))
				self.Y.append(self.listy.pop(0))
			except IndexError:
				return
		self.sc.set_offsets(np.column_stack((self.X,self.Y)))
		return self.sc,

	# append a point
	def add(self,points):
		self.numberPoints = len(points)
		for i in range(self.numberPoints):
			self.listx.append(points[i][0])
			self.listy.append(points[i][1])

P = PolygonHandler()

with open("output.plt","r") as f:
	for i in range(1000):
		try:
			P.add(load(f))
		except EOFError:
			break

print len(P.listx)
try:
	ani = animation.FuncAnimation(P.fig, P.update, interval=10,blit=False)
except IndexError:
	pass

plt.show()
