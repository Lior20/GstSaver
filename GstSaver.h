#ifndef GST_SAVER_H
#define GST_SAVER_H

/***************************************************************
 * INCLUDES
 */
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <gst/gst.h>

class GstSaver
{
public:
    GstSaver(int pattern, int bitrate, int frames_per_file);
    ~GstSaver();

    void Init();
    void Write();
    void Stop();

private:
    GstElement* pipeline = nullptr;
    GstElement* source = nullptr;
    GstElement* sink = nullptr;
    GstElement* encoder = nullptr;
    GstElement* muxer = nullptr;

    int pattern;
    int bitrate;
    int frames_per_file;
    int frame_count = 0;
    int file_count = 0;

    void ErrHandle(GstBus* bus, GstMessage* message);
};

#endif // GST_SAVER_H