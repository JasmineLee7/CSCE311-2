#include <proj1/lib/cli_parser.h>
#include <proj1/lib/sha256.h>
#include <proj1/lib/thread_log.h>
#include <proj1/lib/timings.h>

#include <pthread.h>
#include <sys/sysinfo.h>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Row {
  std::string id;
  std::string data;
  uint32_t iterations;
  size_t thread_index = 0;
  char sha256[65] = {0};
};

struct ThreadDatum {
  pthread_t handle{};
  size_t thread_index = 0;
};

static std::vector<Row> g_rows;
static size_t g_total_rows = 0;

static size_t g_n = 0;
static volatile size_t g_k = 0;
static volatile size_t g_released_k = 0;

static CliMode g_mode;
static uint32_t g_timeout_ms = 0;

void* StartRoutine(void* arg) {
  ThreadDatum* td = reinterpret_cast<ThreadDatum*>(arg);
  size_t t = td->thread_index;

  while (g_k == 0) {
    Timings_SleepMs(1);
  }

  if (t > g_k) {
    ThreadLog("[thread %zu] returned", t);
    return nullptr;
  }

  while (t > g_released_k) {
    Timings_SleepMs(1);
  }

  if (g_mode == CLI_MODE_THREAD && t == g_released_k && t < g_k) {
    g_released_k = t + 1;
  }

  ThreadLog("[thread %zu] started", t);

  Timings_t start = Timings_NowMs();

  for (size_t row = t - 1; row < g_total_rows; row += g_k) {
    if (g_timeout_ms > 0 && Timings_TimeoutExpired(start, g_timeout_ms)) {
      ThreadLog("[thread %zu] timeout", t);
      ThreadLog("[thread %zu] returned", t);
      return nullptr;
    }

    const std::string& id = g_rows[row].id;
    const std::string& data = g_rows[row].data;
    uint32_t counter = g_rows[row].iterations;

    std::vector<uint8_t> seed;
    seed.insert(seed.end(), id.begin(), id.end());
    seed.insert(seed.end(), data.begin(), data.end());

    uint8_t* counter_bytes = reinterpret_cast<uint8_t*>(&counter);
    seed.insert(seed.end(), counter_bytes, counter_bytes + sizeof(counter));

    ComputeIterativeSha256Hex(seed.data(), seed.size(), counter, g_rows[row].sha256);

    g_rows[row].thread_index = t;

    ThreadLog("[thread %zu] completed row %zu", t, row + 1);
  }

  ThreadLog("[thread %zu] returned", t);
  return nullptr;
}

int main(int argc, char* argv[]) {
  CliParse(argc, argv, &g_mode, &g_timeout_ms);

  g_n = static_cast<size_t>(get_nprocs());

  std::vector<ThreadDatum> thread_data(g_n);

  for (size_t i = 0; i < g_n; ++i) {
    thread_data[i].thread_index = i + 1;
    pthread_create(&thread_data[i].handle, nullptr, &StartRoutine, &thread_data[i]);
  }

  std::cin >> g_total_rows;
  g_rows.resize(g_total_rows);

  for (size_t i = 0; i < g_total_rows; ++i) {
    std::cin >> g_rows[i].id >> g_rows[i].data >> g_rows[i].iterations;
  }

  std::cout << "Enter max threads (1 - " << g_n << "): " << std::endl;

  std::ifstream tty_in("/dev/tty");
  size_t k_read = 0;

  if (tty_in) tty_in >> k_read;

  if (k_read < 1) k_read = 1;
  if (k_read > g_n) k_read = g_n;

  g_k = k_read;

  switch (g_mode) {
    case CLI_MODE_ALL:
      g_released_k = g_k;
      break;

    case CLI_MODE_RATE:
      for (size_t i = 1; i <= g_k; ++i) {
        g_released_k = i;
        Timings_SleepMs(1);
      }
      break;

    case CLI_MODE_THREAD:
      g_released_k = 1;
      break;

    default:
      break;
  }

  for (size_t i = 0; i < g_n; ++i) {
    pthread_join(thread_data[i].handle, nullptr);
  }

  std::cout << "Thread\tStart\tEncryption" << std::endl;

  for (size_t i = 0; i < g_total_rows; ++i) {
    std::cout << g_rows[i].thread_index << "\t"
              << g_rows[i].data << "\t"
              << g_rows[i].sha256 << std::endl;
  }

  return 0;
}