#include "ofxCoral.h"

namespace ofxCoral {

// InferenceEngine
InferenceEngine::InferenceEngine() {
    ofLogNotice("ofxCoral") << "InferenceEngine constructor";
}

InferenceEngine::~InferenceEngine() {
    ofLogNotice("ofxCoral") << "InferenceEngine destructor";
}

bool InferenceEngine::setup(const std::string& modelPath, const std::string& labelPath, bool useEdgeTPU) {
    ofLogNotice("ofxCoral") << "InferenceEngine::setup called (stub)";
    (void)modelPath;
    (void)labelPath;
    (void)useEdgeTPU;
    // Pretend we loaded a model successfully
    return true;
}

bool InferenceEngine::isReady() const {
    return true; // Pretend ready
}

bool InferenceEngine::isDetectionModel() const {
    return false; // Assume classification model for stub
}

std::vector<Classification> InferenceEngine::classify(const unsigned char* rgbData, int width, int height) {
    (void)rgbData;
    (void)width;
    (void)height;
    return {};
}

std::vector<Detection> InferenceEngine::detect(const unsigned char* rgbData, int width, int height) {
    (void)rgbData;
    (void)width;
    (void)height;
    return {};
}

int InferenceEngine::getInputWidth() const {
    return 224;
}

int InferenceEngine::getInputHeight() const {
    return 224;
}

int InferenceEngine::getInputChannels() const {
    return 3;
}

// CameraStreamer
CameraStreamer::CameraStreamer() {
    ofLogNotice("ofxCoral") << "CameraStreamer constructor";
}

CameraStreamer::~CameraStreamer() {
    ofLogNotice("ofxCoral") << "CameraStreamer destructor";
}

bool CameraStreamer::setup(int width, int height, int fps, const std::string& device) {
    ofLogNotice("ofxCoral") << "CameraStreamer::setup called (stub)";
    (void)width;
    (void)height;
    (void)fps;
    (void)device;
    return true;
}

void CameraStreamer::start(FrameCallback callback) {
    ofLogNotice("ofxCoral") << "CameraStreamer::start called (stub)";
    (void)callback;
    // Do not actually start camera
}

void CameraStreamer::stop() {
    ofLogNotice("ofxCoral") << "CameraStreamer::stop called (stub)";
}

bool CameraStreamer::isRunning() const {
    return false;
}

// ofxCoral
ofxCoral::ofxCoral() {
    ofLogNotice("ofxCoral") << "ofxCoral constructor";
}

ofxCoral::~ofxCoral() = default;

bool ofxCoral::setup(const std::string& modelPath, const std::string& labelPath, bool useEdgeTPU) {
    return engine_.setup(modelPath, labelPath, useEdgeTPU);
}

bool ofxCoral::setupCamera(int width, int height, int fps, const std::string& device) {
    return camera_.setup(width, height, fps, device);
}

void ofxCoral::start() {
    ofLogNotice("ofxCoral") << "ofxCoral::start called";
    if (!engine_.isReady()) {
        ofLogError("ofxCoral") << "Engine not ready";
        return;
    }
    // In a real implementation, we would start the camera and set up callbacks.
    // For stub, we do nothing.
}

void ofxCoral::stop() {
    ofLogNotice("ofxCoral") << "ofxCoral::stop called";
}

void ofxCoral::update() {
    ofLogNotice("ofxCoral") << "ofxCoral::update called";
    // In a real implementation, we would update the engine and camera here.
    // For the stub, we do nothing.
}

bool ofxCoral::isReady() const {
    return engine_.isReady();
}

bool ofxCoral::isRunning() const {
    return camera_.isRunning();
}

int ofxCoral::getInputWidth() const {
    return engine_.getInputWidth();
}

int ofxCoral::getInputHeight() const {
    return engine_.getInputHeight();
}

std::vector<Classification> ofxCoral::classify(const ofPixels& pixels) {
    if (pixels.getNumChannels() == 3) {
        return engine_.classify(pixels.getData(), pixels.getWidth(), pixels.getHeight());
    }
    return {};
}

std::vector<Detection> ofxCoral::detect(const ofPixels& pixels) {
    if (pixels.getNumChannels() == 3) {
        return engine_.detect(pixels.getData(), pixels.getWidth(), pixels.getHeight());
    }
    return {};
}

} // namespace ofxCoral
