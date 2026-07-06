#define BOOST_TEST_MODULE tests
#include <boost/test/included/unit_test.hpp>

// BOOST_AUTO_TEST_CASE(placeholder) {
//     BOOST_TEST(true);
// }

#include "test_templates.h"

static const float t1 = 0.0f, t2 = 3.0f;

static bool check(const fly_obj& o1, const fly_obj& o2,
                     const Locator& l1, const Locator& l2) {
    auto noise1 = find_pos_normal_noise(o1, l1, t1);
    auto noise2 = find_pos_normal_noise(o2, l2, t2);

    float dt = (t2 - t1) / 2.0f;
    auto coord_t1 = predict_move_in(noise1, dt);
    auto coord_t2 = predict_move_back(noise2, dt);
    float distance = check_Evclid(coord_t1, coord_t2);
    float radius   = radius_attracktion(l1, l2);
    return check_rad_dist(distance, radius);
}

static const fly_obj obj_near1    (1,   10, 10, 10, 20, 20, 20);
static const fly_obj obj_near2    (2,   10, 12, 10, 19, 22, 20);

static const fly_obj obj_far1     (1,   30, 40, 10, 30, 20, 15);
static const fly_obj obj_far2     (2,   30, 40, 13, 30, 20, 15);
static const fly_obj _obj_far2     (2,   30, 60, 33, 30, 30, 15);

static const Locator loc_small_1(1,   3, 3, 3, 2, 1, 2);
static const Locator loc_small_2(2,   3, 3, 3, 2, 2, 2);

static const Locator loc_big_1  (1,   100, 10, 100, 11, 120, 10);
static const Locator loc_big_2  (2,   100, 11, 100, 10, 90, 11);

// 1. малое расстояние - минимальная погрешность
BOOST_AUTO_TEST_CASE(small_distance_small_Loc_error) {
    BOOST_TEST(check(obj_near1, obj_near2, loc_small_1, loc_small_2) == true);
} // одна цель

// 2. малое расстояние - большая погрешность
BOOST_AUTO_TEST_CASE(small_distance_big_Loc_error) {
    BOOST_TEST(check(obj_near1, obj_near2, loc_big_1, loc_big_2) == true);
} // одна

// 3. большое расстояние - минимальная погрешность
BOOST_AUTO_TEST_CASE(big_distance_small_Loc_error) {
    BOOST_TEST(check(obj_far1, _obj_far2, loc_small_1, loc_small_2) == false);
}    // разные цели

// 4. большое расстояние - большая погрешность
BOOST_AUTO_TEST_CASE(big_distance_big_Loc_error) {
    BOOST_TEST(check(obj_far1, _obj_far2, loc_big_1, loc_big_2) == true);
}  // одна цель
