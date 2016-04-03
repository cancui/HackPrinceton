// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.

#define _USE_MATH_DEFINES 

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

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#ifndef NOT_CANS_COMPUTER
	#define INT_SIZE 4
#endif

#include <myo/myo.hpp>
#include "EMG_filtering.h"
#include "Data.h"
#include <windows.h>

using namespace std;

class GestureContainer {
public:
	queue<int> datapoints[14];
};

class DataCollector : public myo::DeviceListener {
public:
    DataCollector() : emgSamples(), onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose() {}
	
    void onUnpair(myo::Myo* myo, uint64_t timestamp) { //called when Myo is disconnected from Myo Connect by user
        emgSamples.fill(0); //clean up leftover state
		roll_w = 0;
		pitch_w = 0;
		yaw_w = 0;
		onArm = false;
		isUnlocked = false;
    }

    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) { //called whenever paired Myo has provided new EMG data, and EMG streaming is enabled
        for (int i = 0; i < 8; i++) {
            emgSamples[i] = emg[i];
			//cout << '[' << static_cast<int>(emgSamples[i]) << ']';
        }
    }

	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
		using std::atan2;
		using std::asin;
		using std::sqrt;
		using std::max;
		using std::min;

		// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
		float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
			1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
			1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));

		// Convert the floating point angles in radians to a scale from 0 to 18.
		roll_w = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 18);
		pitch_w = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 18);
		yaw_w = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 18);
	}

	void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float> &accel) {
		accx = accel.x() * 40;
		accy = accel.y() * 40;
		accz = accel.z() * 40;
	}

	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
		currentPose = pose;

		if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
			// Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
			// Myo becoming locked.
			myo->unlock(myo::Myo::unlockHold);

			// Notify the Myo that the pose has resulted in an action, in this case changing
			// the text on the screen. The Myo will vibrate.
			myo->notifyUserAction();
		}
		else {
			// Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
			// are being performed, but lock after inactivity.
			myo->unlock(myo::Myo::unlockTimed);
		}
	}

	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
		onArm = true;
		whichArm = arm;
	}
	
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
		onArm = false;
	}

	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = true;
	}

	void onLock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = false;
	}

    void print() { //prints the current values that were updated by the on...() functions above
        cout << '\r' ;

        for (size_t i = 0; i < emgSamples.size(); i++) { // Print out the EMG data.
            ostringstream oss;
            oss << static_cast<int>(emgSamples[i]);
            string emgString = oss.str();

            cout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
        }
		
		cout << '[' << roll_w << ',' << pitch_w << ',' << yaw_w << ']';
		
		if (onArm) {
			string poseString = currentPose.toString();
			cout << '[' << (isUnlocked ? "UL" : "LK") << ']'
				 << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
				 << '[' << poseString << string(14 - poseString.size(), ' ') << ']';
		} else { // Print out a placeholder for the arm and pose 
			cout << '[' << string(8, ' ') << ']' << "[?]" << '[' << string(10, ' ') << ']';
		}

        cout << flush;
    }
    
    array<int8_t, 8> emgSamples; // The values of this array are set by onEmgData() above

	bool onArm;
	myo::Arm whichArm;
	bool isUnlocked;
	int roll_w, pitch_w, yaw_w;
	double accx, accy, accz;
	myo::Pose currentPose;
};

void csv_output(ofstream & fout, DataCollector& collector, EMG_Sensor* sensors, ofstream & fout_filtered) {
	for (size_t i = 0; i < collector.emgSamples.size(); i++) { // Print out the EMG data.
		ostringstream oss;
		oss << static_cast<int>(collector.emgSamples[i]);
		string emgString = oss.str();

		fout << emgString << ",";

		sensors[i].filter(static_cast<int>(collector.emgSamples[i]));
	}
	
	for (int j = 0; j < 8; j++) {
		ostringstream oss1;
		oss1 << static_cast<int>(sensors[j].fullData->front());
		sensors[j].fullData->pop();
		string emgString = oss1.str();
		fout_filtered << emgString << ",";
	}

	fout_filtered << collector.roll_w << "," << collector.pitch_w << "," << collector.yaw_w << ",";
	fout_filtered << setprecision(3) << collector.accx << "," << setprecision(3) << collector.accy << "," << setprecision(3) << collector.accz << endl;

	fout << collector.roll_w << "," << collector.pitch_w << "," << collector.yaw_w << ",";
	fout << setprecision(3) << collector.accx << "," << setprecision(3) << collector.accy << "," << setprecision(3) << collector.accz << endl;
}

