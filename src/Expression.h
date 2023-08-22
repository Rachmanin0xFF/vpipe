#pragma once

#include "ofMain.h"

struct Expression {
public:
	glm::vec3 position;
	glm::quat rotation;

	std::vector<glm::vec3> OSF_points;

	Expression& operator+=(const Expression& rhs) {
		for (int i = 0; i < rhs.OSF_points.size(); i++) {
			this->OSF_points[i] += rhs.OSF_points[i];
		}
		return *this;
	}
	friend Expression operator+(Expression lhs, const Expression& rhs) {

	}
};