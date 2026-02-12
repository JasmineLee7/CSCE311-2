# CSCE311-2
Jasmine Lee
Main.cc
This project implements a multi-threaded scheduling program using POSIX threads to demonstrate controlled execution without synchronization primitives.

This program uses two primary structures. A row structure stores each input record, including its id, data string, iteration count, resulting SHA-256 hash, and the index of the thread that processed it. A thread structure stores metadata for each pthread, including its fixed 1-based index. All rows are stored in a shared vector accessible to all threads.

At startup, the program parses command-line options, determines the number of execution contexts using get_nprocs, and immediately creates that many paused threads. Each thread repeatedly sleeps while waiting to be released. The main thread reads all input rows from standard input, then prompts the user for a value k that determines how many threads will actively participate. Threads whose index exceeds k exit immediately once execution begins.

Thread release is controlled by a shared counter. In all modes, all active threads are released simultaneously. In rate mode, threads are released one per millisecond. In thread mode, each running thread releases the next thread in sequence. Threads wait until their index is eligible, log their start, and begin processing work.

Each active thread processes rows spaced k apart according to its index. For each assigned row, the thread generates a seed from the row data and iteratively hashes it using the provided library. The result and processing thread index are stored back into the shared structure, and progress is logged. If a timeout is specified, threads periodically check the elapsed time and exit cleanly when it is exceeded.

After all threads finish or terminate, the main thread joins them and prints a final report summarizing the results. The implementation follows the project's requirement for unsynchronized scheduling, allowing observable race conditions while maintaining correct output.
