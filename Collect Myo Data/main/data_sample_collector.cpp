// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.

// This sample illustrates how to use EMG data. EMG streaming is only supported for one Myo at a time.

#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#define _USE_MATH_DEFINES 

#include <fstream>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#include <myo/myo.hpp>

using namespace std;

ofstream fout1("global_output.txt");

class DataCollector : public myo::DeviceListener {
public:
    DataCollector() : emgSamples(), onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose() {}

	void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
	{
		// Print out the MAC address of the armband we paired with.

		// The pointer address we get for a Myo is unique - in other words, it's safe to compare two Myo pointers to
		// see if they're referring to the same Myo.

		// Add the Myo pointer to our list of known Myo devices. This list is used to implement identifyMyo() below so
		// that we can give each Myo a nice short identifier.
		knownMyos.push_back(myo);
		listOfMyos[listIndex] = myo;

		// Now that we've added it to our list, get our short ID for it and print it out.
		std::cout << "Paired with " << identifyMyo(myo) << "." << std::endl;
		std::cout << "ACTUALLY Paired with " << listOfMyos[listIndex] << "." << std::endl;
		
		if (listIndex == 0) {
			listIndex++;
		}
		else if (listIndex == 1) {
			listIndex--;
		}
		else {
			cout << "LIST INDEX ERROR, value: " << listIndex;
		}
	}


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

		for (size_t i = 0; i < emgSamples.size(); i++) { // Print out the EMG data.
			ostringstream oss;
			oss << static_cast<int>(emgSamples[i]);
			string emgString = oss.str();

			fout1 << emgString << ", ";
		}

		fout1 << roll_w << ", " << pitch_w << ", " << yaw_w << ", ";
		fout1 << '[' << (whichArm == myo::armLeft ? "L" : "R") << "], ";
		fout1 << "r ";
    }

	// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	// as a unit quaternion.
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

	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
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

	// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
	// arm. This lets Myo know which arm it's on and which way it's facing.
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
		myo::WarmupState warmupState) {
		onArm = true;
		whichArm = arm;
	}
	
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
		onArm = false;
	}

	// onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = true;
	}

	// onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
	void onLock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = false;
	}

	void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
		std::cout << "Myo " << identifyMyo(myo) << " has connected." << std::endl;
	}

	void onDisconnect(myo::Myo* myo, uint64_t timestamp) {
		std::cout << "Myo " << identifyMyo(myo) << " has disconnected." << std::endl;
	}

    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData(). For this example, the functions overridden above are sufficient.

    void print() { //prints the current values that were updated by the on...() functions above
        cout << '\r' << '\r';

        for (size_t i = 0; i < emgSamples.size(); i++) { // Print out the EMG data.
            ostringstream oss;
            oss << static_cast<int>(emgSamples[i]);
            string emgString = oss.str();

            cout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
        }
		
		cout << '[' << roll_w << ',' << pitch_w << ',' << yaw_w << ']';
		
		if (onArm) {
			// Print out the lock state, the currently recognized pose, and which arm Myo is being worn on.

			// Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
			// output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
			// that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
			string poseString = currentPose.toString();

			cout << '[' << (isUnlocked ? "UL" : "LK") << ']'
				<< '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
				<< '[' << poseString << string(14 - poseString.size(), ' ') << ']';
		}
		else {
			// Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on.
			cout << '[' << string(8, ' ') << ']' << "[?]" << '[' << string(10, ' ') << ']';
		}

        cout << flush;
    }
    
	// This is a utility function implemented for this sample that maps a myo::Myo* to a unique ID starting at 1.
	// It does so by looking for the Myo pointer in knownMyos, which onPair() adds each Myo into as it is paired.
	
	size_t identifyMyo(myo::Myo* myo) {
		// Walk through the list of Myo devices that we've seen pairing events for.
		for (size_t i = 0; i < knownMyos.size(); ++i) {
			// If two Myo pointers compare equal, they refer to the same Myo device.
			if (knownMyos[i] == myo) {
				return i + 1;
			}
		}

		return 0;
	}

    array<int8_t, 8> emgSamples; // The values of this array are set by onEmgData() above

	bool onArm;
	myo::Arm whichArm;
	bool isUnlocked;
	int roll_w, pitch_w, yaw_w;
	myo::Pose currentPose;

	vector<myo::Myo*> knownMyos;
	myo::Myo* listOfMyos[2];
	int listIndex = 0;
};

void printToFile(ofstream & fout, array<int8_t, 8> emgSamples) {
	for (size_t i = 0; i < emgSamples.size(); i++) { // Print out the EMG data.
		ostringstream oss;
		oss << static_cast<int>(emgSamples[i]);
		string emgString = oss.str();

		fout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
	}
	fout << endl;
}

