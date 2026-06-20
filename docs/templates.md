# iSpindHub TFT Template Reference

Templates are JSON files stored in `/data/templates/` on the device filesystem. They describe what to display on the TFT screen each time iSpindel data is received. You can manage templates from **Settings → Display**.

---

## File format

A template is a JSON object whose keys are either `"g"` (global settings) or a line entry (any key starting with `"line"`). Use unique names for line keys — `"line 1"`, `"line 2"`, etc. (JSON does not allow duplicate keys; the last one wins and earlier ones are silently discarded).

```json
{
    "g": { ... },
    "line 1": { ... },
    "line 2": { ... }
}
```

---

## Global settings block (`"g"`)

Applied once at the start, before any lines are drawn.

| Key  | Type    | Description                                      |
|------|---------|--------------------------------------------------|
| `bc` | string  | Background color — fills the entire screen first |
| `fc` | string  | Default font color for all subsequent lines      |
| `tw` | boolean | Text wrap on/off                                 |
| `r`  | integer | Screen rotation: `0`, `1`, `2`, or `3`          |

```json
"g": {
    "bc": "TFT_BLACK",
    "fc": "TFT_WHITE",
    "tw": false,
    "r": 0
}
```

---

## Line types

Each line entry has a `"t"` key that selects the type.

### `"t": "text"` — Draw text

