#include <iostream>
#include <cmath>

using namespace std;

class Complex {
private:
    double Re;
    double Im;

public:
    Complex() : Re(0), Im(0) {}
    Complex(double real, double imagine) : Re(real), Im(imagine) {}


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
            throw runtime_error("Division by zero\n");
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
        cout << Re << " + " << Im << "i";
    }
};

int main() {
    cout << "Empty: \n";
    Complex c1;
    c1.Complex_cout();
    cout << "\n";

    Complex c2(5, 3);
    Complex c3(3, 4);

    cout << "Sum of 2: \n";
    c2.Complex_cout();
    cout << " + ";
    c3.Complex_cout();
    cout << " = ";
    Complex c4 = c2.add(c3);
    c4.Complex_cout();
    cout << "\n";

    cout << "Subtraction of 2: \n(";
    c2.Complex_cout();
    cout << ") - (";
    c3.Complex_cout();
    cout << ") = ";
    c4 = c2.subtraction(c3);
    c4.Complex_cout();
    cout << "\n";

    cout << "Multiplication of 2: \n(";
    c2.Complex_cout();
    cout << ") * (";
    c3.Complex_cout();
    cout << ") = ";
    c4 = c2.multiplication(c3);
    c4.Complex_cout();
    cout << "\n";

    cout << "Division of 2: \n(";
    c2.Complex_cout();
    cout << ") / (";
    c3.Complex_cout();
    cout << ") = ";
    try {
        c4 = c2.division(c3);
        c4.Complex_cout();
        cout << "\n";

    } catch (runtime_error &problem) {
        cout << "Error: " << problem.what();
    }

    Complex c5;
    cout << "Division of 2: \n(";
    c2.Complex_cout();
    cout << ") / (";
    c5.Complex_cout();
    cout << ") = ";
    try {
        c4 = c2.division(c5);
        c4.Complex_cout();
        cout << "\n";
    } catch (runtime_error &problem) {
        cout << "Error: " << problem.what();
    }

    cout << "Module: \n|";
    c2.Complex_cout();
    cout << "| = " << c2.complex_abs() << "\n";

    cout << "Argument: \narg(";
    c2.Complex_cout();
    cout << ") = " << c2.argument() << "\n";


    return 0;
}