import numpy as np
import librosa


def compute_features(wav_path: str, sr: int = 48000):
    y, sr = librosa.load(wav_path, sr=sr, mono=True)
    S = np.abs(librosa.stft(y, n_fft=2048, hop_length=512))
    spec_centroid = librosa.feature.spectral_centroid(S=S, sr=sr)
    band_edges = [(20, 200), (200, 2000), (2000, 20000)]
    freqs = librosa.fft_frequencies(sr=sr, n_fft=2048)
    band_energy = []
    for lo, hi in band_edges:
        mask = (freqs >= lo) & (freqs < hi)
        band_energy.append(S[mask].mean())
    feats = {
        "centroid_mean": float(spec_centroid.mean()),
        "band_energy": np.array(band_energy, dtype=np.float32),
    }
    return feats


