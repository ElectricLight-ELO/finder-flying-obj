#include <QCoreApplication>
#include <iostream>
#include <random>
#include <vector>
#include <format>
#include <thread>
#include <tuple>

struct Locator {
    int id;

    float x, y, z; // фактическое местоположение
    //mistake calculating
    float sig_x, sig_y, sig_z;
};


struct fly_obj {
    int id;
    float x0, y0, z0; // стартовая позиция объекта

    float Vx, Vy, Vz; // статическая
};


int getRnd_digital(int start, int stop)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(start, stop);
    return dist(gen);
}

float getRnd_digital(float start, float stop)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(start, stop);
    return dist(gen);
}


int main(int argc, char *argv[])
{
    setlocale(0, "RU");
  ///  QCoreApplication a(argc, argv);

    // моменты времени
    const int t1 = 0.0f, t2 = 3.0f;


    std::vector<Locator> locators;
    std::vector<fly_obj> objects;


  // получение значений прозиции локатора
  auto get_loc_digits = [&](){
        float s_x, s_y, s_z;
        Locator lc;

        std::cout << "Enter for id= "<< locators.size()+1 << " x, y, z: " << std::endl;
        std::cin >> s_x >> s_y >> s_z;
        lc.id = locators.size() + 1;

        lc.sig_x = s_x;
        lc.sig_y = s_y;
        lc.sig_z = s_z;

        return lc;
    };


    // создание стартовой позиции объекта
    auto get_rnd_obj = [&](){
        fly_obj ob;
        ob.id = objects.size() + 1;
        ob.x0 = getRnd_digital(1, 10);
        ob.y0 = getRnd_digital(10, 30);
        ob.z0 = getRnd_digital(50, 100);

        ob.Vx = getRnd_digital(10, 50);
        ob.Vy = getRnd_digital(10, 50);
        ob.Vz = getRnd_digital(10, 50);
        return ob;
    };


    locators.push_back(get_loc_digits());
    locators.push_back(get_loc_digits());

    objects.push_back(get_rnd_obj());
    objects.push_back(get_rnd_obj());

    // x(t) = x0 +Vx*t

    auto find_pos = [](Locator& _loc, fly_obj& _obj, int t) { // под поиск реальных позиций в момент времени
        int x, y, z;
        x = _obj.x0 + _obj.Vx + t;
        y = _obj.y0 + _obj.Vy + t;
        z = _obj.z0 + _obj.Vz + t;

        return std::make_tuple<float>(x, y, z);
    };

    auto [x_t1, y_t1, z_t1] = find_pos(locators[0], objects[0], t1);
    auto [x_t2, y_t2, z_t2] = find_pos(locators[1], objects[1], t2);


    // нормальный шум ?? ...



    int dddebug = 0;

    return 0;
}
