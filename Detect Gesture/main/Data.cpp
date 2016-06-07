#include "Data.h"

using namespace std;

Data::Data() : sum(0), movingAvgVal(0) {
	fullData = new queue<int>;
}

Data::~Data() {
	delete fullData;
}

void Data::log_data(int input) {
	fullData->push(input);
	sum += input;
	if (fullData->size() > 100) {
		sum -= fullData->front();
		fullData->pop();
	}
}