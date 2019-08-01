
#pragma once

#include "sensors.h"
#include "poseTracker.h"
#include "rpLidarSensor.h"

class Tracking : public ISensor, public Runnable {
	private:
		IPoseSensor **trackers;
		int trackersCount;
		bool stopThread;
		Thread *thread;

	public:
		Tracking(){
			trackersCount = 1;
			trackers = new IPoseSensor*[1];
			trackers[0] = new PoseTracker(new RPLidar(), Object2D(), "test");
		}

		Tracking(IPoseSensor **trackers, int trackersCount) {
			this->trackersCount = trackersCount;
			this->trackers = trackers;
		}

		void start() {
			stopThread = false;
			thread = new Thread(this);
			for (int i = 0; i < trackersCount; i++) {
				trackers[i]->start();
			}

			thread->start();
		}

		void stop() {
			stopThread = true;
			for (int i = 0; i < trackersCount; i++) {
				trackers[i]->stop();
			}
			thread->stop();
		}

		void run() {
			Sleep(1000);
			while (!stopThread){
				for (int i = 0; i < trackersCount; i++) {
					Pose pose = trackers[i]->getPose();
					printf("Pose(%f,%f,%f)\n", pose.x, pose.y, pose.phi / M_PI * 180);
				}
				Sleep(100);
			}
		}
};
