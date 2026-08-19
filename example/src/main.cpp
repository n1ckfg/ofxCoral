#include "ofMain.h"
#include "ofxCoral.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void exit() override;

    ofxCoral::ofxCoral coral;
    bool showGui = true;
    std::string modelPath = "../../../data/mobilenet_v2_1.0_224_quant_edgetpu.tflite";
    std::string labelPath = "../../../data/imagenet_labels.txt";
    std::string status = "Press 's' to start camera + inference";
    int fps = 0;
    int frameCount = 0;
    float lastFpsTime = 0;
    std::string classificationResults;
    std::string detectionResults;
};

void ofApp::setup() {
    ofSetWindowTitle("ofxCoral Example");
    ofSetFrameRate(60);
    ofBackground(20);

    // Try to setup Coral
    if (coral.setup(modelPath, labelPath, true)) {
        status = "Model loaded. Press 's' to start camera.";
        ofLogNotice() << "ofxCoral ready: " << coral.getInputWidth() << "x" << coral.getInputHeight();
    } else {
        status = "Failed to load model. Check paths:\n  " + modelPath + "\n  " + labelPath;
        ofLogError() << status;
    }

    coral.onClassification = [this](const std::vector<ofxCoral::Classification>& results) {
        std::string s = "Classifications:\n";
        for (const auto& c : results) {
            s += "  " + c.label + ": " + ofToString(c.confidence * 100, 1) + "%\n";
        }
        classificationResults = s;
    };

    coral.onDetection = [this](const std::vector<ofxCoral::Detection>& results) {
        std::string s = "Detections:\n";
        for (const auto& d : results) {
            s += "  " + d.label + ": " + ofToString(d.confidence * 100, 1) + "%\n";
            s += "    [" + ofToString(d.x, 2) + ", " + ofToString(d.y, 2) +
                 " " + ofToString(d.width, 2) + "x" + ofToString(d.height, 2) + "]\n";
        }
        detectionResults = s;
    };
}

void ofApp::update() {
    coral.update();

    // FPS calculation
    frameCount++;
    if (ofGetElapsedTimef() - lastFpsTime >= 1.0f) {
        fps = frameCount;
        frameCount = 0;
        lastFpsTime = ofGetElapsedTimef();
    }
}

void ofApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("ofxCoral Example", 20, 30);
    ofDrawBitmapString("FPS: " + ofToString(fps), 20, 50);
    ofDrawBitmapString(status, 20, 70);
    ofDrawBitmapString("Ready: " + ofToString(coral.isReady()), 20, 90);
    ofDrawBitmapString("Running: " + ofToString(coral.isRunning()), 20, 110);
    ofDrawBitmapString("Input: " + ofToString(coral.getInputWidth()) + "x" + ofToString(coral.getInputHeight()), 20, 130);

    if (showGui) {
        ofDrawBitmapString("\nControls:\n  s - Start camera + inference\n  x - Stop\n  g - Toggle GUI\n  q - Quit", 20, 160);
    }

    // Draw results
    if (!classificationResults.empty()) {
        ofDrawBitmapString(classificationResults, 20, 280);
    }
    if (!detectionResults.empty()) {
        ofDrawBitmapString(detectionResults, 400, 280);
    }
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case 's':
        case 'S':
            if (!coral.isRunning()) {
                if (coral.setupCamera(640, 480, 30, "/dev/video0")) {
                    coral.start();
                    status = "Camera + inference running...";
                } else {
                    status = "Failed to start camera. Check /dev/video0";
                }
            }
            break;
        case 'x':
        case 'X':
            coral.stop();
            status = "Stopped";
            break;
        case 'g':
        case 'G':
            showGui = !showGui;
            break;
        case 'q':
        case 'Q':
            ofExit();
            break;
    }
}

void ofApp::exit() {
    coral.stop();
}

int main() {
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    ofRunApp(new ofApp());
}
