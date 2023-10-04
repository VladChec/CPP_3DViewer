#include "dialog.h"
#include "ui_dialog.h"
#include "gl_viewer.h"
//#include "../Controller/Controller.h"
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog),
      model_(),
      modelController(model_)
{
    ui->setupUi(this);
//    connect(ui->openGLWidget, SIGNAL(objDataChanged(const ObjData*)), this, SLOT(setObjData(const ObjData*)));
//    connect(ui->pB_openFile_16, &QPushButton::clicked, this, &Dialog::openFile);
    setWindowTitle("3Dviewer");
    // Создаем экземпляр ModelController и передаем ему объект Model

//    modelController = s21::modelController(model);
    connect(ui->widget, SIGNAL(on_dial_rotateX_sliderMoved(int)), ui->dial_rotateX, SLOT(setValue(int)));
    connect(ui->widget, SIGNAL(on_dial_rotateY_sliderMoved(int)), ui->dial_rotateY, SLOT(setValue(int)));
    connect(ui->widget, SIGNAL(on_dial_rotateZ_sliderMoved(int)), ui->dial_rotateZ, SLOT(setValue(int)));

    xRot = 0;
    yRot = 0;
    zRot = 0;
    dx= 0;
    dy= 0;
    dz= 0;
    timer_for_gif = new QTimer(0);
    connect(timer_for_gif, SIGNAL(timeout()), this, SLOT(create_screen()));
    ui->widget->dl_settings();
    if (ui->widget->parallel == false)
        ui->radioButton_central->setChecked(true);
    if (ui->widget->lineForm == 2)
        ui->checkBox_dashed->setChecked(true);
    ui->doubleSpinBox_thick->setValue(ui->widget->lineWidth);
    ui->spinBox_pointsSize->setValue(ui->widget->pointSize);
    switch (ui->widget->pointForm) {
        case 2:
            ui->comboBox_disMetod->setCurrentIndex(1);
            break;
        case 0:
            ui->comboBox_disMetod->setCurrentIndex(2);
            break;
    }
 }

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_button_openFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл", ".", "*.obj");

    if (!filename.isEmpty()) {
        std::string name = filename.toStdString();
        std::string base_filename = name.substr(name.find_last_of("/\\") + 1) ;
        QString res_name = QString::fromStdString(base_filename);
        ui->label_name->setText(res_name);

        QByteArray buff = filename.toLocal8Bit();
        char *char_name = buff.data();

        if (modelController.loadModelFromFile(char_name)) {
                    std::vector<double> &vertexData = modelController.getVertexData();
                    std::vector<unsigned int> &facetData = modelController.getFacetData();
                    QString temp = QString::number(vertexData.size() / 3);
                    ui->label_vertexes->setText(temp);

                    temp = QString::number(facetData.size());
                    ui->label_facets->setText(temp);
                    ui->widget->setVertexData(vertexData);
                    ui->widget->setFaceData(facetData);
//                    // Сообщаем контроллеру о перемещении и повороте (предположим, что у нас есть соответствующие значения dx, dy, dz)
//                            modelController.moveModel(dx,dy,dz);
////                            (dx, dy, dz)
//                            modelController.rotateModel(xRot, yRot, zRot);
                } else {
                    ui->label_facets->setText("invalid file");
                }
            } else {
                ui->label_facets->setText("file not found");
            }

            // Обнуляем значения движения и поворота
//            ui->widget->modelController.moveModel(0,0,0);
////            ui->widget->move.x_pos = 0;
////            ui->widget->move.y_pos = 0;
////            ui->widget->move.z_pos = 0;
//            ui->widget->modelController.rotateModel(0,0,0);

            // Обновляем значения виджетов управления
            ui->dial_rotateX->setValue(0);
            ui->dial_rotateY->setValue(0);
            ui->dial_rotateZ->setValue(0);
            ui->spinBox_rotateX->setValue(0);
            ui->spinBox_rotateY->setValue(0);
            ui->spinBox_rotateZ->setValue(0);
            ui->slider_translateX->setSliderPosition(0);
            ui->slider_translateY->setSliderPosition(0);
            ui->slider_translateZ->setSliderPosition(0);
            ui->spinBox_translateX->setValue(0);
            ui->spinBox_translateY->setValue(0);
            ui->spinBox_translateZ->setValue(0);
        }
