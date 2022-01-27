#ifndef OS_EX2_THREAD_H
#define OS_EX2_THREAD_H

#include <setjmp.h>
#include <signal.h>
#include "uthreads.h"

/**
 * An enum class that represents the possible states of a thread.
 */
enum State {
    ready, blocked, running
};

/**
 * A class for the thread object.
 */
class Thread {
public:

    /** A variable that represents the environment of a given thread.  */
    sigjmp_buf _buf;

    /** A variable that represents the stack of a given thread. */
    char _stack [STACK_SIZE];

    /**
     * A C'tor for the thread object other then the main one.
     * @param id The thread's id.
     * @param priority The thread's priority.
     * @param state The thread's state.
     * @param f The thread's function.
     */
    Thread(int id, int priority, State state, void (*f)());

    /**
     * A C'tor for the main thread.
     * @param id The thread's id.
     * @param priority The thread's priority.
     * @param state The thread's state.
     */
    Thread(int id, int priority, State state);

    /**
     * A method that returns the id of the given thread.
     * @return The id of the given thread.
     */
    int get_id() const { return _id; };

    /**
     * A method that returns the priority of the thread.
     * @return The priority of the thread.
     */
    int get_priority() const { return _priority; };

    /**
     * A method that changes the priority of the thread to the given priority.
     * @param priority The given priority to change to.
     */
    void set_priority(int priority) { _priority = priority; };

    /**
     * A method that returns the state of the thread.
     * @return The state of the thread.
     */
    State get_state() const { return _state; };

    /**
     * A method that returns the quantum time of the thread.
     * @return The quantum time of the thread.
     */
    int get_qt_time() const { return quantum_time; };

    /**
     * A method that increments the quantum time of the thread.
     */
    void add_qt_time() { quantum_time++; };

    /**
     * A method that changes the state of the thread to the given state.
     * @param state The given state to change to.
     */
    void set_state(State state) { _state = state; };

private:

    int _id, _priority, quantum_time;
    State _state;
    void (*_f)();
};

#endif //OS_EX2_THREAD_H
