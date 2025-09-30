#include <catch2/catch_all.hpp>
#include "biquad.hpp"

using namespace autoeq;

TEST_CASE("Biquad unity with 0dB peaking", "[biquad]") {
    Biquad biq;
    biq.design(Biquad::Type::Peaking, 48000.0f, 1000.0f, 1.0f, 0.0f);
    float y = 0.0f;
    for (int i = 0; i < 1000; ++i) {
        y = biq.process(i == 0 ? 1.0f : 0.0f);
    }
    REQUIRE(std::abs(y) < 1.0f); // impulse response should settle
}


