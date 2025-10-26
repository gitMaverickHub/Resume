#include <matplot/matplot.h>
#include <vector>

int main()
{
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {1, 4, 9, 16, 25};
    matplot::plot(x, y);
    matplot::show();
    return 0;
}