#include "amorphousarray.h"

AmorphousArray::AmorphousArray()
{

}

/*
double PartArray::destiny(bool simple){
    double surfVol = this->size.x * this->size.y;
    if (config::Instance()->dimensions()==3) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    if (simple)
        return (config::Instance()->vol * this->parts.size()) / surfVol;
    else{
        double destiny=0;
        std::vector < Part* >::iterator iterator1 = this->parts.begin();
        while (iterator1!=this->parts.end()){
            destiny+=(*iterator1)->volume()/surfVol;
            iterator1++;
        }
        return destiny;
    }
}

void PartArray::dropRandom(double maxDestiny) {

    double surfVol = this->size.x * this->size.y;
    if (config::Instance()->dimensions()==3) surfVol *= this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    Part* temp; //временная частица
    int partCount = this->parts.size(); //количество сброшеных частиц
    double destiny; //плотность заполнения образца

    std::vector < Part* >::iterator iterator1; // итератор для обхода массива частиц
    bool regenerate; //Флаг, нужен для проверки перекрещивания частиц
    int dropErrorCount=0; //количество ошибочных сбросов подряд

    do {

        //std::cout <<"Drop "<<partCount<<endl;

        temp = new Part();
        dropErrorCount=0;
        do {
            regenerate = false;
            //генерим координаты
            temp->pos.x = config::Instance()->rand() % ((int)(this->size.x-config::Instance()->partR*2)*100);
            temp->pos.y = config::Instance()->rand() % ((int)(this->size.y-config::Instance()->partR*2)*100);
            if (config::Instance()->dimensions()!=3) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            temp->pos.z = temp->pos.z / 100 + config::Instance()->partR;

            //проверяем чтобы сгенная точка не пересекалась ни с какой другой (это значит что площади их сфер не пересекались)
            iterator1 = this->parts.begin();
            while (iterator1 != this->parts.end()) {
                if (temp->pos.radius((*iterator1)->pos).length()<=config::Instance()->partR*2){
                    regenerate=true;

                    //std::cout<<"Drop "<<partCount<<" particle error, repeat"<<endl;
                    break;
                }
                ++iterator1;
            }
            dropErrorCount++;
        } while (regenerate && dropErrorCount<50000);

        if (dropErrorCount>=50000) break; //если частицы уже не кидаются, брэйк

        //генерируем вектор частицы
        double longitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2*M_PI;
        double lattitude;
        if (config::Instance()->dimensions()==2)
            lattitude=0; // если частица 2-х мерная то угол отклонения должен быть 0
        else
            lattitude=(double)config::Instance()->rand()/(double)config::Instance()->rand_max*2-1;

        temp->m.x = config::Instance()->m * cos(longitude)*sqrt(1-lattitude*lattitude);
        temp->m.y = config::Instance()->m * sin(longitude)*sqrt(1-lattitude*lattitude);
        temp->m.z = config::Instance()->m * lattitude;

        //добавляем частицу на экземпляр
        insert(temp);
        partCount++;

        //считаем плотность заполнения экземпляра
        destiny = (config::Instance()->vol * partCount) / surfVol;
    } while (destiny < maxDestiny);
}

void PartArray::dropRandom(int count) {
    this->clear();
    //double surfVol = this->size.x * this->size.y * this->size.z; //считаем объем (площадь) поверхности, в которую кидаем частицы
    Part* temp; //временная частица
    int partCount = this->parts.size(); //количество сброшеных частиц
    count+=partCount;

    std::vector < Part* >::iterator iterator1; // итератор для обхода массива частиц
    bool regenerate; //Флаг, нужен для проверки перекрещивания частиц
    while (partCount < count) {
        //std::cout <<"Drop "<<partCount<<endl;
        temp = new Part();
        do {
            regenerate = false;
            //генерим координаты
            temp->pos.x = config::Instance()->rand() % ((int)(this->size.x-config::Instance()->partR*2)*100);
            temp->pos.y = config::Instance()->rand() % ((int)(this->size.y-config::Instance()->partR*2)*100);
            if (config::Instance()->dimensions()==2) //если работаем в плоскости, то генерить третью координату не надо
                temp->pos.z = 0;
            else
                temp->pos.z = config::Instance()->rand() % ((int)(this->size.z-config::Instance()->partR*2)*100);

            temp->pos.x = temp->pos.x / 100 + config::Instance()->partR;
            temp->pos.y = temp->pos.y / 100 + config::Instance()->partR;
            temp->pos.z = temp->pos.z / 100 + config::Instance()->partR;

            //проверяем чтобы сгенная точка не пересекалась ни с какой другой (это значит что площади их сфер не пересекались)
            iterator1 = this->parts.begin();
            while (iterator1 != this->parts.end()) {
                if (temp->pos.radius((*iterator1)->pos).length()<=config::Instance()->partR*2){
                    regenerate = true;
                    //std::cout<<"Drop "<<partCount<<" particle error, repeat"<<endl;
                    break;
                }
                ++iterator1;
            }
        } while (regenerate);

        //генерируем вектор частицы
        double longitude = ((double)config::Instance()->rand()/(double)config::Instance()->rand_max) * 2. * M_PI;
        double lattitude;
        if (config::Instance()->dimensions()==2) lattitude=0; else lattitude=(double)config::Instance()->rand() / (double)config::Instance()->rand_max * 2. - 1.; // если частица 2-х мерная то угол отклонения должен быть 0

        temp->m.x = config::Instance()->m * cos(longitude) * sqrt(1-lattitude*lattitude);
        temp->m.y = config::Instance()->m * sin(longitude) * sqrt(1-lattitude*lattitude);
        temp->m.z = config::Instance()->m * lattitude;

        //добавляем частицу на экземпляр
        this->insert(temp);
        partCount++;
    }
}

void PartArray::scaleSystem(double coff){
    this->size.x *= coff;
    this->size.y *= coff;
    if (!config::Instance()->dimensions()==2)
        this->size.z *= coff;

    vector<Part*>::iterator iter;
    iter = this->parts.begin();
    while (iter!=this->parts.end()){
        (*iter)->pos.x *= coff;
        (*iter)->pos.y *= coff;
        if (!config::Instance()->dimensions()==2)
            (*iter)->pos.z *= coff;
        iter++;
    }
}

*/
