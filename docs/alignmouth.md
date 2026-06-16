# $alignmouth

> [!WARNING]
> **Unfinished - not recommended for project use.**
> `$alignmouth` is a work in progress (and shares the experimental `$aligneyes`
> machinery). The behaviour - especially the `forward` / normal-averaging direction
> logic, flex resolution, and offset handling - is likely to change, and planned
> changes may break existing usage. Don't rely on it in shipping models yet - prefer
> a hand-authored `$attachment` for now.

Auto-generates an attachment centered on the mouth, so you don't have to hand-tune
the `rotate` angles and position of a "mouth" attachment. It is the flex-driven
sibling of [`$aligneyes`](aligneyes.md): instead of eyeballs, it targets a set of
flexes and centers on the vertices those flexes deform.

It computes the attachment's **origin** as the centroid of the vertices deformed by
the targeted flexes (so it lands in the middle of the mouth), and its **forward
direction** either from a literal world-space vector you supply or from the averaged
mesh normals of those vertices. The result is computed in world space and stored as
an absolute attachment, so it is correct regardless of how the head/jaw bone happens
to be oriented. The parent bone is chosen automatically as the bone those vertices
are weighted to most.

This replaces having to write something like:

```
$attachment "mouth" "ValveBiped.Bip01_Head1" 0.80 -5.80 -0.15 rotate 0 -80 -90
```

## Syntax

```
$alignmouth "<name>" {
    flexgroup      "<group name>"        // repeatable
    flexcontroller "<controller name>"   // repeatable
    forward <x y z>                     // optional
    offset  <x y z>                    // optional
}
```

- **`<name>`** - the name of the generated attachment (e.g. `"mouth"`).
- **`flexgroup "<name>"`** - matches every flex controller whose **group/type** is
  `<name>`, and gathers the flexes those controllers drive. Repeatable.
- **`flexcontroller "<name>"`** - targets a single flex controller by name, and
  gathers the flexes it drives. Repeatable.
- **`forward <x y z>`** *(optional)* - a literal **world-space** direction the
  attachment points along. Source models are typically X-forward / Z-up, so
  `1 0 0` aims it down world +X. If omitted or set to `0 0 0`, the direction is
  taken from the **averaged mesh normals** of the deformed vertices.
- **`offset <x y z>`** *(optional, default `0 0 0`)* - a **world-aligned** offset
  added to the computed mouth centroid.

You can mix `flexgroup` and `flexcontroller` entries in one block; at least one of
them is required.

## How flexes are resolved

The driven flexes are found by walking the model's flex rules: any rule that fetches
a targeted flex controller marks the flex (flexdesc) it drives, and every flex key
for that flex contributes its deformed vertices to the centroid.

## Example

```
$alignmouth "mouth" {
    flexgroup "jaw"
    flexgroup "lips"
    forward 1 0 0
    offset  0 0 0
}
```

## Ordering

The attachment slot is reserved at the point `$alignmouth` appears in the QC, so the
generated attachment keeps its declaration order relative to surrounding
`$attachment` (and `$aligneyes`) commands. For example, an `$aligneyes "eyes"`
declared before `$alignmouth "mouth"` produces `eyes` ahead of `mouth` in the model.

## Notes / limitations

- At least one `flexgroup` or `flexcontroller` is required; if nothing matches, or
  the matched flexes deform no vertices, compilation errors.
- The parent bone is auto-selected (most-weighted bone of the deformed verts); there
  is currently no way to force a specific bone.
- When relying on averaged normals (`forward` omitted), degenerate/cancelling normals
  will error - supply an explicit `forward` in that case.
- As noted above, this command is **not finished** and its behaviour may change.
