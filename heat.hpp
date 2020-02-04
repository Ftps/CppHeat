//
// Created by User on 03/02/2020.
//

#ifndef HEAT_HPP
#define HEAT_HPP

#include "cg.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include <cmath>

template <int n, typename T>
class Heat{
//private:
    const double alpha;
    const int m;
    const double dt;
    const int dim;
    Matrix<T> M;

public:
    //Constructors
    Heat(double a, int mm, double delta_t) : alpha(a), m(mm), dt(delta_t), dim(pow(m,n)){
        M = createM();
    }

    //Methods
    //Creates an identity matrix dim x dim
    template<typename U=T>
    Matrix<U> identity(){
        Matrix<U> I(dim,dim);
        for (auto i = 0; i < dim; ++i) {
            I[{i,i}] = 1;
        }
        return I;
    }

    //Francisco, this is yours :)
    template<typename U=T>
    Matrix<U> createD()
    {
        Matrix<U> D = (n*-2)*identity();

        for(int i = 0; i < dim; ++i){
            for(int j = i+1; j < dim; ++j){
                for(int k = 0; k < n; ++k){
                    if((i+pow(m,k))%pow(m,k+1) == (i%pow(m,k+1))){
                        D[{i, i+pow(m,k)}] += 1;
                    }
                }
            }
        }

        return D;
    }

    //Create M
    template<typename U=T>
    Matrix<U> createM(){
        double const dx=1/(m+1);
        double const coefficient = alpha*dt/pow(dx,2);
        Matrix<U> D = createD();

        M = identity();

        for (typename map<T>::iterator it = D.iter(); !D.end(it); it++) {
            M[it->first] -= coefficient*it->second;
        }
    }

    //Exact Solution
    template<int dim>
    void calc_exact(Vector<T>& x, Vector<T>& u, int& counter, T t){
        for(x[dim]=1; x[dim]<=m; x[dim]++){
            calc_exact(dim-1,x, counter);
        }
    }

    //last iteration - 1 dimension, a line with m nodes
    template<>
    void calc_exact<1>(Vector<T>& x, Vector<T>& u, int& counter, T t){
        double const dx=1/(m+1);
        for(x[1]=1; x[1]<=m; x[1]++){
            for (auto i = 0; i < n ; ++i){ //product of sin(pi*x_i) , 0<i<n-1
                u[counter]*=sin(M_PI*x[i]*dx);
                counter++;
            }
            u[counter]*=exp(-n*M_PI^2*alpha*t);
            counter++;
        }
    }

    template<typename U=T>
    Vector<U> exact(U t) const{
        Vector<U> u(pow(m,n)); //vector with the result
        Vector<int> x(n); //vector that holds the current position
        int counter=0;

        calc_exact(x,u,counter,t);
    }

};

#endif // HEAT_HPP
