#!/bin/bash
cat  pulse_filt.raw | padsp tee /dev/audio
#sox soundfile.wav -t raw -r 8012 -u -b -c 1 | cat > /dev/audio
#play pulse_filt.wav -r 44100

# example sound file
sox -n -r 48k -b 16 test16.wav synth 2 sine 200 gain -9 fade t 0 0 0.1

# redundant piping
sox test16.wav -t wav - | sox -t wav - gain 8 -t wav - | play -

