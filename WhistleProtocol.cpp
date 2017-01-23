//
// Created by ancalled on 1/23/17.
//

#include "WhistleProtocol.h"
#include "CRC.h"


WhistleProtocol::WhistleProtocol() : base32(),
                                     decoder(44100, 385),
                                     synth(44100), listenBufferSize(1024) {

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

    uint32_t expSize = synth.expectedSize(size);
    int16_t samples[expSize];
    uint32_t gensize = synth.generate(samples, expSize, message, 10);

    play(samples, gensize); //todo process errors here
}


string WhistleProtocol::receiveMessage() {
    for (;;) {

        listen(listenBuffer, listenBufferSize);
        decoder.processFrame(listenBuffer);
        if (decoder.gotMessage()) {
            return decoder.popMessage();
        }
    }
}
