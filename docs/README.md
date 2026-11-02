# Product documentation

## Initial baseline

- Development board: FRDM-MCXN236.
- Zephyr target: `frdm_mcxn236/mcxn236`.
- The repository root is the Zephyr application root; application source is in `app/src/`.
- Firmware provides UART startup messages and a main-thread heartbeat; application GPIO is not enabled.
- Native Zephyr Logging uses deferred UART output through `utils/print_log.h` and the `printLog_E/W/I/D` macros.
- No additional application threads, application heap allocation, bootloader or product-specific flash partitions have been introduced. Deferred logging uses its own Zephyr processing thread.
- Main-thread stack size and priority retain the Zephyr/board defaults.
- `config/debug.conf` enables application DEBUG messages. `config/release.conf` only reduces logging; it is not a production security profile. `config/ota.conf` and `sysbuild.conf` do not enable OTA or MCUboot.

## Dependencies and reproducible builds

The repository's `west.yml` pins Zephyr to commit `5075ddf1460b6a8c658a0d1cbdcdac3d971b661a`, the clean source revision used for the existing build baseline. It is a development commit, not a claim that this project uses a validated stable release. Importing Zephyr's manifest also selects its pinned dependencies, including MCUboot; MCUboot does not need a duplicate project entry here and is not enabled in the firmware by this change.

The manifest pins source revisions, not the complete build environment. The SDK version, Python packages and host tools still need a separately recorded baseline before reproducible releases or CI can be claimed.

Adding `west.yml` does not change an existing workspace's active manifest. The current workspace still has `manifest.path = zephyr`. Check the active selection with `python3 -m west config manifest.path`; `west update` follows that selection, not whichever `west.yml` is nearest the terminal.

To intentionally adopt this manifest in the existing workspace, review local dependency changes first, then run `python3 -m west config manifest.path MCXN236-system`. A subsequent `west update` will use this application's pinned dependency authority. This configuration change is not performed automatically by the project or VS Code task. Do not run `west init` again inside an existing workspace.

For a new workspace, use the setup commands below. The example directory is arbitrary; source and workspace may be placed elsewhere. The repository must remain inside its west workspace for normal west discovery.

```bash
mkdir -p ~/mcxn236-workspace
cd ~/mcxn236-workspace
git clone git@github.com:ThanhDuocWork/MCXN236-system.git MCXN236-system
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install west
python3 -m west init -l MCXN236-system
python3 -m west update
python3 -m pip install -r zephyr/scripts/requirements.txt
python3 -m west zephyr-export
cd MCXN236-system
python3 -m west build -b frdm_mcxn236/mcxn236 . -d build
```

`west` supplies workspace management and build commands; Zephyr's Python requirements supply its build tools. These installation commands are for setting up a new environment, not actions performed by the repository. Ubuntu host build dependencies and a compatible Zephyr SDK must also be available; the commands above do not install or pin them. Cloning through SSH requires GitHub SSH access. `zephyr-export` registers that workspace's Zephyr CMake package for discovery.

## Portable VS Code build task

The task runs `python3 -m west` from `${workspaceFolder}` and uses source/build paths relative to the opened repository. It does not contain a developer's home-directory path or assume a particular parent-directory name.

Activate the environment containing `west` before starting VS Code. For the workspace layout above:

```bash
cd ~/mcxn236-workspace
source .venv/bin/activate
code MCXN236-system
```

The task inherits `PATH` from VS Code. Activating an environment in an integrated terminal after VS Code has started may not change the task process environment. Fully close existing VS Code processes and relaunch from the activated shell if necessary. A repository cloned outside a west workspace must first be placed in/initialized as a west workspace; changing the task's working directory alone cannot supply its dependencies.

Before release, define product requirements, pin the remaining toolchain/environment, add CI, and record hardware verification results. Example output is not runtime evidence.

## Verification tracking

Record build success, flash success and observed runtime behavior separately. For UART tests, retain startup/heartbeat logs together with firmware and hardware revisions.

refer to the documentation:
https://mcuxpresso.nxp.com/mcuxsdk/latest/boards/MCX/frdmmcxn236/
https://documentation.espressif.com/en/documentList?eol=false
ESP-AT User Guide: https://docs.espressif.com/projects/esp-at/en/latest/esp32/
Hardware Connection: https://docs.espressif.com/projects/esp-at/en/latest/esp32/Get_Started/Hardware_connection.html
Downloading Guide https://docs.espressif.com/projects/esp-at/en/latest/esp32/Get_Started/Downloading_guide.html
AT Command Set: https://docs.espressif.com/projects/esp-at/en/latest/esp32/AT_Command_Set/
docs.zephyrproject:
FRDM-MCXN236 zephyr: https://docs.zephyrproject.org/latest/boards/nxp/frdm_mcxn236/doc/
github board FRDM-MCXN236: https://github.com/zephyrproject-rtos/zephyr/tree/main/boards/nxp/frdm_mcxn236
Sysbuild (System build): https://docs.zephyrproject.org/latest/build/sysbuild/
Device Management: https://docs.zephyrproject.org/latest/services/device_mgmt/
Device Firmware Upgrade: https://docs.zephyrproject.org/latest/services/device_mgmt/dfu.html
MCUboot Documentation: https://docs.mcuboot.com/
github AT source: https://github.com/zephyrproject-rtos/zephyr/tree/main/drivers/wifi/esp_at