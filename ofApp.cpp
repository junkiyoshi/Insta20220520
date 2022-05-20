#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 90 < 15) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 15, 0.05, 0.002);
	}

	this->face.clear();
	this->frame.clear();
	this->setBoxRecursive(glm::vec3(0, 0, 0), 300);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.6666666);
	ofRotateX(ofGetFrameNum() * 0.3333333);

	this->face.draw();
	this->frame.drawWireframe();

	for (int i = 0; i < this->face.getNumVertices(); i++) {

		ofSetColor(this->frame.getColor(i));
		ofDrawSphere(this->face.getVertex(i), 3);
	}

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxRecursive(glm::vec3 location, float size) {

	vector<glm::vec3> location_list = {
		location + glm::vec3(-size * 0.5, -size * 0.5, size * 0.5),
		location + glm::vec3(-size * 0.5, size * 0.5, size * 0.5),
		location + glm::vec3(size * 0.5, size * 0.5, size * 0.5),
		location + glm::vec3(size * 0.5, -size * 0.5, size * 0.5),
		location + glm::vec3(-size * 0.5, -size * 0.5, -size * 0.5),
		location + glm::vec3(-size * 0.5, size * 0.5, -size * 0.5),
		location + glm::vec3(size * 0.5, size * 0.5, -size * 0.5),
		location + glm::vec3(size * 0.5, -size * 0.5, -size * 0.5)
	};


	if (size < 50) {

		for (auto& l : location_list) {

			auto noise_value = ofNoise(glm::vec4(l * 0.05, this->noise_param));

			if (noise_value < 0.50) {

				this->setBoxToMesh(this->face, this->frame, l, size);
			}
		}
	}
	else {

		for (auto& l : location_list) {

			auto noise_value = ofNoise(glm::vec4(l * 0.05, this->noise_param));

			if (size == 300 || noise_value < 0.5) {

				this->setBoxRecursive(l, size * 0.5);
			}
			else if (noise_value < 0.6) {

				this->setBoxToMesh(this->face, this->frame, l, size * 0.25);
			}
			else {

				this->setBoxToMesh(this->face, this->frame, l, size);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth) {

	int face_index = face_target.getNumVertices();
	int frame_index = frame_target.getNumVertices();

	vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	vertices.push_back(glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	vertices.push_back(glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	vertices.push_back(glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	vertices.push_back(glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	for (auto& vertex : vertices) {

		auto noise_vertex = glm::vec3(
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -width * 0.5, width * 0.5),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -height * 0.5, height * 0.5),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.015), 0, 1, -depth * 0.5, depth * 0.5));

		face_target.addVertex(location + vertex * 0.99);
		frame_target.addVertex(location + vertex);
	}

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);

	face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 1);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 5); face_target.addIndex(face_index + 6);
	face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 1);

	face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 6); face_target.addIndex(face_index + 7);
	face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 2);

	face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 7); face_target.addIndex(face_index + 4);
	face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 3);

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 0);

	frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 5); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 6); frame_target.addIndex(frame_index + 7);
	frame_target.addIndex(frame_index + 7); frame_target.addIndex(frame_index + 4);

	frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 4);
	frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
	frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 6);
	frame_target.addIndex(frame_index + 3); frame_target.addIndex(frame_index + 7);


	int value = ofRandom(255);
	for (int i = 0; i < 8; i++) {

		face_target.addColor(ofColor(value));
		frame_target.addColor(ofColor((value + 128) % 255));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}