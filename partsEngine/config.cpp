#include "config.h"

config*  config::_self;

config::config(){
    //относительные параметры магнитной системы
    partR = 0.5;
    m = 1.;
    hc = 0;

    //меры перевода в абсолютные величины (на одну единицу относительной величины)
    santiMeter = 1E-7; //расстояние
    ergGauss =  1; //Магнитный момент
    ersted = 1; //Напряженность магнитного поля

    //Depticated
    I0s = 500;
    dstep = 0.0001;
    kv = 1E-9;


    set2D();

    rank = 0;
    size = 1;
    this->randmode_standart();
}

config::~config()
{
    delete config::_self;
}

config* config::Instance()
{
    if(!config::_self)
    {
        config::_self = new config();
    }
    return config::_self;
}

void config::set2D(){
    U2D = true;
    vol = M_PI * partR * partR;
}

void config::set3D(){
    U2D = false;
    vol = (4.0/3.0) * M_PI * partR * partR * partR;
}

unsigned short config::dimensions()
{
    if (this->U2D)
        return 2;
    else
        return 3;
}

int config::rand(){
    switch(this->_randmode){
    case 0:
        return std::rand();
        break;
    case 1:
        //если файл кончился, начинаем с начала
        if (this->_randfile.eof()){
            this->_randfile.clear();
            this->_randfile.seekg(0,std::ios_base::beg);
        }
        int i;
        this->_randfile>>i;
        return i;
        break;
    }

    return 0;
}

void config::srand(int num){
    switch(this->_randmode){
    case 0:
        std::srand(num);
        break;
    case 1:
        this->_randfile.seekg (0, std::ios_base::beg);
        char *temp = new char [1024];
        while (num>0){
            if (this->_randfile.eof()){
                this->_randfile.clear();
                this->_randfile.seekg(0,std::ios_base::beg);
            }
            this->_randfile.getline(temp,1024);
            num--;
        }

        break;
    }
}

void config::randmode_file(const char *file, int srand){
    (void)srand;
    this->rand_file_close(); //закрываем старый файл
    this->_randmode = 1;
    this->rand_file_open(file);
    //this->srand(srand);
    this->rand_max = 32767;
}

void config::randmode_standart(int srand){
    (void)srand;
    this->rand_file_close();
    this->_randmode = 0;
    //this->srand(srand);
    this->rand_max = RAND_MAX;
}

void config::rand_file_open(const char *file){
    this->_randfile.open(file,std::ios_base::in);
}

void config::rand_file_close(){
    if (this->_randfile.is_open())
        this->_randfile.close();
}
