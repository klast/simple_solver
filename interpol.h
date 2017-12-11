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
    vector<double> input_x;
    vector<double> input_y;

public:

    L_Interpol()
    {

    }
    void init(int n, double l, vector<double>& input_x, vector<double>& input_y)
    {
        a.resize(n);
        b.resize(n);
        this->input_x.resize(n);
        this->input_y.resize(n);
        nodes = n;
        lenght = l;
        this->input_x = input_x;
        this->input_y = input_y;

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
        int i;
        for(i = 0; i < nodes - 1; i++)
        {
            if(x == input_x[i])
                return input_y[i];
        }
        for(i = 0; i < nodes - 1; i++)
        {
            if(x >= input_x[i] && x<= input_x[i + 1])
                return a[i] * x + b[i];
        }
        throw "OOOPS, INTERP error!";
    }
};

