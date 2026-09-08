# ofxCoral Architecture

## Overview
`ofxCoral` is an openFrameworks addon designed to provide a straightforward interface for machine learning inference using Google Coral Edge TPU and TensorFlow Lite. It enables high-speed image classification and object detection, along with real-time camera streaming.

## Core Components
The architecture is structured around three main classes, separating the high-level API, machine learning inference, and hardware-accelerated video capture.

### 1. `ofxCoral` (Main Facade)
The main class that the user interacts with in their openFrameworks application. It acts as a coordinator between the inference backend and the camera stream.
- **Responsibilities:**
  - Initializes and manages the lifetimes of `InferenceEngine` and `CameraStreamer`.
  - Provides a simple API for setup (`setup`, `setupCamera`).
  - Handles the main execution pipeline (`start`, `stop`, `update`).
  - Provides synchronous inference methods (`classify`, `detect`) that accept openFrameworks image formats (e.g., `ofPixels`).
  - Dispatches asynchronous event callbacks (`onClassification`, `onDetection`) when running continuously on a live camera stream.

### 2. `InferenceEngine`
Responsible for the core TensorFlow Lite operations and interacting with the Edge TPU hardware.
- **Responsibilities:**
  - Loads `.tflite` model files and corresponding label text files.
  - Sets up the TensorFlow Lite Interpreter and attaches the Edge TPU delegate.
  - Retrieves the input requirements of the loaded model (e.g., 224x224x3 for ImageNet models, 300x300x3 for SSD models).
  - Performs tensor memory allocation and invokes the interpreter.
  - Parses the raw output tensors into easy-to-use C++ structs (`Classification` and `Detection`).

### 3. `CameraStreamer`
Handles high-performance, background video capture using GStreamer.
- **Responsibilities:**
  - Sets up a GStreamer pipeline, typically utilizing `v4l2src` for standard V4L2 camera devices.
  - Manages color space conversion (`videoconvert`) to ensure frames are delivered in the RGB format expected by the models.
  - Utilizes `appsink` to capture frames directly into memory and fire a callback.
  - **Threading:** Runs on a separate background thread to ensure that video capture does not stall the main openFrameworks application thread.

## Data Structures
To simplify the usage of model outputs, `ofxCoral` provides two primary structs:
- **`Classification`**: Contains a `label` (string) and a `confidence` score (float, 0.0 - 1.0).
- **`Detection`**: Extends the classification concept with spatial data, including a `label`, `confidence`, and normalized bounding box coordinates (`x`, `y`, `width`, `height` ranging from 0.0 to 1.0).

## Execution Flow (Live Camera Pipeline)
1. **Initialization:** The user calls `coral.setup()` to load the model/labels, and `coral.setupCamera()` to configure the capture device.
2. **Callback Registration:** The user assigns lambda functions to `onClassification` or `onDetection`.
3. **Start:** Calling `coral.start()` spins up the `CameraStreamer` background thread.
4. **Capture & Inference Loop (Background):**
   - `CameraStreamer` grabs a frame via GStreamer.
   - The frame is passed via an internal callback to `InferenceEngine`.
   - `InferenceEngine` formats the input tensor, runs inference on the Edge TPU, and retrieves the output tensors.
5. **Dispatch:** The outputs are converted to `Classification` or `Detection` structs and passed to the user's callbacks.
6. **Update Loop (Main Thread):** The user calls `coral.update()` on the main thread, allowing safe data synchronization or dispatching of events back to the openFrameworks main loop for rendering in `draw()`.
