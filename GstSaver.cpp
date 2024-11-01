#include "GstSaver.h"


void GstSaver::Init()
{
    gst_init(NULL, NULL); 



    // TODO - Keep working on the command line argument parsing!
    

    GError* error = nullptr;
    pipeline = gst_parse_launch(dest_file, &error);

    if(error)
    {
        std:cerr << "Error - parse pipeline: " << error->message << std::endl;
        g_error_free(error);
        gst_object_unref (pipeline);
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
    if (frameCount >= frames_per_file)
    {
        Stop();
        Init();
        frame_count = 0;
        file_count++;
    }
}


void GstSaver::Stop()
{
    if (pipeline != NULL)
    {
        gst_element_send_event(m_pipeline, gst_event_new_eos());

        GstBus *bus = gst_element_get_bus(pipeline);

        GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
            
        if (msg != NULL)
        {
            ErrHandle(bus, msg);
        }

        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = NULL;

        if (bus) gst_object_unref(bus);
    }
}

void ErrHandle(GstBus *bus, GstMessage *message)
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






