import argparse
import torch
from torch.utils.data import TensorDataset, DataLoader
import numpy as np
from model import SimpleEQNet


def heuristic_labels(band_energy: np.ndarray) -> np.ndarray:
    # Center energies then scale to +/-6 dB suggestions
    centered = band_energy - band_energy.mean()
    gains = 6.0 * centered / (np.abs(centered).max() + 1e-6)
    return gains.astype(np.float32)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("features_npz", help="npz with band_energy arrays")
    parser.add_argument("--epochs", type=int, default=50)
    parser.add_argument("--lr", type=float, default=1e-3)
    parser.add_argument("--out", default="model.pt")
    args = parser.parse_args()

    data = np.load(args.features_npz)
    X = data["band_energy"].astype(np.float32)
    y = np.stack([heuristic_labels(x) for x in X])

    ds = TensorDataset(torch.from_numpy(X), torch.from_numpy(y))
    dl = DataLoader(ds, batch_size=32, shuffle=True)

    model = SimpleEQNet()
    opt = torch.optim.Adam(model.parameters(), lr=args.lr)
    loss_fn = torch.nn.MSELoss()

    for _ in range(args.epochs):
        for xb, yb in dl:
            pred = model(xb)
            loss = loss_fn(pred, yb)
            opt.zero_grad()
            loss.backward()
            opt.step()

    torch.save(model.state_dict(), args.out)
    print(f"Saved {args.out}")


if __name__ == "__main__":
    main()


