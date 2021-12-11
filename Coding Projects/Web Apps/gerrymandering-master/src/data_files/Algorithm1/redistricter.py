import geopandas as gpd
#from matplotlib import pyplot as plt
from shapely.ops import cascaded_union 
import random
from datetime import datetime

import os
import time

#EXE VERSION
BASE_DIR = "resources/app/src/data_files/Algorithm1"
#NPM RUN START VERSION
#BASE_DIR = "src/data_files/Algorithm1"


random.seed(datetime.now())
#Read shapefile

md_c = gpd.read_file(BASE_DIR+"/MD_counties/Maryland_Physical_Boundaries__County_Boundaries_Generalized.shp")

#Get rid of unneeded data
md_c = md_c.drop(columns=['district', "county_fip", "creation_d", "last_updat"])
md_c = md_c.sort_values(by='countynum')



numDistricts = 8
numCounties = 24

#Initialize 2d array for intersections
intersections = []
for i in range (numCounties):
    new = []
    for j in range(numCounties):
        new.append(0)
    intersections.append(new)

#Find all intersections
for i, val1 in md_c.iterrows():
    for j, val2 in md_c.iterrows():
        if(val1.countynum == val2.countynum):
            continue
        if val1.geometry.intersects(val2.geometry):
            intersections[val1.countynum-1][val2.countynum-1] = 1

#Print the intersections for debugging purposes
for i in range(len(intersections)):
    cName = md_c.loc[md_c['countynum'] == i+1]
    cName = cName['county'].values[0]
    print(i+1, ": Neighbors of",cName)

    for j in range(len(intersections)):
        if intersections[i][j] == 1:
            nName = md_c.loc[md_c['countynum'] == j+1]
            nName = nName['county'].values[0]
            print("\t",j+1, nName)


#Note, this contains indexes of the intersections array, not the county numbers
permDistrictStarts = []
bannedDistrictStarts = []

#Find counties with only one neighbor
for i in range(len(intersections)):
    cNeighbors = []
    for j in range(len(intersections)):
        if intersections[i][j] == 1:
            cNeighbors.append(j)


    #If only one neighbor, make that neighbor a permDistrictStart
    #And ban the district from being chosen as a start
    if(len(cNeighbors) == 1):
        if not (cNeighbors[0] in permDistrictStarts):
            permDistrictStarts.append(cNeighbors[0])
        if not (i in bannedDistrictStarts):
            bannedDistrictStarts.append(i)

randomDistrictStarts = []
districtGroups = []
badGroups = True

#Keep making groups until we get a good set
while badGroups == True:
    badGroups = False
    randomDistrictStarts =  []
    districtGroups = []
    countyChoices = []
    [countyChoices.append(i) for i in range(numCounties)]
    [countyChoices.remove(i) for i in permDistrictStarts]
    [countyChoices.remove(i) for i in bannedDistrictStarts]


    # Choose remaining starting blocks
    while len(randomDistrictStarts) < (numDistricts - len(permDistrictStarts)):
        randNum = random.choice(countyChoices)
        
        randomDistrictStarts.append(randNum)
        countyChoices.remove(randNum)

    #For simplicity, let's merge the starting blocks
    startingBlocks = randomDistrictStarts + permDistrictStarts

    #Add banned starting choices back into countychoices
    [countyChoices.append(i) for i in bannedDistrictStarts]


    #Add starting blocks to districtGroups
    [districtGroups.append([i]) for i in startingBlocks]


    #Choose two counties to put in each group (makes group of 3)
    for i in range(2):

        #Go through each districtGroup, find all neighbors still in countyChoices
        for group in districtGroups:
            neighbors = []

            #Find all unchosen neighbors for each county in the group
            for county in group:
                for j in range(len(intersections)):
                    if intersections[county][j] == 1 and j in countyChoices:
                        if(j not in neighbors):
                            neighbors.append(j)

            #If it has no neighbors, it won't be a good group, so have to repeat
            if(len(neighbors) == 0):
                #print("This is awkward...")
                badGroups = True
            else:
                #Add one of the neighbors to the group
                neighborChoice = random.choice(neighbors)
                group.append(neighborChoice)
                countyChoices.remove(neighborChoice)
            

