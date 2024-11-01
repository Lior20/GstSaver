#include <iostream>
#include "GstSaver.h"

int main(int argc, char *argv[])
{
    int pattern = 0; // default pattern
    int bitrate = 2000; // default bitrate
    int frames_per_file = 1000; // default frames per file

    pattern = std::stoi(argv[1]);
    bitrate = std::stoi(argv[2]);
    frames_per_file = std::stoi(argv[3]);

    // Initializes and starts the GStreamer pipeline
    GstSaver saver(pattern, bitrate, frames_per_file);

    saver.Init(); // Init() has it's own Error Handling.


    for ( int i = 0; i < frames_per_file; i++ )
    {
        saver.Write();
        g_usleep(100000 / 30); // ~30 fps
    }

    saver.Stop();

    return 0;
}