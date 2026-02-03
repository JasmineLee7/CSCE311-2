#include <iostream>
#include <proj1/lib/cli_parser.h>
#include <proj1/lib/error.h>
//#include <proj1/lib/cli_proj1lib.a>
#include <proj1/lib/sha256.h>
#include <proj1/lib/thread_log.h>
#include <proj1/lib/timings.h>



int main(){
    return 0;
}

/*
int main(int argc, char** argv) {
    try {
        proj1::Timings timings;
        proj1::ThreadLog thread_log;

        proj1::CLIParser parser(argc, argv);
        parser.parse();

        // Example usage of SHA256
        std::string data = "Hello, World!";
        std::string hash = proj1::SHA256::hash(data);
        std::cout << "SHA256('" << data << "') = " << hash << std::endl;

        timings.report();
    } catch (const proj1::Error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


*/
