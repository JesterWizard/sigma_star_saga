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
[] 32. Hitchhiker — First hit on an enemy plants a spore; when it dies, your next volley spawns from its corpse. Turns kills into repositioning — great on big stages.
[] 33. Kindling — Staying on one target makes your shots grow (size + damage); switching targets resets. Rewards focus without being a boring ATK buff.
[] 34. Grudge — The last enemy type that hit you takes bonus damage until you leave the stage. Personal vendetta mechanic, not generic revenge.
[] 35. Second Stomach — Swallow up to 3 nearby EXP gems without collecting them; level-up releases them for a burst heal. EXP economy as a resource, not just pickup.
[] 36. Dream Debt — Overkill damage while at full HP goes into a shield bank; the next hit eats the bank first. Defense through aggression, not a flat barrier.
[] 37. Molt — Every ~30s, shed a clone that fires backward for 5s. Periodic “phase shift” without invincibility.
[] 38. Tag Sale — Marked enemies drop 2× gems but take less damage — you choose who to mark by first hit. Risk/reward targeting puzzle.
[] 39. Bad Review — Each hit on a boss slightly slows its bullet patterns (caps low). Boss-specific debuff that feels sci-fi bureaucratic.
[] 40. Tax Bracket — More unique enemy types killed this stage = higher EXP multiplier. Encourages fighting everything, not farming one spawn.
[] 41. Jury Rig — At stage start, temporarily graft a random vanilla Impact effect; swaps every 60s. Roguelike variance inside one equip slot.
[] 42. Panic Sweat — Fire rate rises as enemies get closer to your ship. Inverts the usual “keep distance” loop.
[] 43. Foreclosure — Borrow +50% ATK for the stage; if you die once, you lose 10% max HP permanently until next save. High-stakes “loan shark” parasite.

## Bullet Data:
[x] 4. Rework charge shot to build up charge and fire independently of the ship
[x] 21. Laser - A solid beam that shoots through enemies
[] 22. Virus - Corrosion DoT dealing 10% damage per second and spreading to nearby enemies on kill
[] 23. Witness — Bullet does nothing until 3 other shots pass near the same enemy, then it detonates. Synergy: Scatter / Sprinkler cannons.
[] 24. Memory Foam — Shots get slower and stronger the farther they travel. Synergy: Rear Guard, long stages.
[] 25. Ghostwrite — Leaves a burning “stain” on the playfield that damages anything crossing it. Synergy: Sweepers, Rotating Cannon.
[] 26. Unsubscribe — Doesn’t destroy enemy bullets — reflects them back at the shooter. Defense without a shield impact.
[] 27. Dud — Every 5th shot is a fake (no damage) but still triggers Impact effects. Tricks Fork, Bubble, Smoulder into firing for free.
[] 28. Alphabet Soup — Shots cycle R→G→B; landing all three on one enemy triggers a small burst. Synergy: Any multi-hit cannon.
[] 29. Care Package — Slow arcing shot; on hit, drops a random on-screen pickup (even off enemies). Synergy: Farming / sustain builds.
[] 30. Hiccup — Uneven fire rhythm — damage comes in bursty clusters enemies can’t smooth out. Pairs with Kindling.
[] 31. Photonegative — Only damages enemies currently off-screen (edge sniping). Synergy: Auto Target, Seeker setups.
[] 32. Sublet — Weaker shots, but node positions keep firing briefly during your hit-flash/i-frames. Synergy: Tank builds.

## Cannon Data:
[x] 29. Auto Target - Shots auto target the nearest enemies
[] 30. Blind Spot — Nodes fire from the opposite side of your movement vector. Forces you to fly aggressively.
[] 31. Creep — All nodes slowly drift toward screen center, tightening into a death beam. Synergy: Laser, Charge Shot.
[] 32. Equalizer — Crowded screen = wide spread; few enemies = tight sniper cone. Adapts to wave density automatically.
[] 33. Tailgate — Nodes trail behind you like a comet tail. Synergy: Hit-and-run + Ghostwrite.
[] 34. Redline — Holding fire accelerates nodes; releasing snaps them back and pauses briefly. Skill-based burst DPS.
[] 35. Echo Locate — New nodes spawn only where your shots recently hit terrain. Synergy: Maze / corridor stages.
[] 36. Doppelgänger — Mirror ship on the opposite screen edge fires an inverted pattern. Double coverage without Twin Nodes.
[] 37. Filibuster — You only fire while holding the button; longer hold = bigger release salvo. Opposite of Rapid Cannon feel.
[] 38. Stage Whisper — Damage ticks up slightly on music downbeats (if you can read BGM timing). Weird skill ceiling, very on-brand.
[] 39. April Fool — First volley each stage fires backward, then corrects forever. Memorable “gotcha” that players learn once.