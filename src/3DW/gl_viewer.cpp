#include "dialog.h"
#include "gl_viewer.h"

GLviewer::GLviewer(QWidget *parent)
    : QOpenGLWidget(parent)
{
//    setGeometry(4000, 2000, 12000, 9000);
      setGeometry(10000, 10000, 12000, 10000);

    modelScale = 1;
    scale = 1;
    ex_scale = 1;

    colorWidget = QColor(Qt::black);
    colorLine = QColor(Qt::darkBlue);
    colorPoint = QColor(Qt::red);

    pointSize = 1;
    lineWidth = 1;
    pointForm = POINT_ROUND;
    lineForm = LINE_SOLID;
    parallel = 1;
}

GLviewer::~GLviewer()
{
}

void GLviewer::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

void GLviewer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void GLviewer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(colorWidget.redF(), colorWidget.greenF(), colorWidget.blueF(), 0);

    glMatrixMode(GL_PROJECTION);
    if (parallel == true)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1, 1, -1, 1, 2, 6);
        glTranslatef(0, 0, -3);
    }

    glTranslatef(0, 0, 0);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);

    drawShape();
    update();
}

void GLviewer::drawShape()
{
    // Projection//
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    projectionSelect();
    glScaled(scale, scale, scale);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vertecies_.data());

    // Points//
    if (pointForm != EMPTY)
    {
        pointSettingForm();
    }
    // Lines//
    if (lineForm != EMPTY)
    {
        lineSettingForm();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLviewer::pointSettingForm()
{
    if (pointForm == POINT_ROUND)
    {
        glEnable(GL_POINT_SMOOTH);
    }
    else if (pointForm == POINT_QUADRO)
    {
        glDisable(GL_POINT_SMOOTH);
    }
    glPointSize(pointSize);
    glColor3d(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());
    glDrawArrays(GL_POINTS, 0, vertecies_.size() / 3);
//    glDrawElements(GL_POINT, faces_.size() / 3 , GL_UNSIGNED_INT, faces_.data());
}

void GLviewer::lineSettingForm()
{
    if (lineForm == LINE_DASHED)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(4, 0x00FF);
    }
    glColor3d(colorLine.redF(), colorLine.greenF(), colorLine.blueF());
    glLineWidth(lineWidth);
    glDrawElements(GL_LINES, faces_.size(), GL_UNSIGNED_INT, faces_.data());
    glDisable(GL_LINE_STIPPLE);
}

void GLviewer::projectionSelect()
{
    if (parallel == EMPTY)
    {
        glEnable(GL_DEPTH_TEST);
        glOrtho(-100, 100, -800, 800, 100, 100);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
        glFrustum(-1000, 1000, -1000, 1000, 1000, 1000);
    }
}

void GLviewer::save_settings()
{
    std::ofstream out;
    out.open(QDir::homePath().toStdString() + "/settings.cfg");
    if (out.is_open())
    {
        out << colorLine.name().toStdString() << std::endl;
        out << colorPoint.name().toStdString() << std::endl;
        out << colorWidget.name().toStdString() << std::endl;

        out << pointForm << std::endl;
        out << pointSize << std::endl;
        out << lineForm << std::endl;
        out << lineWidth << std::endl;
        out << parallel << std::endl;
        out << lineForm << std::endl;
    }
    std::cout << "Settings saved" << std::endl;
    out.close();
}

void GLviewer::dl_settings()
{
    std::string line;
    std::ifstream in(
        QDir::homePath().toStdString() + "/settings.cfg");
    if (in.is_open())
    {
        getline(in, line);
        colorLine.setNamedColor(QString::fromStdString(line));
        getline(in, line);
        colorPoint.setNamedColor(QString::fromStdString(line));
        getline(in, line);
        colorWidget.setNamedColor(QString::fromStdString(line));

        getline(in, line);
        pointForm = std::stoi(line);
        getline(in, line);
        pointSize = std::stoi(line);
        getline(in, line);
        lineForm = std::stoi(line);
        getline(in, line);
        lineWidth = std::stoi(line);
        getline(in, line);
        parallel = std::stoi(line);
        getline(in, line);
        lineForm = std::stoi(line);
    }
    in.close();
    std::cout << "settings downloaded successfull" << std::endl;
}

void GLviewer::mousePressEvent(QMouseEvent *mo)
{
    mPos = mo->pos();
    update();
}

void GLviewer::mouseMoveEvent(QMouseEvent *mo)
{
    if (!(mo->pos().x() >= 625 && mo->pos().x() <= 901 && mo->pos().y() >= 630 && mo->pos().y() <= 851))
    {
        xRot += 1 / M_PI * (mo->pos().y() - mPos.y());
        yRot += 1 / M_PI * (mo->pos().x() - mPos.x());
    }
    mPos = mo->pos();
    update();
}

void GLviewer::change_zoom(double value)
{
    scale += value;
}
