#include "OSFListener.h"
#include <glm/gtx/string_cast.hpp>


std::string OSFListener::OSFFeatureInfo::to_string(void) {
	std::ostringstream oss;
	oss << "================FEATURE INFO================" << std::endl;
	oss << "Time: " << timestamp << std::endl;
	oss << "Rotation: " << glm::to_string(rotation) << std::endl;
	oss << "Position: " << glm::to_string(position) << std::endl;
	oss << "Eye: " << glm::to_string(eye) << std::endl;
	oss << "brow_steep: " << glm::to_string(brow_steep) << std::endl;
	oss << "brow_updown: " << glm::to_string(brow_updown) << std::endl;
	oss << "brow_quirk: " << glm::to_string(brow_quirk) << std::endl;
	oss << "mouth_corner_up: " << glm::to_string(mouth_corner_up) << std::endl;
	oss << "mouth_corner_in: " << glm::to_string(mouth_corner_in) << std::endl;
	oss << "mouth_open: " << mouth_open << std::endl;
	oss << "mouth_wide: " << mouth_wide << std::endl;

	return oss.str();
}

std::string OSFListener::OSFFeatureInfo::to_csv_string(void) {
	std::ostringstream oss;
	oss << glm::to_string(rotation) << ";";
	oss <<  glm::to_string(position) << ";";
	int i = 0;
	for (glm::vec3 v : points) {
		oss << glm::to_string(glm::vec4(v, confidence[i])) << ";";
		i++;
	}
	oss << std::endl;

	return oss.str();
}

OSFListener::OSFListener(const int& port) {
	ofxUDPSettings settings;
	settings.receiveOn(port);
	settings.blocking = false;
	settings.receiveBufferSize = UDP_SIZE;

	udpConnection.Setup(settings);
}

void OSFListener::update(void) {
	memset(udpMessage, 0, sizeof(udpMessage));
	int size = udpConnection.Receive(udpMessage, UDP_SIZE);
	if (size > 0) {
		readPack(udpMessage, latest_frame);
		updateInfo(latest_frame);
	}
	
}

void OSFListener::updateInfo(FacePacket &p) {
	bool changed = (latest_info.timestamp != p.now);
	if (!changed) return;
	latest_info.timestamp = p.now;
	latest_info.rotation = glm::quat(p.quat[0], p.quat[1], p.quat[2], p.quat[3]);
	latest_info.position = glm::vec3(p.translation[0], p.translation[1], p.translation[2]);
	for (int i = 0; i < NUM_POINTS; i++) {
		latest_info.points[i] = glm::vec3(p.xyz_coords[i * 3], p.xyz_coords[i * 3 + 1], p.xyz_coords[i * 3 + 2]);
		latest_info.confidence[i] = p.lms_coords[i * 3 + 2];
	}
	OSFListener::OSFFeatureInfo pred = model_predict(latest_info.rotation, latest_info.position);
	for (int i = 0; i < NUM_POINTS; i++) {
		latest_info.points_predicted[i] = pred.points[i];
		latest_info.points_offset[i] = latest_info.points[i] - pred.points[i];
	}

	// features
	latest_info.eye = glm::vec2(p.feature_params[0], p.feature_params[1]);
	latest_info.brow_steep = glm::vec2(p.feature_params[2], p.feature_params[5]);
	latest_info.brow_updown = glm::vec2(p.feature_params[3], p.feature_params[6]);
	latest_info.brow_quirk = glm::vec2(p.feature_params[4], p.feature_params[7]);
	latest_info.mouth_corner_up = glm::vec2(p.feature_params[8], p.feature_params[10]);
	latest_info.mouth_corner_in = glm::vec2(p.feature_params[9], p.feature_params[11]);
	latest_info.mouth_open = p.feature_params[12];
	latest_info.mouth_wide = p.feature_params[13];
	
	if (log_incoming_to_file) {
		log_file_stream.open(log_file_name, std::ios_base::app);
		log_file_stream << latest_info.to_csv_string();
		log_file_stream.close();
	}
}

void OSFListener::dbDrawLatestInfo(const float& scale) {
	OSFListener::OSFFeatureInfo pred = model_predict(latest_info.rotation, latest_info.position);
	for (int i = 0; i < NUM_POINTS; i++) {

		// iris features should look larger than other if indices are aligned correctly
		ofSetColor(50, 120, 255);
		ofDrawIcoSphere(latest_info.points_offset[i].x * scale, 
			latest_info.points_offset[i].y * scale,
			latest_info.points_offset[i].z * scale,
			(i==66 || i == 67) ?4.0 : 2.0);
		

		ofSetColor(255, 100, 100);
		ofDrawIcoSphere(pred.points[i].x * scale,
			pred.points[i].y * scale,
			pred.points[i].z * scale,
			(i == 66 || i == 67) ? 4.0 : 2.0);
		//std::cout << glm::to_string(pred.points[i]) << std::endl;
	}
}