| Key       | Type    | Required | Description |
|-----------|---------|----------|-------------|
| `text`    | string  | yes      | Text to display. Use `#` variables (see below). |
| `x`       | number\|`"MAX"` | yes | Left edge X position. `"MAX"` = screen width. |
| `y`       | number\|`"MAX"` | yes | Top edge Y position. `"MAX"` = screen height. |
| `x_delta` | number  | no       | Added to `x` after `"MAX"` is resolved. Use negative values. |
| `y_delta` | number  | no       | Added to `y` after `"MAX"` is resolved. Use negative values. |
| `ctr`     | boolean | no       | When `true`, centers the text horizontally (ignores `x`). |
| `f`       | string  | no       | Font name (see [Fonts](#fonts)). Empty string uses the current default. |
| `s`       | number  | no       | Text size multiplier (1, 2, 3…). Only used when `f` is empty. |
| `c`       | string  | no       | Text color (see [Colors](#colors)). Overrides the global `fc`. |
| `var`     | string  | no       | Variable to use for conditional coloring (see [Conditional coloring](#conditional-coloring)). |
| `cs`      | array   | no       | Color thresholds — required when `var` is set. Exactly 3 entries. |
| `def_col` | string  | no       | Fallback color when value exceeds all thresholds in `cs`. |

```json
"line 1": {
    "t": "text",
    "f": "SegLight20",
    "c": "TFT_YELLOW",
    "x": 0,
    "y": 6,
    "ctr": true,
    "text": "#SG °P"
}
```

Position relative to screen bottom using `"MAX"` and `y_delta`:

```json
"line 6": {
    "t": "text",
    "f": "",
    "s": 1,
    "c": "TFT_ORANGE",
    "x": 0,
    "y": "MAX",
    "y_delta": -30,
    "ctr": true,
    "text": "#LastSeen"
}
```

---

### `"t": "line"` — Draw a straight line

| Key   | Type            | Required | Description |
|-------|-----------------|----------|-------------|
| `c`   | string          | yes      | Line color  |
| `x_0` | number          | yes      | Start X     |
| `y_0` | number          | yes      | Start Y     |
| `x_1` | number\|`"MAX"` | yes      | End X. `"MAX"` = screen width. |
| `y_1` | number\|`"MAX"` | yes      | End Y. `"MAX"` = screen height. |

```json
"line 2": {
    "t": "line",
    "c": "TFT_CYAN",
    "x_0": 1,
    "y_0": 20,
    "x_1": "MAX",
    "y_1": 20
}
```

---

## Variables

Use these placeholders inside a `"text"` value. They are replaced with live iSpindel data each update.

| Placeholder    | Value                                  |
|----------------|----------------------------------------|
| `#SG`          | Specific gravity                       |
| `#Temp`        | Temperature                            |
| `#Angle`       | Tilt angle (first 5 characters)        |
| `#Voltage`     | Battery voltage (first 4 characters)  |
| `#RSSI`        | WiFi signal strength (dBm)             |
| `#deviceName`  | iSpindel device name                   |
| `#IP`          | iSpindHub IP address (STA interface)   |
| `#LastSeen`    | Seconds since last data from iSpindel  |

---

## Conditional coloring

Add `"var"`, `"cs"`, and `"def_col"` to a `"t": "text"` line to change text color based on a variable's numeric value.

`"cs"` must be an array of **exactly 3** objects in ascending threshold order:

```json
"line 7": {
    "t": "text",
    "f": "",
    "s": 1,
    "var": "#RSSI",
    "cs": [
        {"val": 65, "col": "TFT_GREEN"},
        {"val": 70, "col": "TFT_YELLOW"},
        {"val": 85, "col": "TFT_ORANGE"}
    ],
    "def_col": "TFT_RED",
    "x": 0,
    "y": "MAX",
    "y_delta": -20,
    "ctr": true,
    "text": "Signal: #RSSI dB"
}
```

Logic: if value < `cs[0].val` → `cs[0].col`; else if value < `cs[1].val` → `cs[1].col`; else if value < `cs[2].val` → `cs[2].col`; else → `def_col`.

Supported variables for `"var"`: `#SG`, `#RSSI`, `#Voltage`, `#Temp`, `#Angle`.

> **Note:** `"cs"` must have exactly 3 entries. Fewer will cause undefined behavior.

---

## Colors

| Name            |
|-----------------|
| `TFT_BLACK`     |
| `TFT_WHITE`     |
| `TFT_RED`       |
| `TFT_GREEN`     |
| `TFT_BLUE`      |
| `TFT_YELLOW`    |
| `TFT_CYAN`      |
| `TFT_MAGENTA`   |
| `TFT_ORANGE`    |
| `TFT_LIGHTGREY` |

Any unrecognized color name falls back to `TFT_WHITE`.

---

## Fonts

Font files (`.vlw`) are stored in the root of the LittleFS filesystem. Use these names in the `"f"` field:

| Name          | Style             | Size |
|---------------|-------------------|------|
| `Arial9`      | Arial             | 9px  |
| `Arial12`     | Arial             | 12px |
| `Arial20`     | Arial             | 20px |
| `RThin9`      | Roboto Thin       | 9px  |
| `RThin12`     | Roboto Thin       | 12px |
| `RThin20`     | Roboto Thin       | 20px |
| `RBold20`     | Roboto Bold       | 20px |
| `TMS10`       | Times             | 10px |
| `TMS12`       | Times             | 12px |
| `SegLight20`  | Segment Light     | 20px |

Leave `"f"` as `""` to keep the current font and use `"s"` for a size multiplier instead.

> **Known issues with font names:** `FreeSans9`, `FreeSansGras12`, and `FreeSans12` are defined in the codebase but do not load the expected font files — prefer the fonts in the table above.

---

## Not yet implemented

The following types are parsed but not rendered:

- **`"t": "var_text"`** — text whose content varies by value (stubbed)
- **`"t": "rectangle"`** — filled rectangle (stubbed)

These entries are silently skipped without error.

---

## Full example

```json
{
    "g": {
        "bc": "TFT_BLACK",
        "fc": "TFT_WHITE",
        "tw": false
    },
    "line 1": {
        "t": "text",
        "f": "SegLight20",
        "c": "TFT_YELLOW",
        "x": 0,
        "y": 6,
        "ctr": true,
        "text": "#SG °P"
    },
    "line 2": {
        "t": "text",
        "f": "RThin20",
        "c": "TFT_RED",
        "x": 0,
        "y": 26,
        "ctr": true,
        "text": "T°: #Temp"
    },
    "line 3": {
        "t": "line",
        "c": "TFT_CYAN",
        "x_0": 0,
        "y_0": 50,
        "x_1": "MAX",
        "y_1": 50
    },
    "line 4": {
        "t": "text",
        "f": "",
        "s": 1,
        "var": "#RSSI",
        "cs": [
            {"val": 65, "col": "TFT_GREEN"},
            {"val": 70, "col": "TFT_YELLOW"},
            {"val": 85, "col": "TFT_ORANGE"}
        ],
        "def_col": "TFT_RED",
        "x": 0,
        "y": "MAX",
        "y_delta": -20,
        "ctr": true,
        "text": "Signal: #RSSI dB"
    }
}
```
