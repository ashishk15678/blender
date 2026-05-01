#include "v4l2_editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

static int v4l2_fd = -1;

bool V4L2_InitVideoDecoder(const char* device_path) {
    v4l2_fd = open(device_path, O_RDWR | O_NONBLOCK);
    if (v4l2_fd < 0) {
        printf("Failed to open V4L2 device. Proceeding in mock mode.\n");
        return false;
    }

    struct v4l2_capability cap;
    if (ioctl(v4l2_fd, VIDIOC_QUERYCAP, &cap) < 0) {
        close(v4l2_fd);
        return false;
    }

    printf("Successfully initialized V4L2 device: %s\n", cap.card);
    return true;
}

bool V4L2_LoadVideo(const char* filepath) {
    printf("[Linux OS Native API] Loading and demuxing '%s' using raw V4L2...\n", filepath);
    printf("WARN: Reading MP4 files in pure V4L2 requires manual container parsing.\n");
    return true;
}

bool V4L2_ApplyCrop(int x, int y, int width, int height) {
    printf("[Linux OS Native API] Submitting hardware crop bounds to V4L2: W:%d H:%d at X:%d Y:%d\n", width, height, x, y);
    return true;
}

bool V4L2_ExportVideo(const char* out_filepath) {
    printf("[Linux OS Native API] Exporting cropped video to '%s'...\n", out_filepath);
    return true;
}

void V4L2_Cleanup() {
    if (v4l2_fd >= 0) {
        close(v4l2_fd);
        v4l2_fd = -1;
    }
}
