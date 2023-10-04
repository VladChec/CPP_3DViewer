#ifndef GL_VIEWER_H
#define GL_VIEWER_H

#include "dialog.h"
#include <fstream>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class GLviewer;
}
QT_END_NAMESPACE

class GLviewer : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLviewer(QWidget *parent = nullptr);
    ~GLviewer();
//    double dx, dy, dz;
    float xRot, yRot, zRot;
    double scale;
    double ex_scale;
    double modelScale;

    QColor colorWidget;
    QColor colorLine;
    QColor colorPoint;

    int pointSize;
    int pointForm;
    int lineWidth;
    int lineForm;
    int parallel;

    enum
    {
        EMPTY,
        POINT_ROUND = 1,
        POINT_QUADRO,
        LINE_SOLID = 1,
        LINE_DASHED,
        IS_CHECKED = 1
    };

    void pointSettingForm();
    void lineSettingForm();
    void projectionSelect();

    void change_zoom(double value);
    void save_settings();
    void dl_settings();

    void setVertexData(std::vector<double> &vertecies)
    {
        vertecies_ = vertecies;
    }

    void setFaceData(std::vector<unsigned int> &faces)
    {
        faces_ = faces;
    }

private:
  Dialog* dialog;

    QPoint mPos;

    void drawShape();
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    std::vector<double> vertecies_;
    std::vector<unsigned int> faces_;
};

#endif // GL_VIEWER_H
