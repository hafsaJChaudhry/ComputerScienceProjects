"""	File to test and compute information using census data
"""
# TODO: Find exterior points for entire state.
#		 Start on exterior tract and find corner shared with only 1 other tract,
#		 and repeat till back at start (or some upper limit for testing).
# Tract notes: Pax River 1207

import pandas as pd
import geopandas as gpd	# For parsing shapefiles
import multiprocessing as mp	# For parallel computing
import matplotlib.pyplot as plt	# For plotting
import numpy as np	# For array types
import shapely	# For geometry types

#bad_tracts = [77,134,249,328,532,633,981,1000,1066]	# Tracts with multi-part boundaries or composition

PROC = 12	# Number of processes to spawn in pools

"""	Compare two coordinate pairs for equivalence
"""
def matchPair(xy1, xy2):
	return (xy1[0] == xy2[0]) and (xy1[1] == xy2[1])

"""	For a given geometry, return a list of coordinates
"""
def getCoords(geometry):
	if type(geometry) == shapely.geometry.polygon.Polygon:
		# Boundaries that are multilines
		if type(geometry.boundary) == shapely.geometry.multilinestring.MultiLineString:
			coords = []
			sets = [np.dstack(cds.coords.xy)[0] for cds in geometry.boundary]
			for i in sets:
				coords.extend(i)
			return coords
		else:
			return np.dstack(geometry.boundary.coords.xy)[0]
	# Multipolygon stuff
	elif type(geometry) == shapely.geometry.multipolygon.MultiPolygon:
		coords = []
		sets = [np.dstack(cds.boundary.coords.xy)[0] for cds in geometry]
		for i in sets:
			coords.extend(i)
		return coords
	else:
		print("Odd type:", type(geometry))
		return None

"""	For a list of tracts, with the 0 index containing the target tract, returns corners of tract.
	(*argv) necessary for process pool

	TODO: New method, find common coordinates. Starting at a non-common coordinate,
			walk the boundary points till first common point found. Then, the other
			walk till
"""
def getCorners(*argv):
	tract = [a for a in argv]
	tract_corners = []

	# For all neighboring
	idx = int(tract[0])
	for i in range(1, len(tract)):
		other = int(tract[i])
		#print("Other:", other)

		# Get coordinates, will error on Multipolygon stuff
		try:
			xy1 = getCoords(md_tract.geometry[idx])
		except Exception as e:
			print("Error with tract ", idx, ": ", e, sep='')
			break
		xy2 = getCoords(md_tract.geometry[other])

		# Get all common vertices between two neighboring polygons
		corners = []
		for pair in xy2:
			if matchPair(xy1[0], pair):
				shared = True
				break
		else:
			shared = False
		shared_prev = shared
		#print(xy2)
		for j in range(1,len(xy1)):
			#print(xy1[j], "in xy1")
			# Check if point is in neighboring polygon
			for pair in xy2:
				if matchPair(xy1[j], pair):
					#print(xy2[j], "is a common vertex")
					#print("Common")
					shared = True
					break
			else:
				shared = False
				#print("Not")

			if shared != shared_prev:
				if shared:
					#print("Appending:", xy1[j])
					corners.append(xy1[j])
				else:
					#print("Appending:", xy1[j-1])
					corners.append(xy1[j-1])
				if len(corners) == 2:
					break
				
			shared_prev = shared

		# Avoid duplicates, since removing later is harder
		if len(corners) == 0:
			continue
		for pair in tract_corners:
			if matchPair(corners[0], pair):
				break
		else:
			#print("Adding:", corners[0])
			tract_corners.append(corners[0])

		# If there is only one corner, don't duplicate
		if len(corners) == 1:
			continue
		if not matchPair(corners[1], corners[0]):
			for pair in tract_corners:
				if matchPair(corners[1], pair):
					break
			else:
				#print("Adding:", corners[1])
				tract_corners.append(corners[1])

	#tcts = [137]
	#if idx in tcts:
	#	print(idx, "=", tract_corners)
	return tract_corners


##### Main stuff #####

md_tract = gpd.read_file("data/cb_2018_24_tract_500k.shp")
md_tract = md_tract.drop(columns=['STATEFP','COUNTYFP','TRACTCE','AFFGEOID','GEOID','AWATER','LSAD'])

#print(md_tract)
print("Entries:", len(md_tract))

def getInts(idx, geoVal):
	tracts = []
	for j, val2 in md_tract.iterrows():
		if(idx == j):
			continue
		if geoVal.geometry.intersects(val2.geometry):
			tracts.append(j)

	return tracts

## Process all intersections in parallel
neighbors = []
"""
print("Calculating intersections")
with mp.Pool(PROC) as pool:
	TASKS = [(i,val1) for i,val1 in md_tract.iterrows()]
	results = [pool.apply_async(getInts, t) for t in TASKS]

	for r in results:
		neighbors.append(r.get())
		#print(neighbors[-1])
"""

## Load array of neighbors (if not calculated above)
fp = open("tract-intersections-all.csv", "r")
fp.readline().strip().split(",") # Read and ignore descriptions

# CHANGE THE [1:] FOR FINDING CORNERS
for line in fp:
	neighbors.append([int(a) for a in line.strip().split(",")[1:]])

fp.close()


