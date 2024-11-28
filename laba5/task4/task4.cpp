#include <iostream>
#include <cmath>

//using namespace std;

class Complex final{
private:
    double Re;
    double Im;

public:
    //Complex() : Re(0), Im(0) {}
    explicit Complex(double real = 0, double imagine = 0) : Re(real), Im(imagine) {}


    Complex add(const Complex &second) const {
        return Complex(Re + second.Re, Im + second.Im);
    }

    Complex subtraction(const Complex &second) const {
        return Complex(Re - second.Re, Im - second.Im);
    }

    Complex multiplication(const Complex &second) const {
        return Complex(Re * second.Re - Im * second.Im, Im * second.Re + Re * second.Im);
    }

    Complex division(const Complex &second) const {
        if (second.Im == 0 && second.Re == 0) {
            throw std::logic_error("Division by zero\n");
        }

        double division = second.Re * second.Re + second.Im * second.Im;
        return Complex((Re * second.Re + Im * second.Im) / division,
                       (Im * second.Re - Re * second.Im) / division);
    }

    double complex_abs() const {
        return sqrt(Im * Im + Re * Re);
    }

    double argument() const {
        return atan(Im / Re);
    }

    void Complex_cout() const {
        std::cout << Re << " + " << Im << "i";
    }
};

int main() {
    std::cout << "Empty: \n";
    Complex c1;
    c1.Complex_cout();
    std::cout << "\n";

    Complex c2(5, 3);
    Complex c3(3, 4);

    std::cout << "Sum of 2: \n";
    c2.Complex_cout();
    std::cout << " + ";
    c3.Complex_cout();
    std::cout << " = ";
    Complex c4 = c2.add(c3);
    c4.Complex_cout();
    std::cout << "\n";

    std::cout << "Subtraction of 2: \n(";
    c2.Complex_cout();
    std::cout << ") - (";
    c3.Complex_cout();
    std::cout << ") = ";
    c4 = c2.subtraction(c3);
    c4.Complex_cout();
    std::cout << "\n";

    std::cout << "Multiplication of 2: \n(";
    c2.Complex_cout();
    std::cout << ") * (";
    c3.Complex_cout();
    std::cout << ") = ";
    c4 = c2.multiplication(c3);
    c4.Complex_cout();
    std::cout << "\n";

    std::cout << "Division of 2: \n(";
    c2.Complex_cout();
    std::cout << ") / (";
    c3.Complex_cout();
    std::cout << ") = ";
    try {
        c4 = c2.division(c3);
        c4.Complex_cout();
        std::cout << "\n";

    } catch (std::logic_error &problem) {
        std::cout << "Error: " << problem.what();
    }

    Complex c5;
    std::cout << "Division of 2: \n(";
    c2.Complex_cout();
    std::cout << ") / (";
    c5.Complex_cout();
    std::cout << ") = ";
    try {
        c4 = c2.division(c5);
        c4.Complex_cout();
        std::cout << "\n";
    } catch (std::logic_error &problem) {
        std::cout << "Error: " << problem.what();
    }

    std::cout << "Module: \n|";
    c2.Complex_cout();
    std::cout << "| = " << c2.complex_abs() << "\n";

    std::cout << "Argument: \narg(";
    c2.Complex_cout();
    std::cout << ") = " << c2.argument() << "\n";


    return 0;
}