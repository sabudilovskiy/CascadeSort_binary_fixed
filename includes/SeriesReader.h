//
// Created by MRV on 5/17/2022.
//

#ifndef CASCADESORT_SERIESREADER_H
#define CASCADESORT_SERIESREADER_H
#include <fstream>
#include <functional>
#include "Student.h"
class SeriesReader{
    bool end_of_file;
    bool end_of_series;
    bool begin_series;
    bool cached = false;
    Student prev_student;
    Student buffer;
    std::fstream& in_file;
    std::function<bool(const Student& left, const Student& right)> comparator;
public:
    SeriesReader(std::fstream& file, std::function<bool(const Student& left, const Student& right)>& comparator);
    void start_new_series();
    void reset_file();
    void write_one_series(std::fstream& out_file);
    Student get_one_student();
    bool is_end_of_series() const;
    bool is_end_of_file() const;
};
#endif //CASCADESORT_SERIESREADER_H
