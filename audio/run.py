from math import sin
import subprocess

import matplotlib.pyplot as plt

def get_sine(freq=0.03, points=10**6):
    return {
        "x": range(points),
        "y": [sin(x*freq)+1 for x in range(points)],
    }

def show_sine(sine):
    plt.plot(sine["x"][:2000], sine["y"][:2000])
    plt.savefig("./sine.png")
    subprocess.Popen(["feh", "./sine.png"])

def play_sine(sine):
    with open("./audio", "wb") as f:
        for y in sine["y"]:
            f.write(
                int(
                    y * 120
                ).to_bytes(1, "little")
            )
    subprocess.run(["./a.out", "./audio"])

def filter_discrete(sine, resolution=4):
    for i, y in enumerate(sine["y"]):
        sine["y"][i] = int(y * resolution)/resolution


if __name__ == "__main__":
    sine = get_sine()
    filter_discrete(sine)
    show_sine(sine)
    play_sine(sine)
