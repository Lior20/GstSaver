# GStreamer Video Capture and File Rotation

## Overview
This project implements a C++ class `GstSaver` for video capture using GStreamer, that initializes and manages a GStreamer video pipeline to capture video from a test video source, 
encode it, and save it to disk with file rotation based on frame count.

## Prerequisites
- GStreamer 1.0 or later
- C++11 compatible compiler
- CMake 3.10 or later

## Installation (Windows)

### 1. Install GStreamer
1. Download and install GStreamer:
   - Go to the [GStreamer download page](https://gstreamer.freedesktop.org/download/).
   - Download both the Runtime and Development installers for MSVC.
   - Run both installers, selecting "Add GStreamer to PATH" when prompted.

2. Add GStreamer bin directory to PATH
   - typically `C:\gstreamer\1.0\msvc_x86_64\bin` for 64-bit systems.

3. Verify the installation:
   - Open a Command Prompt or PowerShell window.
   - Run `gst-launch-1.0.exe --version`.

### 2. Build the Project
1. Clone the repository:
   - git clone https://github.com/yourusername/gstreamer-video-capture.git
   - cd gstreamer-video-capture

2. Create a build directory and run CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
Run the program with the following command-line arguments:

```bash
# Run with default settings
./video_capture

# Customize parameters
./video_capture [pattern] [bitrate] [frames_per_file]
```

### Parameters
- `pattern`: Integer pattern type for the videotestsrc. (0-22)
- `bitrate`: x264 encoder bitrate (in kbps).
- `frames_per_file`: Number of frames to save in each file before rotating to a new file.

# Example
```bash
./video_capture 0 2000 300
```
This will start capturing video using pattern 0, with a bitrate of 2000 kbps, and create a new file every 300 frames.

## Key Features
- Dynamic GStreamer pipeline generation
- Automatic file rotation
- Configurable video test source patterns
- Bitrate and frame count customization

## Design Decisions

1. **GStreamer Pipeline**: We use `gst_parse_launch` to create the pipeline programmatically, allowing for flexible configuration of video parameters.

3. **Error Handling**: Comprehensive error checking is implemented throughout the code to ensure robust operation.

4. **Memory Management**: RAII principles are followed to ensure proper resource cleanup.

## Future Improvements

1. Implement more flexible video source options (e.g., camera input).
2. Add support for different video codecs and container formats.
3. Implement a GUI for easier parameter configuration.
4. Add more comprehensive logging and error reporting.

## Troubleshooting
- Ensure GStreamer is correctly installed
- Check PATH environment variable
- Verify GStreamer runtime is available
