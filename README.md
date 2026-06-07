# studiomdl_v2

> **Alpha / Experimental probably not useable, I am not a c++ developer so I have close to 0 clue what I am doing**

A standalone fork of Valve's StudioMDL compiler based on [REDxEYE/studiomdl_v2](https://github.com/REDxEYE/studiomdl_v2).

I'll likely rename the project.


## Requirements

`$collisionmodel` and `$collisionjoints` require the following DLLs placed alongside the executable, sourced from TF2's `bin/x64/` folder: (Not sure if other 64-bit source engine game can work)

- `vphysics.dll`
- `tier0.dll`
- `vstdlib.dll`


`DMX model 18` but lower version can maybe still work


## Changes

- Bone limit raised from 256 to 1024, texture limit from 32 to 64
- Replaced nvstrip with meshoptimizer
- TF2/L4D2 compatible VTX output by default; use `-newvtx` for CS:GO/Alien Swarm/Source Film Maker format, `-nodx80` to skip dx80/sw
- `-cullanims` flag to strip unreferenced `$animation` blocks
- `$scale` now affects eyeball, eyelid, dmxeyelid, forceboneposrot, procedural bones, and VTA flex deltas
- New `ignorescale` parameter for `$animation` and `$sequence`
- `$addsearchdir` now works correctly for SMD/DMX source file lookup
- `$renamebone` now propagates to the collision model
- Bone weight cull threshold reduced from 5% to 0.01%
- Fixed crash with blank bodygroup + `$staticprop` (TODO: Test)

## Credits

- Valve — Source Engine and SDK
- [REDxEYE](https://github.com/REDxEYE) — base fork
- [ficool2](https://github.com/ficool2) — studiomdl++ fixes
- [Starfelll](https://github.com/Starfelll) — NekoMDL fixes
