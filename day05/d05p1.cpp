#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

class map_class {
  public:
    map_class() = default;
    /*
    std::string dst_cat;
    std::string src_cat;
    */
    std::vector<long> dst;
    std::vector<long> src;
    std::vector<long> rng;
    int count;

    map_class(
        /*std::string dst_cat, std::string src_cat, */ std::vector<long> rng,
        std::vector<long> src, std::vector<long> dst)
        : /*dst_cat(dst_cat), src_cat(src_cat), */ dst(dst), src(src), rng(rng),
          count(0) {}
};

std::vector<map_class> map_vec;
std::vector<long> seeds;

void collect_seeds(std::string &line) {
    std::string seed_str(line.c_str() + line.find(":") + 1);

    std::stringstream ss(seed_str);

    long seed;
    while (ss >> seed) {
        seeds.push_back(seed);
    }
}

// void parse_map_cats(map_class &curr_map, std::string &line) {
//     std::string to = "-to-";
//     std::string dst_cat(line.c_str() + line.find(to) + 4,
//                         line.c_str() + line.find(" "));
//     std::string src_cat(line.c_str(), line.c_str() + line.find(to));
//
//     curr_map.src_cat = src_cat;
//     curr_map.dst_cat = dst_cat;
// }

void parse_map_info(map_class &curr_map, std::string &line) {
    long dst = 0;
    long src = 0;
    long rng = 0;

    std::stringstream ss(line);

    ss >> dst;
    ss >> src;
    ss >> rng;

    curr_map.dst.push_back(dst);
    curr_map.src.push_back(src);
    curr_map.rng.push_back(rng);
    curr_map.count++;
}

void consume_input(std::string in_file) {
    std::ifstream input(in_file);

    if (!input.is_open())
        std::cout << "FAILED TO OPEN" << std::endl;
    std::string line;

    getline(input, line);
    collect_seeds(line);

    map_class curr_map;
    while (getline(input, line)) {
        if (!isdigit(line[0]) && line != "") {
            curr_map = map_class();
            map_vec.push_back(curr_map);
            // parse_map_cats(map_vec.back(), line);
        } else if (line.length() > 0) {
            parse_map_info(map_vec.back(), line);
        }
    }

    input.close();
}

long solve_for_seed(map_class curr_map, long key) {

    for (int i = 0; i < curr_map.count; i++) {
        if (key > curr_map.src[i] && key < curr_map.src[i] + curr_map.rng[i]) {
            return curr_map.dst[i] + (key - curr_map.src[i]);
        }
    }

    return key;
}

long solve() {
    long lowest_loc = 0;

    for (int i = 0; i < seeds.size(); i++) {
        long key = seeds[i];

        for (int j = 0; j < map_vec.size(); j++) {
            long new_key = solve_for_seed(map_vec[j], key);
            key = new_key;
        }

        if (!lowest_loc)
            lowest_loc = key;
        else
            lowest_loc = std::min(lowest_loc, key);
    }

    return lowest_loc;
}

int main(int argc, char *argv[]) {

    auto start_time = std::chrono::high_resolution_clock::now();

    consume_input(argv[1]);

    long lowest_loc = solve();

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(
                           end_time - start_time) /
                       (double_t)1000;

    std::cout << lowest_loc << " | " << duration_ms.count() << "ms"
              << std::endl;

    return 0;
}
