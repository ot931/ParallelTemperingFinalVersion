#ifndef SAVEHELPER_H
#define SAVEHELPER_H

#include <fstream>
#include <string>
#include <map>
#include "stringfunctions.h"

class PartArray;
class SaveHelper
{
    friend class PartArray;
public:

    /**
     * @brief SaveHelper Конструктор сохранения. Открывает файл для записи
     * @param file Путь до сохраняемого файла
     * @param overwritePreviousData Перезаписать предыдущий файл.
     * Если true, удаляет все данные,
     * если false дописывает в существующий файл.
     * Однако функция go(QString) перезаписывает секцию в любом случае
     */
    SaveHelper(std::string file, bool overwritePreviousData = false);
    virtual ~SaveHelper();

    void go(std::string section);
    void close();

    SaveHelper & operator <<(const double num);
    SaveHelper & operator <<(const int num);
    SaveHelper & operator <<(const unsigned int num);
    SaveHelper & operator <<(const long int num);
    SaveHelper & operator <<(const std::string num);
    SaveHelper & operator <<(const bool num);
    void write(std::string str);

    void line(std::string str = "");

    void writeHeader(PartArray* sys);

private:
    void writeDumped(std::map<std::string, std::string> dumped);
    std::fstream f;
    std::string filename;
    std::map<std::string, std::string> strList;
    std::string section;
};

#endif // SAVEHELPER_H
