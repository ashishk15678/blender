#ifndef V4L2_EDITOR_H
#define V4L2_EDITOR_H

#include <stdbool.h>

// Initializes the Linux V4L2 memory-to-memory hardware decoding device
bool V4L2_InitVideoDecoder(const char* device_path);

// Loads an MP4 video file and begins demuxing it to feed into V4L2
// (Note: V4L2 cannot demux MP4 containers natively, so this step typically requires a container parser like libavformat)
bool V4L2_LoadVideo(const char* filepath);

// Processes a crop operation via V4L2's Selection API (VIDIOC_S_SELECTION)
// Hardware-accelerated cropping on Linux
bool V4L2_ApplyCrop(int x, int y, int width, int height);

// Flushes the encoded output buffers to a new file
bool V4L2_ExportVideo(const char* out_filepath);

// Shuts down V4L2 device nodes
void V4L2_Cleanup();

#endif
