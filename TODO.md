# iSpindHub — Testing TODO (PR #14)

Branch: `fix/phase1-bugs`  
PR: https://github.com/ZeSlammy/iSpindHub/pull/14

Flash each env with `pio run -e <env> -t upload` and `pio run -e <env> -t uploadfs`.

---

## Phase 1 — Bug fixes

### #13 Root serves "Hello World" instead of web UI
- [ ] Flash any env
- [ ] Open `http://<hub-ip>/` in a browser
- **Pass**: dashboard loads (not plain "Hello World")

### #12 Version mismatch
- [ ] Open `http://<hub-ip>/thisVersion/` — confirm `"version": "0.0.9"`

### #9 ILI9341 white screen (2.4" display)
- [ ] Flash `2_4_inches_ILI9341` env
- **Pass**: display shows iSpindel data (SG, temp, battery, etc.)
- **If colours are inverted**: remove `-DTFT_INVERSION_ON=1` from `platformio.ini` env and reflash

---

## Phase 2 — New features

### #7 AP WiFi password
- [ ] Go to **Settings → Advanced → Access Point**
- [ ] Enter a passphrase (8–63 chars) and save
- [ ] Reboot the hub
- **Pass**: the `iSpindHub` WiFi network is now WPA2 protected — phones/laptops prompt for the passphrase
- Edge case: set passphrase back to empty → AP returns to open (no password)

### #6 DST offset
- [ ] Go to **Settings → iSpindHub**, set DST to **+1 hour**, click Update
- [ ] Check `/iSpindInfo/` or the display — timestamps should be 1 hour ahead of UTC+TZ
- [ ] Set DST back to **No DST** — timestamps return to standard offset
- Note: this is a *manual* toggle. Change it twice a year (spring forward / fall back).

### #8 Fermentrack push
- [ ] Go to **Settings → Targets → Fermentrack**
- [ ] Enter your Fermentrack base URL (e.g. `http://fermentrack.local`) and frequency (e.g. 15 min)
- [ ] Wait for the push interval to expire
- **Pass**: gravity reading appears in Fermentrack under the iSpindel device
- Edge case: empty URL → push is silently skipped (no crash)

### #2 BierBot Bricks push
- [ ] Go to **Settings → Targets → BierBot**
- [ ] Enter your BierBot API key and frequency
- [ ] Wait for the push interval
- **Pass**: telemetry appears in BierBot app for the device
- Note: uses HTTPS with no certificate validation (`setInsecure()`). If BierBot returns non-200, check the Serial monitor for the status line.

---

## Issues to close after testing

Once hardware-confirmed, close these with the linked PR:

| Issue | How to close |
|-------|--------------|
| #13 | Close — fixed by removing dead `server.on("/")` handler |
| #12 | Close — version bumped, dead code removed |
| #9  | Close — pin TFT_eSPI + ILI9341 flags; note inversion toggle |
| #7  | Close — AP passphrase wired in, settings UI added |
| #6  | Close — manual DST offset field added |
| #8  | Close — Fermentrack push target implemented |
| #2  | Close — BierBot Bricks target implemented |

## Issues to close with a comment (no code needed)

- **#11 (ESP-Now)**: Good idea, but requires coordinated firmware changes in the iSpindel itself — can't be done unilaterally in the hub. Mark as `enhancement / help wanted`.
- **#4 (Repeater question)**: The hub already operates in WIFI_AP_STA mode (repeater). Littlebock/generic URL targets handle external software forwarding. Answer the question and close.
