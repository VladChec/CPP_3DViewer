#include "Controller.h"
namespace s21
{

  ModelController::ModelController(Model &model) : model(model) {}

  bool ModelController::loadModelFromFile(const std::string &filePath)
  {
    return model.parseObjFile(filePath);
  }

  void ModelController::moveModel(int dx, int dy, int dz)
  {
    model.move(dx, dy, dz);
  }

  void ModelController::rotateModel(int dx, int dy, int dz)
  {
    model.rotate(dx, dy, dz);
  }
  void ModelController::rotateAroundAxisModel(int a, int b, double angle)
  {
      model.rotateAroundAxis(a, b, angle);
  }
  void ModelController::scaleModel(double scale) { model.scale(scale); }

  std::vector<double> &ModelController::getVertexData()
  {
    return model.getVertexData();
  }

  std::vector<unsigned int> &ModelController::getFacetData()
  {
    return model.getFacetData();
  }

  void ModelController::setVertexData(const std::vector<double> &data)
  {
    model.setVertexData(data);
  }

  void ModelController::setFacetData(const std::vector<unsigned int> &data)
  {
    model.setFacetData(data);
  }

}
