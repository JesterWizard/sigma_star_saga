[] Add for audio: https://github.com/ipatix/gba-hq-mixer/pulls / NOT possible

Blocked: ipatix/gba-hq-mixer (and the ygodm8 setup) only replace the m4a/mp2k SoundMainRAM mixer.
Sigma Star Saga uses GAX Sound Engine 3.05A (Shin'en), not m4a — confirmed in-ROM as GAX
Sound Engine 3.05A (Aug 16 2004).

There is no SoundMain / m4aSoundInit CpuSet-to-0x03000000 path to retarget the way ygodm8 does.
Wiring the HQ blob in would never be called by GAX.

[x] Add custom audio support for GAX sound driver
[x] Add custom audio export support
[x] Add CG export support
[] Add custom CG support
[] Fix Recker's broken portrait palette during the final battle
[] Fix the Forgotten Planet's softlock glitch
[] Remove the Forgotten Planet's ghost overlay during the countdown (it's annoying shit)
[x] Add a debug scene teleporter
[x] Add a debug boss fight enabler
[] Confirm the debug menu's provisional midboss planet labels in-game
[x] Allow level ups past 100 (maybe to 255?)
[] Add custom gun data to boost defense
[x] Add a debug anywhere save command
[x] Disable START button debug menu when talking
[] Add debug menu for choosing which ship to pilot
[] Fix the bug that allows ships that are too big for a stage to spawn
[] Display objectives on the map with a red exclamation
[] Introduce a currency system for buying gun data/equippables for your parasite?
[] Display your health as a number and have it increase as you level up?

## Impact Data:
[x] 29. Suction - Pull in EXP drops from afar
[] 30. Training Weights - Cut your ATK power in half for 50% more EXP gain
[x] 31. Phoenix - One revival to full health on death per map

## Bullet Data:
[x] 4. Rework charge shot to build up charge and fire independently of the ship
[x] 21. Laser - A solid beam that shoots through enemies
[] 22. Virus - Shots corrode enemies dealing 10% damage per second

## Cannon Data:
[x] 29. Auto Target - Shots auto target the nearest enemies