#include <algorithm>
#include <cmath>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <iomanip>
#include <ostream>
#include <iostream>
#include <map>
#include <set>
#include <string.h>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace __gnu_pbds;

#ifdef pragma
#pragma GCC optimize("Ofast")
#pragma GCC optimize("no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC optimize("unroll-loops")
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

typedef long long ll;
typedef pair<ll, ll> pll;

#define ordered_set tree <pll, null_type, less<pll>, rb_tree_tag, tree_order_statistics_node_update>
#define f first
#define s second

ll const INF = 1e18;

void acceleration () {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
}

void formatting () {
	cout << fixed;
	cout.precision(10);
	cout << setprecision(10);
}

void use_file () {
    freopen("input.txt", "r", stdin);
    freopen("otput.txt", "w", stdout);
}

struct substituion {
    string first_word, second_word;
    bool is_final;
};

void markov_algorithm();
void test();

int main () {
	//acceleration();
	use_file();
	test();
	markov_algorithm();
}

vector <substituion> schema;
vector <string> words;

void parse(string input) {
    bool is_substitution = false;
    for (ll i = 0 ; i < input.size() ; ++i) {
        if (i + 1 < input.size() && input[i] == '-' && input[i + 1] == '>') {
            is_substitution = true;
            break;
        }
    }
    if (!is_substitution) {
        words.push_back(input);
        return;
    }
    string word_first = "", word_second = "";
    bool is_first = true, is_final = false;
    for (ll i = 0 ; i < input.size() ; ++i) {
        if (input[i] == '-' || input[i] == '>') {
            is_first = false;
        }
        if  (('a' <= input[i] && input[i] <= 'z') ||
             ('A' <= input[i] && input[i] <= 'Z') ||
             ('0' <= input[i] && input[i] <= '9')) {
            (is_first) ? word_first += input[i] : word_second += input[i];
        }
        if (input[i] == '.')
         is_final = true;
    }
    schema.push_back({word_first, word_second, is_final});
}

void read_and_parse_input () {
    string input;
    while (getline(cin, input)) {
        parse(input);
    }
}

void test_parse() {
    //test_1
    parse("hello");
    assert(words[0] == "hello");
    assert(schema.size() == 0);
    words.clear();
    schema.clear();
    //test_2
    parse ("ab  -> a");
    parse ("c -> aaa");
    parse("abc");
    assert(words.size() == 1);
    assert(schema.size() == 2);
    assert(schema[0].first_word == "ab" && schema[0].second_word == "a" && !schema[0].is_final);
    assert(schema[1].first_word == "c" && schema[1].second_word == "aaa" && !schema[1].is_final);
    assert(words[0] == "abc");
    words.clear();
    schema.clear();
    //test_3
    parse("->");
    parse(" -> .x");
    assert(words.size() == 0);
    assert(schema.size() == 2);
    assert(schema[0].first_word == "" && schema[0].second_word == "" && !schema[0].is_final);
    assert(schema[1].first_word == "" && schema[1].second_word == "x" && schema[1].is_final);
    words.clear();
    schema.clear();

}

ll is_applicable (string word, ll substituion_index) {
    if (schema[substituion_index].first_word == "")
        return 0;
    for (ll i = 0 ; i + (ll)schema[substituion_index].first_word.size() - 1 < word.size() ; ++i) {
        bool substituion_is_applicable = true;
        for (ll j = 0 ; j < schema[substituion_index].first_word.size() ; ++j) {
            if (schema[substituion_index].first_word[j] != word[i + j]) {
                substituion_is_applicable = false;
                break;
            }
        }
        if (substituion_is_applicable) return i;
    }
    return -1;
}

void test_is_applicable () {
    //test_1
    schema.push_back({"a", "b", 0});
    assert(is_applicable("aaa", 0) == 0);
    schema.clear();
    //test_2
    schema.push_back({"", "", 0});
    assert(is_applicable("aaa", 0) == 0);
    schema.clear();
    //test_3
    schema.push_back({"a", "b", 0});
    assert(is_applicable("bbb", 0) == -1);
    schema.clear();
    //test_4
    schema.push_back({"a", "b", 0});
    assert(is_applicable("baa", 0) == 1);
    schema.clear();
}

string apply_substitution(string word, ll substituion_index, ll index_entry) {
    string new_word = "";
    for (ll i = 0 ; i < index_entry ; ++i)
        new_word += word[i];
    new_word += schema[substituion_index].second_word;
    for (ll i = index_entry + max((ll)schema[substituion_index].first_word.size(), 0ll) ; i < word.size() ; ++i)
        new_word += word[i];
    return new_word;
}

void test_apply_substitution () {
    //test_1
    schema.push_back({"a", "b", 0});
    string test1_ans = apply_substitution("aaaa", 0, 0);
    assert(test1_ans == "baaa");
    schema.clear();
    //test_2
    schema.push_back({"xxx", "b", 0});
    string test2_ans = apply_substitution("xxx", 0, 0);
    assert(test2_ans == "b");
    schema.clear();
    //test_3
    schema.push_back({"", "aaa", 0});
    string test3_ans = apply_substitution("a", 0, 0);
    assert(test3_ans == "aaaa");
    schema.clear();
    //test_4
    schema.push_back({"aaa", "", 0});
    string test4_ans = apply_substitution("aaab", 0, 0);
    assert(test4_ans == "b");
    schema.clear();
}

string apply_schema (ll word_index) {
    bool schema_is_applicable = true;
    string word = words[word_index];
    while (schema_is_applicable) {
        schema_is_applicable = false;
        for (ll i = 0 ; i < schema.size() ; ++i) {
            ll substituion_is_applicable = is_applicable(word, i);
            if (substituion_is_applicable != -1){
                word = apply_substitution(word, i, substituion_is_applicable);
                if (!schema[i].is_final)
                    schema_is_applicable = true;
                break;
            }
        }
    }
    return word;
}

void test_apply_schema () {
    //test_1
    words.push_back("abc");
    schema.push_back({"a", "b", 0});
    schema.push_back({"b", "c", 0});
    schema.push_back({"c", "x", 1});
    assert("xcc" == apply_schema(0));
    words.clear();
    schema.clear();
    //test_2
    words.push_back("xxxx");
    schema.push_back({"x", "c", 1});
    schema.push_back({"xx", "", 0});
    schema.push_back({"xxx", "b", 1});
    assert("cxxx" == apply_schema(0));
    words.clear();
    schema.clear();
    //test_3
    words.push_back("cab");
    schema.push_back({"ba", "ab", 0});
    schema.push_back({"cb", "bc", 0});
    schema.push_back({"ca", "ac", 0});
    assert("abc" == apply_schema(0));
    words.clear();
    schema.clear();
}

void test() {
    test_parse();
    test_is_applicable();
    test_apply_substitution();
    test_apply_schema();
}

void markov_algorithm() {
    read_and_parse_input();
    for (ll i = 0 ; i < words.size() ; ++i) {
        cout << apply_schema(i) << endl;
    }
}
