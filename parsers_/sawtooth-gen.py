
import numpy as np

# Set frequency to 512 Hz for one period in 512 samples
frequency = 1  # Frequency of the sine wave in Hz
sampling_rate = 2048   # Number of samples



# Time array for one period
t = np.arange(-1.0, 1.0, 2.0/sampling_rate)  

# Sine wave calculation
saw_wave = t -  2*np.floor(1/2+t/2) 

# Scale to the range of -128 to 127
scaled_wave = (saw_wave * 127).astype(int)

# Print the samples
for sample in scaled_wave:
    print(sample, ',')
