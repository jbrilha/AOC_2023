#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> numbers_from_line(const std::string &line) {
    int game_ID = line[line.find(":") - 1] - 0x30;

    std::string winning_numbers(line.c_str() + line.find(": ") + 2,
                                line.c_str() + line.find(" |"));

    std::string obtained_numbers(line.c_str() + line.find("| ") + 2,
                                 line.length());
    
    std::vector<int> obtained_nums_vec;
    std::vector<int> winning_nums_vec;
    std::stringstream ss_wn(winning_numbers);
    std::stringstream ss_on(obtained_numbers);
    
    int number;
    while (ss_wn >> number)
        winning_nums_vec.push_back(number);

    while (ss_on >> number)
        obtained_nums_vec.push_back(number);

    return std::make_pair(winning_nums_vec, obtained_nums_vec);
}

int calc_points(const std::vector<int> &winners, const std::vector<int> &obtained) {
    int points = 0;

    for(int i = 0; i < winners.size(); i++)
        for(int j = 0; j < obtained.size(); j++)
            if(obtained[j] == winners[i]) {
                if(points == 0)
                    points = 1;
                else
                    points = points << 1;
            }

    return points;
}

int main(int argc, char *argv[]) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string line;
    std::ifstream input(argv[1]);

    if (!input.is_open())
        std::cout << "FAILED TO OPEN" << std::endl;

    int total_points = 0;
    while(getline(input, line)) {
        auto game_numbers = numbers_from_line(line);

        const std::vector<int> &winning_nums = game_numbers.first;
        const std::vector<int> &obtained_nums = game_numbers.second;
        
        total_points += calc_points(winning_nums, obtained_nums);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << total_points << " | " << duration.count() << "ms" << std::endl;

    input.close();

    return 0;
}
