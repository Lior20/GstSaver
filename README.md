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

### Test File
A test file named `test_GstSaver.cpp` has been created to implement unit tests for the GstSaver class, covering various scenarios such as initialization, writing video frames, file rotation, and error handling.

### Sample videos
Two sample output videos have been generated during testing and found in this repo
* 'sample_output_0_2000_300' - pattern = 0, bitrate = 2000, frames_per_file = 300.
* 'sample_output_22_5000_600' - pattern = 22, bitrate = 5000, frames_per_file = 600.

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



# Dropbear SSH Server with UDP Listener
## User Guide

---

## Table of Contents

1. [Overview](#overview)
2. [Requirements](#requirements)
3. [Installation](#installation)
4. [Running Dropbear with UDP Listener](#running-dropbear-with-udp-listener)
5. [Sending UDP Packets](#sending-udp-packets)
6. [Packet Structure](#packet-structure)
7. [Troubleshooting](#troubleshooting)
8. [Security Considerations](#security-considerations)
9. [Additional Information](#additional-information)

---

## Overview

This modified version of Dropbear SSH server (version 2025.87) includes a new feature that:

* Listens for UDP packets on port 53
* Processes specially formatted packets to dynamically start TCP listeners on specified ports
* Executes shell commands with dropped privileges

---

## Requirements

* Linux system with build tools (gcc, make)
* Python 3 (for running the example script)
* Root privileges (for binding to port 53)

---

## Installation

### Step 1: Download Dropbear

```
wget https://matt.ucc.asn.au/dropbear/dropbear-2025.87.tar.gz
tar -xzf dropbear-2025.87.tar.gz
cd dropbear-2025.87
```

### Step 2: Apply the Patch

```
patch -p1 < patch.diff
```

### Step 3: Configure and Build

```
./configure
make
```

---

## Running Dropbear with UDP Listener

### Start the Server

The UDP listener feature is enabled with the `-U` command-line switch. Since the listener binds to port 53 (a privileged port), you must run Dropbear with root privileges:

```
sudo ./dropbear -U -F -E -R
```

**Parameter explanation:**
* `-U`: Enable the UDP listener on port 53
* `-F`: Run in foreground (don't fork to background)
* `-E`: Log to standard error rather than syslog
* `-R`: Generate host keys automatically if they don't exist

You should see output similar to:
```
[PID] Apr XX XX:XX:XX UDP listener initialized on port 53
[PID] Apr XX XX:XX:XX UDP listener active and waiting for packets on port 53
```

### Verifying Active Listeners

To verify that Dropbear is listening on the correct ports (both the default SSH port 22 and UDP port 53), you can use:

```
netstat -tulpn | grep dropbear
```

This should show both the TCP listener on port 22 and the UDP listener on port 53. After sending a UDP packet that requests a new port, you should also see an additional TCP listener on the requested port.

---

## Sending UDP Packets

Use the included Python script to send properly formatted UDP packets to the server:

### Running the Example Script

```
python3 example_packet.py
```

The default script sends a packet that:
* Contains the magic number 0xDEADBEEF
* Requests a new TCP listener on port 4444
* Includes a simple command "New UDP packet"

### Customizing the Script

You can modify the following constants in the script to customize behavior:
* `TARGET_HOST`: Change to the address of your Dropbear server
* `TCP_PORT`: Change to request a different TCP port for the server to listen on
* `COMMAND`: Change to a different shell command to execute

Example modification:
```python
TARGET_HOST = "192.168.1.100"  # Change to your server's IP
TCP_PORT = 2222               # Request listening on port 2222
COMMAND = "echo 'Hello from UDP' > /tmp/udp-test.log"  # Custom command
```

---

## Packet Structure

The UDP packets follow this structure:
* `magic`: 32-bit integer, must be 0xDEADBEEF in network byte order
* `port_number`: 16-bit integer, specifies the TCP port to listen on
* `shell_command`: 256-byte null-terminated string, command to execute

```c
typedef struct {
    uint32_t magic;         /* should be 0xDEADBEEF */
    uint16_t port_number;
    char shell_command[256];
} listen_packet_t;
```

---

## Troubleshooting

### Permission Denied

If you see:
```
Failed to bind UDP socket to port 53: Permission denied
```

Ensure you're running Dropbear with sudo or as root. Port 53 is a privileged port.

### Connection Issues

If you have problems connecting to the dynamically created SSH ports, ensure:
1. Your firewall allows connections to the specified port
2. No other service is using the requested port

### Host Key Issues

If you see host key warnings when connecting to dynamic ports, use the `-R` flag when starting Dropbear to automatically generate the necessary keys.

---

## Security Considerations

* The UDP listener binds to port 53 (DNS), which may conflict with DNS services
* Commands are executed with dropped privileges (as 'nobody' user)
* There is no authentication for UDP packets - anyone who can send packets to port 53 can trigger actions

---

## Additional Information

For more information about Dropbear's standard features and settings, see the official documentation at: https://matt.ucc.asn.au/dropbear/dropbear.html