void OSFListener::dbDrawLatestFrame(const float &scale) {
	ofSetColor(255);
	float x1, y1, z1;
	for (int i = 0; i < NUM_COORDS - 6; i+=3) {
		float x0 = latest_frame.xyz_coords[i]*scale;
		float y0 = latest_frame.xyz_coords[i + 1] * scale;
		float z0 = latest_frame.xyz_coords[i + 2] * scale;
		x1 = latest_frame.xyz_coords[i + 3] * scale;
		y1 = latest_frame.xyz_coords[i + 4] * scale;
		z1 = latest_frame.xyz_coords[i + 5] * scale;
		ofDrawLine(x0, y0, z0, x1, y1, z1);
		//ofDrawIcoSphere(x0, y0, z0, 1.0);
	}
	//ofDrawIcoSphere(x1, y1, z1, 1.0);
}

void OSFListener::readPack(const char* message, FacePacket &p) {
	int i = 0;

	memcpy(&p.now, &message[i], sizeof p.now); i += sizeof p.now;
	memcpy(&p.id, &message[i], sizeof p.id); i += sizeof p.id;
	memcpy(&p.width, &message[i], sizeof p.width); i += sizeof p.width;
	memcpy(&p.height, &message[i], sizeof p.height); i += sizeof p.height;
	memcpy(&p.blinkL, &message[i], sizeof p.blinkL); i += sizeof p.blinkL;
	memcpy(&p.blinkR, &message[i], sizeof p.blinkR); i += sizeof p.blinkR;
	memcpy(&p.success, &message[i], sizeof p.success); i += sizeof p.success;
	memcpy(&p.php_error, &message[i], sizeof p.php_error); i += sizeof p.php_error;
	memcpy(p.quat, &message[i], 4 * (sizeof p.quat[0])); i += 4 * (sizeof p.quat[0]);
	memcpy(p.euler, &message[i], 3 * (sizeof p.euler[0])); i += 3 * (sizeof p.euler[0]);
	memcpy(p.translation, &message[i], 3 * (sizeof p.translation[0])); i += 3 * (sizeof p.translation[0]);
	memcpy(&(p.xyz_coords[0]), &message[i], NUM_COORDS * sizeof(float)); i += NUM_COORDS * sizeof(float);
	memcpy(&(p.lms_coords[0]), &message[i], NUM_COORDS * sizeof(float)); i += NUM_COORDS * sizeof(float);
	memcpy(&(p.feature_params[0]), &message[i], NUM_FEATURES * sizeof(float)); i += NUM_FEATURES * sizeof(float);
}


OSFListener::OSFFeatureInfo OSFListener::model_predict(glm::quat rot, glm::vec3 pos) {
	OSFListener::OSFFeatureInfo output;
	float x0 = rot.w; float x1 = rot.x; float x2 = rot.y; float x3 = rot.z;
	float x4 = pos.x; float x5 = pos.y; float x6 = pos.z;
	float inputs[36] = {
		1,		x0,		x1,		x2,		x3,		x4,		x5,		x6,
		x0 * x0,x0 * x1,x0 * x2,x0 * x3,x0 * x4,x0 * x5,x0 * x6,
		x1 * x1,x1 * x2,x1 * x3,x1 * x4,x1 * x5,x1 * x6,
		x2 * x2,x2 * x3,x2 * x4,x2 * x5,x2 * x6,
		x3 * x3,x3 * x4,x3 * x5,x3 * x6,
		x4 * x4,x4 * x5,x4 * x6,
		x5 * x5,x5 * x6,
		x6 * x6
	};
	for (int i = 0; i < model_coeffs.size()-2; i+=3) {
		float xsum = 0.f;
		float ysum = 0.f;
		float zsum = 0.f;
		for (int j = 0; j < 36; j++) {
			xsum += inputs[j] * model_coeffs[i][j];
			ysum += inputs[j] * model_coeffs[i+1][j];
			zsum += inputs[j] * model_coeffs[i+2][j];
		}
		output.points[i / 3] = glm::vec4(xsum, ysum, zsum, 1.0);
	}
	
	return output;
}

void OSFListener::loadCorrectionModel(const char* path) {
	std::string line;
	std::ifstream datfile(path);
	if (!datfile.is_open()) {
		std::cerr << "Could not open OSF correction model file at " << path << "! Aborting!" << std::endl;
		return;
	}
	std::getline(datfile, line);
	while (datfile.good()) {
		std::getline(datfile, line);
		std::stringstream ss(line);
		std::vector<float> this_vec;
		string substr;
		while (ss.good()) {
			getline(ss, substr, ',');
			this_vec.push_back(std::stof(substr));
		}
		model_coeffs.push_back(this_vec);
	}
}