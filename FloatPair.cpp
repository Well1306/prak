#include <iostream>
#include <algorithm>
#include <cstdio>

class FloatPair {
private:
    float a;
    float b;
protected:
    const float GetA() const { return a; }
    const float GetB() const { return b; }
    void SetA(float v) { a = v; }
    void SetB(float v) { b = v; } 
public:
    virtual float Measure() const = 0;
};

class BadSegment {};
class BadAddition {
private:
    float A1;
    float B1;
    float A2;
    float B2;
public:
    BadAddition(float a1, float b1, float a2, float b2) : A1(a1), B1(b1), A2(a2), B2(b2) {}
    const float GetA1() const { return A1; } 
    const float GetB1() const { return B1; } 
    const float GetA2() const { return A2; } 
    const float GetB2() const { return B2; }     
    void SetA1(float v) { A1 = v; } 
    void SetB1(float v) { B1 = v; } 
    void SetA2(float v) { A2 = v; } 
    void SetB2(float v) { B2 = v; } 
};

class Segment : public FloatPair {
public:
    explicit Segment(float a1 = 0, float b1 = 0) {
        try {
            BadSegment a;
            if(b1 - a1 < 0) throw a;
        } catch(int) {};
        (*this).SetA(a1);
        (*this).SetB(b1);
    }
    
    float Measure() const { return (*this).GetB() - (*this).GetA(); }

    Segment operator+(Segment s) {
        try {
            if((s.GetA() > (*this).GetB()) || ((*this).GetA() > s.GetB())) {
                BadAddition err((*this).GetA(), (*this).GetB(), s.GetA(), s.GetB());
                throw err;
            }
        } catch(int) {};
        Segment sum;
        sum.SetA(std::min((*this).GetA(), s.GetA()));
        sum.SetB(std::max((*this).GetB(), s.GetB()));
        return sum;
    }
};

int main() {

     try { 

             Segment f (1,2), g(0.5, 5), h(2.5, 6.5);;

             printf("%3.3f, %3.3f, %3.3f\n", (f+g).Measure(), (g+h).Measure(), (f+g+h).Measure());

             printf("%3.3f \n", (f+h).Measure()); 

     }

     catch (const BadAddition &bad) { printf("Bad addition: [%3.3f; %3.3f] + [%3.3f; %3.3f]\n", 

bad. GetA1(), bad. GetB1(), bad. GetA2(), bad. GetB2()); }

     catch (BadSegment b) { printf("Bad segment\n"); }

     return 0; 

}
