#ifndef GST_SAVER_H
#define GST_SAVER_H

/***************************************************************
 * INCLUDES
 */
#include <iostream>
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
    GstElement *pipeline = nullptr;
    GstElement *source = nullptr;
    GstElement *sink = nullptr;
    GstElement *encoder = nullptr;
    GstElement *muxer = nullptr;

    GstBus *bus = nullptr;
    GstMessage *msg = nullptr;
    GstStateChangeReturn ret;

    int frame_count = 0;
    int file_count = 0;
    gchar *dest_file;
}

#endif // GST_SAVER_H