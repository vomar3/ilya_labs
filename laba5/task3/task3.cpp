#include <iostream>

using namespace std;

class logical_values_array {
private:
    unsigned int value;
public:
    logical_values_array() : value(0) {}
    logical_values_array(unsigned int num) : value(num) {}

    unsigned int get_value() const {
        return value;
    }

    static logical_values_array inversion(const logical_values_array &first) {
        return logical_values_array(~(first.value));
    }

    static logical_values_array disjunction(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(first.value | second.value);
    }

    static logical_values_array conjunction(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(inversion(disjunction(inversion(first.value), inversion(second.value))));
    }

    static logical_values_array implication(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(disjunction(inversion(first.value), second.value));
    }

    static logical_values_array coimplication(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(inversion(disjunction(inversion(first.value), second.value)));
    }

    static logical_values_array XOR(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array (disjunction(
                conjunction(first.value, inversion(second.value)),
                      conjunction(inversion(first.value), second.value)));
    }

    static logical_values_array equivalent(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(inversion(XOR(first.value, second.value)));
    }

    static logical_values_array Pier_arrow(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(inversion(disjunction(first.value, second.value)));
    }

    static logical_values_array Schaeffer(const logical_values_array &first, const logical_values_array &second) {
        return logical_values_array(inversion(conjunction(first.value, second.value)));
    }

    static bool equals(const logical_values_array &first, const logical_values_array &second) {
        return first.value == second.value;
    }

    static bool get_bit(const logical_values_array &first, unsigned int bit) {
        if (bit >= sizeof(unsigned int) * 8) {
            throw runtime_error("Bit more than size of unsigned int number\n");
        }

        return (first.value >> bit) & 1;
    }

    static void convertation(char *string, const logical_values_array &first) {
        if (string == nullptr) {
            throw logic_error("Null ptr\n");
        }

        int i;
        const short number_with_bits = sizeof(unsigned int) * 8;

        for (i = 0; i < number_with_bits; ++i) {
            if ((first.value & (1 << i)) == 0)
                string[number_with_bits - i - 1] = '0';
            else
                string[number_with_bits - i - 1] = '1';
        }

        string[number_with_bits] = '\0';
    }

};

int main() {

    bool answer;
    logical_values_array a(10);
    logical_values_array b(12);
    char for_convert[33];

    logical_values_array c = logical_values_array::inversion(a);
    cout << "Inversion: " << "~" << a.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::conjunction(a, b);
    cout << "Conjunction: " << a.get_value() << " & " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::disjunction(a, b);
    cout << "Disjunction: " << a.get_value() << " | " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::implication(a, b);
    cout << "Implication: " << a.get_value() << " -> " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::coimplication(a, b);
    cout << "Complication: " << a.get_value() << " <- " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::XOR(a, b);
    cout << "XOR: " << a.get_value() << " + " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::equivalent(a, b);
    cout << "Equivalent: " << a.get_value() << " ~ " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::Pier_arrow(a, b);
    cout << "Pier_arrow: " << a.get_value() << " | " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::Schaeffer(a, b);
    cout << "Schaeffer: " << a.get_value() << " || " << b.get_value() << " = " << c.get_value() << "\n";
    c = logical_values_array::equals(a, b);
    cout << "Check equals: " << a.get_value() << " == " << b.get_value() << " = " << c.get_value() << "\n";

    try {
        answer = logical_values_array::get_bit( b, 2);
        cout << "Check your bit: " << b.get_value() << " bit 2" << " = " << answer << "\n";

    } catch (runtime_error &problem){
        cout << "Error: " << problem.what();
    }

    try {
        logical_values_array::convertation(for_convert, a);
        cout << "Binary representation: " << a.get_value() << " = " << for_convert << "\n";
    } catch (logic_error &problem) {
        cout << "Error: " << problem.what();
    }

    return 0;
}