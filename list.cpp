#include <iostream>
#include <cstring>

class Array {
public:
    Array* ptr;

    virtual void printv(std::ostream& s) = 0;
    virtual Array* next() = 0;
    virtual void del()  = 0;
    virtual int& operator()(int n) = 0;

    friend std::ostream& operator<<(std::ostream& s, Array *A) {
        Array *p;
        p = A;
        while(p) {
            p->printv(s);
            p = p->ptr;
        }
        return s;
    }

    ~Array() {
        delete ptr;
    }
};

class IntArray : public Array {
private:
    int value;
    void del() {
        if(this->ptr) { this->next()->del(); }
        else {
            delete this;
        }
    }

public:
    explicit IntArray(int v = 0) : value(v) { ptr = NULL; };
    Array* next() {
        return ptr;
    }
    void printv(std::ostream& s) { s << value << ' '; }

    int& operator()(int n) {
        int i = 1;
        IntArray* tmp = this;
        while(tmp && (i < n)) {
            tmp = (IntArray*) tmp->ptr;
            i++;
        }
        if((i == n) && tmp) {
            return tmp->value;
        }
        else {
            std::cerr << "Segmentation fault" << std::endl;
            return tmp->value;
        }
    }

    ~IntArray() {
        if(this) this->del();
    }
};


class CharArray : public Array {
private:
    char value;
    void del() {
        if(this->ptr) { this->next()->del(); }
        else {
            delete this;
        }
    }
public:
    explicit CharArray(char v = '0') : value(v) { ptr = NULL; };
    Array* next() {
        return ptr;
    }
    void printv(std::ostream& s) { s << (char) value << ' '; }

    int& operator()(int n) {
        int i = 1;
        CharArray* tmp = this;
        while(tmp && (i < n)) {
            tmp = (CharArray*) tmp->ptr;
            i++;
        }
        if((i == n) && tmp) {
            return (int&) tmp->value;
        }
        else {
            std::cerr << "Segmentation fault" << std::endl;
            return (int&) tmp->value;
        }
    }

    ~CharArray() {
        if(this) this->del();
    }
};

Array* addchar(Array* A, char v) {
    if(A->ptr) { addchar(A->next(), v); }
    else {
        A->ptr = new CharArray(v);
    }
    return A;
}

Array* addint(Array* A, int v) {
    if(A->ptr) { addint(A->next(), v); }
    else {
        A->ptr = new IntArray(v);
    }
    return A;
}

int main() {
    Array *A;
    A = new CharArray;
    addint(A, 1);
    addchar(A, 'h');
    addchar(A, '&');
    addint(A, 87);
    addchar(A, 'l');
    addchar(A, 'k');
    std::cout << A << '!' << std::endl;
    std::cout << A << '!' << std::endl;
    std::cout << (*A)(5) << std::endl;
    delete A;
    return 0;
}