print(districtGroups)



print("District Groups")

#Print names of groups
for group in districtGroups:
    names = []
    for county in group:
        tName = md_c.loc[md_c['countynum'] == county+1]
        tName = tName['county'].values[0]
        names.append(tName)
    print(names)


groupIntersections = []
numNeighbors = []
colors = [0 for i in range(numDistricts)]

#Initialize new arrays
for i in range (numDistricts):
    new = []
    for j in range(numDistricts):
        new.append(0)
    groupIntersections.append(new)


#Use original intersections to determine group intersections
for group in range(len(districtGroups)):
    #Check each county in the group
    for county in range(len(districtGroups[group])):
        #Compare to each group
        for i in range(len(districtGroups)):
            #If it's not the current group, compare to check for connections
            if i != group:
                for j in range(len(districtGroups[i])):
                    currNum = districtGroups[group][county]
                    compNum = districtGroups[i][j]

                    if intersections[currNum][compNum] != 0:
                        groupIntersections[group][i] = 1

#Get counts of group intersections, will be needed
for group in range(len(groupIntersections)):
    print(groupIntersections[group])
    count = 0
    for i in groupIntersections[group]:
        if i == 1:
            count = count+1
    numNeighbors.append([group, count])

#Coloring algorithm: http://mrsleblancsmath.pbworks.com/w/file/fetch/46119304/vertex%20coloring%20algorithm.pdf
# Modified to only allow 2 of each color, forcing it to become a 4 color algorithm
numNeighbors.sort(key = lambda x: x[1], reverse=True)
uncolored = True
colorCount = 1
while uncolored:
    first = numNeighbors[0]
    currentList = [first]

    #Color first vertex
    colors[first[0]] = colorCount
    numColor = 1

    #Go through remaining and look for ones that are not connected
    for i in range(1, len(numNeighbors)):
        comp = numNeighbors[i]
        connectionToAbove = False
        
        #See if it's connected to anything that has already been colored this color
        for j in range(len(currentList)):
            above = currentList[j]
            if groupIntersections[above[0]][comp[0]] == 1:
                connectionToAbove = True
            
        #If not connected, color it
        if not connectionToAbove and numColor < 2:
            colors[comp[0]] = colorCount
            currentList.append(comp)
            numColor = numColor + 1

    #Removed colored nodes from list
    for i in currentList:
        numNeighbors.remove(i)

    #Sort just incase things got messed up
    numNeighbors.sort(key = lambda x: x[1], reverse=True)

    #Move to next color
    colorCount = colorCount + 1

    #If no nodes left, we've colored everything
    if len(numNeighbors) == 0:
        uncolored = False


#Replace group color numbers with color names
colorNames = ['red', 'blue', 'green', 'cyan']
group_colors = []
for i in colors:
    group_colors.append(colorNames[i-1])

print(group_colors)

#Color & District Label each county
df_colors = []
[df_colors.append(0) for i in range(numCounties)]
group_numbers = []
[group_numbers.append(0) for i in range(numCounties)]

#Add color to each county
for group in range(len(districtGroups)):
    for county in districtGroups[group]:
        df_colors[county] = group_colors[group]
        group_numbers[county] = group+1


#Add color&numbering columns
md_c['fill'] = df_colors
md_c['dist_num'] = group_numbers
md_c['stroke'] = [0.25 for i in range(len(group_numbers))]
#md_c.plot(color=md_c['fill'])
#plt.show()

#Save as geoJSON file

OUTFILE = BASE_DIR+"/md_c.geojson"
md_c.to_file(OUTFILE, driver="GeoJSON")
exit()

