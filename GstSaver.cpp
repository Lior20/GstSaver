/***************************************************************
 * @file GstSaver.cpp
 * @brief Implementation file for the GstSaver class.
 *
 * This file contains the implementation of the GstSaver class, which is responsible for
 * initializing and controlling the GStreamer pipeline for video recording.
 * The class provides methods for initializing the pipeline, writing video frames to files,
 * and stopping the pipeline. It also handles errors and end-of-stream messages.
 */

/***************************************************************
 * INCLUDES
 */
#include "GstSaver.h"

/*****************************************************************************************************************
 * @brief Constructor for the GstSaver class.
 *
 * Initializes the GstSaver object with the provided pattern, bitrate, and frames per file.
 * The pipeline, frame count, and file count are also initialized to default values.
 *
 * @param pattern The video pattern to be used for the video source.
 * @param bitrate The bitrate for the encoded video in kilobits per second.
 * @param frames_per_file The number of frames to be captured and saved in each output file.
 */
GstSaver::GstSaver(int pattern, int bitrate, int frames_per_file)
    : pattern(pattern), bitrate(bitrate), frames_per_file(frames_per_file),
      pipeline(nullptr), frame_count(0), file_count(0) {}

/*****************************************************************************************************************
 * @brief Destructor for the GstSaver class.
 *
 * The destructor stops the video capture pipeline and releases any allocated resources.
 * It calls the Stop() function to ensure proper cleanup before the object is destroyed.
 */
GstSaver::~GstSaver()
{
    Stop();
}

/*****************************************************************************************************************
 * @brief Initializes the GStreamer pipeline for video capture and encoding.
 *
 * This function constructs a GStreamer pipeline dynamically based on the provided pattern, bitrate, and frames per file.
 * It initializes the pipeline, frame count, and file count, and sets the multifilesink element to rotate output files.
 * The function also handles errors during pipeline initialization.
 *
 * @param pattern The video pattern to be used for the video source.
 * @param bitrate The bitrate for the encoded video in kilobits per second.
 * @param frames_per_file The number of frames to be captured and saved in each output file.
 *
 * @return void
 */
void GstSaver::Init()
{
    gst_init(NULL, NULL); 

    // Construct pipeline string dynamically
    std::ostringstream pipeline_des;
    pipeline_des << "videotestsrc pattern=" << pattern << " num-buffers=" << frames_per_file
                  << " ! video/x-raw,width=1280,height=720,framerate=30/1"
                  << " ! x264enc bitrate=" << bitrate
                  << " ! mp4mux ! filesink location=output_" << file_count << ".mp4";

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

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_READY);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the ready state.\n");
        gst_object_unref(pipeline);
        pipeline = NULL;
        return;
    }

    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
    }
}

/*****************************************************************************************************************
 * @brief Writes a frame to the video file.
 *
 * This function increments the frame count and checks if the maximum number of frames per file has been reached.
 * If the maximum number of frames is reached, it stops the current pipeline, rotates the output file, resets the frame count,
 * and increments the file count. It then initializes a new pipeline for video capture and encoding.
 *
 * @return void
 */
void GstSaver::Write()
{
    frame_count++;
    if (frame_count >= frames_per_file)
    {
        Stop();
        g_print("Rotating file... \n\n");  // Debug print
        frame_count = 0;
        file_count++;
        Init();
    }

    g_usleep(1000000 / 30); // ~30 fps
}

/*****************************************************************************************************************
 * @brief Stops the video capture pipeline and releases any allocated resources.
 *
 * This function checks if the pipeline is not NULL. If the pipeline is not NULL, it prints a debug message,
 * retrieves the pipeline's bus, sends an End-of-Stream (EOS) event, waits for an error or EOS message,
 * handles the message, sets the pipeline's state to paused and then to null, releases the pipeline,
 * sink, and bus objects, and prints another debug message.
 *
 * @return void
 */
void GstSaver::Stop()
{
    if (pipeline != NULL)
    {
        g_print("\nStopping pipeline due to: ");  // Debug print

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

        GstStateChangeReturn ret = gst_element_get_state(pipeline, NULL, NULL, GST_SECOND);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            g_printerr("Failed to stop pipeline\n");
        }

        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = NULL;

        if (sink) gst_object_unref(sink);
        sink = NULL;

        if (bus) gst_object_unref(bus);

        g_print("Pipeline stopped\n");  // Debug print
    }
}

/*****************************************************************************************************************
 * @brief Handles errors and end-of-stream messages from the GStreamer pipeline.
 *
 * This function is called when an error or end-of-stream message is received from the GStreamer pipeline.
 * It parses the message to extract relevant information and prints it to the standard error stream.
 *
 * @param bus The GStreamer bus object that received the message.
 * @param message The GStreamer message to be handled.
 *
 * @return void
 */
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
            g_print ("End-of-stream.\n");
            break;
        default:
            g_printerr ("Unknown error.\n");
            break;
    }
    gst_message_unref(message);
}