void csv_output(ofstream & fout, DataCollector& collector) {
	for (size_t i = 0; i < collector.emgSamples.size(); i++) { // Print out the EMG data.
		ostringstream oss;
		oss << static_cast<int>(collector.emgSamples[i]);
		string emgString = oss.str();

		fout << emgString << ", ";
	}
	
	fout << collector.roll_w << ", " << collector.pitch_w << ", " << collector.yaw_w << ", ";
	fout << '[' << (collector.whichArm == myo::armLeft ? "L" : "R") << "], ";
	fout << "r ";

	//fout << endl;
}

int main(int argc, char** argv) {
try {
    myo::Hub hub("com.example.emg-data-sample");
    cout << "Attempting to find a Myo..." << endl;

	/*
	//MYO 1 ///////////////////////////////////////////
    myo::Myo* myo1 = hub.waitForMyo(10000);
    if (!myo1) { //check if failed
        throw runtime_error("Unable to find a Myo 1!");
    }

    cout << "Connected to a Myo armband (1) !" << endl << endl;
    myo1->setStreamEmg(myo::Myo::streamEmgEnabled); //enable EMG streaming

    DataCollector collector1; //construct device listener, to be registered with Hub

	//MYO 2 //////////////////////////////////////////
	myo::Myo* myo2 = hub.waitForMyo(10000);
	if (!myo2) { //check if failed
		throw runtime_error("Unable to find a Myo 2!");
	}

	cout << "Connected to a Myo armband (2) ! " << endl << endl;
	myo2->setStreamEmg(myo::Myo::streamEmgEnabled); //enable EMG streaming

	DataCollector collector2; //construct device listener, to be registered with Hub

	//////////////////////////////////////////////////
	

    // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
    // Hub::run() to send events to all registered device listeners.
    hub.addListener(&collector1);
	hub.addListener(&collector2);
	*/

	DataCollector collector3;
	hub.addListener(&collector3);
	
	ifstream fin("output_count.txt");
	if (!fin) {
		throw runtime_error("Failed to open output counter file!");
	}
	int output_count;
	fin >> output_count;
	fin.close();
	ofstream finEdit("output_count.txt");
	finEdit << output_count + 1;
	finEdit.close();
	string output_first_name = "myo_output";
	string output_name = output_first_name + to_string(output_count) + ".csv";
	ofstream fout(output_name);
	if (!fout) {
		throw runtime_error("Failed to create output text file!");
	}

	//std::vector<myo::Myo*>::iterator it1 = (collector3.knownMyos).begin();
	/*
	for (size_t i = 0; i < knownMyos.size(); ++i) {
		// If two Myo pointers compare equal, they refer to the same Myo device.
		if (knownMyos[i] == myo) {
			return i + 1;
		}
	}
	
	size_t iter = 0;
	size_t size = collector3.knownMyos.size();
	*/
	//myo::Myo* firstMyo = collector3.knownMyos.back();
	//myo::Myo* secondMyo = collector3.knownMyos.front();
	
	//cout << iter << " " << size; */

	/*collector3.listOfMyos[0];
	collector3.listOfMyos[1];*/

	if (!collector3.listOfMyos[0] || !collector3.listOfMyos[1]) {
		//throw runtime_error("One of the Myo pointers is null!");
		cout << "One of the Myo pointers is null!";
	}

	//(collector3.listOfMyos[1])->setStreamEmg(myo::Myo::streamEmgEnabled);

	// Finally we enter our main loop.
    while (true) {
        /*
		hub.run(1000/20); 
        collector1.print();
		collector2.print();
		//printToFile(fout, collector1.emgSamples);
		csv_output(fout, collector1);
		csv_output(fout, collector2);
		fout << endl;
		*/

		hub.run(1000/20);
		
		

		/*
		//(collector3.listOfMyos[1])->vibrate(myo::Myo::vibrationShort);
		(collector3.listOfMyos[1])->setStreamEmg(myo::Myo::streamEmgEnabled);
		//collector3.print();
		//csv_output(fout, collector3);
		
		
		(collector3.listOfMyos[1])->setStreamEmg(myo::Myo::streamEmgDisabled);

		
		hub.run(1000 / 20);
		(collector3.listOfMyos[1])->setStreamEmg(myo::Myo::streamEmgEnabled);
		//collector3.print();
		//csv_output(fout, collector3);
		
		(collector3.listOfMyos[1])->setStreamEmg(myo::Myo::streamEmgDisabled);
		*/
    }

} catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "Press enter to continue.";
        cin.ignore();
        return 1;
}
}
