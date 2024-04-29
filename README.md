# Overview
The project aims to develop an embedded system for controlling power windows in a vehicle using the Tiva C microcontroller running FreeRTOS. The system includes features such as manual open/close, one-touch auto open/close, window lock, and jam protection.

# Basic Operation
The system utilizes a Finite State Machine (FSM) to manage the control logic, with transitions dependent on various inputs including limit switches, push buttons, and an ON/OFF switch. When the power window switch is operated, the system responds accordingly based on the current state and input conditions.

# System Components
The hardware components required for the project include:
* Tiva C microcontroller
* Top and bottom limit switches
* DC motor for window operation
* Push buttons for window control
* ON/OFF switch for window lock
* Simulink State Machine for control logic modeling

# Finite State Machine (FSM)
The control logic is implemented using a Finite State Machine. The FSM transitions between states based on inputs such as button presses, limit switch activations, and system conditions. The control flow ensures proper operation of the power window system.

# Deliverables
1. Project folder containing code implementation using FreeRTOS features such as queues, semaphores, and Mutex.
2. Documentation report detailing the system design, implementation, and testing procedures.
3. Short video demonstrating the functionality of the system, including test cases and corner cases.
