#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

class map_class {
  public:
    map_class() = default;

    std::string dst_cat;
    std::string src_cat;
    std::vector<long> dst;
    std::vector<long> src;
    std::vector<long> rng;
    int count;

    map_class(std::string dst_cat, std::string src_cat, std::vector<long> rng,
              std::vector<long> src, std::vector<long> dst)
        : dst_cat(dst_cat), src_cat(src_cat), dst(dst), src(src), rng(rng),
          count(0) {}
};

class seed_class {
  public:
    seed_class() = default;

    long strt;
    long rng;
    int count;

    seed_class(long strt, long rng) : strt(strt), rng(rng), count(0) {}
};

std::vector<seed_class> seed_vec;
std::vector<map_class> map_vec;

void parse_seed_info(seed_class &curr_seed, long strt, long rng) {
    curr_seed.strt = strt;
    curr_seed.rng = rng;
    curr_seed.count++;
}

void collect_seeds(std::string &line) {
    std::string seed_str(line.c_str() + line.find(":") + 1);

    std::stringstream ss(seed_str);

    long seed;
    long seeds[2];
    seed_class curr_seed;
    for (int i = 0; ss >> seed; i++) {
        seeds[i] = seed;

        if (i == 0) {
            curr_seed = seed_class();
            seed_vec.push_back(curr_seed);
        }
        if (i == 1) {
            parse_seed_info(seed_vec.back(), seeds[0], seeds[1]);
            i = -1;
        }
    }
}

void parse_map_cats(map_class &curr_map, std::string &line) {
    std::string to = "-to-";
    std::string dst_cat(line.c_str() + line.find(to) + 4,
                        line.c_str() + line.find(" "));
    std::string src_cat(line.c_str(), line.c_str() + line.find(to));

    curr_map.src_cat = src_cat;
    curr_map.dst_cat = dst_cat;
}

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
            parse_map_cats(map_vec.back(), line);
        } else if (line.length() > 0) {
            parse_map_info(map_vec.back(), line);
        }
    }

    input.close();
}

long solve_for_seed(map_class curr_map, long key, long k) {
    bool processing_seed = false;
    if (key == -1) {
        processing_seed = true;
        key = k;
    }
    // else
    //     key = in_key;

    for (int i = 0; i < curr_map.count; i++) {
        if(processing_seed) {
            if (key > curr_map.src[i] + curr_map.rng[i]) {
                return curr_map.src[i] + curr_map.rng[i];
            }
            if (key < curr_map.src[i]) {
                return curr_map.src[i];
            }
        }
        if (key > curr_map.src[i] && key < curr_map.src[i] + curr_map.rng[i]) {
            return curr_map.dst[i] + (key - curr_map.src[i]);
        }
    }

    return key;
}

long solve() {
    long lowest_loc = 0;
    int cc = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < seed_vec.size(); i++) {
        for (long k = seed_vec[i].strt; k < seed_vec[i].strt + seed_vec[i].rng; k++) {
            long key = -1; // rng
            
            for (int j = 0; j < map_vec.size(); j++) {

                // std::cout << map_vec[j].src_cat << ": " << k << "|" << key << " | "
                //           << "loc: " << lowest_loc << " in "
                //           << " | it: " << cc++ << std::endl;
                key = solve_for_seed(map_vec[j], key, k /* , seed_pair, j */);
                if (!lowest_loc)
                    lowest_loc = key;
                else
                    lowest_loc = std::min(lowest_loc, key);

                auto end_time = std::chrono::high_resolution_clock::now();

                auto duration_ms = std::chrono::duration_cast<std::chrono::minutes>(
                    end_time - start_time);

                std::cout << "\r" << duration_ms.count() << std::flush;
            }
            // std::cout << "key: " << key << std::endl;
        }
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
