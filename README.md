# Auto-EQ (DSP + ML)

C++ DSP core with Python ML stubs and a pybind11 bridge. Target platform: Linux (works on Windows for development via CMake).

**This repository was re-created after being mistakenly pushed to the wrong GitHub account.  
As a result, the full commit history from the original repository is not reflected here.**

## Build

- Windows PowerShell:

```powershell
./scripts/build_windows.ps1
```

- Linux/WSL:

```bash
bash scripts/build_linux.sh
```

## Example

After building the Python module, process a WAV with fixed gains:

```bash
python examples/process_wav.py input.wav output.wav --gains 6 0 3
``)

Or run ML-driven inference (heuristic fallback if no model):

```bash
python -m pip install -r ml/requirements.txt
python ml/infer.py input.wav output.wav
```

## Layout

- `src/`, `include/`: DSP code (biquad + 3-band EQ)
- `bindings/`: pybind11 module exposing `Eq3Band`
- `tests/`: Catch2-based tests
- `ml/`: feature extraction, toy model, training and inference scripts
- `examples/`: small Python example


