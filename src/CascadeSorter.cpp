//
// Created by MRV on 5/17/2022.
//
#include "CascadeSorter.h"
#include "../includes/CascadeSorter.h"

void next_level(std::vector<int> &series) {
    auto copy = series;
    int n = series.size();
    series[n - 1] = copy[0];
    for (int i = n - 2; i > -1; i--){
        series[i] = series[i+1] + copy[n - i - 1];
    }
}

void CascadeSorter::clear() {
    for (auto& file : files){
        file.close();
    }
    std::filesystem::remove_all(temp_folder);
    files.clear();
    expected_number_series.clear();
    paths_to_files.clear();
    series_readers.clear();
}

void CascadeSorter::read_one_series(int i, CascadeSorter::REGIME regime) {
    std::vector<std::pair<Student, int>> students;
    if (regime == BEFORE_I){
        for (int j = 0; j < i; j++){
            if (!series_readers[j].is_end_of_file()){
                series_readers[j].start_new_series();
                Student temp = series_readers[j].get_one_student();
                if (!series_readers[j].is_end_of_file()){
                    students.emplace_back(temp, j);
                }
            }
        }
    }
    else
    {
        for (int j = n - 1; j > i; j--){
            if (!series_readers[j].is_end_of_file()){
                series_readers[j].start_new_series();
                Student temp = series_readers[j].get_one_student();
                if (!series_readers[j].is_end_of_file()){
                    students.emplace_back(temp, j);
                }
            }
        }
    }
    while (students.size() > 1){
        int min_index = 0;
        for (int j = 1; j < students.size(); j++){
            if (!comparator(students[j].first, students[min_index].first)) {
                min_index = j;
            }
        }
        auto& [value, file] = students[min_index];
        files[i] << value;
        files[i].flush();
        if (Student temp = series_readers[file].get_one_student(); !series_readers[file].is_end_of_file() && !series_readers[file].is_end_of_series()){
            value = temp;
        }
            //если конец файла или серии, то убираем из тех, откуда надо читать
        else{
            students.erase(students.begin() + min_index);
        }
    }
    //остатки серии из последнего файла можно дочитать
    if (students.size() > 0){
        auto [last_elem, last_file] = students[0];
        files[i] <<last_elem;
        series_readers[last_file].write_one_series(files[i]);
        files[i].flush();
    }
}

void CascadeSorter::reopen_files() {
    for (int i = 0; i < n; i++) {
        files[i].close();
        files[i].open(paths_to_files[i], std::ios::binary|std::ios::in);
        series_readers[i].reset_file();
        if (files[i].fail()) throw std::invalid_argument("");
    }
}

void CascadeSorter::read_to_the_bone(int i, CascadeSorter::REGIME regime) {
    files[i].close();
    files[i].open(paths_to_files[i], std::ios::out|std::ios::in|std::ios::trunc|std::ios::binary);
    if (regime == BEFORE_I){
        while (expected_number_series[i-1] > 0){
            read_one_series(i, regime);
            for (int j = 0; j < i; j++) expected_number_series[j]--;
            expected_number_series[i]++;
        }
    }
    else {
        while (expected_number_series[i+1] > 0){
            read_one_series(i, regime);
            for (int j = n - 1; j > i; j--) expected_number_series[j]--;
            expected_number_series[i]++;
        }
    }
    files[i].close();
    files[i].open(paths_to_files[i], std::ios::in|std::ios::binary);
}

void CascadeSorter::sort(std::string_view path_out) {
    int cur = n - 1;
    do{
        reopen_files();
        if (cur == n - 1){
            for (;cur > 0; cur--){
                read_to_the_bone(cur, BEFORE_I);
            }
        }
        else{
            for (;cur < n - 1; cur++){
                read_to_the_bone(cur, AFTER_I);
            }
        }
    }
    while (expected_number_series[1] != 0);
    files[n - 1 - cur].close();
    std::filesystem::remove(path_out);
    std::filesystem::copy_file(paths_to_files[n - 1 - cur], path_out);
    clear();
}



CascadeSorter::~CascadeSorter() {
    clear();
}

CascadeSorter::CascadeSorter(const std::string &in_file_paths, size_t n,
                             std::function<bool(const Student &, const Student &)> comparator) : comparator(comparator), n(n){
    if (n < 3) throw std::invalid_argument("Некорректное количество серий");
    std::fstream in(in_file_paths, std::ios::in|std::ios::binary);
    if (!in.is_open()) throw std::invalid_argument("нет доступа к файлу");
    files.reserve(n);
    paths_to_files.reserve(n);
    std::filesystem::remove_all(temp_folder);
    if (!std::filesystem::create_directory(temp_folder)) {
        throw std::invalid_argument("Нет возможности создать папку с временными файлами");
    }
    for (int i = 0; i < n; i++){
        paths_to_files.emplace_back(temp_folder + "/" + std::to_string(i) + ".txt");
        files.emplace_back(paths_to_files[i], std::ios::out | std::ios::trunc| std::ios::binary);
        if (!files[i].is_open())
        {
            throw std::invalid_argument("Нет возможности создать временные файлы");
        }
    }
    int k = n - 1;
    expected_number_series.resize(k);
    expected_number_series[k-1] = 1;
    std::vector<int> read;
    read.resize(k);
    int cur_temp_file = 0;
    size_t cur = k-1;
    SeriesReader series_reader(in, comparator);
    while (!series_reader.is_end_of_file()){
        if (cur < k && read[cur] < expected_number_series[cur]){
            series_reader.start_new_series();
            series_reader.write_one_series(files[cur]);
            read[cur]++;
        }
        else if (cur < k) cur++;
        else {
            next_level(expected_number_series);
            cur = 0;
        }
    }
    expected_number_series.push_back(0);
    series_readers.reserve(n);
    for (int i = 0; i < n; i++) series_readers.emplace_back(files[i], comparator);
    reopen_files();
}
