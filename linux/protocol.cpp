//
// Created by ancalled on 2/3/17.
//

#include "PAWhistleProtocol.h"

int main(int argc, char *argv[]) {
    PAWhistleProtocol protocol(44100);
    protocol.sendMessage("41241");
}