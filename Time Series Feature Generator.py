#Script to process time series data from MYO, slice it into time slices
#that correspond to the length of time of a gesture, and then apply basic
#stastical techniques in order to generate non-time dependent features about
#each time interval that can be process by the Azure machine learning algo

#Note - Script will be uploaded to Azure and run in the web app.  Only the C++
#and java files will be run locally



import numpy as np
import pandas
import math

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
    numSamples = 0.4/sampleRate
    startingList = columnVector[:numSamples]
    return np.mean(startingList)


def endingPoint (columnVector):
    '''
    Takes a column vector of data corresponding to one sensor on the Myo
    Returns the final information by taking the mean of the data in the last
    0.4 seconds, using the sampleRate global variable to determine how many
    entries should be sampled
    '''
    numSamples = 0.4/sampleRate
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

