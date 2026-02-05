#include <iostream>
#include "../lib/cli_parser.h" // only runs this formet, otherwise error saying that the file doesnt exist
#include <lib/error.h>
#include "../lib/sha256.h"
#include "../lib/thread_log.h"
#include "../lib/timings.h"

#include <vector>
#include <fstream>
#include <sys/sysinfo.h>

void* StartRoutine(void* arg);

int main(){
    // read file in 
   int n = get_nprocs();
   std::cout<<"nprocs: "<<n<<std::endl;
   
   // ask user for k
  // change back tty - What does this mean?
  int k;
  while(k=0){
    std::ifstream tty_in("/dev/tty");
      if (tty_in) {
          int var;
      tty_in >> var;
      if (var<=n){
        k=var;
        std::cout<<"you entered Var: "<<var<<std::endl;
      }
        else {
        k=0; // validate that k<=n, we need to do this atsp 
        std::cout<<"Please enter a value less than or equal to "<<n<<std::endl;
        }
    }
  }
  
    // "make realse threads as described" ---- Is this how you described it Lewis?
    // Copied and Paste from the github repo 
    std::cout << "Creating " << n << " threads\n";
    int thread_index = 0;
    for (::pthread_t& thread : thread_pool) {
        ::pthread_create(&thread, nullptr, StartRoutine, &thread_index);
        std::cout<<"I made thread "<<thread_index<<std::endl;
        ++thread_index;
    }

    // Lewis reccomendaitno -- when do i use this? 
    struct ThreadDatum {
        int thread_index:
        int k;
    }

    struct Row;
    std::vector<Row> rows_in;

    // add threads to vector --  
    for(int i=1; i<=(n-k); i++){
        Row r;
        r.thread_index = i;
        r.k = k;
        rows_in.push_back(r);
    }


    //-- all --rate --thread 

    // keep curr_thread and max_thread='k'
    int curr_thread = 0;
    int max_thread = 1;
}

// work loop, pay attention to time. it should time out after 
//   CliMode mode;
//   Time_T timeout_ms;
//   CliParse(argc, argv, &mode, &timeout_ms);

void* StartRoutine(void* arg) {
  int *index = static_cast<int *>(arg);
  std::cout << "StartRoutine received index " << *index << std::endl;

  return nullptr;
}