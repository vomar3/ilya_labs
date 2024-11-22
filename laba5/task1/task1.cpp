#include <iostream>
#include <bitset>

using namespace std;

void add(int &a, int &b);

void multiplication(int &a, int &b);

class binary_int {
private:
    int number;
public:
    binary_int() : number(0) {}
    explicit binary_int(int num) : number(num) {}

    int get_number() const {
        return number;
    }

    binary_int& operator - () {
        int one = 1;
        this->number = ~this->number;
        add(this->number, one);
        return *this;
    }

    binary_int& operator ++ () { //pre
        int one = 1;
        add(this->number, one);
        return *this;
    }

    binary_int operator ++ (int) {
        int one = 1;
        binary_int copy = *this;
        add(this->number, one);
        return copy;
    }

    binary_int &operator -- () {
        int num = 1, one = 1;
        num = ~num;
        add(num, one);
        add(this->number, num);
        return *this;
    }

    binary_int operator -- (int) {
        binary_int copy = *this;
        int num = 1, one = 1;
        num = ~num;
        add(num, one);
        add(this->number, num);
        return copy;
    }

    binary_int &operator += (int num) {
        add(this->number, num);
        return *this;
    }

    binary_int operator + (int num) const {
        int new_number = this->number;
        add(new_number, num);
        return binary_int(new_number);
    }

    binary_int &operator -= (int num) {
        int one = 1;
        num = ~num;
        add(num, one);
        add(this->number, num);
        return *this;
    }

    binary_int operator - (int num) const {
        int new_number = this->number, one = 1;
        num = ~num;
        add(num, one);
        add(new_number, num);
        return binary_int(new_number);
    }

    binary_int &operator *= (int num) {
        multiplication(this->number, num);
        return *this;
    }

    binary_int operator * (int num) const {
        int new_number = this->number;
        multiplication(new_number, num);
        return binary_int(new_number);
    }

    binary_int &operator <<= (int num) {
        this->number = this->number << num;
        return *this;
    }

    binary_int &operator >>= (int num) {
        this->number = this->number >> num;
        return *this;
    }

    binary_int operator << (int num) const {
        int new_number = this->number;
        new_number = new_number << num;
        return binary_int(new_number);
    }

    binary_int operator >> (int num) const {
        int new_number = this->number;
        new_number = new_number >> num;
        return binary_int(new_number);
    }

    friend ostream &operator << (ostream &os, const binary_int &answer) {
        os << bitset<sizeof(int) * 8>(answer.get_number());
        return os;
    }

    static pair <binary_int, binary_int> get_bites(const binary_int &answer){
        int numb = answer.get_number();
        size_t size = sizeof(numb) * 8;
        size_t half = size / 2;

        int high_mask = numb >> half;
        int low_mask = (numb << half) >> half;

        return pair <binary_int, binary_int> (binary_int(high_mask), binary_int(low_mask));
    };

};

int main() {

    binary_int c1(52), c2, c3, c4, c5, c6;
    cout << "Number: " << c1.get_number() << "\n";
    -c1;
    cout << "-Number: " << c1.get_number() << "\n";
    -c1;
    cout << "-Number again: " << c1.get_number() << "\n";
    c1++;
    cout << "Number++: " << c1.get_number() << "\n";
    ++c1;
    cout << "++Number: " << c1.get_number() << "\n";
    c1--;
    cout << "Number--: " << c1.get_number() << "\n";
    --c1;
    cout << "--Number: " << c1.get_number() << "\n";
    c1 += 2;
    cout << "Number + 2: " << c1.get_number() << "\n";
    c1 -= 2;
    cout << "Number - 2: " << c1.get_number() << "\n";
    c2 = c1 + 10;
    cout << "c2 = c1 + 10; ";
    cout << "c1: " << c1.get_number() << ", c2: " << c2.get_number() << "\n";
    c2 = c1 - 123;
    cout << "c2 = c1 - 123; ";
    cout << "c1: " << c1.get_number() << " c2: " << c2.get_number() << "\n";
    c2 *= 5;
    cout << "Number (c2) * 5: " << c2.get_number() << "\n";
    c3 = c1 * 11;
    cout << "c3 = c1 * 11; ";
    cout << "c1: " << c1.get_number() << ", c3: " << c3.get_number() << "\n";
    c3 <<= 2;
    cout << "Number <<= 2: " << c3.get_number() << "\n";
    c3 >>= 2;
    cout << "Number >>= 2: " << c3.get_number() << "\n";
    c4 = c3 >> 2;
    cout << "c4 = c3 >> 2; ";
    cout << "c3: " << c3.get_number() << ", c4: " << c4.get_number() << "\n";
    c4 = c3 << 2;
    cout << "c4 = c3 << 2; ";
    cout << "c3: " << c3.get_number() << ", c4: " << c4.get_number() << "\n";
    cout << "Binary number: " << c4.get_number() << " = " << c4 << "\n";

    cout << "Pair\n";
    c4 *= 100;
    cout << "new bin c4 " << c4 << "\n";
    tie(c5, c6) = binary_int::get_bites(c4);
    cout << "c4: " << c4.get_number() << " High bites: " << c5.get_number() <<  " Low bits: " << c6.get_number();


    return 0;
}

void add(int &a, int &b) { // a - this->number, b - число
    int sum_ones, shift;

    while (b != 0) {
        sum_ones = a ^ b;
        shift = (a & b) << 1;

        a = sum_ones;
        b = shift;
    }
}

void multiplication(int &a, int &b) {
    int res = 0, prom;

    while (b != 0) {

        if (b & 1) {
            prom = a;
            add(res, prom);
        }

        a <<= 1;
        b >>= 1;
    }

    a = res;
}
