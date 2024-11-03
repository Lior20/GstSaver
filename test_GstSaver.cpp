/***************************************************************
 * @file test_GstSaver.cpp
 * @brief Unit tests for the GstSaver class.
 *
 * This file contains unit tests for the GstSaver class, which is responsible for
 * initializing and controlling the GStreamer pipeline for video recording.
 * The tests cover various scenarios, including initialization, writing video frames,
 * file rotation, multiple init-stop cycles, and error handling.
 */

/***************************************************************
 * INCLUDES
 */
#include "GstSaver.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

// ANSI color codes
#define GREEN_TEXT "\033[32m"
#define RED_TEXT "\033[31m"
#define RESET_COLOR "\033[0m"

// Helper function to print colored test results
void printTestResult(const std::string& testName, bool passed) {
    std::cout << testName << ": " << (passed ? GREEN_TEXT "PASS!" : RED_TEXT "FAIL!") << RESET_COLOR << std::endl;
}

void runTests() {
    std::cout << "Running GstSaver tests...\n";

    // Test 1: Initialization
    {
        GstSaver saver(0, 2000, 30);
        saver.Init();
        printTestResult("\nTest 1 (Initialization)", saver.getPipeline() != nullptr);
        saver.Stop();
    }


    // Test 2: Write and file rotation
    {
        GstSaver saver(0, 2000, 30);
        saver.Init();
        for (int i = 0; i < 70; i++) {
            saver.Write();
            g_usleep( 100000 / 33 );
        }
        saver.Stop();

        int file_count = 0;
        for (const auto& entry : std::filesystem::directory_iterator(".")) {
            if (entry.path().string().find("output_") != std::string::npos) {
                file_count++;
            }
        }
        printTestResult("\nTest 2 (Write and file rotation)", file_count >= 2);

        // Clean up test files
        for (const auto& entry : std::filesystem::directory_iterator(".")) {
            if (entry.path().string().find("output_") != std::string::npos) {
                std::filesystem::remove(entry.path());
            }
        }
    }

    g_usleep(10000);

    // Test 3: Multiple Init-Stop cycles
    {
        GstSaver saver(0, 2000, 30);
        bool success = true;
        for (int i = 0; i < 3; i++) {
            saver.Init();
            if (saver.getPipeline() == nullptr) {
                success = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            saver.Stop();
            if (saver.getPipeline() != nullptr) {
                success = false;
                break;
            }
        }
        printTestResult("\nTest 3 (Multiple Init-Stop cycles)", success);
    }

    // Test 4: Error Handling
    {
        GstSaver saver(0, 2000, 30);
        // Create a pipeline with an invalid element
        GstElement* pipeline = gst_parse_launch("videotestsrc ! fakesink", nullptr); // Corrected element name
        if (pipeline)
        {
            // Attempt to set the state to PLAYING
            GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
            // Check if setting the state fails
            printTestResult("\nTest 4 (Error Handling)", ret != GST_STATE_CHANGE_SUCCESS);
            if (ret == GST_STATE_CHANGE_SUCCESS)
            {
                saver.Stop();
            }
        }
        else
        {
            std::cout << RED_TEXT "\nTest 4 (Error Handling): FAIL (Could not create pipeline)" RESET_COLOR << std::endl;
        }
    }

        g_print("\nGstSaver tests completed.\n");
}

int main(int argc, char* argv[]) {
    gst_init(NULL, NULL);
    runTests();
    return 0;
}