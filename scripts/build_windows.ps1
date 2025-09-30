param(
  [string]$BuildDir = "build",
  [switch]$Debug
)

$cfg = if ($Debug) { "Debug" } else { "Release" }
cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=$cfg -DBUILD_TESTS=ON -DBUILD_PYBIND=ON | cat
cmake --build $BuildDir --config $cfg -j | cat
ctest --test-dir $BuildDir -C $cfg --output-on-failure | cat


