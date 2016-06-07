#ifndef EMG_FILTERING_H
#define EMG_FILTERING_H

#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include <fstream>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>

#include <queue>

using namespace std;

class EMG_Sensor {
public:

	queue<int>* movingAvg;

	int sum;
	int movingAvgVal;

	queue<int>* fullData;

	EMG_Sensor();
	EMG_Sensor(unsigned int processed_data_size);
	~EMG_Sensor();

	int filter(int input);
	void log_data(int input);

};

#endif