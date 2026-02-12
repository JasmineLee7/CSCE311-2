#include <proj1/lib/cli_parser.h>
#include <proj1/lib/sha256.h>
#include <proj1/lib/thread_log.h>
#include <proj1/lib/timings.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Row structure
struct Row {
  std::string id;
  std::string data;
  uint32_t iterations;
  size_t thread_index = 0;
  char sha256[65] = {0};
};

// Thread data 
struct ThreadDatum {
  pthread_t handle{};
  size_t thread_index = 0;
};

// Row Vector Initialization
static std::vector<Row> rows;
static size_t total_rows = 0;

// Thread and iteration tracking
static size_t n = 0;
static size_t k = 0;
static size_t released_k = 0;

// CLI mode and timeout
static CliMode mode;
static uint32_t timeout_ms = 0;

// Thread routine for processing rows
void* StartRoutine(void* arg) {
// make threads
  ThreadDatum* td = reinterpret_cast<ThreadDatum*>(arg);
  size_t t = td->thread_index;

// Prevents all the threads from being made before the program prompts for K
  while (k == 0) {
    Timings_SleepMs(1);
  }

// tracking threads
  if (t > k) {
    ThreadLog("[thread %zu] returned", t);
    return nullptr;
  }

  while (t > released_k) {
    Timings_SleepMs(1);
  }

// thread released 
  if (mode == CLI_MODE_THREAD && t == released_k && t < k) {
    released_k = t + 1;
  }

// log thread start
  ThreadLog("[thread %zu] started", t);
  Timings_t start = Timings_NowMs();

// print out threads as they complete rows, and check for timeout
  for (size_t row = t - 1; row < total_rows; row += k) {
    if (timeout_ms > 0 && Timings_TimeoutExpired(start, timeout_ms)) {
      ThreadLog("[thread %zu] timeout", t);
      ThreadLog("[thread %zu] returned", t);
      return nullptr;
    }
    
// compute the SHA256 hash for the current row
    const std::string& id = rows[row].id;
    const std::string& data = rows[row].data;
    uint32_t counter = rows[row].iterations;

// create a seed by concatenating the id, data, and counter
    std::vector<uint8_t> seed;
    seed.reserve(id.size() + data.size() + sizeof(counter));

    const uint8_t* id_bytes = reinterpret_cast<const uint8_t*>(id.data());
    seed.insert(seed.end(), id_bytes, id_bytes + id.size());

    const uint8_t* data_bytes = reinterpret_cast<const uint8_t*>(data.data());
    seed.insert(seed.end(), data_bytes, data_bytes + data.size());

    const uint8_t* counter_bytes = reinterpret_cast<const uint8_t*>(&counter);
    seed.insert(seed.end(), counter_bytes, counter_bytes + sizeof(counter));

    ComputeIterativeSha256Hex(seed.data(), seed.size(), counter, rows[row].sha256);

    rows[row].thread_index = t;

    ThreadLog("[thread %zu] completed row %zu", t, row + 1);
  }

  ThreadLog("[thread %zu] returned", t);
  return nullptr;
}

int main(int argc, char* argv[]) {
  CliParse(argc, argv, &mode, &timeout_ms);

  n = static_cast<size_t>(get_nprocs());

  std::vector<ThreadDatum> thread_data(n);

// create threads
  for (size_t i = 0; i < n; ++i) {
    thread_data[i].thread_index = i + 1;
    pthread_create(&thread_data[i].handle, nullptr, &StartRoutine, &thread_data[i]);
  }

  std::cin >> total_rows;
  rows.resize(total_rows);

  for (size_t i = 0; i < total_rows; ++i) {
    std::cin >> rows[i].id >> rows[i].data >> rows[i].iterations;
  }

  std::cout << "Enter max threads (1 - " << n << "): " << std::endl;

// read k from /dev/tty to avoid blocking stdin
  std::ifstream tty_in("/dev/tty");
  size_t k_read = 0;
  if (tty_in) tty_in >> k_read;

// ensure k is between 1 and n
  k_read = std::max<size_t>(1, std::min(k_read, n));
  k = k_read;

// release threads based on mode
  switch (mode) {
    case CLI_MODE_ALL:
      released_k = k;
      break;

    case CLI_MODE_RATE:
      for (size_t i = 1; i <= k; ++i) {
        released_k = i;
        Timings_SleepMs(1);
      }
      break;

    case CLI_MODE_THREAD:
      released_k = 1;
      break;

    default:
      break;
  }
// wait for threads to finish
  for (size_t i = 0; i < n; ++i) {
    pthread_join(thread_data[i].handle, nullptr);
  }

  std::cout << "Thread\tStart\tEncryption" << std::endl;
// print out results
  for (size_t i = 0; i < total_rows; ++i) {
    std::cout << rows[i].thread_index << "\t"
              << rows[i].data << "\t"
              << rows[i].sha256 << std::endl;
  }

  return 0;
}
