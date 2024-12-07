#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::vector<int> duration;
std::vector<int> record;

void consume_input(std::string in_file) {
    std::string input_str;
    std::string line;
    std::ifstream input;

    try {
        input.open(in_file);
        
        int number;

        std::getline(input, line);
        std::string duration_str(line.c_str() + line.find(":") + 1, line.length());
        std::stringstream ss_d(duration_str);

        while(ss_d >> number)
            duration.push_back(number);

        std::getline(input, line);
        std::string record_str(line.c_str() + line.find(":") + 1, line.length());
        std::stringstream ss_r(record_str);

        while(ss_r >> number)
            record.push_back(number);

        input.close();

    } catch (std::ifstream::failure e) {
        std::cout << "Couldn't read " << in_file << std::endl;
    }

    input.close();
}

long solve() {
    int record_breaker_total = 1;

    for(int i = 0; i < duration.size(); i++) {
        int record_breakers = 0;

        for(int j = 1; j < duration[i] / 2; j++) {
            int time_held = j;
            int time_left = duration[i] - time_held;
            int distance = time_left * time_held;

            if(distance > record[i]) {
                record_breakers = ((duration[i] / 2) - (j - 1)) * 2;

                if(duration[i] % 2 == 0) 
                    record_breakers--;

                break;
            }
        }

        record_breaker_total *= record_breakers;
    }
    
    return record_breaker_total;
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
