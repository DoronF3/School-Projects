#include "uthreads.h"
#include "Thread.h"
#include <iostream>
#include <list>
#include <setjmp.h>
#include <map>
#include <vector>
#include <sys/time.h>

#define MICRO_TO_SECONDS 1000000
#define FAILURE -1
#define SYS_ERR "system error: "
#define LIB_ERR "thread library error: "

/** A pointer to the time array. */
static std::vector<int> time_arr;

static std::vector<Thread *> to_delete;

/** The id of the current running thread. */
static int trun_id = 0;

/** A counter to the quantums passed. */
static int total_quantums = 0;

/** A list for the ready threads. */
static std::list<Thread *> ready_list;

/** A Map matching id to thread. */
static std::map<int, Thread *> all_threads;

/** A struct for the sigaction functions. */
static struct sigaction sa;

/** A struct for the timer. */
static struct itimerval timer;

/** Function for blocking SIGVTALRM signal. */
inline void block() { sigprocmask(SIG_BLOCK,&sa.sa_mask, nullptr); }

/** Function for unblocking SIGVTALRM signal. */
inline void unblock() { sigprocmask(SIG_UNBLOCK,&sa.sa_mask, nullptr); }

/**
 * A function that deletes the threads from the map.
 */
void delete_resources() {
    for (int i = 1; i < MAX_THREAD_NUM; ++i) {
        delete all_threads[i];
    }
    delete all_threads[0];
}

/**
 * A function that sets timer to the the given thread by id,
 * and the priority as given.
 * @param id The given thread id to set the time.
 */
void set_timer(int id) {
    int t = time_arr[all_threads[id]->get_priority()];
    timer.it_value.tv_usec = t % MICRO_TO_SECONDS;
    timer.it_value.tv_sec = t / MICRO_TO_SECONDS;
    int ret_val = setitimer(ITIMER_VIRTUAL, &timer, nullptr);
    if (ret_val == FAILURE) {
        std::cerr << SYS_ERR << "itimer failure" << std::endl;
        delete_resources();
        exit(1);
    }
}

/**
 * Running the next thread from the ready list, adds qt time, total quantums,
 * and sets alarm timer.
 * @param id of thread to run.
 */

void run_thread(int id) {
    all_threads[id]->set_state(running);
    ready_list.pop_front();
    trun_id = all_threads[id]->get_id();
    all_threads[id]->add_qt_time();
    total_quantums++;
    set_timer(trun_id);
}

/**
 * A method that switches the threads when a thread blocks or terminates itself.
 * @param deleted A flag to see if there are any threads to delete.
 */
void switch_threads(bool deleted = false) {
    if (!deleted) {
        int ret_val = sigsetjmp(all_threads[trun_id]->_buf, 1);
        if (ret_val == 1) {
            return;
        }
    }
    run_thread(ready_list.front()->get_id());
    siglongjmp(all_threads[trun_id]->_buf, 1);
}

/**
 * A function that acs as the handler for the SIGVTALRM.
 * @param sigNum The signal number given.
 */
void catch_ALRM(int) {
    ready_list.push_back(all_threads[trun_id]);
    all_threads[trun_id]->set_state(ready);
    if (!to_delete.empty()) {
        delete to_delete.front();
        to_delete.clear();
    }
    switch_threads();
}

/**
 * Generates id based on the next free thread id available.
 * if Max_THREAD_NUM is reached returns -1.
 * @return The id between 1-MAX_THREAD_NUM.
 */
int id_generator() {
    for (auto &all_thread : all_threads) {
        if (all_thread.second == nullptr) {
            return all_thread.first;
        }
    }
    return -1;
}

