#include <cassert>
#include <cstdint>
#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef uint8_t u8;

const bool LINEAR_ADDITIVE_RULE = false;
const bool NONLINEAR_ADDITIVE_RULE = true;
// M = linearity
const bool linearity = NONLINEAR_ADDITIVE_RULE;

// Indices for impact coefficients that match the
// variable names in the paper
enum { P, Q, R, S, T };
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
	out << "{";
	size_t last = v.size() - 1;
	for(size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if (i != last) 
			out << ", ";
	}
	out << "}";
	return out;
}

/* Impact coefficients are used to denote connectivity in the
 * additive rule */
vector<bitset<5>> enumerate_impact_coefficients()
{
	vector<vector<bool>> bases = {
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 1 },
		{ 0, 0, 1, 1, 1 },
		{ 0, 1, 1, 1, 1 }};
	vector<bitset<5>> connectivities;
	for (auto& base : bases) {
		do {
			// Constraint from paper
			if (base[P] & base[T])
				continue;
			bitset<5> x;
			for (auto i = 0; i < base.size(); i++)
				x[i] = base[i];
			cout << x << endl;
			connectivities.push_back(x);
		} while (next_permutation(base.begin(), base.end()));
	}

	sort(connectivities.begin(), connectivities.end(), [](const auto & lhs, const auto & rhs)
			{ return lhs.to_ulong() < rhs.to_ulong(); });
	return connectivities;
}

int main()
{
	bitset<8> byte(232);
	size_t sz = byte.size();
	vector<bool> impact_coefficients = { 0, 0, 0, 0, 1 };

	vector<bitset<5>> connectivities = enumerate_impact_coefficients();	
	cout << "Class unspecific connectivities: " << connectivities.size() << endl;
	size_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	vector<bitset<5>> class4_rules;
	for (auto& connectivity : connectivities) {
		switch (connectivity.count()) {
			case 1: c1 += 1; break;
			case 2: c2 += 1; break;
			case 3: c3 += 1; break;
			case 4: c4 += 1; class4_rules.push_back(connectivity); break;
			default: break;
		}	
	}

	cout << "Class 1 rules: " << c1 << endl;
	cout << "Class 2 rules: " << c2 << endl;
	cout << "Class 3 rules: " << c3 << endl;
	cout << "Class 4 rules: " << c4 << endl;

	for (auto& rule : class4_rules) {

		for (auto i = 0; i < 25; i++) {
			// Additive rule CA	
			cout << "value: " << byte.to_ulong() << endl;
			for (auto i = 0; i < sz; i++) {
				// TODO: Connectivity, P, Q, R, S, T
				byte[i] = linearity 
					^ rule[P] & byte[(i - 2) % sz] 
					^ rule[Q] & byte[(i - 1) % sz] 
					^ rule[R] & byte[i]
					^ rule[S] & byte[(i + 1) % sz]
					^ rule[T] & byte[(i + 2) % sz];
			}
		}
	}
}
