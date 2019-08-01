
#include "..\headers\lighthouse.h"




#include <algorithm>

// Destructor
Lighthouse::~Lighthouse() {

	if (m_pHMD != NULL)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}

}


// Constructor
Lighthouse::Lighthouse(double device, std::string path) {
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);
	poseCount = 0;
	this->device = device;
	if (path.size() == 0)
		path = "csv";
	this->path = path;
	file.setFolderName(path);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
	}
}

double Lighthouse::correctAngle(double angle) {
	if (angle > 180.0) {
		return angle - 360.0;
	}
	if (angle < -180.0) {
		return angle + 360.0;
	}
	return angle;
}


double* Lighthouse::getEuler(vr::HmdMatrix34_t posMatrix) {

	double yaw = 180 / M_PI * atan2(posMatrix.m[1][0], posMatrix.m[0][0]);
	double pitch = 180 / M_PI * atan2(-1 * posMatrix.m[2][0], sqrt(pow(posMatrix.m[2][1], 2) + pow(posMatrix.m[2][2], 2)));
	double roll = 180 / M_PI * atan2(posMatrix.m[2][1], posMatrix.m[2][2]);
	double x = posMatrix.m[0][3];
	double y = posMatrix.m[1][3];
	double z = posMatrix.m[2][3];

	double *values = new double[6];

	*(values + 0) = x ;
	*(values + 1) = y ;
	*(values + 2) = z ;
	*(values + 3) = correctAngle(yaw );
	*(values + 4) = correctAngle(pitch);
	*(values + 5) = correctAngle(roll);

	return values;
}

Matrix4 Lighthouse::getTransformMatrix(double* values) {
	Matrix4 m = Matrix4();
	m.translate(*(values), *(values + 1), *(values + 2));
	m.rotateX(*(values + 5));
	m.rotateY(*(values + 4));
	m.rotateZ(*(values + 3));

	return m;
}

Pose Lighthouse::vectorToPose(double *vector) {
	file.open("lightHouseRound" + SSTR(poseCount++) + ".csv");
	Pose pose = Pose(vector[0], vector[2], vector[4]);
	file.appendToFile(pose.getAsCSV());
	file.close();
	return pose;
}

//-----------------------------------------------------------------------------
// Purpose: Prints out position (x,y,z) and rotation (qw,qx,qy,qz) into the console.
//-----------------------------------------------------------------------------
Pose Lighthouse::getPose()
{

	if (m_pHMD == NULL) return Pose();


	// Process SteamVR device states
	for (int nDevice = 0; nDevice <= vr::k_unMaxTrackedDeviceCount; nDevice++) {
		if (!m_pHMD->IsTrackedDeviceConnected(nDevice))
			continue;

		vr::VRControllerState_t state;

		if (m_pHMD->GetControllerState(nDevice, &state, sizeof(state)))
		{

			vr::TrackedDevicePose_t trackedDevicePose;
			vr::TrackedDevicePose_t trackedControllerPose;
			vr::VRControllerState_t controllerState;
			vr::HmdMatrix34_t poseMatrix;
			vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(nDevice);

			switch (trackedDeviceClass) {
			case vr::ETrackedDeviceClass::TrackedDeviceClass_HMD:

				vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &trackedDevicePose, 1);
				// print positiona data for the HMD.

				poseMatrix = trackedDevicePose.mDeviceToAbsoluteTracking; // This matrix contains all positional and rotational data.


				if (device == 0) {
					double* euler = getEuler(poseMatrix);
					return vectorToPose(euler);
				}

				break;

			case vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker:
				vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &trackedDevicePose, 1);
				poseMatrix = trackedDevicePose.mDeviceToAbsoluteTracking; // This matrix contains all positional and rotational data.

				if (device == 3) {
					double* euler = getEuler(poseMatrix);
					return vectorToPose(euler);
				}
				break;

			case vr::ETrackedDeviceClass::TrackedDeviceClass_Controller:
				vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, nDevice, &controllerState,
					sizeof(controllerState), &trackedControllerPose);
				poseMatrix = trackedControllerPose.mDeviceToAbsoluteTracking; // This matrix contains all positional and rotational data.

				auto trackedControllerRole = vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(nDevice);

				switch (trackedControllerRole)
				{
				case vr::TrackedControllerRole_Invalid:
					// invalid
					break;

				case vr::TrackedControllerRole_LeftHand:
					if (device == 1) {
						double* euler = getEuler(poseMatrix);
						return Pose(euler[0], euler[2], euler[4]);
					}
					break;
				case vr::TrackedControllerRole_RightHand:
					if (device == 2) {
						double* euler = getEuler(poseMatrix);
						return vectorToPose(euler);
					}

					break;
				}
				break;
			}
		}
	}

	return Pose();
}