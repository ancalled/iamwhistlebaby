//
// Created by ancalled on 11/24/16.
//

#ifndef AUDIOPROTOCOL_SEGMENT_H
#define AUDIOPROTOCOL_SEGMENT_H


class Segment {
public:
    struct Candidate {
        int tau;
        float freq;
        int count;
        float weight;
    };

    void addFreq(int tau, float freq);

private:
    Candidate* candidates;

};


#endif //AUDIOPROTOCOL_SEGMENT_H
