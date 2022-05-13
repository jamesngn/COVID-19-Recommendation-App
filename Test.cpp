/*
Psedocode:
nhập thông tin của bệnh nhân
--> lưu vào array và vector
thay đổi thông tin của bệnh nhân
-->thay đổi bằng array/vector




*/

#include <iostream>
using namespace std;

struct Person {
    string name;
    int age;
};

int main() {
    Person p;
    cout<<"Your name is: "<<endl;
    cin >> p.name;
    cout<<"Your age is: "<<endl;
    cin >> p.age;

    cout<<"----- OUTPUT: name: "<<p.name<<" , age: "<<p.age;
    cout<<"Change your name: "<<endl;
    cin >> p.name;

}