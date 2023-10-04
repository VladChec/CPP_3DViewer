#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // Настройка контекста OpenGL
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);

    // Инициализация данных объекта
    objData.vertices = nullptr;
    objData.vertexCount = 0;
    objData.polygons = nullptr;
    objData.polygonCount = 0;
}

void OpenGLWidget::loadObjFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите obj файл", "", "OBJ Files (*.obj)");
    if (!filename.isEmpty()) {
        QByteArray ba = filename.toLocal8Bit();
        const char* file = ba.data();

        // Очистка предыдущих данных объекта
        freeObjData();

        // Загрузка нового файла obj
        parseObjFile(file);

        // Перерисовка окна
        update();
    }
}

void OpenGLWidget::initializeGL()
{
    // Инициализация OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    // Установка области отображения OpenGL
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL()
{
    // Очистка буфера цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Установка вида
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // Отрисовка полигонов из obj файла
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < objData.polygonCount; ++i) {
        Polygon polygon = objData.polygons[i];
        for (int j = 0; j < polygon.vertexCount; ++j) {
            Vertex vertex = polygon.vertices[j];
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();
}

void OpenGLWidget::parseObjFile(const char* filename)
{
    // Реализация вашего кода для парсинга obj файла
    // ...
    // Вместо текущей реализации parseObjFile()

    // Пример временной реализации parseObjFile() без фактического парсинга файла
    objData.vertices = new Vertex[3];
    objData.vertices[0] = { -1.0f, -1.0f, 0.0f };
    objData.vertices[1] = { 1.0f, -1.0f, 0.0f };
    objData.vertices[2] = { 0.0f, 1.0f, 0.0f };

    objData.vertexCount = 3;

    objData.polygons = new Polygon[1];
    objData.polygons[0].vertices = objData.vertices;
    objData.polygons[0].vertexCount = objData.vertexCount;

    objData.polygonCount = 1;
}

void OpenGLWidget::freeObjData()
{
    // Освобождение ресурсов, занятых objData
    delete[] objData.vertices;
    delete[] objData.polygons;

    objData.vertices = nullptr;
    objData.vertexCount = 0;
    objData.polygons = nullptr;
    objData.polygonCount = 0;
}
