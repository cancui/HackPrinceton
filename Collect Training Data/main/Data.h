#ifndef DATA_H
#define DATA_H

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

class Data {
public:

	int sum;
	int movingAvgVal;

	queue<int>* fullData;

	Data();
	~Data();

	void log_data(int input);

};

#endif