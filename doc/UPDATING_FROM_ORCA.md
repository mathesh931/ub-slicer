# Updating UB Slicer to a newer OrcaSlicer

UB Slicer started as an exact copy of OrcaSlicer **v2.3.1** (commit `f2d10a0`), the first commit on
`main`. Our changes are small and in few places, so a new Orca version can be brought in like this:

1. Make a branch, e.g. `update-orca-2.4.0`.
2. Bring in Orca's changes between the two versions (from a computer with git):
   ```
   git remote add orca https://github.com/OrcaSlicer/OrcaSlicer.git
   git fetch --depth 1 orca tag v2.3.1
   git fetch --depth 1 orca tag v2.4.0
   git diff v2.3.1 v2.4.0 -- . ':!resources/profiles' | git apply --3way
   ```
   Printer profiles of other brands are left out on purpose (UB Slicer shows Unbound printers only);
   copy the newer `resources/profiles/OrcaFilamentLibrary*` and `resources/profiles/Custom*` by hand.
3. Fix any conflicts in the files UB Slicer changes (list below), update `doc/UPDATING_FROM_ORCA.md`
   with the new base version, open a pull request and let the Windows build run.

## Files UB Slicer changes

| File | Change |
|---|---|
| `version.inc` | app name "UB Slicer", app key "UBSlicer" (settings folder, translation files) |
| `src/libslic3r/libslic3r.h` | full name, G-code viewer name |
| `CMakeLists.txt` | translation file name, installer (name, vendor, shortcut, registry key) |
| `src/CMakeLists.txt`, `src/dev-utils/platform/msw/OrcaSlicer.rc.in` | `ub-slicer.exe`, file details |
| `src/slic3r/Utils/Process.cpp`, `src/slic3r/Utils/NetworkAgent.cpp` | `ub-slicer.exe` |
| `src/slic3r/GUI/Plater.cpp`, `InstanceCheck.cpp`, `MainFrame.cpp`, `WebGuideDialog.cpp`, `WebDownPluginDlg.cpp`, `GUI_App.cpp` | window titles, single-instance check, file association id |
| `src/slic3r/GUI/GUI_App.cpp` (`check_new_version_sf`), `src/slic3r/Utils/PresetUpdater.cpp` (`sync_config`) | no OrcaSlicer update check, no online profile download |
| `src/slic3r/GUI/AboutDialog.cpp` | UB Slicer credit line and links |
| `scripts/run_gettext.*` | translation files named `UBSlicer.mo` |
| `resources/images/*` (Orca file names), `resources/web/image/logo.png` | Unbound artwork |
| `resources/web/data/text.js`, `resources/web/guide/*/index.html` | "UB Slicer" in the welcome pages |
| `resources/profiles/` | only `Unbound3D` (Lynx 350/320 from Unbound's earlier slicer; bed pictures with the Unbound logo), `Custom` (kept but empty: no generic printers), `OrcaFilamentLibrary` |
| `src/libslic3r/PresetBundle.cpp` | fallback printer Unbound3D Lynx 350, fallback filament eSUN PLA+ @Unbound3D |
| `.github/` | UB Slicer's own build and profile checks |
| `src/slic3r/GUI/GUI_App.cpp` (`config_wizard_startup`), `WebGuideDialog.cpp` | Unbound3D Lynx 350 ticked on first start and used if setup is closed |
| Theme: ~90 files in `src/slic3r`, `src/libslic3r/Color.hpp`, ~360 icons in `resources/images`, CSS in `resources/web` | Orca teal → black/greys: `#009688`→`#333333`, `#00897B`→`#1A1A1A`, `#26A69A`→`#4A4A4A`, `#BFE1DE`→`#DADADA`, `#E5F0EE`→`#EDEDED`, dark mode `#00675B`→`#6E6E6E`; 3D view accent rgb(90,90,90); top bar `#3B4446`→`#2B2B2B`, selected tab rgb(85,85,85). Re-apply with a search-and-replace after updating Orca |
| Wording: translatable texts in `src/`, `localization/i18n/*.po`, `resources/data/hints.ini` | "Orca Slicer"/"OrcaSlicer"/"Orca" → "UB Slicer" (file names, links and log lines unchanged); calibration models "Calibration Cube", "Tolerance Test", "String Hell" |
| `resources/web/orca/missing_connection.html` | Device tab: written connection steps instead of Orca's recorded animation (`setup_connection.gif` removed) |
| Teal/blue PNG icons (`resources/images/{delete,import_file,new_folder,bullet_blue,humidity_list_background}.png`, `resources/web/{guide,homepage,login,model}/…`) | greyed; Orca logo `resources/web/image/logo2.png` removed |
| `src/slic3r/Utils/PresetUpdater.cpp` (`check_installed_vendor_profiles`) | Unbound3D always installed with the Lynx 350 switched on, also for older settings folders |
| `src/libslic3r/PresetBundle.cpp` (`load_selections`) | a saved printer that no longer exists falls back to the Unbound3D Lynx 350 |
| `resources/profiles/Custom.json` | version raised to 02.03.01.20 so old copies (with MyKlipper) in users' settings folders are replaced |
| `src/slic3r/GUI/GUI_App.cpp` (splash `version_font`) | smaller version number on the splash screen |
