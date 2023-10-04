#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <QtOpenGLWidgets/QtOpenGLWidgets>

#include "dialog.h"


class OpenGLWidget : public QOpenGLWidget ,public QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = nullptr);
    void setModelData(ObjData* objData);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    ObjData objData;

    void parseObjFile(const char* filename);
    void freeObjData();
};
#endif // OPENGLWIDGET_H
