# Quick commands — FRDM-MCXN236

Run the command blocks from the repository root (the directory containing `west.yml` and `CMakeLists.txt`), with a Python environment containing `west` activated. Source and workspace paths are not hardcoded in the commands.

For the standard layout with `.venv` beside the repository, activate it before building:

```bash
source ../.venv/bin/activate
```

If your environment lives elsewhere, activate that environment instead. New-machine setup and the VS Code environment requirements are described in [the existing product documentation](../docs/README.md). These commands assume the repository is inside an initialized west workspace with its dependencies and toolchain available.

## 1. Update dependencies from the manifest

```bash
python3 -m west update
```

`python3 -m west update` fetches the repositories and revisions specified by the current manifest. It may access the network and change dependency checkouts. It does not automatically advance the Zephyr revision in the manifest to the latest release. It is not required before every build.

The application manifest pins Zephyr and imports its dependency manifest. However, adding this file does not change an existing workspace's active manifest. The original workspace still selects `zephyr` until intentionally changed. Check it before updating:

```bash
python3 -m west config manifest.path
```

For a fresh workspace initialized with `west init -l MCXN236-system`, this prints `MCXN236-system`. To intentionally switch the existing workspace to this repository's manifest, run `python3 -m west config manifest.path MCXN236-system` only after reviewing dependency changes; subsequent updates then follow the application pin. Do not run `west init` again in an existing workspace.

## 2. Build the default INFO profile

```bash
python3 -m west build --pristine=auto -b frdm_mcxn236/mcxn236 \
  . \
  -d build
```

`-b` selects the board, the following path selects the application, and `-d` selects the build directory. `--pristine=auto` allows west to clean an incompatible cached build, including a build made before the application-root migration. A successful build generates `build/zephyr/zephyr.elf` and `zephyr.bin` within the project.

To recreate the complete build configuration, use the following block. `--pristine` cleans the build directory, not source files. `EXTRA_CONF_FILE=` removes a previously cached extra configuration.

```bash
python3 -m west build -b frdm_mcxn236/mcxn236 \
  . \
  -d build --pristine \
  -- -DEXTRA_CONF_FILE=
```

## 3. Build a separate DEBUG profile

```bash
python3 -m west build --pristine=auto -b frdm_mcxn236/mcxn236 \
  . \
  -d build-debug \
  -- -DEXTRA_CONF_FILE=config/debug.conf
```

`--` forwards the following arguments to CMake. `config/debug.conf` enables application DEBUG logging; the results remain separate in `build-debug`.

## 4. Flash through J-Link

Run these commands only when you intend to program the board. Select the profile that you built.

**Flash INFO:**

```bash
python3 -m west flash -r jlink -d build -- --speed 1000
```

**Flash DEBUG:**

```bash
python3 -m west flash -r jlink -d build-debug -- --speed 1000
```

`-r jlink` selects the J-Link runner. `--speed 1000` sets the debug interface speed to 1000 kHz. By default, `python3 -m west flash` checks/rebuilds the selected build before programming. A missing `runners.yaml` or build directory usually means that the selected profile needs to be built first.

## 5. View UART logs

Connect the MCU-LINK USB port at J10, then check the serial mapping:

```bash
ls -l /dev/serial/by-id/
```

Find **NXP MCU-LINK FRDM-MCXN236**, rather than the SEGGER J-Link serial device. The board previously appeared as `/dev/ttyACM0`, but that name can change after reconnecting USB devices. If the `by-id` directory is absent, check the USB connection first.

After confirming that MCU-LINK maps to `/dev/ttyACM0`:

```bash
python3 -m serial.tools.miniterm /dev/ttyACM0 115200
```

If the mapping differs, replace `/dev/ttyACM0` with the correct port or its full path under `/dev/serial/by-id/`.

- Open the terminal, then press **RESET** on the board to capture startup logs.
- Press **Ctrl+]** to exit miniterm.
- `Permission denied`: check the account's serial-port permissions.
- `Device or resource busy`: close other terminals holding the port.

Build success, flash success and correct runtime/log behavior are separate verification steps. This file only stores commands for copying; it does not execute updates, builds or flashing.
