#include "GstSaver.h"
#include <thread>
#include <chrono>
#include <filesystem>

int main(int argc, char* argv[])
{
    runTests();
    int pattern = 0; // default pattern
    int bitrate = 2000; // default bitrate
    int frames_per_file = 1000; // default frames per file

    pattern = std::stoi(argv[1]);
    bitrate = std::stoi(argv[2]);
    frames_per_file = std::stoi(argv[3]);

    std::cout << "Starting with pattern=" << pattern 
                  << ", bitrate=" << bitrate 
                  << ", frames_per_file=" << frames_per_file << "\n" << std::endl;

    // Initializes and starts the GStreamer pipeline
    GstSaver saver(pattern, bitrate, frames_per_file);

    saver.Init(); // Init() has it's own Error Handling.

    int total_frames = 2*frames_per_file;
    for (int i = 0; i < total_frames; i++)
    {
        saver.Write();

        // Print progress every second
        if (i % 30 == 0) 
        { 
                std::cout << "Frame: " << i + 1 << "/" << total_frames << "\n" << std::endl;
        }
    }

    std::cout << "Finishing recording... \n" << std::endl;
    saver.Stop();
    std::cout << "Done! \n" << std::endl;

    return 0;
}