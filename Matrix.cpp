#include <iostream>
#include <cstdlib>
#define N 3
#define R "\033[22;33m"
#define E "\033[0m"
using namespace std;


class Matrix
{
    int Nstr;
    int Ncol;
    double** arr;

public:
    Matrix(int n, int k) {
        Nstr = n;
        Ncol = k;
        arr = new double* [n];
        for (int i = 0; i < n; ++i) { arr[i] = new double[k]; }
        for (int i = 0; i < Nstr; ++i) {
            for (int j = 0; j < Ncol; ++j) {
                arr[i][j] = 0;
            }
        }
    }
    Matrix(const Matrix& A) {
        Nstr = A.getstr();
        Ncol = A.getcol();
        arr = new double* [Nstr];
        for (int i = 0; i < Nstr; ++i) { arr[i] = new double[Ncol]; }
        for (int i = 0; i < Nstr; ++i) {
            for (int j = 0; j < Ncol; ++j) {
                arr[i][j] = A(i, j);
            }
        }
    }
    explicit Matrix(double lambda = 0) {
        Nstr = N;
        Ncol = N;
        arr = new double* [N];
        for (int i = 0; i < N; ++i) { arr[i] = new double[N]; }
        for (int i = 0; i < Nstr; ++i) {
            for (int j = 0; j < Ncol; ++j) {
                arr[i][j] = lambda * (i == j);
            }
        }
    }
    
    const int getstr() const { return Nstr; }
    const int getcol() const { return Ncol; }

    const double operator()(int i, int j) const { return arr[i][j]; }
    double& operator()(int i, int j) { return arr[i][j]; }
    
    Matrix& operator=(const Matrix& A) {
        // cout << Nstr << " " << A.getstr() << " " << Ncol << " " << A.getcol() << endl;
        if ((Nstr != A.getstr()) || (Ncol != A.getcol())) { cout << "664Incorrect size!\n"; }
        else {
            for (int i = 0; i < Nstr; ++i) {
                for (int j = 0; j < Ncol; ++j) {
                    arr[i][j] =  A(i, j);
                }
            }
        }
        return *this;
    }
    
    friend ostream& operator<<(ostream& s, Matrix A)
    {
        int n = A.getstr();
        int k = A.getcol();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                s << fixed;
                s.precision(2);
                s << A(i, j) << ' ';
            }
            s << endl;
        }
        return s;
    };

    Matrix& operator+=(const Matrix& A) {
        if (Nstr != A.getstr() || Ncol != A.getcol()) {
            cerr << "2Incorrect size!\n";
        }
        else {
            for (int i = 0; i < A.getstr(); ++i) {
                for (int j = 0; j < A.getcol(); ++j) {
                    arr[i][j] = arr[i][j] + A(i, j);
                }
            }
        }
        return *this;
    };
    Matrix operator+(const Matrix& A) const {
        return Matrix(*this) += A;
    };

    Matrix& operator-=(const Matrix& A) {
        if (Nstr != A.getstr() || Ncol != A.getcol()) {
            cerr << "2Incorrect size!\n";
        }
        else {
            for (int i = 0; i < A.getstr(); ++i) {
                for (int j = 0; j < A.getcol(); ++j) {
                    arr[i][j] = arr[i][j] - A(i, j);
                }
            }
        }
        return *this;
    };
    Matrix operator-(const Matrix& A) const {
        return Matrix(*this) -= A;
    };

    friend Matrix operator*(int lambda, Matrix& A) {
        Matrix* tmp;
        tmp = new Matrix(A.getstr(), A.getcol());
        for (int i = 0; i < A.getstr(); ++i) {
            for (int j = 0; j < A.getcol(); ++j) {
                (*tmp)(i, j) =  A(i,j) * lambda;
            }
        }
        Matrix K(*tmp);
        for(int i = 0; i < (*tmp).Nstr; ++i) { delete[] (*tmp).arr[i]; }
        delete[] (*tmp).arr;
        (*tmp).arr = NULL;
        delete tmp;
        return K;
    }
    friend Matrix operator*(Matrix& A, int lambda) {
        return lambda * A;
    }
    friend Matrix operator*(Matrix& A, Matrix& B) {
        if (A.getcol() != B.getstr()) {
            cerr << "1Incorrect size!\n";
            return A;
        }
        else {
            Matrix* C;
            C = new Matrix(A.getstr(), B.getcol());
            for (int i = 0; i < A.getstr(); ++i) {
                for (int j = 0; j < B.getcol(); ++j) {
                    (*C)(i, j) = 0;
                    for (int r = 0; r < A.getcol(); ++r) {
                        (*C)(i, j) += A(i, r) * B(r, j);
                    }
                }
            }
            Matrix K(*C);
            for(int i = 0; i < (*C).Nstr; ++i) { delete[] (*C).arr[i]; }
            delete[] (*C).arr;
            (*C).arr = NULL;
            delete C;
            return K;
        }
    };

    ~Matrix() {
        if(arr) {
            for (int i = 0; i < Nstr; ++i) {
                delete[] arr[i];
            }
            delete[] arr;
        } 
    }
};



int main()
{
    cout << R;
    int a = 5;
    Matrix A(3);
    A(0,0) = A(0,1) = A(0,2) = A(1,1) = A(1,0) = A(1,2) = A(2,0) = A(2,1) = A(2,2) = 1;
    Matrix B(4,3);
    B(0,0) = B(0,1) = B(0,2) = 1;
    B(1,0) = B(1,1) = B(1,2) = 2.3;
    B(2,0) = B(2,1) = B(2,2) = 3;
    B(3,0) = B(3,1) = B(3,2) = 4;
    Matrix F(3, 4);
    F(0,3) = F(1,3) = F(2,3) = 2;
    F(0,0) = F(0,1) = F(0,2) = 5;
    F(1,0) = F(1,1) = F(1,2) = 8;
    F(2,0) = F(2,1) = F(2,2) = 1;
    Matrix C(A.getstr(), A.getcol());
    C = (A + A);
    cout << 'C' << endl << C << endl;
    C = A * A;
    cout << 'C' << endl << C << endl;
    cout << 'A' << endl << A << endl;
    cout << 'F' << endl << F << endl;
    cout << 'B' << endl << B << endl;
    cout << "5 * B" << endl << 5 * B << endl;
    cout << "B * 5" << endl << B * 5 << endl;
    cout << 'B' << endl << B << endl;
    cout << "F * B"  << endl << F * B << endl;
    cout << "B * F"  << endl << B * F << endl;
    cout << E;
    cin.get();
    return 0;
}
