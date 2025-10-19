# Modular Arduino FSM Framework  
Author: Zane Avan  
Purpose: Reusable Finite State Machine architecture for embedded systems

---

## 1. Overview

This project started as a modular timer and LED control system for Arduino but has grown into a general framework for managing embedded logic using a clean Finite State Machine (FSM) structure.

This architecture can now power more advanced systems — like our Blind Cane Project — by simply swapping modules (for example, using sensors and vibration motors instead of LEDs).

The core idea:  
> The logic stays the same — only the hardware modules change.

---

2. Why Use a Modular FSM?

Arduino projects often become tangled: button logic mixes with output control, timers block other operations, and debugging turns into chaos.  
This framework fixes that by dividing responsibilities into clear modules:

| Module | Responsibility |
|---------|----------------|
| Button | Handles all input presses and debounce logic. |
| Timer | Manages time, alarms, and non-blocking waits. |
| LEDring | Handles output visualization (LED patterns). |
| Manager Algorithm (FSM) | Controls the flow of logic between states. |

Each part is independent, reusable, and replaceable.  
You can test one module without breaking others — a hallmark of scalable embedded design.

---

## 3. File Structure
/src
│
├── button_debounce.h / .cpp → Clean button press & debounce logic
├── clock.h / .cpp → Timer module with non-blocking delay & alarm profiles
├── led_ring.h / .cpp → Output abstraction (visual module)
├── manager_algo.ino → Main FSM logic and state transitions
└── README.md 


## 4. How the FSM Works

The FSM (Finite State Machine) is the brain of the system.  
It defines a fixed number of states and transitions that describe what the system does.

Example (Current System): START → PRIMED → COLOR_SELECT → COUNT_DOWN → ALARM → RESET → START

Each state:
- Runs its own dedicated function.
- Only changes to another state when a specific condition is met (like a button press or timer completion).

For example:

if (button.isShortPressed()) {
    timer.alarm(Timer::PROFILE_I);
    currentState = PRIMED;
}

The LED timer was built as a modular system — each class handles one piece of hardware and one job.
That same modularity allows us to replace any hardware layer while keeping the FSM and timing logic intact.

| Old Module | New Module                           | Purpose                                                  |
| ---------- | ------------------------------------ | -------------------------------------------------------- |
| LEDring    |  Vibration Motor (HapticFeedback)  | Output warning strength or pattern                       |
| Timer      |  Timer (same class)                | Handle timing and vibration intervals                    |
| Button     |  Optional Control Input            | Mode selection or sensitivity setting                    |
| (New)      |  DistanceSensor Interface          | Lidar, Ultrasonic, or Sharp IR input to detect obstacles |


conceptual flow of the FSM:

Distance Sensor  →  FSM Decision Logic  →  Haptic Feedback Output


