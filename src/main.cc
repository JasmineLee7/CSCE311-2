#include <iostream>
#include <proj1/lib/cli_parser.h> // only runs this formet, otherwise error saying that the file doesnt exist
#include <proj1/lib/error.h>
#include <proj1/lib/sha256.h>
#include <proj1/lib/thread_log.h>
#include <proj1/lib/timings.h>

#include <pthread.h>
#include <vector>
#include <fstream>
#include <sys/sysinfo.h>

int k=0;

struct Row {
    std::string id;
    std::string data;
    int iterations;
};

struct ThreadDatum {
    pthread_t handle;
    int thread_index;
    const int *k;
    std::vector<Row> rows_in; // changed *vector to vector, maybe wrong. 
};


void* StartRoutine(void* arg);

int main(int argc, char* argv[]) {
    std::vector<Row> rows_in;

    CliMode mode;   
    Time_t timeout_ms;
    CliParse(argc, argv, &mode, &timeout_ms);

    // read file in 
   int n = get_nprocs();
   std::cout<<"nprocs: "<<n<<std::endl;
   

    // ask user for k
    // change back tty 
    

    // "make realse threads as described" ---- Is this how you described it Lewis?
    std::vector<ThreadDatum> thread_data;
    for(int i=0; i< n-k; i++){
        ThreadDatum td ={pthread_t(),i, &k, rows_in};
        pthread_create(&td.handle, NULL, &StartRoutine, &td);
        td.thread_index = i+1; // thread index starts from 1
        thread_data.push_back(td);
    }

    std::ifstream tty_in("/dev/tty");
      if (tty_in) {
          int var;
      tty_in >> var;
        k=var;
        std::cout<<"you entered Var: "<<var<<std::endl;
    }

    // read in from file
    return 0;
}

// work loop, pay attention to time. it should time out after 
void* StartRoutine(void* arg) {
    int curr_thread = 0;
    int max_thread = k; 

    
    while(curr_thread < max_thread){
        Timings_SleepMs(1000);
    }

  ThreadDatum *index = reinterpret_cast<ThreadDatum *>(arg);
  std::cout << "StartRoutine received index " << (index->thread_index) << std::endl; // removed * from *(index->thread_index). Maybe wrong

  return nullptr;
}