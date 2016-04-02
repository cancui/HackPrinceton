// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.

// This sample illustrates how to use EMG data. EMG streaming is only supported for one Myo at a time.

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <fstream>
#include <ctime>
#include <vector>

#include <myo/myo.hpp>

using namespace std;

class DataCollector : public myo::DeviceListener {
public:
    DataCollector() : emgSamples() {}

    void onUnpair(myo::Myo* myo, uint64_t timestamp) { //called when Myo is disconnected from Myo Connect by user
        emgSamples.fill(0); //clean up leftover state
    }

    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) { //called whenever paired Myo has provided new EMG data, and EMG streaming is enabled
        for (int i = 0; i < 8; i++) {
            emgSamples[i] = emg[i];
        }
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

    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData(). For this example, the functions overridden above are sufficient.

    void print() { //prints the current values that were updated by the on...() functions above
        cout << '\r';

        for (size_t i = 0; i < emgSamples.size(); i++) { // Print out the EMG data.
            ostringstream oss;
            oss << static_cast<int>(emgSamples[i]);
            string emgString = oss.str();

            cout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
        }

        cout << flush;
    }
    
    array<int8_t, 8> emgSamples; // The values of this array are set by onEmgData() above
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

int main(int argc, char** argv) {
try {
    myo::Hub hub("com.example.emg-data-sample");
    cout << "Attempting to find a Myo..." << std::endl;

    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) { //check if failed
        throw runtime_error("Unable to find a Myo!");
    }

    std::cout << "Connected to a Myo armband!" << std::endl << std::endl;
    myo->setStreamEmg(myo::Myo::streamEmgEnabled); //enable EMG streaming

    DataCollector collector1; //construct device listener, to be registered with Hub

    // Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
    // Hub::run() to send events to all registered device listeners.
    hub.addListener(&collector1);

	ofstream fout("myo_output.txt");

	if (!fout) {
		throw runtime_error("Failed to create text file!");
	}

    // Finally we enter our main loop.
    while (true) {
        hub.run(1000/20);
        collector1.print();
		printToFile(fout, collector1.emgSamples);
    }

} catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
}
}
