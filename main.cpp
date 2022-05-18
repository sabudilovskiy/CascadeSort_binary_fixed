#include <iostream>
#include <vector>
#include "CascadeSorter.h"
#include "Student.h"
#include "Support.h"

int main() {
    setlocale(LC_ALL, "Russian");
    std::fstream file("in.txt", std::ios::binary|std::ios::in);
    if (file.is_open() && !file.eof()){
        std::cout << "До сортировки: ";
        print_file(file);
        file.close();
        std::cout << "================\n";
        try{
            CascadeSorter cascade_sorter("in.txt", 10, [](const Student& left, const Student& right){
                return left.get_average_mark() > right.get_average_mark();
            });
            cascade_sorter.sort("out.txt");
            file.open("out.txt", std::ios::in|std::ios::binary);
            if (file.is_open()){
                std::cout << "После сортировки: \n";
                print_file(file);
            }
        }
        catch (std::exception& error){
            std::cout << error.what();
        }
    }
    else std::cout << "Входной файл не найден.\n";
    std::cout << "нажмите любую клавишу для завершения работы...";
    std::cin.get();
}
