#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

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


std::vector<std::pair<long, long>> solve_for_seed(map_class curr_map,
        std::vector<std::pair<long, long>> src_rng) {

    std::vector<std::pair<long, long>> in_rng;

    for (int i = 0; i < curr_map.count; i++) {
        std::vector<std::pair<long, long>> new_rng;

        long map_start = curr_map.src[i];
        long map_end = curr_map.src[i] + curr_map.rng[i];
        long map_dst = curr_map.dst[i];

        while (src_rng.size() > 0) {
            std::pair<long, long> key_pair = src_rng.back();
            src_rng.pop_back();

            long start = key_pair.first;
            long end = key_pair.second;

            std::pair<long, long> bfr_dst = {start, std::min(end, map_start)};

            std::pair<long, long> in_dst = {std::max(start, map_start),
                                           std::min(end, map_end)};

            std::pair<long, long> aft_dst = {std::max(start, map_end), end};

            if (bfr_dst.second > bfr_dst.first)
                new_rng.push_back(bfr_dst);

            if (in_dst.second > in_dst.first)
                in_rng.push_back(
                    std::make_pair(in_dst.first - map_start + map_dst,
                                   in_dst.second - map_start + map_dst));

            if (aft_dst.second > aft_dst.first)
                new_rng.push_back(aft_dst);
        }
        src_rng = new_rng;
    }

    in_rng.insert(in_rng.end(), src_rng.begin(), src_rng.end());
    return in_rng;
}

long solve() {
    long lowest_loc = 0;
    for (int i = 0; i < seed_vec.size(); i++) {
        std::pair<long, long> key_pair = {seed_vec[i].strt,
                                          seed_vec[i].strt + seed_vec[i].rng};

        std::vector<std::pair<long, long>> key_pair_vec;
        key_pair_vec.push_back(key_pair);

        for (int j = 0; j < map_vec.size(); j++) {
            key_pair_vec = solve_for_seed(map_vec[j], key_pair_vec);
        }

        if (!lowest_loc)
            lowest_loc =
                std::min_element(key_pair_vec.begin(), key_pair_vec.end())
                    ->first;
        else
            lowest_loc = std::min(
                std::min_element(key_pair_vec.begin(), key_pair_vec.end())
                    ->first,
                lowest_loc);
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
