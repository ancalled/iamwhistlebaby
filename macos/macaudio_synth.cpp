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
//    uint32_t sr = 44100;
    uint32_t sr = 62000;
    Synthesizer synth(sr);

    std::string toEncode = "hjntdb982ilj6etj6e3l\0";
    int16_t samplesPerSoud = (int16_t) (sr * (RAMP_TIME + TOP_TIME) / 1000);
    uint32_t size = (uint32_t) (2 * toEncode.size() * samplesPerSoud) + 1;
    printf("Size: %d\n", size);

    AudioStreamBasicDescription fmt = {0};
    fmt.mSampleRate = sr;
    fmt.mFormatID = kAudioFormatLinearPCM;
    fmt.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    fmt.mFramesPerPacket = 1;
    fmt.mChannelsPerFrame = 1;
    fmt.mBytesPerPacket = fmt.mBytesPerFrame = 2;
    fmt.mBitsPerChannel = 16;

    AudioQueueRef queue;
    OSStatus status = AudioQueueNewOutput(&fmt, callback, NULL, CFRunLoopGetCurrent(),
                                          kCFRunLoopCommonModes, 0, &queue);
    if (status == kAudioFormatUnsupportedDataFormatError) {
        puts("oops!");
    }

    AudioQueueBufferRef queueBuffer;
    AudioQueueAllocateBuffer(queue, size, &queueBuffer);
    queueBuffer->mAudioDataByteSize = size;
    int16_t *samp = (int16_t *) queueBuffer->mAudioData;

    uint32_t gen = synth.generate(samp, size, toEncode.c_str());
    printf("Generated %d samples.", gen);

    AudioQueueEnqueueBuffer(queue, queueBuffer, 0, NULL);
    AudioQueueSetParameter(queue, kAudioQueueParam_Volume, 20.0);
    AudioQueueStart(queue, NULL);

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 5, false);

}


