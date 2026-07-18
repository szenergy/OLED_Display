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

        # 3. Read the selected sheet into a DataFrame
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

        print("\nAvailable columns for speed (km/h):")
        for idx, col in enumerate(numeric_cols, 1):
            if col == x_col: continue
            print(f"  [{idx}] {col}{marker}")

        while True:
            choice = input(f"Select speed column (1 - {len(numeric_cols)}): ").strip()
            if not choice:
                continue
            try:
                col_idx = int(choice) - 1
                if 0 <= col_idx < len(numeric_cols):
                    y_col = numeric_cols[col_idx]
                    break
                else:
                    print("Error: Choice out of range.")
            except ValueError:
                print("Error: Please enter a valid number.")

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

        clean_df = df[[x_col, y_col]].copy()
        clean_df[x_col] = pd.to_numeric(clean_df[x_col], errors='coerce')
        clean_df[y_col] = pd.to_numeric(clean_df[y_col], errors='coerce')
        clean_df = clean_df.dropna().sort_values(by=x_col)

        if clean_df.empty:
            print("Error: No valid numeric data points found.")
            sys.exit(1)

        x_min = float(clean_df[x_col].min())
        x_max = float(clean_df[x_col].max())

        x_grid = []
        curr = x_min
        epsilon = step * 1e-6
        while curr <= x_max + epsilon:
            x_grid.append(curr)
            curr += step

        y_grid = np.interp(x_grid, clean_df[x_col], clean_df[y_col])

        if clamp_min is not None:
            y_grid = np.maximum(y_grid, clamp_min)
        if clamp_max is not None:
            y_grid = np.minimum(y_grid, clamp_max)

        y_uint16 = np.clip(np.floor(y_grid + 0.5), 0, 65535).astype(np.uint16)

        array_name = f"lut_{sanitize_c_identifier(y_col)}"
        size = len(y_uint16)

        clamp_min_str = f"{clamp_min}" if clamp_min is not None else "None"
        clamp_max_str = f"{clamp_max}" if clamp_max is not None else "None"

        # Format array elements beautifully, 12 per line
        lines = []
        line_size = 12
        for i in range(0, size, line_size):
            chunk = y_uint16[i:i+line_size]
            chunk_str = ", ".join(str(val) for val in chunk)
            lines.append(f"    {chunk_str}")
        formatted_values = ",\n".join(lines)

        print("\n" + "="*50)
        print("GENERATED LOOKUP TABLE")
        print("="*50)
        print(f"// Source Excel file: {os.path.basename(file_path)}")
        print(f"// Sheet: {selected_sheet}")
        print(f"// X-axis: {x_col} (min: {x_min:.4f}, max: {x_max:.4f}, step: {step})")
        print(f"// Y-axis: {y_col} (clamp range: [{clamp_min_str}, {clamp_max_str}])")
        print(f"// Total Elements: {size}")
        print(f"const uint16_t {array_name}[{size}] = {{")
        print(formatted_values)
        print("};")
        print("="*50)

    except (KeyboardInterrupt, EOFError):
        print("\n\nOperation cancelled by user.")
        sys.exit(0)

if __name__ == "__main__":
    main()
