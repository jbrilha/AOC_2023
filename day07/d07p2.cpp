#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

long duration;
long record;

void consume_input(std::string in_file) {
    std::string input_str;
    std::string line;
    std::ifstream input;

    try {
        input.open(in_file);
        
        long number;

        std::getline(input, line);
        std::string duration_line(line.c_str() + line.find(":") + 1, line.length());
        std::stringstream ss_d(duration_line);
        std::string duration_str;

        while(ss_d >> number)
            duration_str += std::to_string(number);

        duration = stol(duration_str);

        std::getline(input, line);
        std::string record_line(line.c_str() + line.find(":") + 1, line.length());
        std::stringstream ss_r(record_line);
        std::string record_str;

        while(ss_r >> number)
            record_str += std::to_string(number);

        record = stol(record_str);

        input.close();

    } catch (std::ifstream::failure e) {
        std::cout << "Couldn't read " << in_file << std::endl;
    }

    input.close();
}

long solve() {
    int record_breakers = 0;

    for(int j = 1; j < duration / 2; j++) {
        long time_held = j;
        long time_left = duration - time_held;
        long distance = time_left * time_held;

        if(distance > record) {
            record_breakers = ((duration / 2) - (j - 1)) * 2;

            if(duration % 2 == 0) 
                record_breakers--;

            break;
        }
    }
    
    return record_breakers;
}

int main(int argc, char *argv[]) {

    auto start_time = std::chrono::high_resolution_clock::now();

    consume_input(argv[1]);

    long record_breakers = solve();

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(
                           end_time - start_time) / (double_t)1000;

    std::cout << record_breakers << " | " << duration_ms.count() << "ms"
              << std::endl;

    return 0;
}
