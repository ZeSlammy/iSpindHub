# iSpindHub — TODO & Testing Checklist

---

## Feature backlog

| Status | Feature |
|--------|---------|
| ✅ | Home page showing info about connected iSpindels |
| ✅ | Configuration page — URL target |
| ✅ | Configuration page — Brewfather |
| ✅ | Configuration page — Fermentrack |
| ✅ | Configuration page — BierBot Bricks |
| ✅ | Configuration page — AP name & passphrase |
| ✅ | Jobs: push data to Brewfather, Fermentrack, BierBot |
| ✅ | Handle multiple screen sizes through separate build envs |
| ✅ | Choose display template from settings (no recompile needed) |
| ✅ | Upload a new display template from settings |
| ✅ | Change colors / layout without recompiling (template system) |
| ✅ | Timezone + manual DST offset (requires reboot) |
| ✅ | Handle missing template JSON gracefully (no WDT crash) |
| ⬜ | Configuration page — LittleBock (partially wired, needs testing) |
| ⬜ | BrewPiLess as a push target |
| ⬜ | Page to manage log files (delete / trim) |
| ⬜ | Historical gravity graphs |
| ⬜ | Config persistence across firmware flashes (without re-uploading FS) |
| ⬜ | Log file download from web UI (endpoint `/download` exists, no UI) |

---

## Hardware testing checklist

Flash each env: `pio run -e <env> -t upload` then `pio run -e <env> -t uploadfs`.

### Bug fixes (merged in PR #14 / #15 / #16 / #17)

#### #13 Root serves "Hello World" instead of web UI
- [ ] Open `http://<hub-ip>/` — dashboard loads (not plain "Hello World")

#### #12 Version mismatch
- [ ] Open `http://<hub-ip>/thisVersion/` — confirm `"version": "0.0.9"`

#### #9 ILI9341 white screen (2.4" display)
- [ ] Flash `2_4_inches_ILI9341` — display shows iSpindel data
- Note: if colours are inverted, remove `-DTFT_INVERSION_ON=1` from that env and reflash

#### WDT crash on startup
- [ ] Hub boots cleanly (no `rst cause:4` in serial monitor)
- [ ] `/iSpindInfo/` loads without panic even when no iSpindel data has arrived yet

### New features (merged in PR #14)

#### #7 AP WiFi password
- [ ] Settings → Advanced → Access Point: enter passphrase (8–63 chars), save, reboot
- [ ] `iSpindHub` network now prompts for passphrase
- [ ] Set passphrase back to empty → AP returns to open

#### #6 DST offset
- [ ] Settings → iSpindHub: set DST to **+1 hour**, click Update
- [ ] Timestamps on `/iSpindInfo/` or display are 1 hour ahead of base TZ
- [ ] Set back to **No DST** → timestamps return to base offset

#### #8 Fermentrack push
- [ ] Settings → Targets → Fermentrack: enter base URL + frequency, save
- [ ] Wait for push interval — gravity reading appears in Fermentrack

#### #2 BierBot Bricks push
- [ ] Settings → Targets → BierBot: enter API key + frequency, save
- [ ] Wait for push interval — telemetry appears in BierBot app

### Template management (merged in PR #19)

#### Template selection
- [ ] Settings → Display: dropdown shows available templates (e.g. `korev`)
- [ ] Select a different template, click Update
- [ ] On next iSpindel data receive, display switches to the new template
- [ ] Selection persists after reboot

#### Template upload
- [ ] Settings → Display: choose a `.json` file, click Upload
- [ ] File appears in the dropdown immediately after upload
- [ ] New template can be selected and applied

---

## Issues to close after hardware testing

| Issue | Resolution |
|-------|-----------|
| #13 | Fixed — removed dead `server.on("/")` handler |
| #12 | Fixed — version bumped to 0.0.9 |
| #9  | Fixed — pinned TFT_eSPI + ILI9341 colour flags |
| #7  | Fixed — AP passphrase wired in, settings UI added |
| #6  | Fixed — manual DST offset field added |
| #8  | Fixed — Fermentrack push target implemented |
| #2  | Fixed — BierBot Bricks target implemented |

## Issues to close with a comment (no code)

- **#11 (ESP-Now)**: Requires coordinated firmware changes in the iSpindel itself — can't be done unilaterally in the hub. Mark as `enhancement / help wanted`.
- **#4 (Repeater question)**: Hub already operates in WIFI_AP_STA mode. Answer the question and close.
