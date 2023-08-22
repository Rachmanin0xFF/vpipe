#pragma once

#include "OSFListener.h"

class Head
{
public:
	OSFListener OSF_input;
	Head() {
		OSF_input.loadCorrectionModel("models/modelA.txt");
	}
	void update() { OSF_input.update(); }
	void draw() { }
};

