#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
#include <random>
#include <vector>
#include <functional> 

using namespace boost::multiprecision;
using namespace std;

cpp_int modInverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m;
    cpp_int y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        cpp_int q = a / m;
        cpp_int t = m;

        m = a % m, a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}


class Participant {
    public:
    cpp_int n;
    vector<cpp_int> s, v, b;
    int k;

    Participant(int k, cpp_int n) : k(k), n(n) {
        s.resize(k);
        v.resize(k);
        b.resize(k);
        cpp_int neg(1);
        // cpp_int neg = negate<cpp_int>(1);

        // Генерация секретов
        for (int i = 0; i < k; i++) {
            s[i] = rand() % (n - 1) + 1;
            b[i] = rand() % 2;
            // v[i] = powm(neg, b[i], n) * powm(powm(s[i], 2, n), neg, n);
            if (b[i] == 0){
                v[i] = 1;
            } else{
                v[i] = powm(neg, b[i], n) * powm(powm(s[i], 2, n), neg, n);
            }
        }
    }

    cpp_int generateX() {
        cpp_int r = rand() % (n - 1) + 1;
        return powm(r, 2, n);
    }

    cpp_int generateY(vector<cpp_int> e) {
        cpp_int y = 1;
        for (int i = 0; i < k; i++) {
            y = (y * powm(s[i], e[i], n)) % n;
        }
        return y;
    }
};

class Verifier {
    public:
    cpp_int n;
    vector<cpp_int> v;
    int k;

    Verifier(Participant& participant) : k(participant.k), n(participant.n), v(participant.v) {}

    vector<cpp_int> generateE() {
        vector<cpp_int> e(k);
        for (int i = 0; i < k; i++) {
            e[i] = rand() % 2;
        }
        return e;
    }

    bool verify(cpp_int x, cpp_int y, vector<cpp_int> e) {
        cpp_int z = powm(y, 2, n);
        for (int i = 0; i < k; i++) {
            z = (z * powm(v[i], e[i], n)) % n;
        }
        return z == x || z == -x;
    }
};

int main() {
    int k = 10;             // Количество раундов
    cpp_int n = 21;  // Пример числа n

    Participant participant(k, n);
    Verifier verifier(participant);

    for (int i = 0; i < k; i++) {
        cpp_int x = participant.generateX();
        vector<cpp_int> e = verifier.generateE();
        cpp_int y = participant.generateY(e);

        if (!verifier.verify(x, y, e)) {
            cout << "Auth not complete" << endl;
            return 0;
        }
    }

    cout << "Auth complete" << endl;
    return 0;
}
