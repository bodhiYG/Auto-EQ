#pragma once
#include <vector>
#include "biquad.hpp"

namespace autoeq {

// Simple 3-band EQ using shelving + peaking filters per channel
class Eq3Band {
public:
    Eq3Band(int sampleRate, int channels);

    void setGainsDb(float bassDb, float midDb, float trebleDb);
    void processInterleaved(float* samples, size_t numFrames);
    void reset();

private:
    int sampleRate_;
    int channels_;
    struct ChannelFilters {
        Biquad lowShelf;
        Biquad midPeak;
        Biquad highShelf;
    };
    std::vector<ChannelFilters> filters_;
};

} // namespace autoeq


