#ifndef __Qte__
#define __Qte__

#include <QtWidgets/qmainwindow.h>
#include "SDF/SDF.h"
#include "realtime.h"
#include "meshcolor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Assets; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::Assets* uiw;           //!< Interface

    bool m_intersect_with_ray_marching = true;
    int* m_circle_radius_pointer;
    float m_erosion_sphere_radius = 0.05;

    MeshWidget* meshWidget;   //!< Viewer
    MeshColor meshColor;		//!< Mesh.
    SDF* m_current_sdf = nullptr;

public:
    MainWindow();
    ~MainWindow();
    void CreateActions();
    void UpdateGeometry();

public slots:
    void editingSceneLeft(const Ray&);
    void editingSceneRight(const Ray&);
    void paintErosion(const std::vector<Ray>& ray);
    void BoxMeshExample();
    void SphereImplicitExample();
    void TestSDF();
    void ResetCamera();
    void UpdateMaterial();
private slots:
    void on_erosion_sphere_radius_input_textChanged(const QString &arg1);
    void on_erosion_sphere_spread_input_textChanged(const QString &arg1);
    void on_use_ray_marching_checkbox_stateChanged(int arg1);
};

#endif
