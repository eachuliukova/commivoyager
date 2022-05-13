#include <iostream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <exception>


using namespace std;


class tsp_graph
{
private:
    vector <vector<double >> main;
    vector <vector<double >> main_tmp;
    vector <double> m_str;
    vector <double> m_col;
    int count_edge = 0;
    vector <bool> black_str;
    vector <bool> black_col;
    double sumcost = 0;
public:
    bool ending()
    {
        if (count_edge == main.size())
            return true;
        else
            return false;
    }

    double takecost()
    {
        return sumcost;
    }
class Coordinates
{
private:
    double x, y;
public:
    Coordinates(double a, double b) : x(a), y(b) {}
    friend double distantion(Coordinates a, Coordinates b);
}
;


    tsp_graph(vector <Coordinates>& coordinates)
    { 
        main.resize(coordinates.size(), vector<double>(coordinates.size(), -1));
        for (int i = 0; i < coordinates.size(); i++)
               for (int j = 0; j < coordinates.size(); j++)
                    if (i != j)
                    main[j][i] = distantion(coordinates[j], coordinates[i]);
        m_str.resize(coordinates.size(), -1);
        m_col.resize(coordinates.size(), -1);
        black_str.resize(coordinates.size(), false);
        black_col.resize(coordinates.size(), false);
        main_tmp = main;
    }

    void decrease_str()
    {
        double count;
        for (int i = 0; i < main.size(); i++)
        {
            if (black_str[i])
            continue;
            count = -1;
            for (int j = 0; j < main.size(); j++)
                if ((i != j) and (main[i][j] >= 0) and ((count == -1) or (main[i][j] < count)))
                count = main[i][j];
            m_str[i] = count;
            for (int k = 0; k < main.size(); k++)
                if ((i != k) and (main[i][k] > 0))
                main[i][k] = main[i][k] - count;
        }
    }

    void decrease_col()
    {
        double count;
        for (int i = 0; i < main.size(); i++)
        {
            if (black_col[i])
            continue;
            count = -1;
            for (int j = 0; j < main.size(); j++)
                if ((i != j)&&(main[j][i] >= 0)&&((count == -1)or(main[j][i] < count)))
                count = main[j][i];
            m_col[i] = count;
            for (int j = 0; j < main.size(); j++)
                if ((i != j) and (main[j][i] > 0))
                main[j][i] = main[j][i] - count;
        }
    }

    double take_m(int i, int j)
    {
        double result = 0;
        double mloc = -1;
        for (int q = 0; q < main.size(); q++)
            if (((mloc == -1)or(mloc > main[i][q]))&&(main[i][q] >= 0)&&(q != j))
            mloc = main[i][q];
        result += mloc;
        mloc = -1;
        for (int r = 0; r < main.size(); r++)
            if (((mloc == -1)or(mloc > main[r][j]))&&(main[r][j] >= 0)&&(r != i))
            mloc = main[i][r];
        result += mloc;
        return result;
    }

    void takecost_edge()
    {
        double counter = 0;
        double cost_edge = 0;
        pair <int, int> tmp;
        for (int i = 0; i < main.size(); i++)
            for (int j = 0; j < main.size(); j++)
                if (main[i][j] == 0)
                    if (take_m(i, j) > counter) {
                      tmp = make_pair(i, j);
                      cost_edge = main_tmp[i][j];
                    }
        count_edge++;
        main[tmp.first][tmp.second] = -1; main[tmp.second][tmp.first] = -1;
        sumcost = sumcost + cost_edge;
    }

    void decrease_matr()
    {
        int counter;
        for (int i = 0; i < main.size(); i++) {
            counter = 0;
            for (int j = 0; j < main.size(); j++) {
                if (main[i][j] == -1) {
                    counter++;
                }
                if (counter >= 2) {
                  black_str[i] = true;
                  for (int j = 0; j < main.size(); j++)
                   main[i][j] = -2;
                  break;
                }
            }
        }

        for (int i = 0; i < main.size(); i++) {
            counter = 0;
            for (int j = 0; j < main.size(); j++) {
                if (main[j][i] == -1) counter++;
                if (counter >= 2) {
                  black_col[i] = true;
                  for (int j = 0; j < main.size(); j++) {
                      main[j][i] = -2;
                  }
                  break;
                }
            }
        }

    }

};



vector <string> setfiles(string dir)
{
    vector <string> res;
    for (const auto& entry : filesystem::directory_iterator(dir))
        res.push_back(entry.path().string().substr(5));
    return res;
}

Coordinates part(string& data, string file_debug = "")
{
    auto pos = data.find(" ");
    int com;
    if (data.find("  ") != string::npos) {
        com = 2;
    }
    else com = 1;
    return Coordinates(stod(data.substr(0, pos)), stod(data.substr(pos + com)));
}


int main()
{
    vector <string> data = setfiles("data");
    string tmp;
    ofstream fout;
    fout.open("result.txt");
    string x;    
        for (int j = 0; j < data.size(); j++)
        {
            x = data[j];
            cout << "Starting " << x << endl << flush;
            vector <Coordinates> cords;
            ifstream file("data/" + x);
            getline(file, tmp);
            while (getline(file, tmp))
            {
                if (tmp == "") {
                    continue;
                }
                cords.push_back(part(tmp, x));
            }
            file.close();
            tsp_graph test(cords);
            int count = 0;
            while (!test.ending())
            {
                test.decrease_str();
                test.decrease_col();
                test.takecost_edge();
                test.decrease_matr();
                count++;
                cout << count << " edge ended." << flush << endl;
            }
            cout << x << " test finished " << endl;
            fout << x << ":" << test.takecost() << endl;
        }
    
 
    fout.close();
}
double distantion(Coordinates a, Coordinates b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}


