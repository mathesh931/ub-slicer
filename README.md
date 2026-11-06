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
| Name and logo | UB Slicer, Unbound's swoosh (window, icons, splash, installer, About) |
| Printers | **Unbound only**: Unbound Lynx 350 (0.4 mm nozzle), plus Orca's "Custom printer" option |
| Print settings | 0.20mm Standard and 0.20mm Fast @Unbound Lynx 350 (tested on the Lynx 350) |
| Filaments | Unbound PLA, PETG, ABS, TPU, plus Orca's generic filament library |
| Updates | No OrcaSlicer update pop-ups and no online profile downloads: updates come as new UB Slicer installers |
| Settings folder | `%APPDATA%\UBSlicer` on Windows (separate from OrcaSlicer, both can be installed) |

## Download

Repository → **Releases** → `UB_Slicer_Windows_Installer_V….exe`. Builds of every change are under
**Actions** → *Build UB Slicer for Windows* → a run → **Artifacts**.

## Where things are

| What | Where |
|---|---|
| Unbound printer, settings, filaments | `resources/profiles/Unbound/` (+ `Unbound.json`) |
| Logos and icons | `resources/images/` (Orca's file names, Unbound artwork) |
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
