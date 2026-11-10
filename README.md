<div align="center">
<img alt="UB Slicer" src="resources/images/OrcaSlicer_192px.png" width="96">

# UB Slicer

**The slicer for Unbound3D printers.** Built on [OrcaSlicer](https://github.com/OrcaSlicer/OrcaSlicer).
</div>

UB Slicer is OrcaSlicer 2.3.1 with Unbound's name and logo, and with Unbound's printers built in:
every Orca feature (3D plate, all print settings, preview, supports painting, cut, calibrations …) is here.

## What is different from OrcaSlicer

| | UB Slicer |
|---|---|
| Name and logo | UB Slicer with the Unbound 3D Industries logo (window, icons, splash, installer, About) |
| Look | Black and white (Orca's teal replaced everywhere, light and dark mode) |
| Printers | **Unbound only**: Unbound3D Lynx 350 and Lynx 320 (0.4 mm hardened nozzle); the Lynx 350 is ticked on first start |
| Print settings | 0.20mm Standard @Unbound3D and Unbound's tested settings per filament |
| Filaments | Unbound's tested filaments (eSUN, Numakers, JamgHe, Fibreel …), plus Orca's generic filament library |
| Updates | No OrcaSlicer update pop-ups and no online profile downloads: updates come as new UB Slicer installers |
| Settings folder | `%APPDATA%\UBSlicer` on Windows (separate from OrcaSlicer, both can be installed) |

## Download

Repository → **Releases** → `UB_Slicer_Windows_Installer_V….exe`. Builds of every change are under
**Actions** → *Build UB Slicer for Windows* → a run → **Artifacts**.

## Where things are

| What | Where |
|---|---|
| Unbound printers, settings, filaments | `resources/profiles/Unbound3D/` (+ `Unbound3D.json`) |
| Logos and icons | `resources/images/` (Orca's file names, Unbound artwork) |
| Colours | Orca's teal swapped for black/greys in code and icons (`doc/UPDATING_FROM_ORCA.md`) |
| Name, version | `version.inc` |
| Installer | `CMakeLists.txt` (CPack section) |
| Windows build on GitHub | `.github/workflows/build_windows.yml` |
| Profile checks | `.github/workflows/check_profiles.yml` |
| Updating to a newer OrcaSlicer | `doc/UPDATING_FROM_ORCA.md` |
| Orca's own README | `doc/OrcaSlicer-README.md` |

## Licence

UB Slicer is free software under the **GNU Affero General Public License v3** (`LICENSE.txt`), like
OrcaSlicer, BambuStudio and PrusaSlicer it is based on. Anyone who receives UB Slicer can ask for this
source code. OrcaSlicer © 2022-2025 Li Jiang and contributors; UB Slicer changes © Unbound3D.
