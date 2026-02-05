#include <iostream>
#include "../lib/cli_parser.h"
#include "../lib/error.h"
#include "../lib/sha256.h"
#include "../lib/thread_log.h"
#include "../lib/timings.h"

#include <vector>
#include <fstream>
#include <sys/sysinfo.h>

// validate that k<=n, we need to do this atsp 
int main(){
    // read file in 
   int n = get_nprocs();
   std::cout<<"nprocs: "<<n<<std::endl;

   struct Row;
   std::vector<Row> rows_in;

  // change back tty 
  std::ifstream tty_in("/dev/tty");
      if (tty_in) {
          int var;
      tty_in >> var;
     }
    int k = 1;

    // ask user for k
    std::cout<<"Enter a value for K"<<std::endl;
    std::cin>>k;
    std::cout<<"You entered "<<k<<std::endl;


    // make realse threads as described 
    //-- all --rate --thread 

    // keep curr_thread and max_thread='k'
    int curr_thread = 0;
    int max_thread = k;
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