# KitsuneMDL (studiomdl_v2)

> **Experimental**

A standalone fork of Valve's StudioMDL compiler based on [REDxEYE/studiomdl_v2](https://github.com/REDxEYE/studiomdl_v2).

## Requirements

`$collisionmodel` and `$collisionjoints` require the following DLLs placed alongside the executable, sourced from TF2's `bin/x64/` folder: (Not sure if other 64-bit source engine game can work)

- `vphysics.dll`
- `tier0.dll`
- `vstdlib.dll`


`DMX model 18` but lower version can maybe still work

## Features

- 64 Bit
- Increase some compile limit
  - Bone limit 256 -> 1024 (Will warn if the model is above 256 as most engine branch are either 128 or 256)
  - Texture limit 32 -> 64 (Still recommend to stay below 32 for all engine branch aside from Source Film Maker)
- Replaced nvstrip with meshoptimizer
- Minor code changes for faster compile
- `$addsearchdir` now works correctly for SMD/DMX source file lookup
- New `ignorescale` parameter for `$animation` and `$sequence`
- New `$driverbone` and `driverlookat` to create procedural bone without the need for VRD files
- New `$rendermesh` for DMX models containing multiple DMEMesh elements
- New `$if $elif $else` and `$switch` conditional commands.
- New `$staticproppose <animation_file> <frame>` to bake a custom pose into the geometry skeleton to a single `static_prop`. Cannot be used together with `$staticprop`. (Doesn't work as expected!)
- New `$deltaproportions` to generate the `a_reference` and `a_proportions`. See `docs/deltaproportions.txt`.
- Multiple engine branch support but requires additional launch parameter `-newvtx` for Alien Swarm to CS:GO Engine Branch.
- Can compile for DirectX8 and can be opted out with `-nodx80` similar to StudioMDL++
- Recreated some features from StudioMDL++ and NekoMDL
  - `-cullanims` flag to strip unreferenced `$animation` blocks
  - Bone weight cull threshold reduced from 5% to 0.01%
  - `$scale` now affects eyeball, eyelid, dmxeyelid, forceboneposrot, procedural bones, and VTA flex deltas
  - `$renamebone` now propagates to the collision model
  - Fixed crash with blank bodygroup + `$staticprop` (TODO: Test)

## Credits

- Valve — Source Engine and SDK
- [REDxEYE](https://github.com/REDxEYE) — base fork
- [ficool2](https://github.com/ficool2) — studiomdl++ fixes ideas
- [Starfelll](https://github.com/Starfelll) — NekoMDL fixes ideas