## Get all corners
corners = []
"""
print("Computing corners")
with mp.Pool(PROC) as pool:
	results = [pool.apply_async(getCorners, (i,tract)) for i,tract in enumerate(neighbors,0)]

	for r in results:
		corners.append(r.get())
"""

fp = open("tract-corners-all.csv", "r")
fp.readline()

for line in fp:
	elems = line.strip().split(",")
	try:
		res = [float(a) for a in elems[2:]]
	except:
		res = []
	lats = np.array(res[::2])
	lons = np.array(res[1::2])
	corners.append(np.dstack((lats,lons))[0])

corners = np.array(corners)



# Plot the tract with all neighbors (highlighting specific ones)
#colors = ['black' for i in md_tract.iterrows()]
#isolated = [137,536,562,834,858,981]
#for df in neighbors[0][3:4]:
#	colors[df] = 'red'
#md_tract.iloc[isolated].plot()


###	Attempt to follow the boundary by finding corners that only share 1 neighbor	###
start_tract = 1207 #	Pax river is boundary county
print("Start nbrs:", neighbors[start_tract])

cur_tract = start_tract
prev_tract = -1
N = 200 #	Limit jumps for testing
it = 0 #	Corner iterator

max_len = 1
max_track = [start_tract]
track = [start_tract] #	Track tracts
pt_track = []
max_pt_track = []
it_track = []
while len(it_track) < N and it <= len(corners[cur_tract]):
	# Backtracking
	if it == len(corners[cur_tract]):
		#print(track)
		#print(it_track)
		prev_tract = track.pop()

		if len(track) == 0:
			break

		pt_track.pop()
		cur_tract = track[-1]
		it = it_track.pop() + 1

		print("Backtracking to:", cur_tract, "\tit:", it)
		continue

	share_count = 0 #	How many neighbors share this corner
	share_nbr = 0
	cur_corner = corners[cur_tract][it]

	for nbr in neighbors[cur_tract]:
		for pair in corners[nbr]:
			if matchPair(cur_corner, pair):
				share_count += 1
				share_nbr = nbr
				#print(nbr, "shares it")
				break

	#print(corners[cur_tract][it], "has" , share_count, "nbrs")

	cornerInTrack = False
	for cd in pt_track:
		if matchPair(cd, cur_corner):
			cornerInTrack = True
			break

	#	Jump to next neighbor
	if share_count == 1 and share_nbr != prev_tract and not cornerInTrack:
		pt_track.append(corners[cur_tract][it])

		print("Prev:", prev_tract, "\tCur:", cur_tract, "\tNxt:", share_nbr)
		prev_tract = cur_tract
		cur_tract = share_nbr

		track.append(cur_tract)
		it_track.append(it)

		# Update max track
		if len(track) >= max_len:
			max_track = track.copy()
			max_len = len(track)
			max_pt_track = pt_track.copy()

		it = -1

	it += 1 #	Standard iterate
#print("Track:", max_track, "(", len(max_track), ")")
print("Pts:", len(max_pt_track))


#print("\n".join([str(a) for a in getCoords(md_tract.geometry[neighbors[cur_tract][2]])]))

pattern = ['red', 'green', 'blue']
colors = ['darkgray' for i in md_tract.iterrows()]
for i,tract in enumerate(max_track,0):
	colors[tract] = pattern[i % len(pattern)]
#print(colors)
#md_tract.plot(color='lightblue',figsize=(10,5),edgecolor=colors)
#md_tract.iloc[[cur_tract] + neighbors[cur_tract][2:3]].plot()
#md_tract.iloc[max_track].plot(color=colors, figsize=(15,10), edgecolor='black')
ax = md_tract.plot(figsize=(16,9), color=colors, edgecolor='lightgrey')

#t = 249 # 1383 has a bad corner, 249 has correct one
#md_tract.iloc[[t] + neighbors[t]].plot(color='black',edgecolor='gray')
#a = [t] + [b for b in neighbors[t]]
#print(neighbors[t])
#print(getCorners(*neighbors[t])) # 1000 is problem nbr for 954
#print(corners[t])
geo = [shapely.geometry.Point(xy) for crnrs in corners for xy in crnrs]
#crs = {'init': 'epsg:4326'}
#df = pd.DataFrame(corners[t],columns=['longitude','latitude'])
gdf = gpd.GeoDataFrame(geometry=geo)
gdf.plot(ax=ax,color='brown', markersize=5)
plt.show()


### Save results to files ###

## Write neighbors to file
"""
fp = open("tract-intersections.csv", "w")
fp.write("polygon, intersecting polygons\n")
for i in range(len(neighbors)):
	data = str(i) + ','
	data += ','.join([str(a) for a in neighbors[i]])
	data += '\n'
	fp.write(data)
fp.close()
"""

## Write corners to file
"""
fp = open("tract-corners.csv", "w")
fp.write("polygon, num_corners, corner x, corner y, ...\n")
for i,tract in enumerate(corners, 0):
	data = str(i) + "," + str(len(tract)) + ','
	data += ','.join([str(a[0])+','+str(a[1]) for a in tract])
	data += '\n'
	fp.write(data)
fp.close()
"""

## Write track to file
fp = open("exterior.csv", "w")
fp.write("Tract/Polygon\n")
for i in max_track:
	data = str(i) + '\n'
	fp.write(data)
fp.close()
