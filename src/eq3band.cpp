#include "eq3band.hpp"

namespace autoeq {

Eq3Band::Eq3Band(int sampleRate, int channels)
    : sampleRate_(sampleRate), channels_(channels), filters_(channels) {
    setGainsDb(0.0f, 0.0f, 0.0f);
}

void Eq3Band::setGainsDb(float bassDb, float midDb, float trebleDb) {
    // Crossover centers: 150 Hz (bass), 1000 Hz (mid), 8000 Hz (treble)
    const float qShelf = 0.707f;
    const float qPeak = 1.0f;
    for (auto& ch : filters_) {
        ch.lowShelf.design(Biquad::Type::LowShelf, static_cast<float>(sampleRate_), 150.0f, qShelf, bassDb);
        ch.midPeak.design(Biquad::Type::Peaking, static_cast<float>(sampleRate_), 1000.0f, qPeak, midDb);
        ch.highShelf.design(Biquad::Type::HighShelf, static_cast<float>(sampleRate_), 8000.0f, qShelf, trebleDb);
    }
}

void Eq3Band::processInterleaved(float* samples, size_t numFrames) {
    for (size_t n = 0; n < numFrames; ++n) {
        for (int ch = 0; ch < channels_; ++ch) {
            float& s = samples[n * channels_ + ch];
            auto& f = filters_[ch];
            s = f.highShelf.process(f.midPeak.process(f.lowShelf.process(s)));
        }
    }
}

void Eq3Band::reset() {
    for (auto& ch : filters_) {
        ch.lowShelf.reset();
        ch.midPeak.reset();
        ch.highShelf.reset();
    }
}

} // namespace autoeq


