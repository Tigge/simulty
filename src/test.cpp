#include <vector>
#include <iostream>

using namespace std;

class buffer {

    private:
    vector<char> bf;

    public:

    buffer &operator<< (const char &c);
    buffer &operator>> (char &c);

    void print();
};

buffer &buffer::operator<< (const char &c)
{
    bf.push_back(c);
    return *this;
}

buffer &buffer::operator>> (char &c)
{
    c = bf[0]; bf.erase(bf.begin());
    return *this;

}


void buffer::print()
{
    for(int i = 0; i < bf.size(); i++)
        cout << bf[i];
    cout << endl;

}


int main()
{
    buffer b;
    b << 'a' << 'b' << 'c';

    b.print();

    char c;
    b >> c;

    cout << ":" << c << ":" << endl;

    b.print();

    return 0;

}
