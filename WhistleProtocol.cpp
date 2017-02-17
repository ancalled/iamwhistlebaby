//
// Created by ancalled on 1/23/17.
//

#include "WhistleProtocol.h"
#include "CRC.h"


WhistleProtocol::WhistleProtocol(uint32_t sampleRate) : base32(),
                                                        decoder(sampleRate, 385, true),
                                                        synth(sampleRate), listenBufferSize(1024) {
    listenBuffer = new int16_t[listenBufferSize];
}

void WhistleProtocol::sendMessage(string text) {

    char *body = base32.toWhistle(text.c_str(), text.length());
    size_t size = strlen(body) + 4;
    char message[size];
    char *tail = generateCrc(body);
    strcpy(message, "hj");
    strcat(message, body);
    strcat(message, tail);

    printf("Playing %s...\n", message);

    uint32_t expSize = synth.expectedSize(size);
    int16_t samples[expSize];
    uint32_t gensize = synth.generate(samples, expSize, message, 10);

    play(samples, gensize); //todo process errors here
}


string WhistleProtocol::receiveMessage() {
    for (;;) {
        size_t read = listen(listenBuffer, listenBufferSize);
        if (read >= listenBufferSize) {
            decoder.processFrame(listenBuffer);
        } else {
//            if (decoder.gotMessage()) {
                const string &res = decoder.popMessage();
                return res;
//            } else {
//                return "";
//            }
        }

    }
}
