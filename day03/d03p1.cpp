#include <iostream>
#include <fstream>

bool is_symbol(char ch) {
    if( ch == 0x2E )    // '.' not a symbol
        return false;

    if(( 0x21 <= ch && ch <= 0x2F ) ||  // between [SPACE] and digits
        ( 0x3A <= ch && ch <= 0x40 ))   // between digits and uppercase chars
        return true;

    return false;
}

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

bool is_part_number(char** schematic, int d_start, int d_end, int row, int ll) {

    for(int i = std::max(row - 1, 0); i <= std::min(row + 1, ll - 1); i++)
        for(int j = std::max(d_start - 1, 0); j <= std::min(d_end + 1, ll - 1) ; j++)
            if(is_symbol(schematic[i][j]))
                return true;

    return false;
}

int sum_of_parts(char** schematic, int ll){
    int sum_of_parts = 0;
    for(int i = 0; i < ll; i++)
        for(int j = 0; j < ll; j++)
            if(isdigit(schematic[i][j])) {
                std::string digit_str;
                int d_start = -1;

                while(isdigit(schematic[i][j])){
                    if(d_start == -1)
                        d_start = j;

                    digit_str += schematic[i][j];
                    j++;
                }

                int d_end = j - 1;

                int part_num = parse_digit(digit_str);

                int row = i;

                if(is_part_number(schematic, d_start, d_end, row, ll))
                    sum_of_parts += part_num;
            }

    return sum_of_parts;
}

int main(int argc, char *argv[]){
    
    std::ifstream input(argv[1]);
    if(!input.is_open())
        std::cout << "FAILED TO OPEN" << std::endl;

    int ll = get_line_length(input);
    char** schematic = fill_schematic(input, ll);
    int parts_sum = sum_of_parts(schematic, ll);

    std::cout << parts_sum << std::endl;
    delete_schematic(schematic, ll);
    input.close();

    return 0;
}
