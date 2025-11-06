#include "./tasks/CommonTask.cpp"

int main() {
    using namespace utils;

    Point start(2, 3);
    Point end(8, 12);

    CommonTask task(start, end, 0);
    task.ejecutar();

    std::cout << task << '\n';
}