//        QString temp;
//        int error = parser(char_name, &ui->widget->Mesh);
//        if (!error) {
//            temp = QString::number(ui->widget->Mesh.vertexCount/3-1);
//            ui->label_vertexes->setText(temp);
//            temp = QString::number(ui->widget->Mesh.facetCount);
//            ui->label_facets->setText(temp);
//        } else {
//            ui->label_facets->setText("invalid file");
//        }
//    } else {
//        ui->label_facets->setText("file not found");
//    }

//    ui->widget->move.dx = 0;
//    ui->widget->move.dy = 0;
//    ui->widget->move.dz = 0;

//    ui->widget->move.x_pos = 0;
//    ui->widget->move.y_pos = 0;
//    ui->widget->move.z_pos = 0;

//    ui->widget->rotate.dx = 0;
//    ui->widget->rotate.dy = 0;
//    ui->widget->rotate.dz = 0;

//    ui->dial_rotateX->setValue(0);
//    ui->dial_rotateY->setValue(0);
//    ui->dial_rotateZ->setValue(0);

//    ui->spinBox_rotateX->setValue(0);
//    ui->spinBox_rotateY->setValue(0);
//    ui->spinBox_rotateZ->setValue(0);

//    ui->slider_translateX->setSliderPosition(0);
//    ui->slider_translateY->setSliderPosition(0);
//    ui->slider_translateZ->setSliderPosition(0);

//    ui->spinBox_translateX->setValue(0);
//    ui->spinBox_translateY->setValue(0);
//    ui->spinBox_translateZ->setValue(0);
//}


void Dialog::wheelEvent(QWheelEvent *event){
   if (event->angleDelta().y() > 0) {
       ui->spinBox_scale->setValue(ui->widget->scale * 0.9);
   } else {
       ui->spinBox_scale->setValue(ui->widget->scale * 1.1);
   }
}

