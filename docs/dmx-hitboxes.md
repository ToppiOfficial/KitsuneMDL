# DMX hitboxes (and capsule hitboxes)

KitsuneMDL can read hitboxes directly from a DMX `hitboxSetList` when the DMX is
referenced by `$body`, `$bodygroup`, or `$model`. This lets a DMX authoring tool
ship hitboxes (including CS:GO-style capsule hitboxes) inside the model file
instead of, or in addition to, the QC `$hbox` command.

This is independent of the QC `$hbox` / `$hboxset` commands, which continue to
work unchanged. Both paths write into the same model-global hitbox state.

## How DMX hitboxes are structured

A hitbox is a child of a hitbox **set**. The DMX mirrors the internal layout:

```
hitboxSetList (CDmeHitboxSetList)
  └─ hitbox set (CDmeHitboxSet)        named; e.g. "default"
       └─ hitbox (CDmeHitbox)          one hitbox
```

Each `CDmeHitbox` is read for the following attributes:

| Attribute | Type | Meaning |
|---|---|---|
| `boneName` | string | Bone the hitbox is attached to (resolved to an index later) |
| `groupId` | int | Hit group |
| `minBounds` / `maxBounds` | vector3 | OBB bounds (or capsule end points if a radius is set) |
| `radius` | float | `-1` = OBB box (default), `>= 0` = capsule radius |
| `orientation` | vector3 | Rotation offset in degrees (pitch, yaw, roll) |

`radius` and `orientation` are the capsule fields. They map to the same
`flCapsuleRadius` / `angOffsetOrientation` fields the QC `$hbox` command sets, so
a DMX-authored capsule produces an identical `mstudiobbox_t` to the equivalent
`$hbox <group> <bone> <min> <max> <orientation> <radius>`.

`orientation` is stored as a `vector3` (not a `qangle`) so tools that can only
emit `vector3` attributes can author it. The three components are pitch, yaw,
roll, in degrees.

Hitbox bounds are scaled by the active `$scale`, matching the QC `$hbox` path.

Whether a capsule actually behaves as a capsule at runtime depends on the target
engine: TF2-era engines ignore `flCapsuleRadius` and treat every hitbox as an
OBB; CS:GO and newer honor it.

## Which hitboxes win: first source to define them

Hitboxes are model-global. The first source to define any hitbox set wins, and
later definitions are ignored:

- If a QC `$hbox` / `$hboxset` runs before the DMX is loaded, the QC set wins and
  the DMX `hitboxSetList` is skipped with a warning.
- If several `$body` / `$bodygroup` / `$model` DMX files each carry a
  `hitboxSetList`, only the first one loaded is used; the rest warn and are
  skipped.

Within a single DMX, every set in the `hitboxSetList` is loaded. The same hitbox
appearing in two differently-named sets is kept in both - sets are independent,
runtime-selectable configurations, so this is not a duplicate.

> Tip: to make a QC `$hbox` reliably override a DMX `hitboxSetList`, place the
> `$hbox` / `$hboxset` lines **above** the `$body` / `$bodygroup` that loads the
> DMX.

## Load order matters for `$animation` / `$sequence`

`$animation` and `$sequence` load their source through the same loader as model
bodies. To prevent an animation DMX from injecting model data:

- **Hitboxes:** are only read from model-body sources
  (`$body` / `$bodygroup` / `$model`). An `$animation` / `$sequence` DMX never
  contributes hitboxes, regardless of order.
- **Jigglebones:** are loaded from the skeleton of any DMX source, including
  animation sources, and dedupe by **bone name, first definition wins**. A
  jigglebone the model body already defined will win over one in an animation
  DMX (with a `"Jiggle Bone: ... already defined, ignoring additional
  declarations"` warning).

Because jigglebone dedup is first-wins by load order, **declare model bodies
(`$body` / `$bodygroup` / `$model`) before `$animation` / `$sequence` in the
QC.** That way the body's jigglebones load first and win; any stray jigglebone
data inside an animation DMX is harmlessly skipped. Loading an animation first
could let its jigglebone definition win instead.
