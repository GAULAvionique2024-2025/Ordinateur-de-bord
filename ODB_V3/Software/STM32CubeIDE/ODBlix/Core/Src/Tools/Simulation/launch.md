Tu peux copier-coller ceci directement :

***

# SITL Flight Simulator Guide

This guide explains how to compile and run the Software-In-The-Loop (SITL) simulator. This tool allows you to test the rocket's Flight State Machine (FSM) using simulated flight profiles from OpenRocket, completely independently from the STM32 hardware.

## 1. Prerequisites

- **Compiler**: You must have `gcc` installed on your computer (via MSYS2/MinGW on Windows, or the default GCC on Linux/macOS).
- **Terminal Location**: Open your terminal (Git Bash, MSYS2, or PowerShell) and navigate to the **root of your project** (the `ODBlix` directory).
- **Flight Data**: Ensure your OpenRocket CSV export files are placed in the dedicated data folder:
  - `Core/Src/Tools/Simulation/data/booster_data.csv`
  - `Core/Src/Tools/Simulation/data/sustainer_data.csv`
- **Configuration Files**: The simulator automatically loads flight parameters from the config directory. Make sure these exist:
  - `Core/Src/Tools/Simulation/Config/booster.cfg`
  - `Core/Src/Tools/Simulation/Config/sustainer.cfg`

## 2. Compiling the Simulator

The simulator uses a "Mock" hardware layer (located in the `Mocks/` folder). This tricks the compiler into thinking the STM32 registers exist, allowing your embedded C code to compile smoothly on a standard PC.

Run the following command from the **project root (`ODBlix/`)** to compile the simulator:

```bash
gcc Core/Src/Tools/Simulation/flight_sitl.c Core/Src/Systems/flight_fsm.c Core/Src/Systems/config.c Core/Src/Drivers/LowLevel/kalman_nav.c -I Core/Inc -I Core/Src/Tools/Simulation/Mocks -Wno-address-of-packed-member -lm -o Core/Src/Tools/Simulation/simulation.exe
```

## 3. Running the Simulation

Once compiled, run the executable and pass your flight data CSV file as an argument. You must run these commands from the root of the project. 

*Note: The simulator uses a "Smart Finder" to automatically detect and load the correct `.cfg` file based on the CSV filename (it looks for the word "sustainer").*

**To simulate the Booster flight profile:**
```bash
./Core/Src/Tools/Simulation/simulation.exe Core/Src/Tools/Simulation/data/booster_data.csv
```

**To simulate the Sustainer flight profile:**
```bash
./Core/Src/Tools/Simulation/simulation.exe Core/Src/Tools/Simulation/data/sustainer_data.csv
```

**(Optional) Override the Configuration File:**
If you want to test a custom configuration file, you can force the simulator to use it by providing its path as a second argument:
```bash
./Core/Src/Tools/Simulation/simulation.exe Core/Src/Tools/Simulation/data/my_data.csv Core/Src/Tools/Simulation/Config/my_custom_config.cfg
```

## 4. Viewing the Results

If the execution is successful, the terminal will print the loaded configuration path and the location of the generated report, like this:
```text
>>> Configuration chargee avec succes depuis : Core/Src/Tools/Simulation/Config/booster.cfg
>>> Rapport CI genere : Core/Src/Tools/Simulation/rapport_vol.html