/**
 * A function that initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * an array of the length of a quantum in micro-seconds for each priority.
 * It is an error to call this function with an array containing non-positive integer.
 * @param quantum_usecs An array of given numbers.
 * @param size The size of the array.
 * @return On success 0, On failure -1.
*/
int uthread_init(int *quantum_usecs, int size) {
    if (size <= 0) // Check valid size.
    {
        std::cerr << LIB_ERR << "inappropriate size." << std::endl;
        return -1;
    }
    for (int i = 0; i < size; i++) // Check valid time.
    {
        if (quantum_usecs[i] <= 0) {
            std::cerr << LIB_ERR << "inappropriate time." << std::endl;
            return -1;
        }
        time_arr.push_back(quantum_usecs[i]);
    }
    total_quantums++;
    for (int i = 0; i < MAX_THREAD_NUM; ++i) {
        all_threads[i] = nullptr;
    }

    // sigaction init
    sa.sa_handler = &catch_ALRM;
    if (sigaction(SIGVTALRM, &sa, nullptr) < 0) {
        std::cerr << SYS_ERR << "sigaction failure" << std::endl;
        delete_resources();
        exit(1);
    }
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGVTALRM);

    // timer init
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;

    //init thread 0
    auto main = new Thread(0, 0, running);
    int ret_val = sigsetjmp(main->_buf, 1);
    trun_id = 0;
    all_threads[0] = main;
    set_timer(0);
    if (ret_val == 0) {
        siglongjmp(main->_buf, 1);
    }
    main->add_qt_time();
    return 0;
}

/**
 * A function that creates a new thread, whose entry point is the
 * function f with the signature void f(void). The thread is added to the end
 * of the READY threads list. The uthread_spawn function should fail if it
 * would cause the number of concurrent threads to exceed the limit
 * (MAX_THREAD_NUM). Each thread should be allocated with a stack of size
 * STACK_SIZE bytes.
 * @param f A given function.
 * @param priority - The priority of the new thread.
 * @return On success the ID of the created thread, On failure -1.
*/
int uthread_spawn(void (*f)(void), int priority) {
    if (priority < 0 || (int) time_arr.size() <= priority) // Check that priority is in bounds.
    {
        std::cerr << LIB_ERR << "invalid priority." << std::endl;
        return -1;
    }
    int ID = id_generator();
    if (ID == -1) // Check size of map.
    {
        std::cerr << LIB_ERR << "maximum amount of threads reached." << std::endl<< std::flush;
        return -1;
    }
    block();
    auto tmp = new Thread(ID, priority, ready, f);
    all_threads[ID] = tmp;
    ready_list.push_back(tmp);
    unblock();
    return ID;
}

/**
 * A function that checks that the given tid is valid.
 * @param tid The given thread ID to check.
 * @return 0 if the tid is valid, otherwise -1.
 */
int check_tid(int tid) {
    if (tid < 0 || tid >= MAX_THREAD_NUM) // Check tid is in bounds.
    {
        std::cerr << LIB_ERR << "invalid thread ID." << std::endl;
        return -1;
    }
    if (all_threads[tid] == nullptr) // Check thread with given id exists.
    {
        std::cerr << LIB_ERR << "thread with given ID does not exists." << std::endl;
        return -1;
    }
    return 0;
}

/**
 * A function that changes the priority of the thread with ID tid.
 * If this is the current running thread, the effect should take place only the
 * next time the thread gets scheduled.
 * @param tid The given thread ID to check.
 * @param priority The given number to set.
 * @return On success 0, On failure -1.
*/
int uthread_change_priority(int tid, int priority) {
    if (check_tid(tid) == -1) // Check tid.
    {
        return -1;
    }
    if (priority < 0 || priority >= (int) time_arr.size()) // Check that priority is in bounds.
    {
        std::cerr << LIB_ERR << "invalid priority." << std::endl;
        return -1;
    }
    block();
    all_threads[tid]->set_priority(priority);
    unblock();
    return 0;
}


