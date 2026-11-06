# TrainingSlowMo

*Adapt game speed while training.*

A [BakkesMod](https://bakkesmod.com) plugin for Rocket League custom training. It changes the game speed
(`sv_soccar_gamespeed`) to match your accuracy: shots you keep missing get slower, shots you keep making get faster.

## How it works

- Every 10 shots, your hit rate is compared with a target accuracy (default 70%).
  - More than 30 points off the target: speed changes by two increments.
  - Between 5 and 30 points off: speed changes by one increment.
  - Speed stays between the minimum and maximum limits (default 50–150%).
- A shot counts as a hit when the ball goes in, and as a miss when the ball explodes without a goal.
- With **"Touching the ball counts as success"** enabled, any touch counts as a hit – useful for defence packs.

## Overlay

Each line can be toggled on or off:

- Current streak
- A 10-dot red/green bar for the current set
- Game speed %
- Accuracy of the previous set
- Total shots and total accuracy
- Accuracy over the last 100 shots

## Settings

Open the BakkesMod menu (F2) → **Plugins** → **TrainingSlowMo**.

| Setting | Cvar | Default |
|---|---|---|
| Enable | `slowMoEnabled` | off |
| Touching the ball counts as success | `onTouchEnabled` | off |
| Target accuracy | `idealAccuracy` | 0.7 |
| Speed increment | `slowMoSpeedIncrement` | 0.05 (5%) |
| Maximum speed | `slowMoMaxSpeed` | 150 |
| Minimum speed | `slowMoMinSpeed` | 50 |
| Interval (seconds) | `slowMoIntervalTime` | 60 |
| Mode | `currMode` | Adapt to accuracy |

The second mode, *Increase speed over time* (which uses the interval setting), has a setting but is not implemented yet.

## Building

Requirements:

- Windows with Visual Studio 2019 or newer and the **v142** C++ toolset (newer Visual Studio versions can retarget the project).
- [BakkesMod](https://bakkesmod.com) installed. `BakkesMod.props` finds the SDK through the registry key
  `HKCU\Software\BakkesMod\AppPath\BakkesModPath`, which the BakkesMod installer creates.

Open `TrainingSlowMo.sln`, choose **Release | x64** and build. The DLL lands in `plugins\`; the post-build step patches it
with `bakkesmod-patch.exe`. The pre-build step runs `update_version.ps1`, which bumps the build number in `version.h`.

To install a build by hand, copy `TrainingSlowMo.dll` into `%APPDATA%\bakkesmod\bakkesmod\plugins` and run
`plugin load TrainingSlowMo` in the BakkesMod console (F6).

## Easy Anti-Cheat

Since April 2026 Rocket League on PC runs Easy Anti-Cheat. BakkesMod only loads when the game is started
**without** anti-cheat, which limits it to offline play: freeplay, custom training, LAN and replays.

## Third-party code

- `fmt/` – [{fmt}](https://github.com/fmtlib/fmt) 7.0.3, MIT licence (see `fmt/LICENSE.rst`).
- `IMGUI/` – [Dear ImGui](https://github.com/ocornut/imgui) (MIT) with BakkesMod's additions, as shipped by the
  BakkesMod plugin template.

## Licence

MIT – see [LICENSE](LICENSE).
