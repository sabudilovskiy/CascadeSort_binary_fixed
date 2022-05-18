//
// Created by MRV on 5/17/2022.
//
#include "SeriesReader.h"
#include <iostream>


SeriesReader::SeriesReader(std::fstream &file, std::function<bool(const Student& left, const Student& right)>& comparator) :
        end_of_file(false), in_file(file), end_of_series(false), begin_series(true), comparator(comparator){
    if (file.eof() || !file.is_open()) {
        end_of_file = true;
    }
}

void SeriesReader::write_one_series(std::fstream &out_file) {
    if (!end_of_file && !end_of_series){
        if (cached){
            prev_student = buffer;
            begin_series = false;
            out_file << buffer;
            out_file.flush();
            cached = false;
        }
        while (!end_of_file && !end_of_series){
            if (!in_file.eof()){
                Student temp;
                in_file >> temp;
                if (!in_file.fail()){
                    if (begin_series){
                        begin_series = false;
                        prev_student = temp;
                        out_file << temp;
                        out_file.flush();
                    }
                    else if (comparator(temp, prev_student)){
                        prev_student = temp;
                        out_file << temp;
                        out_file.flush();
                    }
                        //серия закончилась
                    else{
                        end_of_series = true;
                        cached = true;
                        buffer = temp;
                    }
                }
                else end_of_file = true;
            }
            else end_of_file = true;
        }
    }
}

Student SeriesReader::get_one_student() {
    Student answer;
    if (!end_of_file && !end_of_series){
        if (!cached){
            Student temp;
            in_file >> temp;
            if (!in_file.fail())
            {
                if (begin_series){
                    begin_series = false;
                    prev_student = answer = temp;
                }
                else if (comparator(temp, prev_student)){
                    prev_student = answer = temp;
                }
                else
                {
                    end_of_series = true;
                    cached = true;
                    buffer = temp;
                }
            }
            else {
                end_of_file = true;
            }
        }
        else{
            answer = prev_student = buffer;
            cached = false;
        }
    }
    return answer;
}

bool SeriesReader::is_end_of_series() const {
    return end_of_series;
}

bool SeriesReader::is_end_of_file() const {
    return end_of_file;
}

void SeriesReader::start_new_series() {
    begin_series = true;
    end_of_series = false;
}

void SeriesReader::reset_file() {
    begin_series = true;
    end_of_series= false;
    end_of_file = false;
}

