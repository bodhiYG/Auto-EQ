import argparse
import numpy as np
import soundfile as sf
import torch
from model import SimpleEQNet
from features import compute_features
import autoeq_py  # built module


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_wav")
    parser.add_argument("output_wav")
    parser.add_argument("--sr", type=int, default=48000)
    args = parser.parse_args()

    feats = compute_features(args.input_wav, sr=args.sr)
    band = feats["band_energy"].astype(np.float32)

    # Model: if model.pt exists, load it; else use heuristic mapping
    gains_db = None
    try:
        model = SimpleEQNet()
        model.load_state_dict(torch.load("model.pt", map_location="cpu"))
        model.eval()
        with torch.no_grad():
            gains_db = model(torch.from_numpy(band).unsqueeze(0)).squeeze(0).numpy()
    except Exception:
        centered = band - band.mean()
        gains_db = 6.0 * centered / (np.abs(centered).max() + 1e-6)

    audio, sr = sf.read(args.input_wav)
    if audio.ndim == 1:
        audio = np.stack([audio, audio], axis=1)
    frames = audio.shape[0]

    eq = autoeq_py.Eq3Band(sr, audio.shape[1])
    eq.set_gains_db(float(gains_db[0]), float(gains_db[1]), float(gains_db[2]))
    eq.process_interleaved(audio)

    sf.write(args.output_wav, audio, sr)
    print("Wrote", args.output_wav)


if __name__ == "__main__":
    main()


