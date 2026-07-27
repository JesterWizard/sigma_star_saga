[] Add for audio: https://github.com/ipatix/gba-hq-mixer/pulls / NOT possible

Blocked: ipatix/gba-hq-mixer (and the ygodm8 setup) only replace the m4a/mp2k SoundMainRAM mixer.
Sigma Star Saga uses GAX Sound Engine 3.05A (Shin'en), not m4a — confirmed in-ROM as GAX
Sound Engine 3.05A (Aug 16 2004).

There is no SoundMain / m4aSoundInit CpuSet-to-0x03000000 path to retarget the way ygodm8 does.
Wiring the HQ blob in would never be called by GAX.

[x] Add custom audio support for GAX sound driver
[x] Add custom audio export support
[] Make GAX_SPEECH playback IRQ-safe (encoder must match 0x0805708C; VOICE cues are gated no-ops)
[] Add CG export support
[] Add custom CG support
[] Fix Recker's broken portrait palette during the final battle
[] Fix the Forgotten Planet's softlock glitch
[] Remove the Forgotten Planet's ghost overlay during the countdown (it's annoying shit)
[] Add a debug scene teleporter
[] Add a debug boss fight enabler
[] Allow level ups past 100 (maybe to 255?)
[] Add custom gun data to boost defense
[] Add a debug anywhere save command
