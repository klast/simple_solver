#include <cstdlib>
#include <vector>
#include <array>

using namespace std;


//линейна¤ интерпол¤ци¤
//коэффициенты a и b выведенты из уравнени¤
// y = a*x + b
//по двум точкам

class L_Interpol
{
public:
    int nodes;
    double lenght;

    vector<double> a;
    vector<double> b;

public:

    L_Interpol()
    {

    }
    void init(int n, double l, vector<double>& input_x, vector<double>& input_y)
    {
        nodes = n;
        lenght = l;

        for (int i = 0; i < nodes - 1; i++)
        {
            a[i] = (input_y[i + 1] - input_y[i]) / (input_x[i + 1] - input_x[i]);
            b[i] = input_y[i] - a[i] * input_x[i];
        }
    }
    L_Interpol(int n, double l, vector<double>& input_x, vector<double>& input_y)
    {
        init(n, l, input_x, input_y);
    }

    double y(double x)
    {
        int dx = int((x*nodes)/lenght);
        return a[dx]*x + b[dx];
    }
};

