//
// Created by Giuseppe Muschetta on 11/05/23.
//

#ifndef THREAD_POOL_ULTIMATE_PROTOTYPES_H
#define THREAD_POOL_ULTIMATE_PROTOTYPES_H

int getRandomNumber();
void displayVector(vector<double>& v, vector<double>& r);
void init(vector<double>& vec);
void ripristina(vector<double>& vec);

[[maybe_unused]] double f(double x);
double g(double x);
void sequentialBody(vector<double>& v, vector<double>& r);
int parallelBody(int index, vector<double>& v, vector<double>& r);


#endif //THREAD_POOL_ULTIMATE_PROTOTYPES_H
