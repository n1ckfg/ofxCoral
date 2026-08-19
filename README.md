# ofxCoral

OpenFrameworks addon for Google Coral Edge TPU inference using TensorFlow Lite.

## Features

- **Edge TPU acceleration** - Automatic detection and use of Google Coral USB Accelerator / Dev Board
- **Camera integration** - GStreamer-based camera capture with v4l2src
- **Classification & Detection** - Support for both image classification and object detection models
- **Cross-platform** - Works on Linux (x64, ARMv7, ARM64), with stubs for other platforms

## Requirements

- openFrameworks 0.12+
- Google Coral Edge TPU runtime (`libedgetpu1`, `libedgetpu-dev`)
- TensorFlow Lite (`libtensorflow-lite`)
- GStreamer 1.0 with v4l2 and appsink plugins
- Compatible Coral model (.tflite compiled for Edge TPU)

### Install dependencies (Raspberry Pi / Debian arm64)

```bash
# Add Coral repository
echo "deb https://packages.cloud.google.com/apt coral-edgetpu-stable main" | sudo tee /etc/apt/sources.list.d/coral-edgetpu.list
curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
sudo apt-get update

# Install Edge TPU runtime and TensorFlow Lite
sudo apt-get install libedgetpu1-std libedgetpu-dev
sudo apt-get install python3-tflite-runtime  # includes libtensorflow-lite

# GStreamer
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad gstreamer1.0-libav
```

### Download models

```bash
cd /home/pi/openFrameworks/of_v0.12.1_linuxaarch64_release/addons/ofxCoral
sh docs/download_models.sh
# Models will be in docs/all_models/
```

## Usage

### Basic setup

```cpp
#include "ofxCoral.h"

ofxCoral::ofxCoral coral;

void setup() {
    // Load model and labels
    coral.setup("path/to/model_edgetpu.tflite", "path/to/labels.txt", true);

    // Setup camera (optional - can also run inference on static images)
    coral.setupCamera(640, 480, 30, "/dev/video0");

    // Set callbacks
    coral.onClassification = [](const std::vector<ofxCoral::Classification>& results) {
        for (const auto& c : results) {
            ofLogNotice() << c.label << ": " << c.confidence;
        }
    };

    coral.onDetection = [](const std::vector<ofxCoral::Detection>& results) {
        for (const auto& d : results) {
            ofLogNotice() << d.label << ": " << d.confidence
                          << " @" << d.x << "," << d.y << " " << d.width << "x" << d.height;
        }
    };

    // Start the pipeline
    coral.start();
}

void update() {
    coral.update();
}

void draw() {
    // Draw results...
}

void exit() {
    coral.stop();
}
```

### Manual inference on images

```cpp
ofPixels pixels;
// ... load or capture pixels ...

// Classification
auto results = coral.classify(pixels);
for (const auto& c : results) {
    cout << c.label << ": " << c.confidence << endl;
}

// Detection (if model supports it)
auto detections = coral.detect(pixels);
for (const auto& d : detections) {
    cout << d.label << ": " << d.confidence
         << " bbox: " << d.x << "," << d.y << " " << d.width << "x" << d.height << endl;
}
```

## Model Compatibility

### Classification models (ImageNet, iNaturalist)
- Input: 224x224x3 RGB
- Output: Single tensor with class probabilities
- Examples: MobileNet v1/v2, Inception v1-v4

### Detection models (SSD)
- Input: 300x300x3 RGB (varies by model)
- Output: 4 tensors [num_detections, boxes, classes, scores]
- Examples: MobileNet SSD v1/v2 COCO, MobileNet SSD v2 Face

## Architecture

```
ofxCoral (main class)
├── InferenceEngine - TensorFlow Lite + Edge TPU delegate
│   ├── Model loading (.tflite)
│   ├── Label loading
│   ├── Edge TPU delegate attachment
│   ├── classify() - returns top-K classifications
│   └── detect() - returns bounding box detections
└── CameraStreamer - GStreamer pipeline
    ├── v4l2src camera capture
    ├── videoconvert to RGB
    ├── appsink callback to inference
    └── Runs in separate thread
```

## Platform Notes

| Platform | Edge TPU | Camera | Status |
|----------|----------|--------|--------|
| Linux x64 | ❌ | ✅ | CPU only |
| Linux ARMv7 | ✅ | ✅ | Full support |
| Linux ARM64 | ✅ | ✅ | Full support (Pi 4, Coral Dev Board) |
| Android | ✅* | ✅* | Requires NDK libs |
| iOS | ❌ | ❌ | Not supported |
| Emscripten | ❌ | ❌ | Not supported |

*Android requires prebuilt `libedgetpu.so` and `libtensorflow-lite.so` in `libs/<arch>/`

## License

MIT - See LICENSE file

## References

- [Coral Examples](https://github.com/google-coral/examples-camera)
- [TensorFlow Lite C++ API](https://www.tensorflow.org/lite/guide/build_cmake)
- [Edge TPU Python API](https://coral.ai/docs/edgetpu/api-intro/)