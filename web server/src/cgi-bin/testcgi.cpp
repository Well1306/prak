#include <iostream>

int main() {
    std::cout << "TestCGI program has turned on\n";
    std::cout << "Name: " << getenv("name") << std::endl;
    std::cout << "Surname: " << getenv("name") << std::endl;
    std::cout << "Mail: " << getenv("name") << std::endl;
    std::cout << "TestCGI program shut down\n";
    return 0;
}
