#include "implementation.h"

implementation::implementation(CadGraphicsScene *scene)
    : newScene(scene) {
}


void implementation::addLine(const DRW_Line &data) {
    newScene->addLine(data.basePoint.x,data.basePoint.y,
                      data.secPoint.x, data.secPoint.y);
}

void implementation::addCircle(const DRW_Circle& data) {

}


CadGraphicsScene* implementation::scene() {
    return newScene;
}
