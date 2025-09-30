import argparse
import numpy as np
import soundfile as sf
import autoeq_py


def main():
    p = argparse.ArgumentParser()
    p.add_argument("input_wav")
    p.add_argument("output_wav")
    p.add_argument("--gains", type=float, nargs=3, default=[3.0, 0.0, 3.0], help="bass mid treble dB")
    args = p.parse_args()

    audio, sr = sf.read(args.input_wav)
    if audio.ndim == 1:
        audio = np.stack([audio, audio], axis=1)

    eq = autoeq_py.Eq3Band(sr, audio.shape[1])
    eq.set_gains_db(*args.gains)
    eq.process_interleaved(audio)
    sf.write(args.output_wav, audio, sr)
    print("Wrote", args.output_wav)


if __name__ == "__main__":
    main()


