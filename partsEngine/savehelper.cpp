#include "savehelper.h"
#include "PartArray.h"

SaveHelper::SaveHelper(std::string file, bool overwritePreviousData):
filename(file)
{
    if (!overwritePreviousData){
        f.open(file,ios_base::out);
        if (f.fail())
            fprintf(stderr,"saveHelper: file %s is unwriteable or not found\n", file.c_str());
        std::string temp, section;
        while (!f.eof()) {
            std::getline(f,temp);
            if (temp[0]=='[' && temp[temp.length()-1]==']'){
                temp.erase(temp.begin()); temp.erase(temp.end()-1);
                section = temp;
                strList[section].clear();
            } else{
                if (!strList[section].empty())
                    strList[section].append("\n");
                strList[section].append(temp);
            }
        }
        f.close();
    } else {
        f.open(file, ios_base::out|ios_base::trunc);
        if (f.fail())
            fprintf(stderr,"saveHelper: file %s is unwritable or not found\n", file.c_str());
    }
}

SaveHelper::~SaveHelper()
{
    this->close();
}

void SaveHelper::go(std::string section)
{
    this->section = section;
    strList[section].clear();
}

void SaveHelper::close()
{
    if (f.is_open()){
        if (strList.find("header")!=strList.end() && strList.find("parts")!=strList.end()){
            //пишем Header
            f<<"[header]"<<std::endl;
            f<<trim_copy(strList["header"]);
            f<<std::endl;

            //пишем parts
            f<<std::endl<<"[parts]"<<std::endl;
            f<<trim_copy(strList["parts"]);
            f<<std::endl;

            auto iter = strList.begin();
            while (iter != strList.end()){
                if (iter->first != "header" && iter->first != "parts"){
                    f<<std::endl<<"["<<iter->first<<"]"<<std::endl;
                    f<<trim_copy(iter->second);
                    f<<std::endl;
                }
                iter++;
            }
        }
        f.close();
    }
}

SaveHelper &SaveHelper::operator <<(const double num)
{
    char buf[100];
    snprintf(buf,100,"%.10e",num);
    write( std::string(buf) ); //write down 10 digits after comma
    return *this;
}

SaveHelper &SaveHelper::operator <<(const int num)
{
    write(std::to_string(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const unsigned int num)
{
    write(std::to_string(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const long int num)
{
    write(std::to_string(num));
    return *this;
}

SaveHelper &SaveHelper::operator <<(const std::string num)
{
    write(num);
    return *this;
}

SaveHelper &SaveHelper::operator <<(const bool num)
{
    write(std::to_string((int)num));
    return *this;
}

void SaveHelper::write(std::string str)
{
    if (section.empty())
        fprintf(stderr,"saveHelper: You can not write to the empty section\n");
    else
        strList[section].append(str).append("\t");
}

void SaveHelper::line(std::string str)
{
    if (section.empty())
        fprintf(stderr,"saveHelper: You can not write to the empty section\n");
    else
        strList[section].append(str).append("\n");
}

void SaveHelper::writeHeader(PartArray *sys)
{
    go("header");
    line("version=2");
    line(std::string("dimensions=")+std::to_string(config::Instance()->dimensions()));
    line(std::string("type=")+sys->type());
    line(std::string("size=")+std::to_string(sys->count()));
    line(std::string("emin=")+std::to_string(sys->eMin));
    line(std::string("emax=")+std::to_string(sys->eMax));
    line(std::string("state=")+sys->state.toString());
    line(std::string("minstate=")+sys->minstate.toString());
    line(std::string("maxstate=")+sys->maxstate.toString());
    line(std::string("interactionrange=")+std::to_string(sys->interactionRange()));
    line("sizescale=1");
    line("magnetizationscale=1");
}

void SaveHelper::writeDumped(std::map<std::string, std::string> dumped)
{
    auto iter = dumped.begin();
    while (iter != dumped.end()){
        if (strList.find(iter->first)==strList.end()){ //if not contains
            strList[iter->first] = iter->second;
        }
        iter++;
    }
}