/**
 * A function that terminates the thread with ID tid and deletes
 * it from all relevant control structures. All the resources allocated by
 * the library for this thread should be released. If no thread with ID tid
 * exists it is considered an error. Terminating the main thread
 * (tid == 0) will result in the termination of the entire process using
 * exit(0) [after releasing the assigned library memory].
 * @param tid The given thread ID to check.
 * @return 0 if the thread was successfully terminated, and -1 otherwise.
 * If a thread terminates itself or the main thread is terminated,
 * the function does not return.
*/
int uthread_terminate(int tid) {
    block();
//    std::cout << tid <<std::endl;
    if (check_tid(tid) == -1) // Check tid.
    {
        return -1;
    }
    if (tid == 0) // Terminate main thread.
    {
        delete_resources();
        quick_exit(EXIT_SUCCESS);
    }
    if (trun_id == tid) // Terminate itself.
    {
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        to_delete.push_back(all_threads[tid]);
        all_threads[tid] = nullptr;
        switch_threads(true);
        return 0;
    }
    if (all_threads[tid]->get_state() == ready) {
        ready_list.remove(all_threads[tid]);
    }
    to_delete.push_back(all_threads[tid]);
    all_threads[tid] = nullptr;
    unblock();
    return 0;
}

/**
 * A function that blocks the thread with ID tid. The thread may
 * be resumed later using uthread_resume. If no thread with ID tid exists it
 * is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision
 * should be made. Blocking a thread in BLOCKED state has no
 * effect and is not considered an error.
 * @param tid The ID of the thread to check.
 * @return On success 0, On failure -1.
*/
int uthread_block(int tid) {
    if (tid == 0) // Check tid.
    {
        std::cerr << LIB_ERR << "invalid id to block." << std::endl;
        return -1;
    }
    if (check_tid(tid) == -1) {
        return -1;
    }
    if (all_threads[tid]->get_state() == blocked) {
        return 0;
    }
    if (all_threads[tid]->get_state() == ready) {
        block();
        all_threads[tid]->set_state(blocked); // Change state.
        ready_list.remove(all_threads[tid]); // Remove from ready list.
        unblock();
    }
    if (trun_id == tid) // Block itself.
    {
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        all_threads[tid]->set_state(blocked);
        switch_threads();
//        Thread *running_thread = ready_list.front();
//        int ret_val = sigsetjmp(all_threads[tid]->_buf, 1);
//        if (ret_val == 1) {
//            return 0;
//        }
//
//        run_thread(running_thread->get_id());
//        siglongjmp(running_thread->_buf, 1);
    }
    return 0;
}

/**
 * A function that resumes a blocked thread with ID tid and moves
 * it to the READY state if it's not synced. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered an error.
 * @param tid The ID of the thread to check.
 * @return On success 0, on failure -1.
*/
int uthread_resume(int tid) {
    if (check_tid(tid) == -1) // Check tid.
    {
        return -1;
    }
    if (all_threads[tid]->get_state() == blocked) // Thread is blocked.
    {
        block();
        ready_list.push_back(all_threads[tid]);
        all_threads[tid]->set_state(ready);
        unblock();
    }
    return 0;
}

/**
 * A function that returns the thread ID of the calling thread.
 * @return The ID of the calling thread.
*/
int uthread_get_tid() {
    return trun_id;
}

/**
 * A function that returns the total number of quantums since
 * the library was initialized, including the current quantum.
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number
 * should be increased by 1.
 * @return The total number of quantums.
*/
int uthread_get_total_quantums() {
    return total_quantums;
}

/**
 * A function that returns the number of quantums the thread with
 * ID tid was in RUNNING state. On the first time a thread runs, the function
 * should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state
 * when this function is called, include also the current quantum). If no
 * thread with ID tid exists it is considered an error.
 * @param tid The ID of the thread to check.
 * @return On success the number of quantums of the thread with ID tid, On failure -1.
*/
int uthread_get_quantums(int tid) {
    if (check_tid(tid) == -1) // Check tid.
    {
        return -1;
    }
    return all_threads[tid]->get_qt_time();
}


