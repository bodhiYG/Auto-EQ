#pragma once
#include <array>
#include "dsp_config.hpp"

namespace autoeq {

// Direct Form I biquad filter
class Biquad {
public:
    enum class Type { LowShelf, HighShelf, Peaking, LowPass, HighPass, BandPass, Notch };

    Biquad();

    void setCoefficients(sample_t b0, sample_t b1, sample_t b2, sample_t a0, sample_t a1, sample_t a2);

    // Convenience designer for common EQ types
    void design(Type type, sample_t sampleRate, sample_t freq, sample_t q, sample_t gainDb);

    sample_t process(sample_t x);
    void reset();

private:
    std::array<sample_t, 6> coeffs_{}; // b0,b1,b2,a0,a1,a2
    sample_t x1_ = 0, x2_ = 0, y1_ = 0, y2_ = 0;
};

} // namespace autoeq


