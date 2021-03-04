#include <iostream>
#define N 3
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
    Matrix& fillMatrix(int n, ...) {
        int i = 0;
        int j = 0;
        for (int* ptr = &n; n > 0; n--, j++) {
            if (j == Ncol) { j = 0; i++; }
            arr[i][j] = *(++ptr);
        }
        return *this;
    }

    const double operator()(int i, int j) const { return arr[i][j]; }
    double& operator()(int i, int j) { return arr[i][j]; }
    
    Matrix& operator=(const Matrix& A) {
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
    
    friend ostream& operator<<(ostream& s, Matrix& A)
    {
        int n = A.getstr();
        int k = A.getcol();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                s << fixed;
                s.precision(3);
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

    friend Matrix& operator*(int lambda, Matrix& A) {
        for (int i = 0; i < A.getstr(); ++i) {
            for (int j = 0; j < A.getcol(); ++j) {
                A(i, j) *= lambda;
            }
        }
        return A;
    }
    friend Matrix& operator*(Matrix& A, int lambda) {
        return lambda * A;
    }
    Matrix& operator*=(const Matrix& A) {
        if (Ncol != A.getstr()) {
            cerr << "1Incorrect size!\n";
            return *this;
        }
        else {
            Matrix C(*this);
            for (int i = 0; i < Nstr; ++i) {
                for (int j = 0; j < A.getcol(); ++j) {
                    arr[i][j] = 0;
                    for (int r = 0; r < Ncol; ++r) {
                        arr[i][j] += C(i, r) * A(r, j);
                    }
                }
            }
            return *this;
        }
    }
    friend Matrix& operator*(Matrix& A, Matrix& B) {
        if (A.getcol() != B.getstr()) {
            cerr << "1Incorrect size!\n";
            return A;
        }
        else {
            Matrix* C;
            C = new Matrix;
            (*C).Nstr = A.getstr();
            (*C).Ncol = B.getcol();
            (*C).arr = new double* [(*C).Nstr];
            for (int i = 0; i < (*C).Nstr; ++i) { (*C).arr[i] = new double[(*C).Ncol]; }
            for (int i = 0; i < A.getstr(); ++i) {
                for (int j = 0; j < B.getcol(); ++j) {
                    (*C)(i, j) = 0;
                    for (int r = 0; r < A.getcol(); ++r) {
                        (*C)(i, j) += A(i, r) * B(r, j);
                    }
                }
            }
            return *C;
        }
    };

    ~Matrix() {
        for (int i = 0; i < Nstr; ++i) {
            delete[] arr[i];
        }
        delete[] arr; 
    }
};



int main()
{
    int a = 5;
    Matrix A(3);
    A.fillMatrix(9, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    Matrix B(4,3);
    B.fillMatrix(12, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    Matrix F(3, 4);
    F.fillMatrix(12, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    Matrix C(A.getstr(), A.getcol());
    C = (A + A);
    cout << C << endl;
    C = A * A;
    cout << C << endl;
    cout << B * F << endl;
    cout << F * B << endl;
    cin.get();
    return 0;
}
