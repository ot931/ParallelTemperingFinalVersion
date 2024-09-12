#include "loadhelper.h"
#include "PartArray.h"

LoadHelper::LoadHelper(std::string file):
    filename(file)
{
    this->f.open(file);
    if (!f.good())
        cerr<<"file "<<file<<" not found"<<endl;
}

LoadHelper::~LoadHelper()
{
    this->close();
}

bool LoadHelper::validate()
{
    //@todo дописать validate
    return LoadHelper::version(this->filename)>1;
}

void LoadHelper::parseHeader()
{
    go("header");
    std::string str;
    std::string::size_type strpos;
    while(this->getSectionLine(str)){
        strpos = str.find('=');
        if (strpos == std::string::npos)
            cerr<<"Skip param "<<str<<" in file "<<this->filename<<endl;
        else {
            this->params[str.substr(0,strpos)]=str.substr(strpos+1);
        }
    }
}

bool LoadHelper::go(std::string section)
{
    std::streampos p = f.tellg();
    f.seekg(0);
    section = '['+section+']';
    std::string str;
    while (!f.eof() && str != section){
        std::getline(f,str);
        rtrim(str);
    }
    if (str==section)
        return true;
    else {
        f.seekg(p);
        return false;
    }
}

bool LoadHelper::go(unsigned int num)
{
    f.seekg(0);
    for (int i=0;i<(int)num;i++)
        while(!line().empty()){}
    return f.eof();
}

void LoadHelper::close()
{
    if (f.is_open())
        f.close();
}

std::string LoadHelper::line()
{
    std::string str;
    if (!f.eof()){
        std::getline(f,str);
        rtrim(str);
    }
    return str;
}

bool LoadHelper::getSectionLine(std:: string & s)
{
    std::streampos p = f.tellg();
    std::string str;
    std::getline(f,str);
    rtrim(str);
    if (str.empty()){
        return false;
    } else if (str[0]=='[' && str[str.length()-1]==']'){
        f.seekg(p);
        return false;
    } else {
        s = str;
        return true;
    }
}

bool LoadHelper::end()
{
    
    std::streampos p = f.tellg();
    std::string str;
    bool res = getSectionLine(str);
    f.seekg(p);
    return !res;
}

LoadHelper &LoadHelper::operator >>(double &num)
{
    f>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(int &num)
{
    f>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(long &num)
{
    f>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(unsigned int &num)
{
    f>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(std::string &num)
{
    f>>num;
    return *this;
}

LoadHelper &LoadHelper::operator >>(bool &num)
{
    int n;
    f>>n;
    num = (bool)n;
    return *this;
}

void LoadHelper::applyHeader(PartArray *sys)
{
    this->applyHeader(sys,false);
}

int LoadHelper::version(std::string file)
{
    std::ifstream f(file);

    if (f.good()) {
        std::string s;
        std::getline(f,s);
        rtrim(s);
        if (s=="[header]"){
            f.close();
            return 2;
        } else {
            std::getline(f,s); //read 2 line
            std::getline(f,s); //read 3 line
            std::getline(f,s); //read 4 line
            if (s=="x\ty\tz\tMx\tMy\tMz\tr"){
                f.close();
                return 1;
            } else {
                f.close();
                return 0;
            }
        }
        f.close();
    } else {
        cerr<<"file "<<file<<" not found";
    }
    return 0;
}

void LoadHelper::applyHeader(PartArray *sys, bool readAnyWay)
{
    //read all parameters
    if (params.size()==0)
        this->parseHeader();

    if (readAnyWay)
        sys->setType(params["type"]);

    if (sys->type()!=params["type"])
        return;

    switch(std::stoi(params["dimensions"])){
    case 2:
        config::Instance()->set2D();
        break;
    case 3:
        config::Instance()->set3D();
        break;
    default:
        fprintf(stderr,"Dimensions in file %s are setted as %s, which is wrong!",this->filename,params["dimensions"]);
    }

    if (params.find("interactionrange") != params.end())
        sys->setInteractionRange(std::stod(params["interactionrange"]));

    if (params.find("emin") != params.end())
        sys->eMin = std::stod(params["emin"]);

    if (params.find("emax") != params.end())    
        sys->eMax = std::stod(params["emax"]);
    
    if (params.find("minstate") != params.end())
        sys->minstate.fromString(params["minstate"]);

    if (params.find("maxstate") != params.end())
        sys->maxstate.fromString(params["maxstate"]);

    //check the state of the system
    if (sys->state.toString() != params["state"]){
        fprintf(stderr,"Something gonna worng while reading system: system state and state in the header are not the same");
    }

    //check the system size
    if (sys->count() != stoi(params["size"])){
        fprintf(stderr,"Something gonna worng while reading system: system size and size in the header are not the same");
    }
}

std::map<std::string,std::string> LoadHelper::dumpFileContent()
{
    std::map< std::string, std::string > strList;
    f.seekg(0);
    std::string temp, section;
    while (!f.fail()) {
        std::getline(f,temp);
        rtrim(temp);
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
    return strList;
}
