#pragma once
#include "geom.h"
#include "light.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>



class Scene {
public:
    Point evil_eye;

    int x_screen_size, y_screen_size;
    Point ulcorner;

    Point x_step, y_step;

    vector <Figure*> figures;
    vector<LightPoint*> lights;
    vector <pair<Point, Color> > ans; // Набор точек, который будет нарисован в paintEvent
    mutex mtx;

    Scene(){}
    Scene(vector<Figure*> figures_, vector<LightPoint*> lights_,
          Point ulcorner_, Point urcorner_, Point dlcorner_,
          Point evil_eye_, size_t x_screen_size_, size_t y_screen_size_):
            figures(figures_), lights(lights_),
            x_screen_size(x_screen_size_), y_screen_size(y_screen_size_), evil_eye(evil_eye_), ulcorner(ulcorner_){
        /* Выравнивание пикс. размера  */
        x_step = (urcorner_ - ulcorner_) * (1 / (long double)x_screen_size_);
        y_step = (dlcorner_ - ulcorner_) * (1 / (long double)y_screen_size_);
        long double xl = x_step.len();
        long double yl = y_step.len();
        if(xl < yl){
            y_step.make_of_len(xl);
        }
        else{
            x_step.make_of_len(yl);
        }
    }

    pair<int, Point> nearest_object_index(size_t index, Point ray, Point origin){
        unsigned int nearest = figures.size();
        Point nearest_pnt(1000000, 1000000, 100000);
        for(unsigned int o = 0; o < figures.size(); o++){
            if(o != index){
                auto inter = figures[o]->ray_intersect(Ray(origin, ray));
                if(inter.first && ((nearest_pnt - origin).len() > (inter.second - origin).len()
                                   || nearest == figures.size())){
                    nearest = o;
                    nearest_pnt = inter.second;
                }
            }
        }
        return {nearest, nearest_pnt};
    }

    Color real_color(size_t index, Point point){
        Color res_color(0, 0, 0);
        auto obj = figures[index];
        for(unsigned int l = 0; l < lights.size(); l++){
            auto light = lights[l];
            auto ray = point - light->position;
            bool accessible = true;
            unsigned int nearest_to_candle_i = index;
            auto nearest_to_candle_pnt = point;
            for(unsigned int o = 0; o < figures.size(); o++){
                auto inter = figures[o]->ray_intersect(Ray(light->position, ray));
                if(inter.first &&
                        (nearest_to_candle_pnt - light->position).len() > (inter.second - light->position).len()){
                        nearest_to_candle_i = o;
                        nearest_to_candle_pnt = inter.second;

                    }
                if(nearest_to_candle_i != index)
                    accessible = false;

            }
            if(accessible){
                auto norm = obj->normal(point);
                res_color + Color(obj->color, (light->power / (ray.len() * ray.len())) * fabs(cosa(norm, ray)));
            }
        }
        return res_color;
    }

    void color_this_part(size_t begin_i, size_t end_i, size_t begin_j, size_t end_j){
        for(size_t i = begin_i; i < end_i; i++){
            for(size_t j = begin_j; j < end_j; j++){

                Point screen_point = ulcorner + x_step * (long double)i + y_step * (long double)j;
                auto dir = screen_point - evil_eye;
                dir.normalize();
                Ray ray(evil_eye, dir);
                Point nearest(100000.0, 100000.0, 100000.0);
                auto nearest_obj = figures[0];
                unsigned int nearest_ind;
                bool found = false;
                Color nearest_color(0, 0, 0);
                Color res_color(0, 0, 0);
                for(unsigned int k = 0; k < figures.size(); k++){
                    auto res = figures[k]->ray_intersect(ray);
                    if(res.first){
                        found = true;
                        auto inter_pnt = res.second;
                        if((nearest - evil_eye).len() > (inter_pnt - evil_eye).len()){
                            nearest_color = figures[k]->color;
                            nearest = inter_pnt;
                            nearest_obj = figures[k];
                            nearest_ind = k;
                        }
                    }
                }
                //ans.push_back({Point(i, j, 0.0), nearest_color});

                Color reflected_color(0, 0, 0);

                if(found){ // вычислим отражённый цвет

                    auto l = nearest - evil_eye;
                    auto norm = nearest_obj->normal(nearest);
                    auto reflected_ray = l - norm * 2 * (v_dot_product(l, norm) / v_dot_product(norm, norm));

                    auto reflected_point = nearest_object_index(nearest_ind, reflected_ray, nearest);

                    if(reflected_point.first != figures.size())
                        reflected_color = real_color(reflected_point.first, reflected_point.second);
                }


                if(found){ //подкорректируем цвет
                    res_color = Color(reflected_color, nearest_obj->alpha);
                    res_color + Color(real_color(nearest_ind, nearest), 1 - nearest_obj->alpha);
                }

                mtx.lock();
                ans.push_back({Point(i, j, 0.0), res_color});
                mtx.unlock();
            }
        }
    }

    vector<pair<Point, Color>> antialiasing(vector<pair<Point, Color>> points){
        vector<pair<Point, Color>> ans;
        vector<vector<Color>> screen(2 * x_screen_size, vector<Color>(2 * y_screen_size, Color(0, 0, 0)));
        for(size_t i = 0; i < points.size(); i++){
            screen[points[i].first.x][points[i].first.y] = points[i].second;
        }

        for(size_t i = 0; i < x_screen_size; i++){
            for(size_t j = 0; j < y_screen_size; j ++){
                auto new_r = screen[2 * i][2 * j].r + screen[2 * i + 1][2 * j].r + screen[2 * i][2 * j + 1].r + screen[2 * i + 1][2 * j + 1].r;
                auto new_g = screen[2 * i][2 * j].g + screen[2 * i + 1][2 * j].g + screen[2 * i][2 * j + 1].g + screen[2 * i + 1][2 * j + 1].g;
                auto new_b = screen[2 * i][2 * j].b + screen[2 * i + 1][2 * j].b + screen[2 * i][2 * j + 1].b + screen[2 * i + 1][2 * j + 1].b;
                ans.push_back({Point(i, j, 0), Color(new_r / 4, new_g / 4, new_b / 4)});
            }
        }
        return ans;
    }

    const int tdim1 = 1, tdim2 = 1;
    vector<pair<Point, Color>> process(){

        //ans.push_back({Point(x_screen_size, y_screen_size, 0), Color(0, 0, 0)});
        const int numthreads = tdim1 * tdim2;
        thread t[numthreads];

        auto epxx = x_screen_size * 2;
        auto epxy = y_screen_size * 2;
        x_step = x_step * 0.5;
        y_step = y_step * 0.5;
        size_t chunk_i = epxx / tdim1;
        size_t chunk_j = epxy / tdim2;

        for(size_t i = 0; i < tdim1; i++){
            for(size_t j = 0; j < tdim2; j++){
                t[i + tdim1 * j] = thread(&Scene::color_this_part, this, i * chunk_i, (i + 1) * chunk_i, j * chunk_j, (j + 1) * chunk_j);
            }
        }

        for (int tt = 0; tt < numthreads; ++tt)
            t[tt].join();

        //return ans;
        return antialiasing(ans);
    }
};
