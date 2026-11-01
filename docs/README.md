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

This project currently uses the Zephyr and module checkouts in the parent workspace. It does not yet have its own `west.yml`, pinned dependency revisions or a pinned SDK version. This scaffold is not a production release baseline.

Before release, define product requirements, pin dependencies and the toolchain, add CI, and record hardware verification results. Example output is not runtime evidence.

## Verification tracking

Record build success, flash success and observed runtime behavior separately. For UART tests, retain startup/heartbeat logs together with firmware and hardware revisions.
