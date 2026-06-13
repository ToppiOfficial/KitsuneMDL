# $rendermesh

Defines a named mesh filter that can be used in place of a source filename in
`$body`, `$bodygroup` (`studio`), and `$model`. It can filter by DmeMesh object
name, by material name, or both, without modifying the source file or
duplicating it on disk.

DmeMesh filtering (`"<meshName>"` entries and `defaultState`) requires a DMX
source. SMD, OBJ, and VRM sources are not compatible with that part and will
produce a warning if mesh overrides are listed. `removematerial` works on any
source format.

## Syntax

```
$rendermesh <name> <file> <defaultState> {
    "<meshName>"
    ...
    removematerial "<materialName>"
    ...
}
```

## Parameters

**`<name>`**

Identifier used to reference this definition from `$body`, `$bodygroup`,
or `$model`. Must be unique within the QC file.

**`<file>`**

Path to the source file, relative to the current `$cd` directory.
Same rules as a normal `$body` filename. DMX is required for DmeMesh
filtering; any format works for `removematerial`-only definitions.

**`<defaultState>`** (`0` or `1`)

Controls what happens to meshes not listed in the block:
- `0` - all meshes are hidden by default; only listed meshes are shown.
- `1` - all meshes are shown by default; only listed meshes are hidden.

**`"<meshName>"`** (zero or more)

Name of a DmeMesh object inside the DMX file. Each listed mesh takes
the state opposite to `defaultState`:
- `defaultState 0` - listed mesh is shown (isolate)
- `defaultState 1` - listed mesh is hidden (exclude)

The compiler errors if a listed name does not exist in the DMX.

**`removematerial "<materialName>"`** (zero or more)

Removes all faces assigned to the named material, regardless of which
DmeMesh object they belong to. The material name is matched
case-insensitively against the texture name (without path or extension)
as it appears in the source file. A warning is emitted if the name does
not match any material in the source.

`removematerial` filters are applied after the DmeMesh mesh filter, so
you can combine both to first isolate a subset of meshes and then strip
individual materials from the result.

`removematerial` does not require DmeMesh tracking data and works on any
source format (DMX, SMD, OBJ, VRM). It runs after the DmeMesh mesh filter
when both are present.

## Usage in $body / $bodygroup / $model

Anywhere a source filename would normally appear, write the `$rendermesh` name
instead. The compiler resolves it, loads the underlying file (using the
normal source cache), clones the geometry, and applies the filter before
the model is processed.

```
$body  <partName>  <rendermeshName>
studio <rendermeshName>
$model <partName>  <rendermeshName>
```

Multiple body parts or `studio` entries can reference the same `$rendermesh`
name or the same source file through different `$rendermesh` definitions.
Each gets its own independent copy of the geometry.

## Examples

Two body parts from one DMX, each showing a different mesh subset:

```
$rendermesh hair_mesh "thirdperson_model.dmx" 0 {
    "c_hair_lod0"
    "c_hair_lod0_edgeline"
}

$rendermesh body_mesh "thirdperson_model.dmx" 1 {
    "c_hair_lod0"
    "c_hair_lod0_edgeline"
}

$body hair hair_mesh
$body body body_mesh
```

Bodygroup with two filtered variants of the same file:

```
$rendermesh outfit_A "character.dmx" 0 {
    "outfit_A_mesh"
}

$rendermesh outfit_B "character.dmx" 0 {
    "outfit_B_mesh"
}

$bodygroup outfit {
    studio outfit_A
    studio outfit_B
    blank
}
```

Strip a material from an otherwise-complete mesh:

```
$rendermesh body_no_decal "character.dmx" 1 {
    removematerial "decal_layer"
}

$body body body_no_decal
```

Combine DmeMesh isolation with material removal:

```
$rendermesh hair_clean "character.dmx" 0 {
    "c_hair_lod0"
    removematerial "hair_alpha_edge"
}

$body hair hair_clean
```

## Notes

- `$rendermesh` is a definition only; it does not load the source file at parse
  time. Loading happens when the name is first encountered in `$body`/`$bodygroup`/`$model`.
- The underlying source file is loaded through the normal source cache. The
  filtered copy is separate and does not affect other references to the same file.
- Vertex animations (blend shapes) are remapped to match the filtered vertex
  set automatically.
- `$rendermesh` has no effect on `$sequence`, `$animation`, or any other command.
- `removematerial` names are matched case-insensitively. Use the base name
  only (no path, no `.vmt` extension), matching how materials are referenced
  inside the source file.
- DmeMesh filtering (`"<meshName>"` entries) is DMX-only. Listing mesh names
  for an SMD/OBJ/VRM source will warn and skip that part; `removematerial`
  entries on the same definition still apply normally.
