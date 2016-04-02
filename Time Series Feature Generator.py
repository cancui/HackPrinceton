#Script to process time series data from MYO, slice it into time slices
#that correspond to the length of time of a gesture, and then apply basic
#stastical techniques in order to generate non-time dependent features about
#each time interval that can be process by the Azure machine learning algo

#Note - Script will be uploaded to Azure and run in the web app.  Only the C++
#and java files will be run locally