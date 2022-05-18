//
// Created by MRV on 5/17/2022.
//

#ifndef CASCADESORT_STUDENT_H
#define CASCADESORT_STUDENT_H
#include <string>
#include <fstream>
class Student{
    char FIO[20];
    double average_mark;
public:
    Student(Student&& another){
        strcpy_s(FIO, another.get_fio());
        average_mark = another.average_mark;
    }
    Student( const Student& another) : average_mark(another.average_mark){
        strcpy_s(FIO, another.get_fio());
    }
    Student& operator=(Student&& another){
        strcpy_s(FIO, another.get_fio());
        average_mark = another.average_mark;
        return *this;
    }
    Student& operator=(const Student& another){
        strcpy_s(FIO, another.get_fio());
        average_mark = another.average_mark;
        return *this;
    }
    Student(){
        FIO[0] = 0;
    }
    Student(std::string fio, double average_mark):average_mark(average_mark){
        strcpy_s(FIO, fio.data());
    }
    const char *get_fio() const;
    double get_average_mark() const;
    void set_average_mark(double average_mark);
    friend std::fstream &operator>>(std::fstream &file, Student &student);
    friend std::fstream &operator<<(std::fstream &file, Student &student);
};
#endif //CASCADESORT_STUDENT_H
