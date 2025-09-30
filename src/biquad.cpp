#include <cmath>
#include "biquad.hpp"

namespace autoeq {

Biquad::Biquad() { reset(); }

void Biquad::setCoefficients(sample_t b0, sample_t b1, sample_t b2, sample_t a0, sample_t a1, sample_t a2) {
    coeffs_[0] = b0 / a0;
    coeffs_[1] = b1 / a0;
    coeffs_[2] = b2 / a0;
    coeffs_[3] = 1.0f; // normalized a0
    coeffs_[4] = a1 / a0;
    coeffs_[5] = a2 / a0;
}

static inline sample_t db_to_lin(sample_t db) { return std::pow(10.0f, db / 20.0f); }

void Biquad::design(Type type, sample_t sampleRate, sample_t freq, sample_t q, sample_t gainDb) {
    const sample_t A = db_to_lin(gainDb);
    const sample_t w0 = static_cast<sample_t>(2.0 * M_PI) * freq / sampleRate;
    const sample_t cosw0 = std::cos(w0);
    const sample_t sinw0 = std::sin(w0);
    const sample_t alpha = sinw0 / (2.0f * q);

    sample_t b0=0,b1=0,b2=0,a0=1,a1=0,a2=0;
    switch (type) {
        case Type::Peaking: {
            b0 = 1 + alpha * A;
            b1 = -2 * cosw0;
            b2 = 1 - alpha * A;
            a0 = 1 + alpha / A;
            a1 = -2 * cosw0;
            a2 = 1 - alpha / A;
            break;
        }
        case Type::LowShelf: {
            const sample_t sqrtA = std::sqrt(A);
            const sample_t twoSqrtAalpha = 2 * sqrtA * alpha;
            b0 = A * ((A + 1) - (A - 1) * cosw0 + twoSqrtAalpha);
            b1 = 2 * A * ((A - 1) - (A + 1) * cosw0);
            b2 = A * ((A + 1) - (A - 1) * cosw0 - twoSqrtAalpha);
            a0 = (A + 1) + (A - 1) * cosw0 + twoSqrtAalpha;
            a1 = -2 * ((A - 1) + (A + 1) * cosw0);
            a2 = (A + 1) + (A - 1) * cosw0 - twoSqrtAalpha;
            break;
        }
        case Type::HighShelf: {
            const sample_t sqrtA = std::sqrt(A);
            const sample_t twoSqrtAalpha = 2 * sqrtA * alpha;
            b0 = A * ((A + 1) + (A - 1) * cosw0 + twoSqrtAalpha);
            b1 = -2 * A * ((A - 1) + (A + 1) * cosw0);
            b2 = A * ((A + 1) + (A - 1) * cosw0 - twoSqrtAalpha);
            a0 = (A + 1) - (A - 1) * cosw0 + twoSqrtAalpha;
            a1 = 2 * ((A - 1) - (A + 1) * cosw0);
            a2 = (A + 1) - (A - 1) * cosw0 - twoSqrtAalpha;
            break;
        }
        case Type::LowPass: {
            const sample_t alphaLP = sinw0 / (2.0f * q);
            b0 = (1 - std::cos(w0)) / 2.0f;
            b1 = 1 - std::cos(w0);
            b2 = b0;
            a0 = 1 + alphaLP;
            a1 = -2 * cosw0;
            a2 = 1 - alphaLP;
            break;
        }
        case Type::HighPass: {
            const sample_t alphaHP = sinw0 / (2.0f * q);
            b0 = (1 + std::cos(w0)) / 2.0f;
            b1 = -(1 + std::cos(w0));
            b2 = b0;
            a0 = 1 + alphaHP;
            a1 = -2 * cosw0;
            a2 = 1 - alphaHP;
            break;
        }
        case Type::BandPass: {
            b0 = sinw0 / 2.0f;
            b1 = 0;
            b2 = -b0;
            a0 = 1 + alpha;
            a1 = -2 * cosw0;
            a2 = 1 - alpha;
            break;
        }
        case Type::Notch: {
            b0 = 1;
            b1 = -2 * cosw0;
            b2 = 1;
            a0 = 1 + alpha;
            a1 = -2 * cosw0;
            a2 = 1 - alpha;
            break;
        }
    }
    setCoefficients(b0, b1, b2, a0, a1, a2);
}

sample_t Biquad::process(sample_t x) {
    const sample_t b0 = coeffs_[0];
    const sample_t b1 = coeffs_[1];
    const sample_t b2 = coeffs_[2];
    const sample_t a1 = coeffs_[4];
    const sample_t a2 = coeffs_[5];

    const sample_t y = b0 * x + b1 * x1_ + b2 * x2_ - a1 * y1_ - a2 * y2_;

    x2_ = x1_;
    x1_ = x;
    y2_ = y1_;
    y1_ = y;
    return y;
}

void Biquad::reset() {
    x1_ = x2_ = y1_ = y2_ = 0.0f;
}

} // namespace autoeq