void csv_output2(DataCollector& collector, EMG_Sensor* sensors, ofstream & fout_filtered) {
	for (size_t i = 0; i < collector.emgSamples.size(); i++) { // Print out the EMG data.
		sensors[i].filter(static_cast<int>(collector.emgSamples[i]));
	}

	for (int j = 0; j < 8; j++) {
		ostringstream oss1;
		oss1 << static_cast<int>(sensors[j].fullData->front());
		sensors[j].fullData->pop();
		string emgString = oss1.str();
		fout_filtered << emgString << ",";
	}

	fout_filtered << collector.roll_w << "," << collector.pitch_w << "," << collector.yaw_w << ",";
	fout_filtered << setprecision(3) << collector.accx << "," << setprecision(3) << collector.accy << "," << setprecision(3) << collector.accz << endl;

}


int main() {
try {
    myo::Hub hub("com.example.emg-data-sample");
    cout << "Attempting to find a Myo..." << endl;
    myo::Myo* myo1 = hub.waitForMyo(10000);
    if (!myo1) { throw runtime_error("Unable to find a Myo 1!"); }
    cout << "Connected to a Myo armband (1) !" << endl << endl;
    myo1->setStreamEmg(myo::Myo::streamEmgEnabled); myo1->setStreamEmg(myo::Myo::streamEmgDisabled); myo1->setStreamEmg(myo::Myo::streamEmgEnabled); //enable EMG streaming
    DataCollector collector1;
    hub.addListener(&collector1);

	string name = "src\\input.csv";
	ofstream fout2; // (name);
		if (!fout2) { throw runtime_error("Failed to create filtered output text file!"); }
		ofstream fout; // ("src\\trigger.txt");
		if (!fout) { throw runtime_error("Failed to create output text file!"); }

	EMG_Sensor sensors[8];

	int counter_trigger = 0;

    while (true) {
		//myo1->vibrate(myo::Myo::vibrationShort);
		//string name = "Data" + to_string(counter_trigger) + ".csv";
		
		fout2.open("C:\\Users\\Can\\Eclipse Workspace\\Squadcaller\\src\\input.csv");
		fout.open("C:\\Users\\Can\\Eclipse Workspace\\Squadcaller\\src\\trigger.txt");
		
		myo1->unlock(myo::Myo::unlockHold);
		hub.run(1000/25); 

		/*int cur_pitch = collector1.pitch_w;
		int cur_yaw = collector1.yaw_w;
		int cur_roll = collector1.roll_w;

		while ((abs(collector1.pitch_w - cur_pitch) < 2 || abs(collector1.pitch_w - cur_pitch) > 16)
			&& (abs(collector1.yaw_w - cur_yaw) < 2 || abs(collector1.yaw_w - cur_yaw) > 16)
			&& (abs(collector1.roll_w - cur_roll) < 1 || abs(collector1.roll_w - cur_roll) > 17)) {
			hub.run(1000 / 25);
		}*/

		cout << "Enter any value to begin";
		string waiting;
		cin >> waiting;
		cout << "Starting." << endl;

		for (int i = 0; i < 100; i++) {
			cout << '\r' << endl << (100 - i) * 40;
			hub.run(1000 / 25);
			csv_output2(collector1, sensors, fout2);
			cout << flush;
		}

		fout2 << "0,0,0,0,0,0,0,0,0,0,0,0,0,0,SPACE" << endl;
		fout2.close();
		cout << "hit" << endl;
		counter_trigger++;
		fout << to_string(counter_trigger);
		cout << "hit" << endl;
		fout.close();
		cout << to_string(counter_trigger);
		Sleep(500);
    }
} catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "Press enter to continue.";
        cin.ignore();
        return 1;
}
}
