#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using std::string;

std::pair<string, int> cubes_color_max[] = {
    {"red", 12}, {"green", 13}, {"blue", 14}
};

int parse_line(string line, char splt_ch){
    string game_substr = "Game ";
    string game(line.c_str(), line.find(splt_ch));

    game.erase(0, game_substr.length());
    int32_t game_ID = std::stoi(game.c_str());

    string cubes(line.c_str() + line.find(splt_ch) + 2, line.length());
    
    for (const auto &[color, max] : cubes_color_max) {
        size_t pos = cubes.find(color);

        while(pos != string::npos) {
            int found = cubes[pos - 2] - 0x30;
            if(isdigit(cubes[pos - 3]))
               found += (cubes[pos - 3] - 0x30) * 10;

            if(found > max)
                return 0;
                        
            pos = cubes.find(color, pos + 1);
        }
    }
        
    return game_ID;
}

int main(int argc, char *argv[]) {
    string line;
    std::ifstream input(argv[1]);

    if (!input.is_open())
        printf("FAILED\n");

    int sum = 0; 
    while (getline(input, line)) {
        sum += parse_line(line, ':');
    }
    std::cout << sum << std::endl;

    input.close();

    return 0;
}
