import random
from datetime import datetime
import math
import geopandas as gpd
from shapely.geometry import Point
#from matplotlib import pyplot as plt
import multiprocessing as mp
import os.path

random.seed(datetime.now())
NUM_DISTRICTS = 8
POPULATION = 0
DISTRICT_POP = 0
PERCENT_DIFF = .01
GROUPS = []

#EXE VERSION
BASE_DIR = "resources/app/src/data_files/Algorithm2"
#NPM RUN START VERSION
#BASE_DIR = "src/data_files/Algorithm2"

#Read file, get rid of everything except for polygon and population
tracts = gpd.read_file(BASE_DIR + "/MD_tracts/Maryland_Census_Data__Census_Tracts.shp")



#Get rid of unneeded data
tracts = tracts.drop(columns=['REPORT_2_P', 'REPORT_9_P', 'ALAND10', 'AWATER10',
'CNTY2010', 'CT10', 'CT2010', 'CNTY20101', 'CT101', 'Production', 'FID_TL_201',
'GEODESC', 'ACRES', 'SQ_MILES',  'HU100', 'NHW', 'NHB', 'NHAI', 'NHA', 
'NHNH', 'NHO', 'NHT', 'HISP', 'PNHW', 'PNHB', 'PNHAI', 'PNHA', 'PNHO', 'PNHT', 'PHISP',
'PNHNH', 'POP65_', 'PCTPOP65_', 'MEDAGE', 'VACNS', 'PVACNS', 'PHOWN', 'PWOMORT', 'PRENT',
'PLT18SP', 'ACRES_1', 'SQMILES', 'PSQM'])

#Find all neighbors of tract
def find_neighbors(index, tract):
    results = []
    for i, comp in tracts.iterrows():
        if tract.geometry.intersects(comp.geometry) and index != i:
            results.append(1)
        else:
            results.append(0)
    return results

#Save array (intersections) to a csv file so we don't have to regenerate it
def write_to_csv(array, file_name):
    csv = open(file_name+".csv", "w")
    for i in array:
        string = ""
        for j in i:
            string = string + str(j) + ","
        string = string[:-1]
        string = string + "\n"
        csv.write(string)
    csv.close()

#Convert intersections graph to a BFS dictionary
def convert_to_bfs(intersections):
    graph = {}

    for i in range(len(intersections)):
        neighbors = []
        for j in range(len(intersections[i])):
            if intersections[i][j] != 0:
                neighbors.append(j)
        graph[i] = neighbors

    return graph

#Get populations of each district. Used for balancing.
def get_group_populations():
    populations = [0 for i in range(NUM_DISTRICTS)]

    for index, row in tracts.iterrows():
        group = row['group']
        populations[group] = populations[group] + row['POP100']

    print(populations)
    return populations

#Checks if the district populations are within a margin of error for balancing
def isInThreshold(populations):
    offAmount = DISTRICT_POP * PERCENT_DIFF
    for i in populations:
        if i >= (DISTRICT_POP + offAmount) or i <= (DISTRICT_POP - offAmount):
            return False
        
    return True

#Finds the average center of each group
def getAverageCenter(group):
    members = tracts.loc[tracts['group'] == group]

    highestPop = 0
    avgCenter = 0
    count = 0
    xSum = 0
    ySum = 0

    #Get sum of all center points in the group
    for index, row in members.iterrows():
        xSum = xSum + (row['center'].x)
        ySum = ySum + (row['center'].y)
        count = count + 1

    #Get the average center point
    xAvg = xSum / count
    yAvg = ySum / count
    avgCenter = Point(xAvg, yAvg)


    return avgCenter



