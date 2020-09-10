#ifndef COMPLEX_H
#define COMPLEX_H

template <typename T> class complex {
    private:
        T imag = 0;
        T real = 0;

    public:
        // --------------------Constructors--------------------
        complex(){};
        complex(T, T);

        // Destructor
        virtual ~complex();
        // -------------Complex/Complex Arithmetic---------------

        complex<T> operator+( complex<T>& other);
        complex<T> operator-( complex<T>& other);
        complex<T> operator*( complex<T>& other);
        complex<T> operator*( double& other);
        complex<T> operator/( int& other);

        // ----------------------Accessors---------------------
        int abs();
        T get_real();
};

#include "complex.cpp"

#endif
