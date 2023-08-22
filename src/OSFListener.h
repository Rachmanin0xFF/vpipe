#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "Expression.h"

struct ExpressionInfo {
	std::vector<glm::vec3> OSFpoints;
	std::vector<glm::vec3> OSFoffsets;
	glm::vec3 position;
	glm::vec3 rotation;
};

class OSFListener
{
public:
	static const int NUM_COORDS = 210; // total num of landmark coordinates (i.e. # of points * 3)
	static const int NUM_POINTS = NUM_COORDS / 3;
	static const int UDP_SIZE = 16384;
	static const int DEFAULT_PORT = 11378;
	static const int NUM_FEATURES = 14;

	struct FacePacket {
		double now; // Timestamp
		int id;
		float width;
		float height;
		float blinkL; // maybe should be named "wink" lol
		float blinkR;
		char success;
		float php_error;
		float quat[4]; // Quaternion rotation
		float euler[3]; // Euler angles
		float translation[3]; // Position
		float xyz_coords[NUM_COORDS]; // coords in 3d space ([x0, y0, z0, x1, y1, z1, ..., xn, yn, zn])
		float lms_coords[NUM_COORDS]; // coords in landmark (screen) space with confidence as z
		float feature_params[NUM_FEATURES];
	};
	struct OSFFeatureInfo {
		double timestamp;
		glm::quat rotation;
		glm::vec3 position;

		glm::vec3 points[NUM_POINTS];
		float confidence[NUM_POINTS];

		glm::vec3 points_predicted[NUM_POINTS];
		glm::vec3 points_offset[NUM_POINTS];
		
		// following vars are calculated by OSF (they are not the best)
		// vec2s are (x=left side, y=right side)
		glm::vec2 eye;
		glm::vec2 brow_steep;
		glm::vec2 brow_updown;
		glm::vec2 brow_quirk;
		glm::vec2 mouth_corner_up;
		glm::vec2 mouth_corner_in;
		float mouth_open;
		float mouth_wide;

		std::string to_string(void);
		std::string to_csv_string(void);
	};
	OSFListener(const int& port);
	OSFListener() : OSFListener(DEFAULT_PORT) {}
	FacePacket latest_frame;
	OSFFeatureInfo latest_info;
	void update(void);

	OSFFeatureInfo model_predict(glm::quat rot, glm::vec3 pos);
	void loadCorrectionModel(const char* path);

	void dbDrawLatestFrame(const float &scale);
	void dbDrawLatestInfo(const float &scale);

private:
	ofxUDPManager udpConnection;
	char udpMessage[UDP_SIZE];
	void readPack(const char* message, FacePacket &p);
	void updateInfo(FacePacket& p);

	bool log_incoming_to_file = false;
	const char* log_file_name = "modelB_data.csv";
	std::ofstream log_file_stream;

	std::vector<std::vector<float>> model_coeffs;
};

