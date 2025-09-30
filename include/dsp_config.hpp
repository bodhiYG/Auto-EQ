#pragma once

namespace autoeq {

using sample_t = float; // single-precision for realtime DSP

struct DspConfig {
    int sampleRate = 48000;
    int channels = 2;
};

} // namespace autoeq


