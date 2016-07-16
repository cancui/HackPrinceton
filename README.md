# Hack Princeton
Made by Can Cui, Jeremy Malloch, and Stephen Jia

##Project Description
We created software that allows a Myo armband to learn and recognize actual American Sign Language gestures.

The two main peices of software are the training/sampling module and the recognition module. The following describes how they work.

####Training/Sampling Module
The software establishes bluetooth connection with the Myo armband (referred to as armband from now on). It pulls raw EMG and magnetometer data from the armband as the user is performing a gesture. The EMG signal is processed in real time into a signal that reflects the intensity of muscle flexion at the muscle of each particular sensor (there are 8 on the armband). All 8 streams of this processed EMG data, along with the 3 streams of magnetometer data (one for each axis) are packaged into a .csv file and then converted to a JSON file. 

The armband interfacing, signal processing, and .csv generation code was written in C++ and used the Myo SDK to connect and access raw armband data. 

####Recognition module
The recognition module works in the same was as the training module up to the point of .csv generation. 

After several hundred training samples, the software could recognize 4 distinct sign language gestures with 80% accuracy. 

---------------------------------

#####Also, shoutout Steph for being a great host at Princeton!
