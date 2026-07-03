#include <QCoreApplication>
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <cmath>
struct Locator {
    int id;

    float sig_x, sig_y, sig_z;

    float sig_Vx, sig_Vy, sig_Vz;
    // int t;
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

float get_normal_distribution(float M, float sigma){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // std::uniform_normal_distribution<float> distrib(M, sigma);
    // sigma = sigma * sigma;
    std::normal_distribution<float> distrib(M, sigma);

    while (true) {
        float digital = distrib(gen);
        if (std::fabs(digital - M) <= 3.0f * sigma)
        {
            return digital;
        }

    }
}

int main(int argc, char *argv[])
{
    setlocale(0, "RU");
    ///  QCoreApplication a(argc, argv);

    // моменты времени
    const float t1 = 0.0f, t2 = 3.0f;


    std::vector<Locator> locators;
    std::vector<fly_obj> objects;


    auto get_sko_for_loc = [&](){
        float s_x, s_y, s_z;
        float s_Vx, s_Vy, s_Vz;
        Locator lc;

        std::cout << "Enter for noise id= "<< locators.size()+1 << " sig_x, sig_y, sig_z: " << std::endl;
        std::cin >> s_x >> s_y >> s_z;

        std::cout << "Enter for noise id= "<< locators.size() << " sig_Vx, sig_Vy, sig_Vz: " << std::endl;
        std::cin >> s_Vx >> s_Vy >> s_Vz;

        lc.id = locators.size() + 1;

        lc.sig_x = s_x;
        lc.sig_y = s_y;
        lc.sig_z = s_z;

        lc.sig_Vx = s_Vx;
        lc.sig_Vy = s_Vy;
        lc.sig_Vz = s_Vz;
        return lc;
    };


    // создание стартовой позиции объекта
    auto get_rnd_obj = [&](){
        fly_obj ob;
        ob.id = objects.size() + 1;
        ob.x0 = getRnd_digital(5, 30);
        ob.y0 = getRnd_digital(5, 30);
        ob.z0 = getRnd_digital(5, 30);

        ob.Vx = getRnd_digital(10, 50);
        ob.Vy = getRnd_digital(10, 50);
        ob.Vz = getRnd_digital(10, 50);
        return ob;
    };


    locators.push_back(get_sko_for_loc());
    locators.push_back(get_sko_for_loc());

    objects.push_back(get_rnd_obj());
    objects.push_back(get_rnd_obj());


    // fly_obj ob1;
    // ob1.id = 1;
    // ob1.x0 = 10; ob1.y0 = 10; ob1.z0 = 10;
    // ob1.Vx = 20; ob1.Vy = 20; ob1.Vz = 20;

    // fly_obj ob2;
    // ob2.id = 2;
    // ob2.x0 = 10; ob2.y0 = 12; ob2.z0 = 10;
    // ob2.Vx = 15; ob2.Vy = 20; ob2.Vz = 20;

    // objects.push_back(ob1);
    // objects.push_back(ob2);
    // результат- Only one object

    // x(t) = x0 +Vx*t

    auto find_pos = [](fly_obj& _obj, float t) { // под поиск реальных позиций в момент времени
        float x, y, z;
        x = _obj.x0 + _obj.Vx * t;
        y = _obj.y0 + _obj.Vy * t;
        z = _obj.z0 + _obj.Vz * t;

        return std::make_tuple(x, y, z);
    };

    auto find_pos_normal_noise = [&](fly_obj& _obj,Locator& loc, float t){
        fly_obj obj;
        auto[x_real, y_real, z_real] = find_pos(_obj, t); // найдена реальная позиция
        // зашумление местоположений
        obj.x0 = x_real + get_normal_distribution(0.0f, loc.sig_x);
        obj.y0 = y_real + get_normal_distribution(0.0f, loc.sig_y);
        obj.z0 = z_real+ get_normal_distribution(0.0f, loc.sig_z);

        // зашумление скоростей
        obj.Vx = _obj.Vx + get_normal_distribution(0.0f, loc.sig_Vx);
        obj.Vy = _obj.Vy +get_normal_distribution(0.0f, loc.sig_Vy);
        obj.Vz = _obj.Vz +get_normal_distribution(0.0f, loc.sig_Vz);
        obj.id = 0;
        return obj;
    };


    // поиск реальных позиций ... нормальный шум  ... нормальное распределение ... зашумление измерений
    fly_obj marks_noise1 = find_pos_normal_noise(objects[0], locators[0], t1);
    fly_obj marks_noise2 = find_pos_normal_noise(objects[1], locators[1], t2);

    // евклидова обработка
    auto check_Evclid = [](std::tuple<float, float, float>& a, std::tuple<float, float, float>& b){
        auto[x1, y1, z1] = a;
        auto[x2, y2, z2] = b;
        float dx = x1 - x2;
        float dy = y1 -y2;
        float dz = z1 - z2;
        float _d = std::sqrt(dx*dx + dy*dy + dz*dz);

        return _d;
    };
    // радиус притяжения
    auto radius_attracktion = [] (Locator& loc1, Locator& loc2) {

        float sum_sigma = loc1.sig_x * loc1.sig_x + loc1.sig_y * loc1.sig_y+ loc1.sig_z * loc1.sig_z +
                          loc2.sig_x * loc2.sig_x + loc2.sig_y * loc2.sig_y+ loc2.sig_z * loc2.sig_z;

        float result = 3 * std::sqrt(sum_sigma);
        return result;
    };

    //1. ищещь dt между t1 и t2
    float dt = (t2 - t1)/2.0f;
    //2. прогнозируешь положение цели на t1+dt t2-dt
    //    x = x(t) + v_x * dt

    auto predict_move_in = [=](fly_obj& obj){
        float x = obj.x0 + obj.Vx * dt;
        float y = obj.y0 + obj.Vy * dt;
        float z = obj.z0 + obj.Vz * dt;
        return std::make_tuple (x, y, z);
    };
    auto predict_move_back = [=](fly_obj& obj){
        float x = obj.x0 - obj.Vx * dt;
        float y = obj.y0 - obj.Vy * dt;
        float z = obj.z0 - obj.Vz * dt;
        return std::make_tuple (x, y, z);
    };

    auto coord_t1 = predict_move_in(marks_noise1);
    auto coord_t2 = predict_move_back(marks_noise2);

    //3. расстояние между двумя спрогнозированными точками
    float distance = check_Evclid(coord_t1, coord_t2);
    //4. радиус притяжения coord_t1, coord_t2
    float raduis_attr = radius_attracktion(locators[0], locators[1]);
    //5. радиус притяжения и сравнение с расстоянием между 2 точками
    if(distance < raduis_attr){
        std::cout << "Only one object" << std::endl;
    }
    else {
        std::cout << "Multiple objects" << std::endl;
    }

    int dddebug = 0;

    return 0;
}
