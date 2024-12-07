#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    std::string line;
    std::ifstream input(argv[1]);

    std::pair<std::string, char> numbers[] = {
        {"one", '1'},   {"two", '2'},   {"three", '3'},
        {"four", '4'},  {"five", '5'},  {"six", '6'},
        {"seven", '7'}, {"eight", '8'}, {"nine", '9'},
    };

    if (!input.is_open())
        std::cout << "FAILED" << std::endl;

    int calibration = 0;
    while (getline(input, line)) {
        int first_digit = 0, last_digit = 0;

        auto first_digit_pos = std::string::npos; // using unsigned so I can just compare using <
        decltype(first_digit_pos) last_digit_pos = 0;

        for (const auto &[spelling, digit] : numbers) {
            auto found_first_number = line.find(spelling); // uintmax_t same as auto same as auto which is unsigned long
            auto found_first_digit = line.find(digit);
            auto found_last_number = line.rfind(spelling);
            auto found_last_digit = line.rfind(digit);

            if (found_first_number < first_digit_pos) {
                first_digit_pos = found_first_number;
                first_digit = digit - 0x30;
            }
            if (found_first_digit < first_digit_pos) {
                first_digit_pos = found_first_digit;
                first_digit = digit - 0x30;
            }
            
            if (found_last_digit != std::string::npos &&
                found_last_digit >= last_digit_pos) {
                    last_digit_pos = found_last_digit;
                    last_digit = digit - 0x30;
                }
            if (found_last_number != std::string::npos &&
                found_last_number >= last_digit_pos) {
                    last_digit_pos = found_last_number;
                    last_digit = digit - 0x30;
                }
        }
        calibration += first_digit * 10 + last_digit;
    }
    std::cout << calibration << std::endl;

    input.close();

    return 0;
}
