/**
  Утилиты, полезные при исследовании
  */
#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <iostream>

#define MEASUREN(x,y,n) { \
    unsigned start_time =  clock(); \
    for (unsigned i=0;i<n;i++) x; \
    std::cout<<y<<": "<<std::setprecision(10)<<(std::clock()-start_time)/CLOCKS_PER_SEC/n<<endl;\
    }

#define MEASURE(x,y) MEASUREN(x,y,1);

#endif // UTILS_H
