#include <iostream>
#include <fstream>

int get_line_length(std::ifstream &input) {
    std::string line;

    getline(input, line);
    input.seekg(0);

    return line.length();
}

char** fill_schematic(std::ifstream &input, int ll) {
    char** schematic = new char*[ll];

    std::string line;
    for(int row = 0; getline(input, line); row++) {
        schematic[row] = new char[ll];

        for(int col = 0; col < line.length(); col++)
            schematic[row][col] = line[col];
    }

    return schematic;    
}

void delete_schematic(char** schematic, int ll) {
    for (int i = 0; i < ll; ++i)
        delete[] schematic[i];

    delete[] schematic;
}

int parse_digit(std::string digit_str) {
    // I forgot stoi was a thing but I'm keeping this here anyway
    int digit = 0;
    int len = digit_str.length();
    for(int i = 0; i < len; i++) {
        int dig = (digit_str[len - i - 1] - 0x30) ;

        digit += dig * pow(10, i);
    }

    return digit;
}

int ratio_deez_gears(char** schematic, int ll, int row, int col) {
    
    int gear_count = 0;
    int gears[2];

    for(int i = std::max(row - 1, 0); i <= std::min(row + 1, ll - 1); i++)
        for(int j = std::max(col - 1, 0); j <= std::min(col + 1, ll - 1) ; j++) {
            std::string digit_str;

            if(isdigit((schematic[i][j]))) {
                if(gear_count == 2)
                    return 0;

                int first_digit_j = j;
                
                while(isdigit(schematic[i][j])) { // parse forward
                    digit_str += schematic[i][j];            
                    j++;
                }

                int end_digit_j = j - 1;
                
                j = first_digit_j - 1; // "reset" j for this digit

                while(isdigit(schematic[i][j])) { // parse bacwkards
                    digit_str = schematic[i][j] + digit_str;            
                    j--;
                }

                j = end_digit_j; // "reset" j so there's no repetition

                gears[gear_count] = stoi(digit_str);
                gear_count++;
            }
        }

    if(gear_count < 2) return 0;

    return gears[0] * gears[1];
}

int sum_of_gear_ratios(char** schematic, int ll){
    int sum_of_gear_ratios = 0;

    for(int i = 0; i < ll; i++)
        for(int j = 0; j < ll; j++)
            if(schematic[i][j] == '*')
                sum_of_gear_ratios += ratio_deez_gears(schematic, ll, i, j);

    return sum_of_gear_ratios;
}

int main(int argc, char *argv[]){
    
    std::ifstream input(argv[1]);
    if(!input.is_open())
        std::cout << "FAILED TO OPEN" << std::endl;

    int ll = get_line_length(input);
    char** schematic = fill_schematic(input, ll);
    int gear_ratios_sum = sum_of_gear_ratios(schematic, ll);

    std::cout << gear_ratios_sum << std::endl;

    delete_schematic(schematic, ll);

    input.close();

    return 0;
}
