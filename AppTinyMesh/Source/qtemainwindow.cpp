#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"

#define SDF_MESHING_SAMPLES 96

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;

    m_circle_radius_pointer = new int;
    m_erosion_rays_count_pointer = new int;
    *m_circle_radius_pointer = 50;
    *m_erosion_rays_count_pointer = 10;
    meshWidget->SetCircleRadiusPointer(m_circle_radius_pointer);
    meshWidget->SetErosionCountPointer(m_erosion_rays_count_pointer);

	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

	meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
    delete m_circle_radius_pointer;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->testSDFButton, SIGNAL(clicked()), this, SLOT(TestSDF()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
    connect(meshWidget, SIGNAL(_signalPaintErosion(const std::vector<Ray>&)), this, SLOT(paintErosion(const std::vector<Ray>&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::paintErosion(const std::vector<Ray>& rays)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const Ray& ray : rays)
    {
        Vector inter_point;
        if (m_intersect_with_ray_marching)
            inter_point = meshColor.intersectRayMarching(m_current_sdf, ray, 0.03f, -1);
        else
            inter_point = meshColor.intersect(ray);

        SDF* new_sphere = nullptr;
        if (inter_point != Vector::Null)
            new_sphere = SDF::create_sphere_sdf(inter_point, m_erosion_sphere_radius);

        if (new_sphere)
        {
            SDF* new_sdf = SDF::create_smooth_difference(m_current_sdf, new_sphere, 0.3f);
            m_current_sdf = new_sdf;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " milliseconds for mesh intersection with " << *m_erosion_rays_count_pointer << " rays using " << (m_intersect_with_ray_marching ? std::string("ray marching strategy") : std::string("standard ray tracing + BVH strategy")) << std::endl;


    //Recomputing the mesh
    std::function<float(const Vector& point)> function = std::bind(&SDF::Value, m_current_sdf, std::placeholders::_1);

    Mesh implicitMesh;
    implicitMesh.set_corresponding_sdf(m_current_sdf);

    start = std::chrono::high_resolution_clock::now();
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));
    stop = std::chrono::high_resolution_clock::now();

    std::cout << "Polygonize time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));

	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
    AnalyticScalarField implicit;

    Mesh implicitMesh;
    implicit.Polygonize(31, implicitMesh, Box(2.0));

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TestSDF()
{
    SDF* sphere1 = SDF::create_sphere_sdf(Vector(1, 0, 0), 2);
    SDF* sphere2 = SDF::create_sphere_sdf(Vector(-3, 0, 0), 2);
    SDF* sphere3 = SDF::create_sphere_sdf(Vector(-1, 3.6f, 0), 2);
    SDF* torus = SDF::create_torus_sdf(2.0f, 0.75f, RotationX(90) * Translation(-1, 1.75, -1.25));

    SDF* sdf_two_spheres = SDF::create_smooth_union(sphere1, sphere2, 1.5f);
    SDF* three_spheres = SDF::create_smooth_union(sdf_two_spheres, sphere3, 1.0f);

    delete m_current_sdf;
    m_current_sdf = SDF::create_difference(three_spheres, torus);

    std::function<float(const Vector& point)> function = std::bind(&SDF::Value, m_current_sdf, std::placeholders::_1);

    Mesh implicitMesh;
    implicitMesh.set_corresponding_sdf(m_current_sdf);
    AnalyticScalarField::Polygonize_from_function(function, SDF_MESHING_SAMPLES, implicitMesh, Box(6.0));

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
	meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}

void MainWindow::on_erosion_sphere_radius_input_textChanged(const QString &erosion_sphere_radius_input)
{
    bool ok;
    float circle_radius = erosion_sphere_radius_input.toFloat(&ok);

    if (ok)
        m_erosion_sphere_radius = circle_radius;
}

void MainWindow::on_erosion_sphere_spread_input_textChanged(const QString &sphere_spread_input)
{
    bool ok;
    float sphere_spread = sphere_spread_input.toInt(&ok);

    if (ok)
        *m_circle_radius_pointer = sphere_spread;
}

void MainWindow::on_erosion_count_input_textChanged(const QString &erosion_count_input)
{
    bool ok;
    float rays_count = erosion_count_input.toInt(&ok);

    if (ok)
        *m_erosion_rays_count_pointer = rays_count;
}

void MainWindow::on_use_ray_marching_checkbox_stateChanged(int arg1)
{
    m_intersect_with_ray_marching = (bool)arg1;
}

void MainWindow::on_save_obj_button_clicked()
{
    meshColor.SaveObj("saved_obj.obj", "implicit_obj");
}
