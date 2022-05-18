//
// Created by MRV on 5/18/2022.
//

#include "Support.h"
#include <iostream>
#include <fstream>
#include "Student.h"
void print_file(std::fstream& file) {
    while (!file.eof()){
        Student temp;
        file >> temp;
        if (!file.fail()) std::cout << temp.get_fio() << ' ' << temp.get_average_mark() << '\n';
    }
}
