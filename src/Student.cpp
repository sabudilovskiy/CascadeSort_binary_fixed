#include "Student.h"
template <class T>
std::pair<char*, size_t> binary_pair(T& t){
    return std::make_pair((char*)(&t), sizeof(t));
}
template<class T, size_t size>
std::pair <char*, size_t> binary_pair(T* t){
    return std::make_pair((char*)(t), size);
}
std::fstream &operator>>(std::fstream &file, Student &student) {
    auto [pointer, size] = binary_pair<const char, 20>(student.get_fio());
    file.read(pointer, size);
    auto [pointer2, size2] = binary_pair(student.average_mark);
    file.read(pointer2, size2);
    return file;
}
std::fstream &operator<<(std::fstream &file, Student &student) {
    auto [pointer, size] = binary_pair<const char, 20>(student.get_fio());
    file.write(pointer, size);
    auto [pointer2, size2] = binary_pair(student.average_mark);
    file.write(pointer2, size2);
    return file;
}
void Student::set_average_mark(double average_mark) {
    Student::average_mark = average_mark;
}

double Student::get_average_mark() const {
    return average_mark;
}

const char *Student::get_fio() const {
    return FIO;
}
