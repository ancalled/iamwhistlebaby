//
// Created by Andrey Koltochnik on 1/7/17.
// http://stackoverflow.com/questions/4863811/how-to-use-audioqueue-to-play-a-sound-for-mac-osx-in-c
#include <cstdio>
#include <iostream>
#include "AudioToolbox/AudioToolbox.h"
#include "Synthesizer.h"


void callback(void *ptr, AudioQueueRef queue, AudioQueueBufferRef buf_ref) {
    printf("Callback!\n");
}


int main(int argc, char *argv[]) {
    int sr = 44100;
    Synthesizer synth(sr * 2);

    std::string toEncode = "hjntdb982ilj6etj6e3l\0";
    int16_t samplesPerSoud = (int16_t) (sr * (RAMP_TIME + TOP_TIME) / 1000);
    uint32_t size = (uint32_t) (toEncode.size() * samplesPerSoud) + 1;

    AudioQueueRef queue;
    AudioStreamBasicDescription fmt = {0};
    fmt.mSampleRate = sr;
    fmt.mFormatID = kAudioFormatLinearPCM;
    fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    fmt.mFramesPerPacket = 1;
    fmt.mChannelsPerFrame = 1;
    fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2;
    fmt.mBitsPerChannel = 16;

    AudioQueueBufferRef queueBuffer;
    OSStatus status = AudioQueueNewOutput(&fmt, callback, NULL, CFRunLoopGetCurrent(),
                                          kCFRunLoopCommonModes, 0, &queue);

    if (status == kAudioFormatUnsupportedDataFormatError) {
        puts("oops!");
    }

    AudioQueueAllocateBuffer(queue, size, &queueBuffer);

    queueBuffer->mAudioDataByteSize = size;
    printf("Size: %d\n", queueBuffer->mAudioDataByteSize);

    int8_t *samp = (int8_t *) queueBuffer->mAudioData;
    uint32_t gen = synth.generate(samp, size, toEncode.c_str());
    printf("Generated %d samples.", gen);

    AudioQueueEnqueueBuffer(queue, queueBuffer, 0, NULL);
    AudioQueueSetParameter(queue, kAudioQueueParam_Volume, 1.0);
    AudioQueueStart(queue, NULL);

    CFRunLoopRunInMode(
            kCFRunLoopDefaultMode,
            5, // seconds
            false // don't return after source handled
    );

}


