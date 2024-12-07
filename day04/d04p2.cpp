#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

int total_cards = 0;

std::vector<std::string> vectorize_lines(std::ifstream &input) {
    std::vector<std::string> line_vec;
    std::string line;

    while (getline(input, line)) {
        line_vec.push_back(line);
    }

    return line_vec;
}

std::tuple<int, std::vector<int>, std::vector<int>> numbers_from_line(const std::string &line) {
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

    return std::make_tuple(game_ID, winning_nums_vec, obtained_nums_vec);
}

int calc_wins(const std::vector<int> &winners,
              const std::vector<int> &obtained) {
    int cards = 0;

    for (int i = 0; i < winners.size(); i++)
        for (int j = 0; j < obtained.size(); j++)
            if (obtained[j] == winners[i])
                cards++;

    return cards;
}

int count_cards(const std::vector<std::string> &lines, int i, int j) {

    auto game_numbers = numbers_from_line(lines[i]);
    int game_ID = std::get<0>(game_numbers);
    const std::vector<int> &winning_nums = std::get<1>(game_numbers);
    const std::vector<int> &obtained_nums = std::get<2>(game_numbers);

    int winners = calc_wins(winning_nums, obtained_nums);

    for (int k = i; k < winners + i; k++) {
        total_cards++;
        count_cards(lines, k + 1, winners + j);
    }

    return winners;
}

int main(int argc, char *argv[]) {
    std::string line;
    std::ifstream input(argv[1]);

    auto start_time = std::chrono::high_resolution_clock::now();
    if (!input.is_open())
        std::cout << "FAILED TO OPEN" << std::endl;

    const std::vector<std::string> &lines = vectorize_lines(input);

    for (int i = 0; i < lines.size(); i++) {
        total_cards++;
        count_cards(lines, i, lines.size() - 1);
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << total_cards << " | " << duration.count() << "ms" << std::endl;

    input.close();

    return 0;
}
