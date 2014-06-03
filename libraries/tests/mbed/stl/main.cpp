/*
start
one
two
three
elements in the map:
a1 => 50
c3 => 150
{success}
{end}
 */
#include "test_env.h"

#include <string>
#include <queue>
#include <map>

using namespace std;

int main() {
    printf("start"NL);
    queue<string> queueObject;
    queueObject.push("one");
    queueObject.push("two");
    queueObject.push("three");

    while (!queueObject.empty()) {
        string& s = queueObject.front();
        printf("%s"NL, s.c_str());
        queueObject.pop();
    }

    map<string,string> m;
    map<string,string>::iterator it;

    m["a1"] = "50";
    m["b2"] = "100";
    m["c3"] = "150";
    m["d4"] = "200";

    it = m.find("b2");
    if (it == m.end()) {
        printf("lookup error"NL);
    }

    m.erase(it);
    m.erase(m.find("d4"));

    printf("elements in the map:"NL);
    printf("a1 => %s"NL, m.find("a1")->second.c_str());
    printf("c3 => %s"NL, m.find("c3")->second.c_str());

    notify_completion(true);
}
