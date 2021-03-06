#include "Skeleton.h"

Skeleton::Skeleton(RenderableObject *object) : object(object) { }

Skeleton::~Skeleton() {}

void Skeleton::render(const Transform *worldTransform) {
    const Transform *localTransform = object->getWorldTransfrom();

    Transform newWorldTransform = *localTransform;
    newWorldTransform.transform(*worldTransform);

    object->render(&newWorldTransform);

    for (Skeleton *s : children) {
        s->render(&newWorldTransform);
    }
}

void Skeleton::addChild(Skeleton *child) {
    children.push_back(child);
}

void Skeleton::clean() {
    for (Skeleton *s : children) {
        s->clean();
    }
    delete object;
}