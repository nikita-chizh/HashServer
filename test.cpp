#include <algorithm>


int main()
{
    size_t size = 6;
    const char buf[] = "check\n";
    auto endPos = std::find(buf, buf + size, 'W');
    return 0;
}
