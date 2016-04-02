#Script to process time series data from MYO, slice it into time slices
#that correspond to the length of time of a gesture, and then apply basic
#stastical techniques in order to generate non-time dependent features about
#each time interval that can be process by the Azure machine learning algo

#Note - Script will be uploaded to Azure and run in the web app.  Only the C++
#and java files will be run locally



import numpy as np
import pandas

#Global variable for the sampling rate in seconds
sampleRate = 0.05


def avAbsDiff (columnVector):
    '''
    Takes a column vector of data corresponding to one sensor on the Myo
    Takes the absolute average difference between each data entry in a time
    series and the mean of the time series.  Returns the result as a float
    '''
    mean = np.mean(columnVector, dtype=np.float64)
    absDiff = 0
    for num in columnVector:
        absDiff+=abs(num-mean)
    return absDiff*1.0/len(columnVector)

def startingPoint (columnVector):
    '''
    Takes a column vector of data corresponding to one sensor on the Myo
    Returns the initial information by taking the mean of the data in the first
    0.4 seconds, using the sampleRate global variable to determine how many
    entries should be sampled
    '''
    numSamples = int(0.4/sampleRate)
    startingList = columnVector[:numSamples]
    return np.mean(startingList)


def endingPoint (columnVector):
    '''
    Takes a column vector of data corresponding to one sensor on the Myo
    Returns the final information by taking the mean of the data in the last
    0.4 seconds, using the sampleRate global variable to determine how many
    entries should be sampled
    '''
    numSamples = int(0.4/sampleRate)
    endingList = columnVector[-numSamples:]
    return np.mean(endingList)


#Note, experiment with both binDist based on proportion in each bin and the
#actual of number of elements in each bin
def binDist (columnVector):
    '''
    Takes a column vector of data corresponding to one sensor on the Myo
    Determines the max and min value, then creates 10 equal sized bins in the
    range between these points, and then returns the number of data points in 
    each bin.  This is returned as a list of 10 elements.
    '''
    rawOutput = np.histogram(columnVector)
    return rawOutput[0]


def callFeatures (columnVector):
    '''
    Calls all feature generating functions, both built in, and coded above.
    Returns the results as a list with features listed in this order:
    average, median, standard deviation, average absolute difference, starting
    point, ending point, and then an array of the binned distribution data
    '''
    results = []
    results.append(np.mean(columnVector))
    results.append(np.median(columnVector))
    results.append(np.std(columnVector))
    results.append(avAbsDiff(columnVector))
    #results.append(startingPoint(columnVector))
    #results.append(endingPoint(columnVector))
    distList = binDist(columnVector)
    for num in distList:
        results.append(num)
    return results

#Note may need to update this function if there are more or less than 22 
#channels of myo data combined between the two myos
def rowSeparation(dataFile):
    '''
    dataFile should be a pandas dataFrame
    Pass the overall file dataFile in, parse the rightmost column that contains the 
    words being signed, looking for a 'NULL' string signifying the separation
    between two different word gestures.  Returns a list of all of the row
    indexes for which this is true to aid in parsing the dataframe file.
    '''
    #List of the row indexes of null rows, used to separate two different
    #gestures
    nullRows = []
    #Last column is reserved for the word that is being signed
    count = 0
    for word in dataFile["Col23"]:
        if word == 'NULL':
            nullRows.append(count)
        count+=1
    return nullRows

            
    
