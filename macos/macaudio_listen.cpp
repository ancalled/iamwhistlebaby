//
// Created by Andrey Koltochnik on 1/7/17.
// https://developer.apple.com/library/content/documentation/MusicAudio/Conceptual/AudioQueueProgrammingGuide/AQRecord/RecordingAudio.html
#include <cstdio>
#include <iostream>
#include <SimpleMessageDecoder.h>
#include "AudioToolbox/AudioToolbox.h"

static const int kNumberBuffers = 3;

struct AQRecorderState {
    AudioStreamBasicDescription mDataFormat;
    AudioQueueRef mQueue;
    AudioQueueBufferRef mBuffers[kNumberBuffers];
    AudioFileID mAudioFile;
    UInt32 bufferByteSize;
    SInt64 mCurrentPacket;
    bool mIsRunning;
};


static void HandleInputBuffer(
        void *aqData,
        AudioQueueRef inAQ,
        AudioQueueBufferRef inBuffer,
        const AudioTimeStamp *inStartTime,
        UInt32 inNumPackets,
        const AudioStreamPacketDescription *inPacketDesc) {

    AQRecorderState *pAqData = (AQRecorderState *) aqData;

    if (inNumPackets == 0 && pAqData->mDataFormat.mBytesPerPacket != 0) {
        inNumPackets = inBuffer->mAudioDataByteSize / pAqData->mDataFormat.mBytesPerPacket;
    }

    if (AudioFileWritePackets(
            pAqData->mAudioFile,
            false,
            inBuffer->mAudioDataByteSize,
            inPacketDesc,
            pAqData->mCurrentPacket,
            &inNumPackets,
            inBuffer->mAudioData) == noErr) {
        pAqData->mCurrentPacket += inNumPackets;
    }

    if (pAqData->mIsRunning == 0)
        return;

    AudioQueueEnqueueBuffer(
            pAqData->mQueue,
            inBuffer,
            0,
            NULL);
}


void DeriveBufferSize(
        AudioQueueRef audioQueue,
        AudioStreamBasicDescription &ASBDescription,
        Float64 seconds,
        UInt32 *outBufferSize) {

    static const int maxBufferSize = 0x50000;

    int maxPacketSize = ASBDescription.mBytesPerPacket;
    if (maxPacketSize == 0) {
        UInt32 maxVBRPacketSize = sizeof(maxPacketSize);
        AudioQueueGetProperty(
                audioQueue,
                kAudioQueueProperty_MaximumOutputPacketSize,
                // in Mac OS X v10.5, instead use
                //   kAudioConverterPropertyMaximumOutputPacketSize
                &maxPacketSize,
                &maxVBRPacketSize
        );
    }

    Float64 numBytesForTime =
            ASBDescription.mSampleRate * maxPacketSize * seconds;
    *outBufferSize =
            UInt32(numBytesForTime < maxBufferSize ?
                   numBytesForTime : maxBufferSize);
}


OSStatus SetMagicCookieForFile(AudioQueueRef inQueue, AudioFileID inFile) {
    OSStatus result = noErr;
    UInt32 cookieSize;

    if (AudioQueueGetPropertySize(
            inQueue,
            kAudioQueueProperty_MagicCookie,
            &cookieSize) == noErr) {

        char *magicCookie = (char *) malloc(cookieSize);
        if (AudioQueueGetProperty(
                inQueue,
                kAudioQueueProperty_MagicCookie,
                magicCookie,
                &cookieSize) == noErr) {

            result = AudioFileSetProperty(
                    inFile,
                    kAudioFilePropertyMagicCookieData,
                    cookieSize,
                    magicCookie
            );
        }

        free(magicCookie);
    }
    return result;
}


int main(int argc, char *argv[]) {
    uint32_t sr = 44100;
//    uint32_t sr = 62000;
//    uint32_t size = 512;
//    MessageDecoder dec(sr, 512);

    const char *filePath = "/Users/ancalled/Documents/tmp/23/test-rec.aac";

    AQRecorderState aqData;

    aqData.mDataFormat.mFormatID = kAudioFormatLinearPCM;
    aqData.mDataFormat.mSampleRate = sr;
    aqData.mDataFormat.mChannelsPerFrame = 1;
    aqData.mDataFormat.mBitsPerChannel = 16;
    aqData.mDataFormat.mBytesPerPacket =
    aqData.mDataFormat.mBytesPerFrame =
            aqData.mDataFormat.mChannelsPerFrame * sizeof(SInt8);
    aqData.mDataFormat.mFramesPerPacket = 1;

//    AudioFileTypeID fileType = kAudioFileAIFFType;
    AudioFileTypeID fileType = kAudioFileAAC_ADTSType;
    aqData.mDataFormat.mFormatFlags =
            kLinearPCMFormatFlagIsBigEndian
            | kLinearPCMFormatFlagIsSignedInteger
            | kLinearPCMFormatFlagIsPacked;


    AudioQueueNewInput(
            &aqData.mDataFormat,
            HandleInputBuffer,
            &aqData,
            NULL,
            kCFRunLoopCommonModes,
            0,
            &aqData.mQueue);

    UInt32 dataFormatSize = sizeof(aqData.mDataFormat);

    AudioQueueGetProperty(
            aqData.mQueue,
            kAudioQueueProperty_StreamDescription,
            &aqData.mDataFormat,
            &dataFormatSize);


    CFURLRef audioFileURL =
            CFURLCreateFromFileSystemRepresentation(
                    NULL,
                    (const UInt8 *) filePath,
                    strlen(filePath),
                    false
            );

    OSStatus osStatus = AudioFileCreateWithURL(
            audioFileURL,
            fileType,
            &aqData.mDataFormat,
            kAudioFileFlags_EraseFile,
            &aqData.mAudioFile
    );



    DeriveBufferSize(
            aqData.mQueue,
            aqData.mDataFormat,
            0.5,
            &aqData.bufferByteSize);

    for (int i = 0; i < kNumberBuffers; ++i) {
        AudioQueueAllocateBuffer(
                aqData.mQueue,
                aqData.bufferByteSize,
                &aqData.mBuffers[i]);

        AudioQueueEnqueueBuffer(
                aqData.mQueue,
                aqData.mBuffers[i],
                0,
                NULL);
    }

    aqData.mCurrentPacket = 0;
    aqData.mIsRunning = true;

    AudioQueueStart(
            aqData.mQueue,
            NULL
    );

// Wait, on user interface thread, until user stops the recording

//    usleep(5000);

    for (int i = 0; i < 10; i++) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.5, false);
    }
    AudioQueueStop(
            aqData.mQueue,
            true
    );

    aqData.mIsRunning = false;

    AudioQueueDispose(
            aqData.mQueue,
            true
    );

    AudioFileClose(aqData.mAudioFile);


}


