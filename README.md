# Task Scheduler Module Based On Timer

This module provides a lightweight cooperative task scheduler based on a fixed 1ms system tick.
It manages multiple periodic tasks at predefined frequencies (1Hz to 200Hz) using a simple
flag mechanism and user-registered callback functions.

# Features:
- Fixed frequency task execution: 1Hz, 2Hz, 5Hz, 10Hz, 20Hz, 50Hz, 100Hz, 200Hz
- Efficient tick-based scheduling with minimal overhead
- Task overflow detection (missed execution)
- Flexible handler registration using function pointers

# Usage:
- Call `task_tick()` from a 1ms tick interrupt or timer
- Call `task_handler()` periodically from the main loop
- Use `task_register_handle()` to assign handlers for each task frequency
- Monitor execution reliability with `task_overflow_count`

# Limitations:
- Only one handler per task frequency (no multi-handler support)
- Tasks must execute quickly to avoid flag overflows
