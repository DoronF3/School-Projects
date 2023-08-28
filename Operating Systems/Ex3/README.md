# Exercise 3: Parallel Processing and MapReduce

This directory contains C++ code for a MapReduce framework implementation along with a barrier class. The MapReduce framework supports multi-threading and includes features for mapping, shuffling, and reducing tasks. The provided `Barrier` class is used to synchronize threads during the MapReduce process.

## Barrier Class

The `Barrier` class (`Barrier.cpp` and `Barrier.h`) provides a synchronization mechanism for threads. It allows a specified number of threads to wait until all of them have reached the barrier before they can proceed. The barrier class is designed to be used in concurrent and parallel programming scenarios where synchronization is required.

### Contents

- `Barrier::Barrier(int numThreads)`: Constructor for creating a barrier with a specified number of threads.
- `Barrier::~Barrier()`: Destructor for cleaning up the barrier resources.
- `Barrier::barrier()`: Method for threads to wait at the barrier until all threads have arrived.

## MapReduce Framework

The MapReduce framework (`Mapreduce.cpp`) is designed to perform MapReduce operations using multiple threads. It includes functions for mapping, shuffling, and reducing tasks, allowing users to process large datasets efficiently.

### Contents

- `ThreadContext`: A struct representing the context of a thread, used during the MapReduce process.
- `JobContext`: A struct representing the context of the entire job, including thread contexts and intermediate data storage.
- `emit2(K2 *key, V2 *value, void *context)`: A function for emitting intermediate key-value pairs during the mapping phase.
- `emit3(K3 *key, V3 *value, void *context)`: A function for emitting final key-value pairs during the reducing phase.
- `startMapReduceJob()`: Function for starting the MapReduce job with the specified client, input vector, output vector, and thread count.
- `waitForJob()`: Function for waiting for the completion of the MapReduce job.
- `getJobState()`: Function for retrieving the current state of the MapReduce job.
- `closeJobHandle()`: Function for releasing resources associated with the MapReduce job.

### Usage

1. Include the provided `Barrier.cpp`, `Barrier.h`, and `Mapreduce.cpp` files in your project.
2. Use the `Barrier` class to synchronize threads during parallel operations as needed.
3. Use the MapReduce framework functions to define and execute MapReduce jobs using multiple threads.

Make sure to understand the functionality and usage of the barrier class and the MapReduce framework functions before integrating them into your project. Refer to the provided comments and documentation within the code for more details.
