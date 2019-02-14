#include <iostream>
#include <cmath>
#include <sstream>
#include<cstring>

using namespace std;

class MyVector { //definicja klasy
public:
    explicit MyVector(size_t n); //ponizej seria konstruktorow (pierwsze dwa tutaj to sa ZWYKLE)
    MyVector(std::initializer_list<double> iLst); //ten konstruktor przyjmuje liste inicjalizatorow (nie inicjalizacyjna)
    MyVector(const MyVector &other); //konstruktor bo nie ma typu zwracanego. Jest to typ KOPIUJACY
    MyVector(MyVector &&other); //to jest konstruktor PRZENOSZACY

    ~MyVector(); //destruktor

    MyVector& operator=(const MyVector &other); //operatory PRZYPISANIA KOPIUJACY
                                                //Konstrukcja: referencja do klasy, operator przypisania i w nawiasie referencja
    MyVector& operator=(MyVector &&other); // to operator PRZYPISANIA PRZENOSZACY (mamy &&). "przenoszacy zawsze oddaje zrodlo"

    double& operator[](size_t idx) { return elem_at(idx); } //operator dostepu do indeksowania
    double& operator[](size_t idx) const { return elem_at(idx); } //tutaj mamy CONST to jest inna funkcja chodz robimy to samo
        //na rzecz obiektu ktory jest staly nie moge wywolac zadnej innej fcji ktora nie jest const

    size_t size() const { return size_; }

private:
    double& elem_at(size_t idx) const; //to jest funkcja prywatna, pomocnicza, ktora robi to samo co powyzej. to jest taki jakby delegat
                                        //mam wywolanie z dwoch miejsc linie 21 i 22 ktore zwracaja te wlasnie funkcje

    double *elems_;
    size_t size_;

    friend ostream& operator<<(ostream&, const MyVector&);
    friend bool operator==(const MyVector&, const MyVector&); //przekazuje dwie wartosci const i zwracam typ logiczny
};


MyVector::MyVector(size_t n) : elems_{new double[n]}, size_{n}
{
    for (size_t i = 0; i < n; ++i) { elems_[i] = 0; }
}

MyVector::MyVector(std::initializer_list<double> iLst) :
    elems_{new double[iLst.size()]},
    size_{iLst.size()}
{
    copy(iLst.begin(), iLst.end(), elems_); //fcja ktora z listy bierze poczatek i koniec i kopiuj wszystko
}

MyVector::MyVector(const MyVector &other) :
    elems_{new double[other.size_]}, //konstruktor kopiujacy - "alokujemy sobie nowa zone ;)
    size_{other.size_}
{
    memcpy(this->elems_, other.elems_, other.size_ * sizeof(double)); //to tez fcja kopiujaca ale DOCZYTAJ O NIEJ
}

MyVector::MyVector(MyVector &&other) : //w przypadku knstruktora przenoszacego ...
    elems_{other.elems_}, // grab the elems / ... ustawiamy wskaznik dokladnie na to samo co jest w kopii - patrz rys
    size_{other.size_}
{
    other.elems_ = nullptr; //podczas przenoszenia ustawiam null-pointer
    other.size_ = 0;
}

MyVector::~MyVector() {
    if (elems_ != nullptr) {
        delete[] elems_; //jak mamy pojedynczy element to mamy pojedynczy DELETE, a jak mamy tablice to mamy DELETE[] !!!!
    }
    elems_ = nullptr;
}

MyVector& MyVector::operator=(const MyVector &other)
{
    //first, check for self-assignment
    if (&other != this) {
        delete[] this->elems_; // first, delete the old elems
        this->elems_ = new double[other.size_];
        this->size_ = other.size_;
        memcpy(this->elems_, other.elems_, other.size_ * sizeof(double));
    }

    return *this;
}

MyVector& MyVector::operator=(MyVector &&other)
{
    //first, check for self-assignment
    if (&other != this) {
        delete[] this->elems_; // first, delete the old elems

        // next, grab the new ones
        this->elems_ = other.elems_;
        this->size_ = other.size_;

        other.elems_ = nullptr; //wskaznik ustawiony na nullpointer - to takie jakby wysypisko wrakow :)
        other.size_ = 0;
    }

    return *this;
}

double& MyVector::elem_at(size_t idx) const {
    if (idx > size_ - 1) { throw out_of_range("error: index out of range"); }
    return elems_[idx];
}

ostream& operator<<(ostream& os, const MyVector& v)
{
    ostringstream sstr;

    sstr << "[";
    for (size_t i = 0; i < v.size_ - 1; ++i) { sstr << v.elems_[i] << ","; }
    sstr << v.elems_[v.size_ - 1] << "]";

    os << sstr.str() << endl;

    return os;
}

bool operator==(const MyVector& v1, const MyVector& v2)
{
    if (v1.size() != v2.size()) {
        return false;
    }
    else {
        //memcmp(b1,b2,size) = 0 => the contents of both memory blocks are equal
        return memcmp(v1.elems_, v2.elems_, v1.size_ * sizeof(double)) == 0;
    }
}

bool operator!=(const MyVector& v1, const MyVector& v2)
{
    return !(v1 == v2);
}

int main()
{
    MyVector mv1 = {0,1,2,3,4,5,6,7,8,9};
    cout << "mv1 = " << mv1 << endl;
    return 0;
}