if __name__ == '__main__':
    intersections = []

    #Generate the intersections csv if it doesn't already exist.
    if not os.path.isfile(BASE_DIR+"/intersections.csv"):
        #Find neighbors using multiple processes
        pool = mp.Pool(processes=max(2, mp.cpu_count()-4))
        TASKS = [(i,val1) for i,val1 in tracts.iterrows()]
        results = [pool.apply_async(find_neighbors, t) for t in TASKS]
        for r in results:
            intersections.append(r.get())

        write_to_csv(intersections,BASE_DIR+"/intersections")
    else:
        #Read the csv
        csv = open(BASE_DIR+"/intersections.csv", 'r')
        lines = csv.readlines()
        csv.close()
        for i in lines:
            #Remove newline character
            i = i[:-1]
            splitLine = i.split(",")
            splitLine = [int(x) for x in splitLine]
            intersections.append(splitLine)



    #Get total population
    for i in tracts['POP100']:
        POPULATION = POPULATION + i

    #Get population per district
    DISTRICT_POP = POPULATION // 8
    print(DISTRICT_POP)

    #Generate centroids
    centers = []
    for i in tracts['geometry']:
        centers.append(i.centroid)

    tracts['center'] = centers


    #Generate a set of random starting points
    starts = []
    for i in range(NUM_DISTRICTS):
        while True:
            c = random.randrange(0, len(intersections))
            if c not in starts:
                starts.append(c)
                break

    #initialize groups
    GROUPS = [0 for i in range(len(intersections))]
    
    #Get graph in BFS form
    bfs_graph = convert_to_bfs(intersections)

    explored = []
    queue = []

    #We need to keep track of the group as well as the number
    for i in range(len(starts)):
        queue.append((i, starts[i]))

    #Check every node
    while queue:
        node = queue.pop(0)
        if node[1] not in explored:
            explored.append(node[1])
            GROUPS[node[1]] = node[0]

            #Add neighbors to queue, with current color
            for neighbor in bfs_graph[node[1]]:
                queue.append((node[0], neighbor))


    tracts['group'] = GROUPS

    '''colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange', 'cyan', 'pink']
    df_colors = []
    for i in GROUPS:
        df_colors.append(colors[i])

    tracts['color'] = df_colors
    tracts.plot(color=tracts['color'])
    plt.show()'''

    #This is the k-means stuff...... try bfs with depth 2 to determine most common color?
    groups = []
    prevPops = [0 for i in range(8)]
    bestStart = list(starts)
    converged = False

    for i in range(NUM_DISTRICTS):
            bestStart[i] = getAverageCenter(i)
    for i in range(20):
        converged = True
        
        distances = [0 for i in range(8)]
        groups = []
        groupPops = [0 for i in range(8)]
        offAmount = DISTRICT_POP * PERCENT_DIFF

        #Finded closests
        for index, row in tracts.iterrows():
            center = row['center']
            minDistance = float('inf')
            minIndex = 0
            d = []

            #Check each center. Find which one is closest to current track
            for j in range(NUM_DISTRICTS):
                cDist = bestStart[j].distance(center)
                #If distance is smaller than current smallest distance
                if cDist < minDistance:
                    #If it won't make us go over the population distribution, set to min
                    if (groupPops[j] + row['POP100']) <= (DISTRICT_POP + offAmount):
                        minDistance = cDist
                        minIndex = j

            #Min has been found, add it to group and update population
            groups.append(minIndex)
            groupPops[minIndex] = groupPops[minIndex] + row['POP100']
    
        #Update the groups and get populations
        tracts['group'] = groups
        groupPops = get_group_populations()

        #If nothing changed, we've converged, so stop the k-means.
        for i in range(len(groupPops)):
            if prevPops[i] != groupPops[i]:
                converged = False
                break

        prevPops = groupPops

        #If we haven't converged, keep going until we've reached the max iterations
        if(isInThreshold(groupPops) or converged == True):
            print("FOUND ONE!!!!")
            #colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange', 'cyan', 'pink']
            #f_colors = []
            #for i in groups:
            #    df_colors.append(colors[i])

           # tracts['color'] = df_colors
            #tracts.plot(color=tracts['color'])
            #plt.show()
            #exit()
            break
        else:
            for i in range(NUM_DISTRICTS):
                bestStart[i] = getAverageCenter(i)

    GROUPS = list(groups)

    #Fractured display
    '''colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange', 'cyan', 'pink']
    df_colors = []
    for i in GROUPS:
        df_colors.append(colors[i])

    tracts['color'] = df_colors
    tracts.plot(color=tracts['color'])
    plt.show()'''


    #Now color correct it
    print("Reclustering...")
    for j in range(20):
        #Use BFS (depth 2) to fix some of the fragmentation. 
        #This will join some of separated tracts to the closest large group.
        for i in bfs_graph:
            explored = []
            queue = []

            queue.append(i)
            for n in bfs_graph[i]:
                queue.append(n)

            for n2 in bfs_graph[n]:
                    queue.append(n2)


            nColors = [0 for i in range(NUM_DISTRICTS)]

            #Give current color some preference

            #Check every node
            while queue:
                node = queue.pop(0)
                if node not in explored:
                    explored.append(node)
                    nColors[GROUPS[node]] = nColors[GROUPS[node]] + 1
            
            groups[i] = nColors.index(max(nColors))


        GROUPS = list(groups)
        tracts['group'] = GROUPS
        groupPops = get_group_populations()
        print(groupPops)

    #Display final solution
    '''colors = ['red', 'blue', 'green', 'yellow', 'purple', 'orange', 'cyan', 'pink']
    df_colors = []
    for i in GROUPS:
        df_colors.append(colors[i])

    tracts['color'] = df_colors
    tracts.plot(color=tracts['color'])
    plt.show()'''


    groupIntersections = []
    numNeighbors = []
    colors = []

    #Initialize arrays
    for i in range (NUM_DISTRICTS):
        new = []
        for j in range(NUM_DISTRICTS):
            new.append(0)
        groupIntersections.append(new)
        colors.append(0)

    
    #Get all groups into 2d array
    districtGroups = [[] for i in range(NUM_DISTRICTS)]
    for i in range(len(GROUPS)):
        districtGroups[GROUPS[i]].append(i)

    #Use original intersections to determine group intersections
    for group in range(len(districtGroups)):
        #Check each tract in group
        for tract in range(len(districtGroups[group])):
            #Compare to each group
            for i in range(len(districtGroups)):
                if i != group:
                    for j in range(len(districtGroups[i])):
                        currNum = districtGroups[group][tract]
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
    
    #Sort by number of neighbors
    numNeighbors.sort(key = lambda x: x[1], reverse=True)
    
    uncolored = True
    colorCount = 1
    #Keep going until everything is colored
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

    #Fix group colors and numbers
    df_colors = []
    for i in GROUPS:
        df_colors.append(group_colors[i])
    for i in range(len(GROUPS)):
        GROUPS[i] = GROUPS[i] + 1

    #Update the dataframe
    tracts['fill'] = df_colors
    tracts['dist_num'] = GROUPS
    tracts['stroke'] = [0.25 for i in range(len(GROUPS))]
    tracts = tracts.drop(columns=['center'])
    #tracts.plot(color=tracts['fill'])
    #plt.show()

    #Save dataframe to file
    tracts.to_file(BASE_DIR+"\k_means_bfs.geojson", driver="GeoJSON")
    exit()

