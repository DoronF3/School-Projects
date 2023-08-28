# Exercise 2: Multithreading and Synchronization

## Thread.cpp

This file (`Thread.cpp`) contains the implementation of the `Thread` class and related functions. The `Thread` class represents a thread object and provides methods for initializing, managing, and interacting with threads.

### Contents

- `Thread::Thread(int id, int priority, State state, void (*f)())`: Constructor for creating a new thread with a given ID, priority, state, and function.
- `Thread::Thread(int id, int priority, State state)`: Constructor for creating a new thread with a given ID, priority, and state.
- `Thread::get_id()`: Method to get the ID of the thread.
- `Thread::get_priority()`: Method to get the priority of the thread.
- `Thread::set_priority(int priority)`: Method to set the priority of the thread.
- `Thread::get_state()`: Method to get the state of the thread.
- `Thread::get_qt_time()`: Method to get the quantum time of the thread.
- `Thread::add_qt_time()`: Method to increment the quantum time of the thread.
- `Thread::set_state(State state)`: Method to set the state of the thread.

## Thread.h

This file (`Thread.h`) contains the header file for the `Thread` class and related declarations.

### Contents

- `enum State`: An enum class representing the possible states of a thread.
- `class Thread`: A class representing the thread object. It contains constructors and methods for managing thread attributes and behavior.

## Uthreads.cpp

This file (`Uthreads.cpp`) contains the implementation of the user-level thread library functions, including thread creation, termination, blocking, resuming, and various utility functions for thread management.

### Contents

- Utility functions for blocking and unblocking signals.
- Functions for managing and switching threads.
- Signal handler function for managing thread scheduling.
- Functions for initializing the thread library (`uthread_init`) and creating threads (`uthread_spawn`).
- Functions for changing thread priorities (`uthread_change_priority`) and terminating threads (`uthread_terminate`).
- Functions for blocking and resuming threads (`uthread_block` and `uthread_resume`).
- Functions for getting thread information, such as thread ID, total quantums, and quantums of a specific thread.

### Usage

To use the provided thread management system, follow these steps:

1. Include the `Thread.cpp` and `Thread.h` files in your project.
2. Include the `Uthreads.cpp` file in your project.
3. Initialize the thread library using `uthread_init` by providing an array of quantum time values for each priority level.
4. Create threads using `uthread_spawn` by passing a function and priority.
5. Use the various thread management functions such as `uthread_terminate`, `uthread_block`, and `uthread_resume` to control thread behavior.

Feel free to explore the contents of each file for more detailed information about the implementation of the thread management system.
