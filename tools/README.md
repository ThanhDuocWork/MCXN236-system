# Quick commands — FRDM-MCXN236

Each block can be copied independently into an Ubuntu terminal. These commands use the existing workspace and `.venv`; no additional tools need to be installed.

## 1. Update dependencies from the manifest

```bash
cd ~/zephyrproject
source .venv/bin/activate
west update
```

`west update` fetches the repositories and revisions specified by the current manifest. It may access the network and change dependency checkouts. It does not automatically advance the Zephyr revision in the manifest to the latest release. It is not required before every build.

## 2. Build the default INFO profile

```bash
cd ~/zephyrproject
source .venv/bin/activate
west build --pristine=auto -b frdm_mcxn236/mcxn236 \
  MCXN236-system \
  -d MCXN236-system/build
```

`-b` selects the board, the following path selects the application, and `-d` selects the build directory. `--pristine=auto` allows west to clean an incompatible cached build, including a build made before the application-root migration. A successful build generates `build/zephyr/zephyr.elf` and `zephyr.bin` within the project.

To recreate the complete build configuration, use the following block. `--pristine` cleans the build directory, not source files. `EXTRA_CONF_FILE=` removes a previously cached extra configuration.

```bash
cd ~/zephyrproject
source .venv/bin/activate
west build -b frdm_mcxn236/mcxn236 \
  MCXN236-system \
  -d MCXN236-system/build --pristine \
  -- -DEXTRA_CONF_FILE=
```

## 3. Build a separate DEBUG profile

```bash
cd ~/zephyrproject
source .venv/bin/activate
west build --pristine=auto -b frdm_mcxn236/mcxn236 \
  MCXN236-system \
  -d MCXN236-system/build-debug \
  -- -DEXTRA_CONF_FILE=config/debug.conf
```

`--` forwards the following arguments to CMake. `config/debug.conf` enables application DEBUG logging; the results remain separate in `build-debug`.

## 4. Flash through J-Link

Run these commands only when you intend to program the board. Select the profile that you built.

**Flash INFO:**

```bash
cd ~/zephyrproject
source .venv/bin/activate
west flash -r jlink -d MCXN236-system/build -- --speed 1000
```

**Flash DEBUG:**

```bash
cd ~/zephyrproject
source .venv/bin/activate
west flash -r jlink -d MCXN236-system/build-debug -- --speed 1000
```

`-r jlink` selects the J-Link runner. `--speed 1000` sets the debug interface speed to 1000 kHz. By default, `west flash` checks/rebuilds the selected build before programming. A missing `runners.yaml` or build directory usually means that the selected profile needs to be built first.

## 5. View UART logs

Connect the MCU-LINK USB port at J10, then check the serial mapping:

```bash
ls -l /dev/serial/by-id/
```

Find **NXP MCU-LINK FRDM-MCXN236**, rather than the SEGGER J-Link serial device. The board previously appeared as `/dev/ttyACM0`, but that name can change after reconnecting USB devices. If the `by-id` directory is absent, check the USB connection first.

After confirming that MCU-LINK maps to `/dev/ttyACM0`:

```bash
cd ~/zephyrproject
source .venv/bin/activate
python -m serial.tools.miniterm /dev/ttyACM0 115200
```

If the mapping differs, replace `/dev/ttyACM0` with the correct port or its full path under `/dev/serial/by-id/`.

- Open the terminal, then press **RESET** on the board to capture startup logs.
- Press **Ctrl+]** to exit miniterm.
- `Permission denied`: check the account's serial-port permissions.
- `Device or resource busy`: close other terminals holding the port.

Build success, flash success and correct runtime/log behavior are separate verification steps. This file only stores commands for copying; it does not execute updates, builds or flashing.
