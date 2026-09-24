# 220V AC Light Dimmer: Phase-Angle Control via Arduino & TRIAC

This repository contains the firmware and architecture for a hardware-level AC load dimmer. It demonstrates precise phase-angle control of a 220V AC sine wave using an Arduino Nano, an LM393 zero-crossing detector, and an opto-isolated BT136 TRIAC. 

## The Concept: Phase-Angle Control
Unlike DC circuits where you can simply lower the voltage or use rapid Pulse Width Modulation (PWM), AC voltage alternates at a fixed frequency (50Hz in this project). To dim an AC load, we use **Phase-Angle Chopping**. 

Instead of altering the peak voltage, the microcontroller waits for the AC sine wave to cross the 0V threshold. Once 0V is detected, the Arduino waits for a calculated delay (the **Firing Angle** or `alpha`). After this delay, it fires the TRIAC, allowing power to flow to the bulb for the remainder of that half-cycle. 
*   **Long Delay (e.g., 9500µs):** The TRIAC fires at the very end of the wave. The bulb is dim.
*   **Short Delay (e.g., 500µs):** The TRIAC fires immediately after 0V. The bulb is bright.

## ⚙️ How It Works (System Architecture)
The project is strictly divided into a **Low-Voltage Logic Stage** and a **High-Voltage Power Stage**, separated by galvanic isolation to protect the microcontroller.

1.  **Zero-Crossing Detection:** The 220V AC mains voltage is stepped down through high-value resistors (220kΩ) and clamped by anti-parallel 1N4007 diodes. The LM393 comparator monitors this clamped AC wave. Every time the wave crosses 0V, the LM393 outputs a sharp 5V logic pulse.
2.  **Hardware Interrupts:** The Arduino Nano monitors Digital Pin 2 (`INT0`). The instant the LM393 pulse arrives, the Arduino halts its main loop and sets a `volatile` memory flag to true. 
3.  **Delay Calculation:** The Arduino continuously polls a 10kΩ potentiometer on Analog Pin A0. It maps this 0-1023 ADC value to a safe microsecond delay window (constrained between 500µs and 9500µs to avoid misfiring exactly at 0V).
4.  **Opto-Isolated Firing:** Once the delay timer finishes, Arduino Pin 8 sends a 5V signal through a 100Ω current-limiting resistor into a MOC3020 optocoupler. The internal LED of the MOC3020 turns on, bridging the high-voltage gap using light.
5.  **TRIAC Latching:** The optocoupler triggers the gate of the BT136 TRIAC. The TRIAC instantly becomes conductive, powering the bulb until the AC wave naturally crosses 0V again, at which point the TRIAC automatically turns off and waits for the next cycle.
