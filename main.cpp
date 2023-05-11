//REALIZZATO DA GIUSEPPE MUSCHETTA
//LAUREANDO MAGISTRALE IN INFORMATICA

#include <random>
#include "threadpool.h"
#include "utimer.h"
#include "prototypes.h"

using namespace std;

const int n = 80000;
const int workers = 16;
const int tp_workers = 16;
const int max_rand = 100;
bool debug = false;


int main() {
    vector<double> r(n);
    vector<double> v(n);
    init(v);
    vector<future<int>> results;


    // INIZIO CALCOLO TEMPO SEQUENZIALE
    long usecsSEQ;
    {
        utimer time_seq("Sequential Computation: ", &usecsSEQ);
        sequentialBody(v, r);
    }
    cout << "I've spent " << usecsSEQ << " usecs with sequential computing" << endl;
    // FINE CALCOLO TEMPO SEQUENZIALE
    ripristina(r);


    // INIZIO CALCOLO PARALLELO USANDO THREAD MANUALI
    vector<thread> tids; //se faccio vector<thread> tids(valore di inizio) mi da errore.
    long usecsPARtm;
    {
        utimer time_par("Parallel computation",&usecsPARtm);
        for (int index = 0; index < workers; index++) {
            tids.emplace_back(parallelBody, index, ref(v), ref(r));
        }

        for (auto& tid : tids){
            tid.join();
        }
    }
    cout << "I've spent " << usecsPARtm << " usecs using parallel computing with "
         << workers << " manual threads"  << endl;
    // FINE CALCOLO PARALLELO USANDO THREADS MANUALI
    ripristina(r);


    // INIZIO CALCOLO PARALLELO USANDO UN THREADPOOL
    long usecsPARtp;
    {
        utimer time_par("Parallel computation",&usecsPARtp);
        threadpool tpool(tp_workers);
        for(int i = 0; i < tp_workers; ++i){
            results.emplace_back(tpool.submit(parallelBody, i,ref(v),ref(r)));
        }
        if(debug){
            for(auto&& result: results){
                result.get();
            }
        }
    }
    cout << "I've spent " << usecsPARtp << " usecs using parallel computing with a threadpool of "
         << tp_workers << " threads " << endl;
    //FINE CALCOLO PARALLELO USANDO UN THREADPOOL


    if(debug){
        displayVector(v,r);
    }

    return 0;
}

// Funzione per generare numeri casuali nell'intervallo [0, max_rand]
int getRandomNumber() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, max_rand);
    return dis(gen);
}



//display vector elements
void displayVector(vector<double>& v, vector<double>& r) {
    for (unsigned long i = 0; i < v.size(); i++) {
        cout << i << ": " << v[i] << " -> " << r[i] << endl;
    }
}

//creo e inizializzo il vettore v con valori casuali
void init(vector<double>& vec) {
    for (double& element : vec) {
        element = getRandomNumber();
    }
}

//creo e inizializzo il v
//creo e inizializzo il vettore v con valori casuali
void ripristina(vector<double>& vec){
    for(double& element : vec){
        element = 0;
    }
}


//funzioni di mappatura f e g:
[[maybe_unused]] double f(double x) {
    return x * x;
}

double g(double x) {
    //faccio fare roba a caso per renderla più pesante
    for (int i = 0; i < 1000; i++) {
        if(x == 1){
            x = 0;
        }
        else {
            x = x + sqrt(x) / log(x);
        }
    }
    return x;
}

//funzione di esecuzione map per il calcolo sequenziale
void sequentialBody(vector<double>& v, vector<double>& r) {
    for (unsigned long i = 0; i < v.size(); i++) {
        r[i] = g(v[i]);
    }
}

//funzione di esecuzione map per il calcolo parallelo
//ho dichiarato int per fargli restituire qualcosa... serve per la future
int parallelBody(int index, vector<double>& v, vector<double>& r){
    //index ovviamente va da 0 a workers - 1
    //ogni worker lavora in parallelo sul proprio chunck (sub-array) di competenza
    //che inizia da start e finisce a end
    int chunk = n/workers;
    int start = index * chunk;

    int end;
    if(index == (workers - 1)){
        end = n;
    }
    else{
        end = (index + 1) * chunk;
    }
    for(int i=start; i<end; i++){
        r[i] = g(v[i]);
    }
    return index;
}



