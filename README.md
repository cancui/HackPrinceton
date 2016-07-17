# Hack Princeton
Made by Can Cui, Jeremy Malloch, and Stephen Jia

##Project Description
We created software that allows a Myo armband to learn and recognize actual American Sign Language gestures.

The two main pieces of software are the training/sampling module and the recognition module. The following describes how they work.

####Training/Sampling Module
The software establishes a bluetooth connection with the Myo armband (referred to as armband from now on). It pulls raw EMG and magnetometer data from the armband as the user is performing a gesture. The EMG signal is processed in real time into a signal that reflects the intensity of muscle flexion at the muscle of each particular sensor (there are 8 on the armband). All 8 streams of this processed EMG data, along with the 3 streams of magnetometer data (one for each axis) are stored
as a time series in CSV or JSON format, depending if the collected data is being used for training or recognition purposes.  

At HackPrinceton, the CSV training file was uploaded to the Azure machine learning studio website, where a Python script converted the time series data into feature vectors describing the gestures.  These feature vectors were then used to train a multi-class logistic regression model.  The model created was turned into a web service, which underpinned the recognition module.

The armband interfacing, signal processing, and .csv generation code was written in C++ and used the Myo SDK to connect and access raw armband data. 

####Recognition module
The recognition module works in the same was as the training module up to the point of CSV file generation. Instead of generating a CSV file, the time series data was sent to Azure in JSON format.  The JSON time series data was parsed, and was converted into a feature vector by the same Python script used by the model training module.  The resulting feature vector was processed by the multi-class regression model.  The return value of this model was sent back to the computer
running the sampling module, which was then displayed to the user.

After two hundred training samples, the software could recognize 4 distinct sign language gestures with 80% accuracy. With more training samples, the accuracy would continue to increase, and more gestures could also be introduced.

---------------------------------

#####Lastly, we owe a shoutout to [Steph](http://i.imgur.com/1O9SZJp.jpg) for being a great host at Princeton!
