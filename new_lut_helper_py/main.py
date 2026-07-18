#!/usr/bin/env python3
import sys
import os
import re
import pandas as pd
import numpy as np

def main():
    if len(sys.argv) != 2:
        print("Error: Invalid arguments.")
        print(f"Usage: python3 {os.path.basename(sys.argv[0])} <excel_file.xlsx>")
        sys.exit(1)

    file_path = sys.argv[1]
    if not os.path.exists(file_path):
        print(f"Error: File '{file_path}' does not exist.")
        sys.exit(1)

    print(f"Loading Excel file: {file_path} ...")
    try:
        xl = pd.ExcelFile(file_path)
        sheets = xl.sheet_names
    except Exception as e:
        print(f"Error reading Excel file: {e}")
        sys.exit(1)

    if not sheets:
        print("Error: No sheets found in the Excel file.")
        sys.exit(1)

    print("\nAvailable worksheets:")
    for idx, sheet in enumerate(sheets, 1):
        print(f"  [{idx}] {sheet}")

    try:
        while True:
            choice = input(f"Select worksheet (1 - {len(sheets)}): ").strip()
            if not choice:
                continue
            try:
                sheet_idx = int(choice) - 1
                if 0 <= sheet_idx < len(sheets):
                    selected_sheet = sheets[sheet_idx]
                    break
                else:
                    print("Error: Choice out of range.")
            except ValueError:
                print("Error: Please enter a valid number.")

        # Read the selected sheet into a DataFrame
        print(f"\nReading worksheet '{selected_sheet}'...")
        df = pd.read_excel(xl, sheet_name=selected_sheet)

        # Filter columns to only those that contain at least some numeric values
        numeric_cols = []
        for col in df.columns:
            try:
                converted = pd.to_numeric(df[col], errors='coerce')
                if converted.notna().any():
                    numeric_cols.append(col)
            except Exception:
                pass

        if not numeric_cols:
            print("Error: No columns with numeric values found in the selected worksheet.")
            sys.exit(1)

        # Select X-axis column (Distance reference)
        print("\nAvailable columns for distance reference:")
        for idx, col in enumerate(numeric_cols, 1):
            print(f"  [{idx}] {col}")

        while True:
            choice = input(f"Select distance reference column (1 - {len(numeric_cols)}): ").strip()
            if not choice:
                continue
            try:
                col_idx = int(choice) - 1
                if 0 <= col_idx < len(numeric_cols):
                    x_col = numeric_cols[col_idx]
                    break
                else:
                    print("Error: Choice out of range.")
            except ValueError:
                print("Error: Please enter a valid number.")

        while True:
            step_input = input("Enter distance reference step: ").strip()
            if not step_input:
                continue
            try:
                step = float(step_input)
                if step <= 0:
                    print("Error: Step must be a positive number.")
                else:
                    break
            except ValueError:
                print("Error: Please enter a valid numeric value.")

        # Select Y1-axis column (Speed)
        available_speed_cols = [col for col in numeric_cols if col != x_col]
        print("\nAvailable columns for speed (km/h):")
        for idx, col in enumerate(available_speed_cols, 1):
            print(f"  [{idx}] {col}")

        while True:
            choice = input(f"Select speed column (1 - {len(available_speed_cols)}): ").strip()
            if not choice:
                continue
            try:
                col_idx = int(choice) - 1
                if 0 <= col_idx < len(available_speed_cols):
                    y1_col = available_speed_cols[col_idx]
                    break
                else:
                    print("Error: Choice out of range.")
            except ValueError:
                print("Error: Please enter a valid number.")

        # Speed clamping options
        clamp_min = None
        clamp_max = None
        while True:
            clamp_choice = input("\nDo you want to clamp the speed values? (y/N): ").strip().lower()
            if clamp_choice in ['y', 'yes']:
                while True:
                    min_input = input("Enter minimum speed (Enter to skip): ").strip()
                    if not min_input:
                        break
                    try:
                        clamp_min = float(min_input)
                        break
                    except ValueError:
                        print("Error: Please enter a valid numeric value.")
                while True:
                    max_input = input("Enter maximum speed (Enter to skip): ").strip()
                    if not max_input:
                        break
                    try:
                        clamp_max = float(max_input)
                        if clamp_min is not None and clamp_max < clamp_min:
                            print("Error: Maximum value must be >= minimum value.")
                        else:
                            break
                    except ValueError:
                        print("Error: Please enter a valid numeric value.")
                break
            elif clamp_choice in ['n', 'no', '']:
                break
            else:
                print("Error: Please answer 'y' or 'n'.")

        # Select Y2-axis column (Secondary / Acceleration)
        available_sec_cols = [col for col in numeric_cols if col != x_col]
        print("\nAvailable columns for acceleration points (uint8):")
        for idx, col in enumerate(available_sec_cols, 1):
            print(f"  [{idx}] {col}")

        while True:
            choice = input(f"Select acceleration column (1 - {len(available_sec_cols)}): ").strip()
            if not choice:
                continue
            try:
                col_idx = int(choice) - 1
                if 0 <= col_idx < len(available_sec_cols):
                    y2_col = available_sec_cols[col_idx]
                    break
                else:
                    print("Error: Choice out of range.")
            except ValueError:
                print("Error: Please enter a valid number.")

        # Get overall distance reference bounds
        x_values = pd.to_numeric(df[x_col], errors='coerce').dropna()
        if x_values.empty:
            print("Error: No valid numeric distance reference data found.")
            sys.exit(1)
        x_min = float(x_values.min())
        x_max = float(x_values.max())

        # Generate distance grid
        x_grid = []
        curr = x_min
        epsilon = step * 1e-6
        while curr <= x_max + epsilon:
            x_grid.append(curr)
            curr += step

        size = len(x_grid)

        # 1. Process Speed (float)
        clean_df1 = df[[x_col, y1_col]].copy()
        clean_df1[x_col] = pd.to_numeric(clean_df1[x_col], errors='coerce')
        clean_df1[y1_col] = pd.to_numeric(clean_df1[y1_col], errors='coerce')
        clean_df1 = clean_df1.dropna().sort_values(by=x_col)

        if clean_df1.empty:
            print(f"Error: No valid numeric data found for speed column '{y1_col}'.")
            sys.exit(1)

        y_grid_speed = np.interp(x_grid, clean_df1[x_col], clean_df1[y1_col])

        if clamp_min is not None:
            y_grid_speed = np.maximum(y_grid_speed, clamp_min)
        if clamp_max is not None:
            y_grid_speed = np.minimum(y_grid_speed, clamp_max)

        # 2. Process Secondary (uint8)
        clean_df2 = df[[x_col, y2_col]].copy()
        clean_df2[x_col] = pd.to_numeric(clean_df2[x_col], errors='coerce')
        clean_df2[y2_col] = pd.to_numeric(clean_df2[y2_col], errors='coerce')
        clean_df2 = clean_df2.dropna().sort_values(by=x_col)

        if clean_df2.empty:
            print(f"Error: No valid numeric data found for secondary column '{y2_col}'.")
            sys.exit(1)

        y_grid_sec = np.interp(x_grid, clean_df2[x_col], clean_df2[y2_col])

        y_uint8 = np.clip(np.floor(y_grid_sec + 0.5), 0, 255).astype(np.uint8)

        # Format arrays beautifully (12 elements per line)
        line_size = 12

        speed_lines = []
        for i in range(0, size, line_size):
            chunk = y_grid_speed[i:i+line_size]
            chunk_str = ", ".join(f"{val:.4f}f" for val in chunk)
            speed_lines.append(f"    {chunk_str}")
        formatted_speed = ",\n".join(speed_lines)

        sec_lines = []
        for i in range(0, size, line_size):
            chunk = y_uint8[i:i+line_size]
            chunk_str = ", ".join(str(val) for val in chunk)
            sec_lines.append(f"    {chunk_str}")
        formatted_sec = ",\n".join(sec_lines)

        # Setup macro value for step
        if step.is_integer():
            step_macro_val = f"(uint16_t){int(step)}"
        else:
            step_macro_val = f"{step}f"

        # Generate header descriptions
        clamp_min_str = f"{clamp_min}" if clamp_min is not None else "None"
        clamp_max_str = f"{clamp_max}" if clamp_max is not None else "None"

        print("\n" + "="*50)
        print("GENERATED LOOKUP TABLE")
        print("="*50)
        print(f"// Source Excel file: {os.path.basename(file_path)}")
        print(f"// Sheet: {selected_sheet}")
        print(f"// Distance column: {x_col} (min: {x_min:.4f}, max: {x_max:.4f}, step: {step})")
        print(f"// Speed (km/h) column: {y1_col} (clamp range: [{clamp_min_str}, {clamp_max_str}])")
        print(f"// Acceleration column: {y2_col}")
        print(f"// Array length: {size}")
        print(f"#define LUT_DISTANCE_STEP  {step_macro_val}")
        print(f"#define LUT_SIZE           (uint16_t){size}")
        print(f"const float lut_dist_kmh[LUT_SIZE] = {{")
        print(formatted_speed)
        print("};")
        print(f"const uint8_t lut_acc[LUT_SIZE] = {{")
        print(formatted_sec)
        print("};")
        print("="*50)

    except (KeyboardInterrupt, EOFError):
        print("\n\nOperation cancelled by user.")
        sys.exit(0)

if __name__ == "__main__":
    main()
