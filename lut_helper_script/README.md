# LUT generator for OLED display

This is a simple python script that generates a lookup table for the OLED display based on data from an Excel file.

## Installation

Ensure you have Python 3 and the necessary dependencies installed.

- For Python 3, you can download it from [python.org](https://www.python.org/downloads/)
- Then install the required packages using pip:

```bash
pip install -r requirements.txt
```

## Usage

Run the script by passing the Excel file path as the command-line argument:

```bash
python3 main.py example.xlsx
```

### Walkthrough of Interactive Prompts:

1. **Select worksheet**: Choose which worksheet from the Excel file contains the data.
2. **Select distance reference column**: Choose the column to use as the distance reference (x-axis).
3. **Enter distance reference step**: Specify the step resolution for distance (e.g., `2` for every 2 meters).
4. **Select speed column**: Choose the column containing km/h values.
5. **Clamp speed values**: (Optional) Enter speed bounds (e.g., min speed `10`, max speed `30`).
6. **Select acceleration column**: Choose the column containing the acceleration points.
7. **Copy and paste**: The tool will generate a lookup table based on your selections. Copy the output and paste it into the C code.

## Example Output

```c
#define LUT_DISTANCE_STEP  (uint16_t)2
#define LUT_SIZE           (uint16_t)641
#define LUT_MIN_SPEED      (float)10
#define LUT_MAX_SPEED      (float)30
const float lut_dist_kmh[LUT_SIZE] = {
    10.0000f, 10.0000f, 10.0000f, 11.1443f, 12.8142f, 14.2841f, ...
};
const uint8_t lut_acc[LUT_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, ...
};
```

---

Created by SZEnergy Team for Shell Eco Marathon.

- Váradi Marcell (varma02@GitHub)
