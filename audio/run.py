import subprocess

import numpy as np
import matplotlib.pyplot as plt

SAMPLE_RATE=44100
C = 261.63

def pipe(functions):
    def inner(t, a):
        for f in functions:
            t, a = f(t, a)
        return a
    return inner

def play_wave(wave_form, seconds, volume, filt=(lambda t, a: (t, a))):
    samples = int(float(seconds) * float(SAMPLE_RATE))
    with open("./audio", "ab") as f:
        for t in range(samples):
            data = wave_form(t * 2*np.pi/SAMPLE_RATE)
            _, data = filt(t, data)
            f.write(
                    int(data * volume + volume).to_bytes(1, "little")
            )

def sine(freq):
    def inner(t):
        return np.sin(t * freq)
    return inner

def saw(freq):
    def inner(t):
        return (float(t) % (2*np.pi/freq))/(np.pi/freq) - 1
    return inner

def triangle(freq):
    def inner(t):
        t2 = float(t) % (2*np.pi/freq)
        if t2 <= np.pi / freq:
            return t2 * 2/(np.pi/freq) - 1
        else:
            return 3 - t2 * 2/(np.pi/freq)
    return inner

def show_wave(wave_form, points):
    plt.plot(range(points), [wave_form(t) for t in range (points)])
    plt.savefig("./sine.png")

def discrete_filter(res):
    return lambda t, a: int(a * res)/res

def dampen():
    def inner(t, a):
        return t, a / (SAMPLE_RATE - t)
    return inner


def get_note(nota):
    return np.power(np.power(2, 1.0/12.0), nota) * C

def plot_audio(points=1000):
    with open("./audio", "rb") as f:
        samples = f.read(points)
        samples = [int(i) for i in samples]
    plt.plot(range(points), samples)
    plt.show()

def add_waves(wave_forms):
    def inner(t):
        a = 0
        fs = 0
        for w, f in wave_forms:
            a += w(t)
            fs += f
        return  a / fs
    return inner

def barre_chord(wave_form, note):
    waves = []
    for i in range(1, 7):
        waves.append(
                (wave_form(get_note(note)), 1)
                #(saw(get_note(note)), i)
        )
        play_wave(
            add_waves(waves),
            1,
            128,
        )
        note = note + (4 if i % 2 else 3)

if __name__ == "__main__":
    import os
    try:
        os.remove("./audio")
    except:
        pass

    #barre_chord(saw, 3)
    play_wave(sine(get_note(9)), 2, 128)

    #plot_audio(points=2500)
    subprocess.run(["./a.out", "./audio"])
