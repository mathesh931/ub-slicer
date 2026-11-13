# Updating UB Slicer to a newer OrcaSlicer

UB Slicer 2.5 starts from an exact copy of OrcaSlicer **v2.5.0** (commit `40ecc66f`; UB Slicer 2.3.1 was based on v2.3.1), the first commit on
`main`. Our changes are small and in few places, so a new Orca version can be brought in like this:

1. Make a branch, e.g. `update-orca-2.6.0`.
2. Bring in Orca's changes between the two versions (from a computer with git):
   ```
   git remote add orca https://github.com/OrcaSlicer/OrcaSlicer.git
   git fetch --depth 1 orca tag v2.5.0
   git fetch --depth 1 orca tag v2.6.0
   git diff v2.5.0 v2.6.0 -- . ':!resources/profiles' | git apply --3way
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
| `src/slic3r/Utils/Process.cpp`, `src/slic3r/Utils/BBLNetworkPlugin.cpp` | `ub-slicer.exe` |
| `src/slic3r/GUI/Plater.cpp`, `InstanceCheck.cpp`, `MainFrame.cpp`, `WebGuideDialog.cpp`, `WebDownPluginDlg.cpp`, `GUI_App.cpp` | window titles, single-instance check, file association id |
| `src/slic3r/GUI/GUI_App.cpp` (`check_new_version_sf`, `ub_download_and_install`), `GUI_App.hpp` (`installer_url`), `src/libslic3r/AppConfig.cpp` (`VERSION_CHECK_URL`, `profile_update_url` empty), `ReleaseNote.cpp` ("Update now") | updates come from UB Slicer's own GitHub releases (nothing about the computer is sent); on Windows "Update now" downloads the release's installer, starts it and closes UB Slicer; no online profile download |
| `src/slic3r/GUI/AboutDialog.cpp` | UB Slicer credit line and links |
| `scripts/run_gettext.*` | translation files named `UBSlicer.mo` |
| `resources/images/*` (Orca file names), `resources/web/image/logo.png` | Unbound artwork |
| `resources/web/data/text.js`, `resources/web/guide/*/index.html` | "UB Slicer" in the welcome pages |
| `resources/profiles/` | only `Unbound3D` (Hercules; bed picture with the Unbound logo; the Lynx 350/320 files are in git history for when they come back), `Custom` (kept but empty: no generic printers), `OrcaFilamentLibrary` |
| `src/libslic3r/PresetBundle.cpp` | fallback printer Unbound3D Hercules, fallback filament eSUN PLA+ @Unbound3D |
| `.github/` | UB Slicer's own build and profile checks |
| `src/slic3r/GUI/GUI_App.cpp` (`config_wizard_startup`), `WebGuideDialog.cpp` | Unbound3D Hercules ticked on first start and used if setup is closed |
| Theme: ~90 files in `src/slic3r`, `src/libslic3r/Color.hpp`, ~360 icons in `resources/images`, CSS in `resources/web` | Orca teal → black/greys: `#009688`→`#333333`, `#00897B`→`#1A1A1A`, `#26A69A`→`#4A4A4A`, `#BFE1DE`→`#DADADA`, `#E5F0EE`→`#EDEDED`, dark mode `#00675B`→`#6E6E6E`; 3D view accent rgb(90,90,90); top bar `#3B4446`→`#2B2B2B`, selected tab rgb(85,85,85). Re-apply with a search-and-replace after updating Orca |
| Wording: translatable texts in `src/`, `localization/i18n/*.po`, `resources/data/hints.ini` | "Orca Slicer"/"OrcaSlicer"/"Orca" → "UB Slicer" (file names, links and log lines unchanged); calibration models "Calibration Cube", "Tolerance Test", "String Hell" |
| `resources/web/orca/missing_connection.html` | Device tab without a connected printer: written connection steps instead of Orca's recorded animation (`setup_connection.gif` removed) |
| Teal/blue PNG icons (`resources/images/{delete,import_file,new_folder,bullet_blue,humidity_list_background}.png`, `resources/web/{guide,homepage,login,model}/…`) | greyed; Orca logo `resources/web/image/logo2.png` removed |
| `src/slic3r/Utils/PresetUpdater.cpp` (`check_installed_vendor_profiles`) | Unbound3D always installed with the Hercules switched on, also for older settings folders |
| `src/libslic3r/PresetBundle.cpp` (`load_selections`) | a saved printer that no longer exists falls back to the Unbound3D Hercules |
| `resources/profiles/Custom.json` | version raised to 02.03.01.20 so old copies (with MyKlipper) in users' settings folders are replaced |
| `src/slic3r/GUI/GUI_App.cpp` (splash `version_font`) | smaller version number on the splash screen |
| `src/dev-utils/platform/osx/Info.plist.in` | Mac app name UB Slicer, id com.unbound3d.ubslicer (the program inside stays `OrcaSlicer`) |
| `src/libslic3r/AppConfig.cpp` (`stealth_mode`), `resources/web/homepage/index.html` (`LoginArea` hidden) | no Orca/Bambu cloud |
| `resources/images/OrcaSlicer_horizontal_{light,dark}.svg` | Unbound logo (Troubleshoot window) |
| `resources/profiles/Unbound3D/**` | brought to Orca 2.5's profile rules with `scripts/orca_profile_tool.py fix-variant / generate-id / normalize / update-index --vendor Unbound3D` (values unchanged) |
| `src/slic3r/GUI/MainFrame.cpp` (`#if 0 // UB Slicer`; `load_printer_url` without the device-manager address) | no "Sync Presets" (OrcaCloud), no "Plugins" (Orca plugin store); Help → Check for Updates checks UB Slicer's releases |
| `src/slic3r/GUI/GUI_App.cpp` ("Profile syncing change") | no Orca Cloud profile-sync notice at start |
| `resources/web/homepage/index.html` | no OrcaCloud and Wiki buttons on Home |
| `OptionsGroup.cpp`, `OG_CustomCtrl.cpp`, `Preferences.cpp` (`get_url`/wiki link) | no Orca wiki link behind settings |
| Wiki/video buttons hidden (`->Hide(); // UB Slicer`) in `UnsavedChangesDialog`, `StepMeshDialog`, `AMSMaterialsSetting`, `calib_dlg`, `PublishSettingsDialog`, `TroubleshootDialog`, `FilamentGroupPopup`, `GLGizmoTextureDisplacement`; `MainFrame.cpp` Calibration Guide menu removed | no Orca help pages or YouTube |
| `src/slic3r/GUI/NativeCommands.cpp` (`#if 0 // UB Slicer`) | command palette: no Wiki, YouTube, Plugins (Check for Updates kept, UB Slicer releases) |
| `StatusPanel`, `SelectMachine`, `SelectMachinePop`, `CameraPopup`, `Widgets/SideTools` | help links go to `https://ubconnect.live` instead of Orca's wiki |
| `resources/data/hints.ini` | Orca `documentation_link` lines removed |
| `src/slic3r/GUI/Preferences.cpp` (`#if 0 // UB Slicer`) | Preferences: no Plugins section, no login region / stealth / login panel, no Cloud Providers, Update & sync or Bambu network plug-in |
| `resources/profiles/Unbound3D/` (Hercules) | Hercules printer from Unbound's own profile (`Unbound 3D Hercules.orca_printer`); Unbound filaments and processes point to it; Lynx-only ones removed; `Unbound3D.json` version 02.05.01.00 so older copies in settings folders are replaced |
| `resources/profiles/Unbound3D/Unbound3D Hercules_cover.png` | Hercules drawing (made from Unbound's photo) on the printer card |
| `src/slic3r/GUI/UBPrinterPicker.{cpp,hpp}` (new), `resources/web/ubslicer/printer_picker.html` (new), `src/slic3r/GUI/Plater.cpp` (`UBPrinterPicker::open` on the printer box), `src/slic3r/CMakeLists.txt`, `localization/i18n/list.txt` | clicking the printer box opens a picker over the blurred window: animated printer drawings with the name below, instead of the drop-down list |
