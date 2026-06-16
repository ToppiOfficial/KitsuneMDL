# $aligneyes

> [!WARNING]
> **Unfinished - not recommended for project use.**
> `$aligneyes` is a work in progress. The behaviour (especially the `forward` /
> normal-averaging direction logic and offset handling) is likely to change, and
> there are planned changes that may break existing usage. Don't rely on it in
> shipping models yet - prefer a hand-authored `$attachment` for now.

Auto-generates an attachment aimed along the gaze of one or more eyeballs, so you
don't have to hand-tune the `rotate` angles and position of an "eyes" attachment.

It computes the attachment's **origin** as the centroid of the referenced
eyeballs' material vertices (so with a left and right eye it lands between them),
and its **forward direction** either from a literal world-space vector you supply
or from the averaged mesh normals of those eyeball vertices. The result is
computed in world space and stored as an absolute attachment, so it is correct
regardless of how the eye/head bone happens to be oriented.

This replaces having to write something like:

```
$attachment "eyes" "ValveBiped.Bip01_Head1" 2.04 -3.5 0 rotate 0 -80.1 -90
```

## Syntax

```
$aligneyes "<name>" {
    eyeball "<eyeball name>"      // repeatable
    forward <x y z>             // optional
    offset  <x y z>            // optional
}
```

- **`<name>`** - the name of the generated attachment (e.g. `"eyes"`).
- **`eyeball "<name>"`** - the **name** of an eyeball previously declared via
  `$model` / `eyeball "<name>" <bone> <x y z> <material> ...` (the first token of
  that declaration, not the material name). Repeatable. All referenced eyeballs
  must share the **same** parent bone, otherwise compilation errors.
- **`forward <x y z>`** *(optional)* - a literal **world-space** direction the
  attachment points along. Source models are typically X-forward / Z-up, so
  `1 0 0` aims it down world +X. If omitted or set to `0 0 0`, the direction is
  taken from the **averaged mesh normals** of the eyeball material's vertices.
- **`offset <x y z>`** *(optional, default `0 0 0`)* - a **world-aligned** offset
  added to the computed eye centroid.

## Example

```
$aligneyes "eyes" {
    eyeball "eye_right"
    eyeball "eye_left"
    forward 0 1 0
    offset  0 0 0
}
```

## Ordering

The attachment slot is reserved at the point `$aligneyes` appears in the QC, so
the generated attachment keeps its declaration order relative to surrounding
`$attachment` commands. For example, an `$aligneyes "eyes"` declared before
`$attachment "mouth"` produces `eyes` ahead of `mouth` in the model.

## Notes / limitations

- All eyeballs in one `$aligneyes` block must be bound to the same bone.
- Referencing an eyeball name that doesn't exist is a hard error.
- When relying on averaged normals (`forward` omitted), degenerate/cancelling
  normals will error - supply an explicit `forward` in that case.
- As noted above, this command is **not finished** and its behaviour may change.
