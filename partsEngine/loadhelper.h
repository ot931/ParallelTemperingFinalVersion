#ifndef LOADHELPER_H
#define LOADHELPER_H

#include <fstream>
#include <string>
#include <map>
#include "stringfunctions.h"

class PartArray;
class LoadHelper
{
    friend class PartArray;
public:
    LoadHelper(std::string file);
    virtual ~LoadHelper();

    bool validate();

    /**
     * @brief Parse the header of the file to the internal structure
     * 
     */
    void parseHeader();

    /**
     * 
     */

    /**
     * @brief Move the reading cursor to desired section. 
     * The cursor wil stand on the next line after section name
     * 
     * @param section name of the section, without brackets
     * @return true if section found and cursor is moved
     * @return false if there is no such section and cursor is left unchanged
     */
    bool go(std::string section);

    /**
     * 
     */

    /**
     * @brief Move the reading cursor to line number 'num'
     * 
     * @param num line number
     * @return true if cursor moved successfully
     * @return false if cursor reached the end of file
     */
    bool go(unsigned int num);
    void close();

    //функции чтения
    std::string line();
    bool end();

    /**
     * @brief Get the one line in the current section.
     * If the section is finished or started next section, 
     * this function returns false and leave string unchanged.
     * 
     * @param s string where to write the line
     * @return true if the line recieved successfully
     * @return false if there is no more lines in the current section
     */
    bool getSectionLine(std::string & s);
    LoadHelper & operator >>(double & num);
    LoadHelper & operator >>(int & num);
    LoadHelper & operator >>(long & num);
    LoadHelper & operator >>(unsigned int & num);
    LoadHelper & operator >>(std::string & num);
    LoadHelper & operator >>(bool & num);

    void applyHeader(PartArray* sys);

    static int version(std::string file);

    std::map<std::string,std::string> params;

private:
    void applyHeader(PartArray *sys, bool readAnyWay);
    std::map<std::string,std::string> dumpFileContent();
    std::ifstream f;
    std::string filename;
};

#endif // LOADHELPER_H
