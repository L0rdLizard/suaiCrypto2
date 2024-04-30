#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
#include <functional>
#include <random>
#include <vector>

using namespace boost::multiprecision;
using namespace std;

cpp_int modInverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m;
    cpp_int y = 0, x = 1;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {
        // q - частное
        cpp_int q = a / m;
        cpp_int t = m;

        // m - это остаток
        m = a % m, a = t;
        t = y;

        // обменять
        y = x - q * y;
        x = t;
    }

    // Обеспечьте, чтобы x был положительным
    if (x < 0) {
        x += m0;
    }

    return x;
}

class Participant {
    public:
    cpp_int n;
    cpp_int r;
    vector<cpp_int> s, v, b;
    int k;

    Participant(int k, cpp_int n) : k(k), n(n) {
        s.resize(k);
        v.resize(k);
        b.resize(k);
        cpp_int neg1(-1);
        cpp_int neg2(1);
        cpp_int s_test[] = {187, 37411, 5204};
        cpp_int b_test[] = {1, 1, 0};
        // cpp_int neg = negate<cpp_int>(1);

        // Генерация секретов
        for (int i = 0; i < k; i++) {
            cout << "n: " << n << endl;
            // s[i] = rand() % (n - 1) + 1;
            s[i] = s_test[i];
            cout << "s[i]: " << s[i] << endl;

            // b[i] = rand() % 2;
            b[i] = b_test[i];
            cout << "b[i]: " << b[i] << endl;

            // v[i] = powm(neg1, b[i], n) * powm(powm(s[i], 2, n), neg2, n);

            cpp_int result1 = powm(neg1, b[i], n);
            cout << "result1: " << result1 << endl;

            cpp_int result2 = powm(s[i], 2, n);
            cout << "result2: " << result2 << endl;

            cpp_int inv_result2 = modInverse(result2, n);
            cout << "inv_result2: " << inv_result2 << endl;

            if (b[i] == 1){
                inv_result2 = abs(n - inv_result2);
            }

            v[i] = inv_result2;
            cout << "v[i]: " << v[i] << endl;
            cout << endl;
        }
    }

    cpp_int generateX() {
        // cpp_int r = rand() % (n - 1) + 1;
        r = rand() % (n - 1) + 1;
        // return powm(r, 2, n);
        r = 1337;
        return 428083;
    }

    cpp_int generateY(vector<cpp_int> e) {
        cpp_int y = r;
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
        e[0] = 0;
        e[1] = 1;
        e[2] = 0;
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
    int k = 3;      // Количество раундов
    // cpp_int n = 21;
    cpp_int n = 553913;

    Participant participant(k, n);
    Verifier verifier(participant);

    for (int i = 0; i < k; i++) {
        cpp_int x = participant.generateX();
        cout << "x: " << x << endl;

        vector<cpp_int> e = verifier.generateE();
        cout << "e: ";
        for (int i = 0; i < k; i++){
            cout << e[i] << " ";
        }
        cout << endl;

        cpp_int y = participant.generateY(e);
        cout << "y: " << y << endl;

        if (!verifier.verify(x, y, e)) {
            cout << "Auth not complete" << endl;
            return 0;
        }
    }

    cout << "Auth complete" << endl;
    return 0;
}
