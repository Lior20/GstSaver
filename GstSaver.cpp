#include "GstSaver.h"

GstSaver::GstSaver(int pattern, int bitrate, int frames_per_file)
    : pattern(pattern), bitrate(bitrate), frames_per_file(frames_per_file),
      pipeline(nullptr), frame_count(0), file_count(0) {}

GstSaver::~GstSaver()
{
    Stop();
}

void GstSaver::Init()
{
    gst_init(NULL, NULL); 

    // Construct pipeline string dynamically
    std::ostringstream pipeline_des;
    pipeline_des << "videotestsrc pattern=" << pattern << " num-buffers = " << frames_per_file
                  << " ! video/x-raw,width=1280,height=720,framerate=30/1"
                  << " ! x264enc bitrate=" << bitrate
                  << " ! mp4mux ! multifilesink name=sink location=output_" << file_count << ".mp4";
    
    std::cout << "Pipeline: " << pipeline_des.str() << "\n" << std::endl;  // Debug print

    GError* error = nullptr;
    pipeline = gst_parse_launch(pipeline_des.str().c_str(), &error);

    if(error)
    {
        std::cerr << "Error - parse pipeline: " << error->message << std::endl;
        g_error_free(error);
        if (pipeline)
        {
            gst_object_unref(pipeline);
            pipeline = NULL;
        }
    }

    // Get the multifilesink element to update its properties
    sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (!sink) {
        std::cerr << "Could not retrieve multifilesink element \n" << std::endl;
        return;
    }
    else
    {
        g_object_set(G_OBJECT(sink), "next-file", 2, NULL);
    }

    GstStateChangeReturn ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) 
    {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
    }
}

void GstSaver::Write()
{
    frame_count++;
    if (frame_count >= frames_per_file)
    {
        std::cout << "Rotating file... \n" << std::endl;  // Debug print
        Stop();
        frame_count = 0;
        file_count++;
        Init();
    }

    g_usleep(1000000 / 30); // ~30 fps
}

void GstSaver::Stop()
{
    if (pipeline != NULL)
    {
        std::cout << "Stopping pipeline... \n" << std::endl;  // Debug print

        GstBus *bus = gst_element_get_bus(pipeline);

        if (!gst_element_send_event(pipeline, gst_event_new_eos()))
        {
            std::cerr << "Failed to send EOS event \n" << std::endl;
        }

        
        GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
            
        if (msg != NULL)
        {
            ErrHandle(bus, msg);
        }

        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = NULL;

        if (sink) gst_object_unref(sink);
        sink = NULL;

        if (bus) gst_object_unref(bus);

        std::cout << "Pipeline stopped \n" << std::endl;  // Debug print
    }
}

void GstSaver::ErrHandle(GstBus *bus, GstMessage *message)
{
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE(message))
    {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(message, &err, &debug_info);
            g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (message->src), err->message);
            g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error (&err);
            g_free (debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print ("End-of-stream\n");
            break;
        default:
            g_printerr ("Unknown error.\n");
            break;
    }
    gst_message_unref(message);
}