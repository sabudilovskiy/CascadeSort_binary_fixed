//
// Created by MRV on 5/17/2022.
//

#ifndef CASCADESORT_CASCADESORTER_H
#define CASCADESORT_CASCADESORTER_H
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "SeriesReader.h"
#include <functional>
void next_level(std::vector<int>& series);

class CascadeSorter{
    std::vector<std::fstream> files;
    std::vector<int> expected_number_series;
    std::vector<std::string> paths_to_files;
    std::vector<SeriesReader> series_readers;
    std::function<bool(const Student& left, const Student& right)> comparator;
    inline const static std::string temp_folder = "TEMPFILESORT";
    const size_t n;
protected:
    void clear();
    enum REGIME : bool{
        BEFORE_I, AFTER_I
    };
    void read_one_series(int i, REGIME regime);
    void reopen_files();
    void read_to_the_bone(int i, REGIME regime);
public:
    void sort(std::string_view path_out);
    CascadeSorter(const std::string& in_file_paths, size_t n, std::function<bool(const Student& left, const Student& right)> comparator);
    ~CascadeSorter();
};
#endif //CASCADESORT_CASCADESORTER_H
