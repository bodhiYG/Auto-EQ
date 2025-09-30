import torch
import torch.nn as nn


class SimpleEQNet(nn.Module):
    """Tiny MLP that maps band energies to 3-band EQ gains in dB."""

    def __init__(self):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(3, 32),
            nn.ReLU(),
            nn.Linear(32, 16),
            nn.ReLU(),
            nn.Linear(16, 3),
        )

    def forward(self, x):
        return self.net(x)


