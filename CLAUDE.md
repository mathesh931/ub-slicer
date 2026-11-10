# CLAUDE.md: UB Slicer

**UB Slicer** is Unbound3D's slicer: OrcaSlicer with Unbound's name, logo and printers (2.3.1 milestone on Orca 2.3.1,
this line on Orca 2.5.0). The owner is **Mathesh** (Unbound3D, GitHub `mathesh931`), first time building software:
explain in plain words with 3D-printer examples, confirm each step before building it, explain GitHub actions.

Rules for this repository:
- Keep changes **small and few**, so newer OrcaSlicer versions can be merged in; `doc/UPDATING_FROM_ORCA.md` lists
  every file UB Slicer changes: update it with each change.
- Licence is AGPL-3.0: keep Orca's licence and credits ("Based on OrcaSlicer"); never put secrets or private Unbound
  code (UBconnect) here. UBconnect lives in `unbound3d-industries/printhub` and stays separate.
- Printers: **Unbound only** (`resources/profiles/Unbound3D`), plus Orca's (empty) `Custom` and `OrcaFilamentLibrary`.
- Look: black and white (no Orca teal); never write "Orca" in anything people see except the About credit; no
  Orca/Bambu cloud.
- Windows and Mac (Apple chip) builds run on GitHub Actions (`.github/workflows/build.yml`); nothing is built locally.

---

OrcaSlicer's own guide to its code follows (still accurate for UB Slicer).

@AGENTS.md
