#include "qte.h"
#include <QtWidgets/qapplication.h>

#include "beziercurve.h"
#include "beziersurface.h"

int main(int argc, char *argv[])
{
//    std::vector<Point> points_curve_1;
//    points_curve_1.push_back(Point(0, 0, 1));
//    points_curve_1.push_back(Point(0, 1, 2));
//    points_curve_1.push_back(Point(0, 2, 3));
//    points_curve_1.push_back(Point(0, 3, 3));
//    points_curve_1.push_back(Point(0, 4, 2));
//    points_curve_1.push_back(Point(0, 5, 1));

//    std::vector<Point> points_curve_2;
//    points_curve_2.push_back(Point(1, 0, 2));
//    points_curve_2.push_back(Point(1, 1, 3));
//    points_curve_2.push_back(Point(1, 2, 4));
//    points_curve_2.push_back(Point(1, 3, 4));
//    points_curve_2.push_back(Point(1, 4, 3));
//    points_curve_2.push_back(Point(1, 5, 2));

//    std::vector<Point> points_curve_3;
//    points_curve_3.push_back(Point(2, 0, 3));
//    points_curve_3.push_back(Point(2, 1, 4));
//    points_curve_3.push_back(Point(2, 2, 5));
//    points_curve_3.push_back(Point(2, 3, 5));
//    points_curve_3.push_back(Point(2, 4, 4));
//    points_curve_3.push_back(Point(2, 5, 3));

//    std::vector<Point> points_curve_4;
//    points_curve_4.push_back(Point(3, 0, 3));
//    points_curve_4.push_back(Point(3, 1, 4));
//    points_curve_4.push_back(Point(3, 2, 5));
//    points_curve_4.push_back(Point(3, 3, 5));
//    points_curve_4.push_back(Point(3, 4, 4));
//    points_curve_4.push_back(Point(3, 5, 3));

//    std::vector<Point> points_curve_5;
//    points_curve_5.push_back(Point(4, 0, 2));
//    points_curve_5.push_back(Point(4, 1, 3));
//    points_curve_5.push_back(Point(4, 2, 4));
//    points_curve_5.push_back(Point(4, 3, 4));
//    points_curve_5.push_back(Point(4, 4, 3));
//    points_curve_5.push_back(Point(4, 5, 2));

//    std::vector<Point> points_curve_6;
//    points_curve_6.push_back(Point(5, 0, 1));
//    points_curve_6.push_back(Point(5, 1, 2));
//    points_curve_6.push_back(Point(5, 2, 3));
//    points_curve_6.push_back(Point(5, 3, 3));
//    points_curve_6.push_back(Point(5, 4, 2));
//    points_curve_6.push_back(Point(5, 5, 1));

//    std::vector<BezierCurve> curves;
//    curves.push_back(BezierCurve(points_curve_1));
//    curves.push_back(BezierCurve(points_curve_2));
//    curves.push_back(BezierCurve(points_curve_3));
//    curves.push_back(BezierCurve(points_curve_4));
//    curves.push_back(BezierCurve(points_curve_5));
//    curves.push_back(BezierCurve(points_curve_6));

//    std::vector<Point> out_points;
//    //std::vector<Point> curve_1_discret = curves[0].discretize(0.01);
//    out_points.insert(out_points.end(), points_curve_1.begin(), points_curve_1.end());
//    out_points.insert(out_points.end(), points_curve_2.begin(), points_curve_2.end());
//    out_points.insert(out_points.end(), points_curve_3.begin(), points_curve_3.end());
//    out_points.insert(out_points.end(), points_curve_4.begin(), points_curve_4.end());
//    out_points.insert(out_points.end(), points_curve_5.begin(), points_curve_5.end());
//    out_points.insert(out_points.end(), points_curve_6.begin(), points_curve_6.end());

//    FILE *f = fopen("line.obj", "w");

//        for (int i = 0; i < out_points.size(); i++)
//            fprintf(f, "v %f %f %f\n", out_points[i].x, out_points[i].y, out_points[i].z);

//    fprintf(f, "\nl");
//    for (int i = 1; i <= out_points.size(); i++)
//        fprintf(f, " %d", i);

//    fclose(f);

//    return 0;

	QApplication app(argc, argv);

	MainWindow mainWin;
	mainWin.showMaximized();

	return app.exec();
}
