# GStreamer Video Capture

## 🚀 Overview
This project implements a C++ class `GstSaver` for video capture using GStreamer, that initializes and manages a GStreamer video pipeline to capture video from a test video source, 
encode it, and save it to disk with file rotation based on frame count.

## 📚 Table of Contents
- [Prerequisites](#%EF%B8%8F-prerequisites)
- [Installation](#%EF%B8%8F-installation-windows)
- [Usage](#-usage)
- [Key Features](#-key-features)
- [Design Decisions](#-design-decisions)
- [Future Improvements](#-future-improvements)
- [Troubleshooting](#-troubleshooting)

## ⚙️ Prerequisites
- GStreamer 1.0 or later
- C++11 compatible compiler
- CMake 3.10 or later

## 🛠️ Installation (Windows)

### 1. Install GStreamer
1. Download and install GStreamer:
   - Go to the [GStreamer download page](https://gstreamer.freedesktop.org/download/).
   - Download both the Runtime and Development installers for MSVC.
   - Run both installers, selecting "Add GStreamer to PATH" when prompted.

2. Add GStreamer bin directory to PATH

   If not added automatically, manually add GStreamer’s bin directory to your
   PATH:
   - Open the Start menu, search for "Environment Variables," and open it.
   - Under "System variables," find Path, click Edit, and add the path to
   - GStreamer’s bin directory (typically C:\gstreamer\1.0\msvc_x86_64\bin for 64-bit systems).

4. Verify the installation:
   - Open a Command Prompt or PowerShell window.
   - Run `gst-launch-1.0.exe --version`.

### 2. Install CMake
   If you don't have CMake installed, download and install it from the [official CMake website](https://cmake.org/download/).
   
### 3. Build the Project
1. Clone the repository:
   - git clone https://github.com/Lior20/ThirdEye.git

2. Create a build directory and run CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Note: To delete the executable files created you can use:
```bash
cmake --build . --target clean
```

## 📦 Usage
To execute the program, please navigate to the directory containing the executable file and run the program with the following command-line arguments:

```bash
# Run with default settings
./GstVideoCapture

# Customize parameters
./GstVideoCapture [pattern] [bitrate] [frames_per_file]
```

### Parameters
- `pattern`: Integer pattern type for the videotestsrc. (0-22)
- `bitrate`: x264 encoder bitrate (in kbps).
- `frames_per_file`: Number of frames to save in each file before rotating to a new file.

Video Test Source Patterns (Starting from 0):

<img src="https://i.gyazo.com/283ca2d5aefe414829fba96426768a10.gif" alt="Video Test Source Patterns" width="400"/>

# Example
```bash
./GstVideoCapture 0 2000 300
```
This will start capturing video using pattern 0, with a bitrate of 2000 kbps, and create a new file every 300 frames.

## 🔑 Key Features
- Dynamic GStreamer pipeline generation
- Automatic file rotation
- Configurable video test source patterns
- Bitrate and frame count customization

## 🧠 Design Decisions

1. **GStreamer Pipeline**: We use `gst_parse_launch` to create the pipeline programmatically, allowing for flexible configuration of video parameters.

3. **Error Handling**: Comprehensive error checking is implemented throughout the code to ensure robust operation.

4. **Memory Management**: RAII principles are followed to ensure proper resource cleanup.

## 🚀 Future Improvements

1. Implement more flexible video source options (e.g., camera input).
2. Add support for different video codecs and container formats.
3. Implement a GUI for easier parameter configuration.
4. Add more comprehensive logging and error reporting.

## 🐞 Troubleshooting
- Ensure GStreamer is correctly installed
- Check PATH environment variable
- Verify GStreamer runtime is available
