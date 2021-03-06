#include <QtWidgets>
#include <memory>

#include "scene.h"
#include "geom.h"
#include "my_project.h"

  SceneView::SceneView(QWidget *parent)
      : QWidget(parent)
  {
      setWindowTitle(tr("RayTracing"));
      resize(800, 600);
  }

  void SceneView::paintEvent(QPaintEvent *)
  {
      setAttribute(Qt::WA_OpaquePaintEvent);
      QPainter painter(this);

      bool from_file = false;

      vector<pair<Point, Color>> points;

      if(!from_file){

          vector<LightPoint*> light_sources;
          vector<Figure*> figs;


          Point ul(0, 0, 60.0);
          Point ur(80, 0, 60.0);
          Point dl(0, 0, 0);
          Point eye(40, -100, 30);



          auto s1 = new Sphere(Point(50, 50, 10), 10, Color(255, 216, 0), 0.0);
          auto s2 = new Sphere(Point(30, 30, 10), 10, Color(216, 100, 20), 0.0);
          auto floor = new Quadrilateral(Point(0, 0, 0), Point(80, 0, 0), Point(80, 60, 0), Point(0, 60, 0), Color(0, 255, 0), 0.75);
          //auto forward = new Quadrilateral(Point(0, 60, 60), Point(80, 60, 60), Point(80, 60, 0), Point(0, 60, 0), Color(255, 0, 0), 0.0);
          //auto left = new Quadrilateral(Point(0, 0, 0), Point(0, 60, 0), Point(0, 60, 60), Point(0, 0, 60), Color(0, 255, 0));
          //auto right = new Quadrilateral(Point(80, 0, 0), Point(80, 60, 0), Point(80, 60, 60), Point(80, 0, 60), Color(0, 255, 0));
          //auto roof = new Quadrilateral(Point(0, 0, 60), Point(80, 0, 60), Point(80, 60, 60), Point(0, 60, 60), Color(0, 255, 0));


          //auto candle = new LightPoint(Point(20, 20, 55), 2000); //20 20 50
          auto candle2 = new LightPoint(Point(30, 30, 30), 70000); //20 20 50
          auto candle3 = new LightPoint(Point(70, 50, 50), 10000); //20 20 50

          /*auto s1 = new Sphere(Point(20, 20, 20), 10, Color(255, 216, 0), 0);
          auto s2 = new Sphere(Point(40, 40, 40), 10, Color(216, 100, 20), 0);
          auto s3 = new Sphere(Point(50, 50, 50), 10, Color(255, 100, 20), 0);
          auto s5 = new Sphere(Point(10, 10, 10), 10, Color(216, 10, 20), 0);
          auto s4 = new Sphere(Point(30, 30, 30), 10, Color(216, 100, 2), 0.95);*/
          auto candle = new LightPoint(Point(20, 20, 55), 3000); //20 20 50*/
          //auto lamp = new Sphere(Point(20, 20, 65), 1, Color(200, 200, 200), 0);
          //auto candle2 = new LightPoint(Point(25, -20, 20), 1500); //20 20 50
          //auto quad = new Quadrilateral(Point(0, 0, 30), Point(40, 0, 30), Point(40, 40, 50), Point(0, 40, 50), Color(50, 170, 10));

          figs.push_back(floor);
          //figs.push_back(lamp);
          figs.push_back(s1);
          figs.push_back(s2);
          //figs.push_back(s3);
          //figs.push_back(s4);
          //figs.push_back(candle_sp);
          //figs.push_back(s5);


          //figs.push_back(tri1);
         light_sources.push_back(candle);
          //light_sources.push_back(candle2);
          //light_sources.push_back(candle3);
         Scene scene(figs, light_sources, ul, ur, dl, eye, 800, 600);
         points = scene.process();

      }
      //painter.drawPoint(10 * (points.size()), 10);
      for(size_t i = 0; i < points.size(); i++){
          QPen linepen(QColor(points[i].second.r, points[i].second.g, points[i].second.b, 255));
          linepen.setCapStyle(Qt::RoundCap);
          linepen.setWidth(1);
          painter.setRenderHint(QPainter::Antialiasing,true);
          painter.setPen(linepen);
        painter.drawPoint(points[i].first.x, points[i].first.y);
      }
  }
