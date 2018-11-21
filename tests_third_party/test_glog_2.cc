#include <iostream>

#include <glog/logging.h>

int main(int argc, char** argv) {
    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);

    // debug argc, argv
    for (int index = 0; index < argc; ++index) {
        std::cout << "index--->" << index << "\targv--->" << argv[index] << std::endl;
    }

    google::ParseCommandLineFlags(&argc, &argv, true);

    // FLAGS_log_dir = "./log";

    LOG(INFO) << "hello world";

    return 0;
}
