/***************************************************************
 * @file GstSaver.h
 * @brief Header file for the GstSaver class.
 *
 * This file contains the declaration of the GstSaver class, which is responsible for
 * initializing and controlling the GStreamer pipeline for video recording.
 */

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

    friend void runTests();

    GstElement* getPipeline() const { return pipeline; }

private:
    GstElement* pipeline = nullptr;
    GstElement* sink = nullptr;

    int pattern;
    int bitrate;
    int frames_per_file = 0;
    int frame_count = 0;
    int file_count = 0;

    void ErrHandle(GstBus* bus, GstMessage* message);
};

#endif // GST_SAVER_H