void Dialog::on_spinBox_rotateX_textChanged(const QString &arg1)
{
    int angle = arg1.toInt();
    modelController.rotateModel(angle, 0, 0);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_dial_rotateX_sliderMoved(int position)
{
    modelController.rotateModel(position, 0, 0);
    ui->spinBox_rotateX->setValue(position);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_spinBox_rotateY_textChanged(const QString &arg1)
{
    int angle = arg1.toInt();
    modelController.rotateModel(0, angle, 0);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_dial_rotateY_sliderMoved(int position)
{
    modelController.rotateModel(0, position, 0);
    ui->spinBox_rotateY->setValue(position);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_spinBox_rotateZ_textChanged(const QString &arg1)
{
    int angle = arg1.toInt();
    modelController.rotateModel(0, 0, angle);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

//void Dialog::on_dial_rotateZ_sliderMoved(int position)
//{
//    int dx = 0;
//    int dy = 0;
//    int dz = position - ui->dial_rotateZ->value();
//    ui->widget->modelController.rotateModel(dx, dy, dz);
//    ui->dial_rotateZ->setValue(position);
//    ui->spinBox_rotateZ->setValue(position);
//    ui->widget->update();
//}


void Dialog::on_dial_rotateZ_sliderMoved(int position)
{
   modelController.rotateModel(0, 0, position);
   ui->spinBox_rotateZ->setValue(position);
   ui->widget->setVertexData(modelController.getVertexData());
   ui->widget->update();
}
// void Dialog::on_slider_translateY_sliderMoved(int position)
// {
//    double xRot = position - ui->widget->dy;
//    modelController.moveModel(0, xRot, 0);
//    ui->widget->dy = position;
//    ui->spinBox_translateY->setValue(position);
//    ui->widget->update();
// }

void Dialog::on_spinBox_translateX_textChanged(const QString &arg1)
{
   int angle = arg1.toInt();
   modelController.moveModel(angle, 0, 0);
   ui->widget->setVertexData(modelController.getVertexData());
   ui->widget->update();
}

void Dialog::on_slider_translateX_sliderMoved(int position)
{
   modelController.moveModel(position, 0, 0);
   ui->spinBox_translateX->setValue(position);
   ui->widget->setVertexData(modelController.getVertexData());
   ui->widget->update();
}

void Dialog::on_spinBox_translateY_textChanged(const QString &arg1)
{
   int angle = arg1.toInt();
   modelController.moveModel(0, angle, 0);
   ui->widget->setVertexData(modelController.getVertexData());
   ui->widget->update();
}

void Dialog::on_slider_translateY_sliderMoved(int position)
{
   modelController.moveModel(0, position, 0);
   ui->spinBox_translateY->setValue(position);
   ui->widget->setVertexData(modelController.getVertexData());
   ui->widget->update();
}

void Dialog::on_spinBox_translateZ_textChanged(const QString &arg1)
{
    int angle = arg1.toInt();
    modelController.moveModel(0, 0, angle);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_slider_translateZ_sliderMoved(int position)
{
    modelController.moveModel(0, 0, position);
    ui->spinBox_translateZ->setValue(position);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}

void Dialog::on_spinBox_scale_textChanged(const QString &arg1)
{
    double scale = arg1.toDouble();
    modelController.scaleModel(scale);
    ui->widget->setVertexData(modelController.getVertexData());
    ui->widget->update();
}
//void Dialog::on_spinBox_rotateX_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
////    rotateAroundAnAxis(&ui->widget->Mesh, 'x', angle - ui->widget->rotate.dx);
//     modelController.rotateModel(angle - ui->widget->rotate.dx, 0, 0);
//    ui->widget->rotateModel.dx = angle;
//}
/*******************/
//void Dialog::on_dial_rotateX_sliderMoved(int position)
//{
//     int dx = position - ui->widget->rotateModel(dx,0,0)
////    int dx = position - ui->widget->rotate.dx;
//    modelController.rotateModel(dx, 0, 0);
//    ui->widget->rotate.dx = position;
//    ui->spinBox_rotateX->setValue(position);
//}

//void Dialog::on_spinBox_rotateY_textChanged(const QString &arg1)
//{
//    int angle = arg1.toInt();
//    int dy = angle - ui->widget->rotate.dy;
//    modelController.rotateModel(0, dy, 0);
//    ui->widget->rotate.dy = angle;
//}
//void Dialog::on_dial_rotateX_sliderMoved(int position)
//{
//    rotateAroundAnAxis(&ui->widget->Mesh, 'x', position - ui->widget->rotate.dx);
//    ui->widget->rotate.dx = position;
//    ui->spinBox_rotateX->setValue(position);
//}

//void Dialog::on_spinBox_rotateY_textChanged(const QString &arg1)
//{
//    // Получаем значение угла из аргумента arg1
//    int angle = arg1.toInt();

//    // Вызываем метод rotateModel у ModelController для вращения модели вокруг оси Y
//    double dx = 0.0;
//    double dy = angle - ui->widget->yRot;
//    double dz = 0.0;
//    modelController.rotateModel(dx, dy, dz);

//    // Обновляем значение yRot в GLviewer
//    ui->widget->yRot = angle;

//    // Обновляем виджет
//    ui->widget->update();
//}
//void Dialog::on_spinBox_rotateY_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
//    rotateAroundAnAxis(&ui->widget->Mesh, 'y', angle - ui->widget->rotate.dy);
//    ui->widget->rotate.dy = angle;
//}


//void Dialog::on_dial_rotateY_sliderMoved(int position)
//{
//    rotateAroundAnAxis(&ui->widget->Mesh, 'y', position - ui->widget->rotate.dy);
//    ui->widget->rotate.dy = position;
//    ui->spinBox_rotateY->setValue(position);
//}


//void Dialog::on_spinBox_rotateZ_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
//    rotateAroundAnAxis(&ui->widget->Mesh, 'z', angle - ui->widget->rotate.dz);
//    ui->widget->rotate.dz = angle;
//}


//void Dialog::on_dial_rotateZ_sliderMoved(int position)
//{
//    rotateAroundAnAxis(&ui->widget->Mesh, 'z', position - ui->widget->rotate.dz);
//    ui->widget->rotate.dz = position;
//    ui->spinBox_rotateZ->setValue(position);
//}


//void Dialog::on_spinBox_translateX_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
//    ui->widget->move.dx = angle - ui->widget->move.x_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dx = 0;
//    ui->widget->move.x_pos = angle;
//    ui->slider_translateX->setValue(angle);
//    ui->widget->update();
//}


//void Dialog::on_slider_translateX_sliderMoved(int position)
//{
//    ui->widget->move.dx = position - ui->widget->move.x_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dx = 0;
//    ui->widget->move.x_pos = position;
//    ui->spinBox_translateX->setValue(position);
//    ui->widget->update();
//}


//void Dialog::on_spinBox_translateY_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
//    ui->widget->move.dy = angle - ui->widget->move.y_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dy = 0;
//    ui->widget->move.y_pos = angle;
//    ui->slider_translateY->setValue(angle);
//    ui->widget->update();
//}


//void Dialog::on_slider_translateY_sliderMoved(int position)
//{
//    ui->widget->move.dy = position - ui->widget->move.y_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dy = 0;
//    ui->widget->move.y_pos = position;
//    ui->spinBox_translateY->setValue(position);
//       ui->widget->update();
//}


//void Dialog::on_spinBox_translateZ_textChanged(const QString &arg1)
//{
//    int angle = (&arg1)->toInt();
//    ui->widget->move.dz = angle - ui->widget->move.z_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dz = 0;
//    ui->widget->move.z_pos = angle;
//    ui->slider_translateZ->setValue(angle);
//       ui->widget->update();
//}


//void Dialog::on_slider_translateZ_sliderMoved(int position)
//{
//    ui->widget->move.dz = position - ui->widget->move.z_pos;
//    moveObj(&ui->widget->Mesh, ui->widget->move);
//    ui->widget->move.dz = 0;
//    ui->widget->move.z_pos = position;
//    ui->spinBox_translateZ->setValue(position);
//       ui->widget->update();
//}


//void Dialog::on_spinBox_scale_textChanged(const QString &arg1)
//{
//    double angle = (&arg1)->toDouble();
//    if (ui->widget->scale != 0 && ui->widget->ex_scale != 0 && angle != 0) {
//        ui->widget->ex_scale = ui->widget->scale;
//        ui->widget->scale = angle;
//        scaleObj(&ui->widget->Mesh, ui->widget->scale / ui->widget->ex_scale);
//    }
//}
/********************/
void Dialog::on_pushButton_vertColor_clicked()
{
    ui->widget->colorPoint = QColorDialog::getColor(Qt::gray);
}


void Dialog::on_pushButton_linesColor_clicked()
{
    ui->widget->colorLine = QColorDialog::getColor(Qt::gray);
}


void Dialog::on_pushButton_backColor_clicked()
{
    ui->widget->colorWidget = QColorDialog::getColor(Qt::gray);
}


void Dialog::on_doubleSpinBox_thick_valueChanged(double arg1)
{
    ui->widget->lineWidth = arg1;
}


void Dialog::on_spinBox_pointsSize_valueChanged(int arg1)
{
    ui->widget->pointSize = arg1;
}


void Dialog::on_comboBox_disMetod_currentIndexChanged(int index)
{
    if (index == 2) {
        ui->widget->pointForm = ui->widget->EMPTY;
    } else if (index == 0) {
        ui->widget->pointForm = ui->widget->POINT_ROUND;
    } else if (index == 1) {
        ui->widget->pointForm = ui->widget->POINT_QUADRO;
    }
}


void Dialog::on_checkBox_dashed_stateChanged(int arg1)
{
    if (arg1) {
        ui->checkBox_solid->setChecked(0);
        ui->widget->lineForm = ui->widget->LINE_DASHED;
    } else {
        ui->checkBox_solid->setChecked(1);
        ui->widget->lineForm = ui->widget->LINE_SOLID;
    }
}


void Dialog::on_checkBox_solid_stateChanged(int arg1)
{
    if (arg1) {
        ui->checkBox_dashed->setChecked(0);
        ui->widget->lineForm = ui->widget->LINE_SOLID;
    } else {
        ui->checkBox_dashed->setChecked(1);
        ui->widget->lineForm = ui->widget->LINE_DASHED;
    }
}


void Dialog::on_pushButton_screenShot_clicked()
{
    QFileDialog file_dialog_photo(this);
    QString f_name_photo =
    file_dialog_photo.getSaveFileName(this,"Save as...", QDir::currentPath(), "BMP (*.bmp);; JPEG (*.jpeg)");
    QFile file(f_name_photo);
    file.open(QIODevice::WriteOnly);
    QRect rect(0, 0, ui->widget->width(), ui->widget->height());
    QPixmap pixmap = ui->widget->grab(rect);
    pixmap.copy(rect);
    pixmap.toImage().save(&file, "jpg");
    QString q_command_line = "open " + f_name_photo;
    QByteArray temp = q_command_line.toLocal8Bit();
    char *command_line = temp.data();
    system(command_line);
}


void Dialog::on_radioButton_central_clicked(bool checked)
{
    if (checked) {
        ui->widget->parallel = ui->widget->EMPTY;
    }
}


void Dialog::on_radioButton_parallel_clicked(bool checked) {
    if (checked) {
        ui->widget->parallel = 1;
    }
}


void Dialog::on_pushButton_gif_clicked()
{
    timer_for_gif->start(100);
    create_screen();
}


void Dialog::save_gif() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save screenshot"), "", tr("GIF screenshot (*.gif);;GIF screenshot (*.gif)"));
    QGifImage gif(QSize(1522, 1642  ));

    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    for (QVector<QImage>::Iterator img = mas_image.begin(); img != mas_image.end(); ++img) {
        gif.addFrame(*img);

    }

    gif.save(fileName);
    mas_image.clear();
    ui->label_Timer_GIF->clear();
    time = 0.0;
}


void Dialog::create_screen()
{
    if (time <= 5.0) {
        mas_image.push_back(ui->widget->grab().toImage());
        time += 0.1;
        ui->label_Timer_GIF->setText(QString::number(time));
    } else {
        save_gif();
        timer_for_gif->stop();
    }
}


void Dialog::on_pushButton_screenShot_2_clicked()
{
    ui->widget->save_settings();
}


void Dialog::on_pushButton_screenShot_3_clicked()
{
    ui->widget->colorWidget.setNamedColor("black");
    ui->widget->colorLine.setNamedColor("blue");
    ui->widget->colorPoint.setNamedColor("red");
    ui->spinBox_pointsSize->setValue(1);
    ui->doubleSpinBox_thick->setValue(1);
    ui->comboBox_disMetod->setCurrentIndex(0);
    ui->checkBox_solid->setChecked(true);
    ui->radioButton_parallel->setChecked(true);
    ui->widget->parallel = 1;
}



//void Dialog::openFile()
//{
//    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "../", "*.obj");

//    if (filePath.isEmpty())
//        return;

//    ObjData objData;
//    parseObjFile(filePath.toUtf8().constData(), &objData);

//    if (objData.vertexCount == 0 || objData.polygonCount == 0) {
//        QMessageBox::warning(this, "Error", "Failed to parse the OBJ file.");
//        return;
//    }
//    QString fin_text = ui->textEdit_fileInfo->toPlainText();

//    // Вывод модели с использованием OpenGLWidget
//    // Предполагается, что у вас есть OpenGLWidget с именем openGLWidget
////    ui->widget->setModelData(&objData);

//    // Очистка памяти, выделенной для objData
//    freeObjData(&objData);
//}


//void Dialog::setModelData(ObjData* objData)
//{
//    // Вывод модели с использованием OpenGLWidget
//    // Предполагается, что у вас есть OpenGLWidget с именем openGLWidget
//    ui->widget->setModel(objData->vertices, objData->vertexCount,
//                               objData->polygons, objData->polygonCount);
//}

//void Dialog::freeObjData(ObjData* objData)
//{
//    if (objData->vertices != nullptr) {
//        free(objData->vertices);
//        objData->vertices = nullptr;
//        objData->vertexCount = 0;
//    }
//    if (objData->polygons != nullptr) {
//        for (int i = 0; i < objData->polygonCount; ++i) {
//            free(objData->polygons[i].vertices);
//        }
//        free(objData->polygons);
//        objData->polygons = nullptr;
//        objData->polygonCount = 0;
//    }
//}


//void Dialog::on_pB_openFile_16_clicked()
//{

////    QString fin_text = ui->textEdit_fileInfo->toPlainText();
////    ui->openGLWidget->filePath = filePath;
////    ui->openGLWidget->flg = 1;
////    ui->openGLWidget->parseObjFile(const char* filename, ObjData* objData);
////    ui->openGLWidget->filePath = filePath;

//        // Выбираем файл и записываем его адрес в переменную
//        QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Object files (*.obj)"));
//        QString fin_text = ui->textEdit_fileInfo->toPlainText();
////         ui->widget->filePath = filePath;
//        // Конвертируем путь к файлу в формат C-style строку
//        QByteArray byteArray = filePath.toLocal8Bit();
//        const char* filename = byteArray.data();

////        // Создаем объект ObjData
//        ObjData objData;

//        // Вызываем функцию для парсинга файла .obj
//        parseObjFile(filename, &objData);
//        ui->widget =

//    }
