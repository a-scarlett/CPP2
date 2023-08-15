#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const unsigned int BASE = 100000;

const int LEN = []() {
    unsigned int number_of_digits = 0;
    int n = BASE;
    do {
        ++number_of_digits;
        n /= 10;
    } while (n != 0);
    return number_of_digits - 1;
}();

struct bigint {
private:
    std::vector<unsigned int> v;

public:
    bigint(unsigned int x = 0) {
        v.clear();
        for (unsigned int i = 1; i <= x; i = i * BASE) {
            v.push_back((static_cast<unsigned int>(x / i)) % BASE);
        }
    }

    explicit bigint(std::string &&x) {
        v.clear();
        x.erase(0, x.find_first_not_of('0'));
        int i = static_cast<int>(x.length());
        if (i == 0) {
            v.push_back(0);
            return;
        }
        i = i - LEN;
        for (; i >= 0; i = i - LEN) {
            v.push_back(std::stoi(x.substr(i, LEN)));
        }
        if (i != (-1) * LEN) {
            v.push_back(std::stoi(x.substr(0, LEN + i)));
        }
    }

    explicit bigint(const std::string &x) {
        v.clear();
        int j = static_cast<int>(x.find_first_not_of('0'));
        int i = static_cast<int>(x.length());
        if (i == j) {
            v.push_back(0);
            return;
        }
        i = i - LEN;
        for (; i >= 0; i = i - LEN) {
            v.push_back(std::stoi(x.substr(i, LEN)));
        }
        if (i != (-1) * LEN) {
            v.push_back(std::stoi(x.substr(0, LEN + i)));
        }
    }

    explicit operator unsigned int() const {
        unsigned int x = 0;
        unsigned int j = 1;
        for (auto t : v) {
            x += t * j;
            j *= BASE;
        }
        return x;
    }

    friend bool operator<=(const bigint &y, const bigint &x) {
        if (y.v.size() != x.v.size()) {
            return y.v.size() < x.v.size();
        } else {
            for (int i = static_cast<int>(y.v.size()) - 1; i >= 0; i--) {
                if (y.v[i] < x.v[i]) {
                    return true;
                }
                if (y.v[i] > x.v[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator<(const bigint &y, const bigint &x) {
        if (y.v.size() != x.v.size()) {
            return y.v.size() < x.v.size();
        } else {
            for (int i = static_cast<int>(y.v.size()) - 1; i >= 0; i--) {
                if (y.v[i] < x.v[i]) {
                    return true;
                }
                if (y.v[i] > x.v[i]) {
                    return false;
                }
            }
        }
        return false;
    }

    friend bool operator!=(const bigint &y, const bigint &x) {
        return x.v != y.v;
    }

    friend bool operator==(const bigint &y, const bigint &x) {
        return y.v == x.v;
    }

    friend bool operator>(const bigint &y, const bigint &x) {
        return !(y <= x);
    }

    friend bool operator>=(const bigint &y, const bigint &x) {
        return !(y < x);
    }

    friend bigint operator+(bigint l, const bigint &r) {
        int i = 0;
        for (; i < static_cast<int>(l.v.size()) &&
               i < static_cast<int>(r.v.size());
             i++) {
            if (l.v[i] + r.v[i] >= BASE) {
                if (i + 1 < static_cast<int>(l.v.size())) {
                    l.v[i + 1]++;
                } else {
                    l.v.push_back(1);
                }
            }
            l.v[i] = (l.v[i] + r.v[i]) % BASE;
        }
        if (l.v.size() < r.v.size()) {
            l.v.insert(
                l.v.end(), r.v.begin() + static_cast<int>(l.v.size()), r.v.end()
            );
        }
        while (i < static_cast<int>(l.v.size()) && l.v[i] >= BASE) {
            l.v[i] = l.v[i] % BASE;
            i++;
            if (i < static_cast<int>(l.v.size())) {
                l.v[i]++;
            } else {
                l.v.push_back(1);
            };
        }
        return l;
    }

    friend bigint operator-(bigint l, const bigint &r) {
        int i = 0;
        unsigned int borrow = 0;
        for (; i < static_cast<int>(l.v.size()) ||
               i < static_cast<int>(r.v.size());
             i++) {
            unsigned int ri = i < static_cast<int>(r.v.size()) ? r.v[i] : 0;
            if (l.v[i] < ri + borrow) {
                l.v[i] = l.v[i] + BASE - borrow - ri;
                borrow = 1;
            } else {
                l.v[i] = l.v[i] - ri - borrow;
                borrow = 0;
            }
        }
        while (l.v.size() > 1 && l.v.back() == 0) {
            l.v.pop_back();
        }
        return l;
    }

    bigint &operator+=(const bigint &r) {
        *this = *this + r;
        return *this;
    }

    bigint &operator-=(const bigint &r) {
        (*this) = (*this) - r;
        return *this;
    }

    [[nodiscard]] std::string to_string() const {
        std::string s;
        std::string ans = std::to_string(v.back());
        for (int i = static_cast<int>(v.size()) - 2; i >= 0; i--) {
            s = std::to_string(v[i]);
            while (static_cast<int>(s.size()) < LEN) {
                s.insert(0, "0");
            }
            ans += s;
        }
        return ans;
    }

    bigint &operator++() {
        (*this) = (*this) + 1;
        return (*this);
    }

    bigint operator++(int) {
        bigint old = *this;
        ++(*this);
        return old;
    }

    bigint &operator--() {
        (*this) = (*this) - 1;
        return *this;
    }

    bigint operator--(int) {
        bigint old = *this;
        --(*this);
        return old;
    }

    friend std::ostream &operator<<(std::ostream &os, const bigint &obj) {
        os << obj.to_string();
        return os;
    }

    friend std::istream &operator>>(std::istream &is, bigint &obj) {
        std::string s;
        is >> s;
        obj = bigint(s);
        return is;
    }
};