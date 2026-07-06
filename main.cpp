#include <QCoreApplication>
#include <iostream>
#include <random>
#include <vector>
#include <tuple>
#include <cmath>

struct Locator {
    int id = 0;

    float sig_x, sig_y, sig_z;

    float sig_Vx, sig_Vy, sig_Vz;
    // int t;
    Locator(int _id, float s_x, float s_y, float s_z, float s_Vx, float s_Vy, float s_Vz){
        id = _id;
        sig_x = s_x;
        sig_y = s_y;
        sig_z = s_z;
        sig_Vx = s_Vx;
        sig_Vy = s_Vy;
        sig_Vz = s_Vz;
    }
    ~Locator() = default;
};


struct fly_obj {
    int id;
    float x0, y0, z0; // стартовая позиция объекта

    float Vx, Vy, Vz; // статическая
    fly_obj() = default;
    fly_obj(int _id, float _x0, float _y0, float _z0, float V_x, float V_y, float V_z){
        id = _id;
        x0 = _x0;
        y0 = _y0;
        z0 = _z0;
        Vx = V_x;
        Vy = V_y;
        Vz = V_z;
    }
    ~fly_obj() = default;
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


std::tuple<float, float, float> find_pos(const fly_obj& _obj, float t) {
    float x = _obj.x0 + _obj.Vx * t;
    float y = _obj.y0 + _obj.Vy * t;
    float z = _obj.z0 + _obj.Vz * t;
    return std::make_tuple(x, y, z);
}

fly_obj find_pos_normal_noise(const fly_obj& _obj, const Locator& loc, float t) {
    fly_obj obj;
    auto [x_real, y_real, z_real] = find_pos(_obj, t);
    // зашумление позиции
    obj.x0 = x_real + get_normal_distribution(0.0f, loc.sig_x);
    obj.y0 = y_real + get_normal_distribution(0.0f, loc.sig_y);
    obj.z0 = z_real + get_normal_distribution(0.0f, loc.sig_z);
    // зашумление скорости
    obj.Vx = _obj.Vx + get_normal_distribution(0.0f, loc.sig_Vx);
    obj.Vy = _obj.Vy + get_normal_distribution(0.0f, loc.sig_Vy);
    obj.Vz = _obj.Vz + get_normal_distribution(0.0f, loc.sig_Vz);
    obj.id = 0;
    return obj;
}

float check_Evclid(const std::tuple<float, float, float>& a,
                   const std::tuple<float, float, float>& b) {
    auto [x1, y1, z1] = a;
    auto [x2, y2, z2] = b;
    float dx = x1 - x2;
    float dy = y1 - y2;
    float dz = z1 - z2;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float radius_attracktion(const Locator& loc1, const Locator& loc2) {
    float sum_sigma =
        loc1.sig_x * loc1.sig_x + loc1.sig_y * loc1.sig_y + loc1.sig_z * loc1.sig_z +
        loc2.sig_x * loc2.sig_x + loc2.sig_y * loc2.sig_y + loc2.sig_z * loc2.sig_z;
    return 3.0f * std::sqrt(sum_sigma);
}

std::tuple<float, float, float> predict_move_in(const fly_obj& obj, float dt) {
    float x = obj.x0 + obj.Vx * dt;
    float y = obj.y0 + obj.Vy * dt;
    float z = obj.z0 + obj.Vz * dt;
    return std::make_tuple(x, y, z);
}

std::tuple<float, float, float> predict_move_back(const fly_obj& obj, float dt) {
    float x = obj.x0 - obj.Vx * dt;
    float y = obj.y0 - obj.Vy * dt;
    float z = obj.z0 - obj.Vz * dt;
    return std::make_tuple(x, y, z);
}


Locator get_sko_for_loc(int index) {
    float s_x, s_y, s_z, s_Vx, s_Vy, s_Vz;

    std::cout << "Enter for noise id= " << index << " sig_x, sig_y, sig_z: " << std::endl;
    std::cin >> s_x >> s_y >> s_z;
    std::cout << "Enter for noise id= " << index << " sig_Vx, sig_Vy, sig_Vz: " << std::endl;
    std::cin >> s_Vx >> s_Vy >> s_Vz;

    Locator lc(index, s_x, s_y, s_z, s_Vx, s_Vy, s_Vz);
    return lc;
}
bool check_rad_dist(float dist, float rad){
    if(std::fabs(dist) < rad){
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    setlocale(0, "RU");
    ///  QCoreApplication a(argc, argv);

    // моменты времени
    const float t1 = 0.0f, t2 = 3.0f;


    std::vector<Locator> locators;
    std::vector<fly_obj> objects;

    locators.push_back(get_sko_for_loc(1));
    locators.push_back(get_sko_for_loc(2));

  //  objects.push_back(get_rnd_obj());
  //  objects.push_back(get_rnd_obj());

                  // x, y, z,  vx, vy, vz
    fly_obj ob1(1,   10, 10, 10, 20, 20, 20);

    fly_obj ob2(2,   11, 12, 10, 19, 22, 20);

    objects.push_back(ob1);
    objects.push_back(ob2);

    // x(t) = x0 +Vx*t

    fly_obj marks_noise1 = find_pos_normal_noise(objects[0], locators[0], t1);
    fly_obj marks_noise2 = find_pos_normal_noise(objects[1], locators[1], t2);


    //1. ищещь dt между t1 и t2
    float dt = (t2 - t1)/2.0f;
    //2. прогнозируешь положение цели на t1+dt t2-dt
    //    x = x(t) + v_x * dt


    auto coord_t1 = predict_move_in(marks_noise1, dt);
    auto coord_t2 = predict_move_back(marks_noise2, dt);


    //3. расстояние между двумя спрогнозированными точками
    float distance = check_Evclid(coord_t1, coord_t2);
    //4. радиус притяжения coord_t1, coord_t2
    float raduis_attr = radius_attracktion(locators[0], locators[1]);
    //5. радиус притяжения и сравнение с расстоянием между 2 точками

    if(check_rad_dist(distance, raduis_attr)){
        std::cout << "Only one object" << std::endl;
    }
    else {
        std::cout << "Multiple objects" << std::endl;
    }
    // if(std::fabs(distance) < raduis_attr){
    //     std::cout << "Only one object" << std::endl;
    // }
    // else {
    //     std::cout << "Multiple objects" << std::endl;
    // }

    return 0;
}
