#include "EMG_filtering.h"

#define MOVINGAVGSIZE 8

EMG_Sensor::EMG_Sensor() : sum(0), movingAvgVal(0) {
	movingAvg = new queue<int>;
	fullData = new queue<int>;
	//processed_data = new int[100];
}

EMG_Sensor::EMG_Sensor(unsigned int processed_data_size) : sum(0), movingAvgVal(0) {
	movingAvg = new queue<int>;
	fullData = new queue<int>;
	//processed_data = new int[processed_data_size];
}

EMG_Sensor::~EMG_Sensor() {
	//delete[] processed_data;
	delete movingAvg;
	delete fullData;
}

int EMG_Sensor::filter(int input) {
	input = abs(input);

	movingAvg->push(input);
	sum += input;

	if (movingAvg->size() > MOVINGAVGSIZE) {
		sum -= movingAvg->front();
		movingAvg->pop();
	}

	movingAvgVal = sum / movingAvg->size(); 
	
	if (movingAvg->size() < 5) {
		log_data(3);
		return 3;
	} else {
		log_data(movingAvgVal);
		return movingAvgVal;
	}
}

void EMG_Sensor::log_data(int input) {
	fullData->push(input);
	if (fullData->size() > 100) {
		fullData->pop();
	}
}