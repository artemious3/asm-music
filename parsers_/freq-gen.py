import numpy as np

# Set frequency to 512 Hz for one period in 512 samples
frequency = 1  # Frequency of the sine wave in Hz
sampling_rate = 2048   # Number of samples



# Time array for one period
t = np.arange(0.0, 1.0, 1.0/sampling_rate)  

# Sine wave calculation
sine_wave = np.sin(2 * np.pi * frequency * t)

# Scale to the range of -128 to 127
scaled_wave = np.clip((sine_wave * 127.5).astype(int), -128, 127)

# Print the samples
for sample in scaled_wave:
    print(sample, ',')
