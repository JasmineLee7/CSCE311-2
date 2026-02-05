#include <iostream>
#include <proj1/lib/cli_parser.h> // only runs this formet, otherwise error saying that the file doesnt exist
#include <proj1/lib/error.h>
#include <proj1/lib/sha256.h>
#include <proj1/lib/thread_log.h>
#include <proj1/lib/timings.h>

#include <vector>
#include <fstream>
#include <sys/sysinfo.h>

void* StartRoutine(void* arg);

struct ThreadDatum {
    int thread_index:
}

int main(){
    // read file in 
   int n = get_nprocs();
   std::cout<<"nprocs: "<<n<<std::endl;
   
    // ask user for k
    // change back tty 
    int k;
    std::ifstream tty_in("/dev/tty");
      if (tty_in) {
          int var;
      tty_in >> var;
        k=var;
        std::cout<<"you entered Var: "<<var<<std::endl;
    }

    // "make realse threads as described" ---- Is this how you described it Lewis?
    std::vector<ThreadDatum> thread_data;
    for(int i=0; i< n-k; i++){
        ThreadDatum td;
        td.thread_index = i+1; // thread index starts from 1
        td.k = k;
        thread_data.push_back(td);
    }

    //-- all --rate --thread 
    // read in from file
    struct Row;
    std::vector<Row> rows_in;

    int curr_thread = 0;
    int max_thread = k;
}

// work loop, pay attention to time. it should time out after 
//   CliMode mode;
//   Time_T timeout_ms;
//   CliParse(argc, argv, &mode, &timeout_ms);