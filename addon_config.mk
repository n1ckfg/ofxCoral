# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.

meta:
	ADDON_NAME = ofxCoral
	ADDON_DESCRIPTION = Addon for Google Coral Edge TPU inference with TensorFlow Lite
	ADDON_AUTHOR = OF Team
	ADDON_TAGS = "machine learning" "tensorflow" "edge tpu" "coral" "inference"
	ADDON_URL = https://github.com/openframeworks/openFrameworks

common:
	# dependencies with other addons
	# ADDON_DEPENDENCIES =

	# include search paths
	ADDON_INCLUDES = src
	ADDON_INCLUDES += include

	# any special flag that should be passed to the compiler when using this addon
	ADDON_CFLAGS = -std=c++17

	# any special flag that should be passed to the linker when using this addon
	ADDON_LDFLAGS =

	# linux only, any library that should be included in the project using pkg-config
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0

	# source files
	ADDON_SOURCES = src/ofxCoral.cpp

linuxaarch64:
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0
	ADDON_LDFLAGS =

linuxarmv7l:
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0
	ADDON_LDFLAGS =

linuxarmv6l:
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0
	ADDON_LDFLAGS =

linux64:
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0
	ADDON_LDFLAGS =

msys2:
	ADDON_PKG_CONFIG_LIBRARIES = gstreamer-1.0 gstreamer-video-1.0
	ADDON_LDFLAGS =

android/arm64-v8a:
	ADDON_LIBS = libs/arm64-v8a/libedgetpu.so
	ADDON_LIBS += libs/arm64-v8a/libtensorflow-lite.so

android/armeabi-v7a:
	ADDON_LIBS = libs/armeabi-v7a/libedgetpu.so
	ADDON_LIBS += libs/armeabi-v7a/libtensorflow-lite.so

ios:
	# iOS not supported for Edge TPU
	ADDON_LIBS_EXCLUDE = %

emscripten:
	# Emscripten not supported for Edge TPU
	ADDON_LIBS_EXCLUDE = %
