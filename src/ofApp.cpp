#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableAntiAliasing();


	box_head.set(10.f, 100.f, 100.f);
	test_box.set(1.f, 1);
}

//--------------------------------------------------------------

void ofApp::update(){
	h.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	glClearDepth(1.0);

	ofClear(0);
	ofEnableDepthTest();
	//ofBackground(255, 0, 255);
	
	float movement = 100.0;

	test_cam.begin();
	//ofDrawGrid();

	glm::mat4 lookie = glm::lookAt(glm::vec3(200, 0, 0), glm::vec3(10000, 0, 0), glm::vec3(0, 0, 1));
	//glm::quat rotquat(last_face.quat0, last_face.quat1, last_face.quat2, last_face.quat3);
	//box_head.setOrientation(rotquat);
	//glm::mat4 rotmat = glm::toMat4(rotquat);
	h.OSF_input.dbDrawLatestInfo(100.0);
	h.OSF_input.dbDrawLatestFrame(100.0);
	ofPushMatrix();

	//ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	//ofRotateXDeg(ofGetMouseX());
	//ofRotateYDeg(ofGetMouseY());

	
	//ofRotateZDeg(90);
	//ofLoadMatrix(lookie);

	//ofTranslate(glm::vec3(-last_face.translation2 * movement,
	//	-last_face.translation1 * movement,
	//	last_face.translation0 * movement));
	
	//ofMultMatrix(rotmat);
	//ofDrawAxis(300);
	//ofDrawGrid();
	
	
	//ofSetColor(0);
	//box_head.draw();
	//ofSetColor(30, 255, 90);
	//box_head.drawWireframe();



	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	ofSetColor(255);
	

	/*
	for (int i = 0; i < NUM_COORDS; i+=3) {
		float x = -last_face.coords[i + 2] * movement - 50;
		float y = last_face.coords[i + 1] * movement;
		float z = last_face.coords[i] * movement;
		test_pts[i/3].set(x, y, z);
	}

	for (int i = 0; i < NUM_COORDS/3 - 1; i++) {
		ofDrawLine(test_pts[i], test_pts[i+1]);
	}
	ofSeedRandom(0);
	for (int i = 0; i < NUM_COORDS; i += 3) {
		
		float x = -last_face.coords[i + 2] * movement - 50;
		float y = last_face.coords[i + 1] * movement;
		float z = last_face.coords[i] * movement;
		test_box.setPosition(x, y, z);
		test_box.setOrientation(glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360)));
		test_box.draw();
	}
	*/
	ofDisableDepthTest();
	//ofDisableBlendMode();
	ofPopMatrix();
	test_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
