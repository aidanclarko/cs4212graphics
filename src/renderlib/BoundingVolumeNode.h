#pragma once
#include <memory>
#include "Ray.h"
#include "BoundingBox.h"
#include "Shape.h"

class BoundingVolumeNode : public Shape {
    public:
        //build BVH
        BoundingVolumeNode(std::vector<std::shared_ptr<Shape>> shapes, int AXIS) {
            unsigned int N = shapes.size();

            if(N == 1) {
                leftChild = shapes[0];
                rightChild = nullptr;
            } else if(N == 2) {
                leftChild = shapes[0];
                rightChild = shapes[1];
                mainBBox = mergeBoundingBoxes(shapes[0]->boundingBox(), shapes[1]->boundingBox());
            } else {
                BoundingBox tmpBox;

                for(unsigned int i = 0; i < shapes.size(); i++) {
                    tmpBox.update(shapes[i]->boundingBox().llb);
                    tmpBox.update(shapes[i]->boundingBox().urf);
                }

                float midPoint = (tmpBox.urf[AXIS] + tmpBox.llb[AXIS]) / 2.0f;

                std::vector<std::shared_ptr<Shape>> leftSide;
                std::vector<std::shared_ptr<Shape>> rightSide;

                for(unsigned int i = 0; i < shapes.size(); i++) {
                    if(shapes[i]->getCenter()[AXIS] <= midPoint) {
                        leftSide.push_back(shapes[i]);
                    } else {
                        rightSide.push_back(shapes[i]);
                    }
                }

                if(leftSide.size() == shapes.size() || rightSide.size() == shapes.size()) {
                    int mid = shapes.size() / 2.0f;
                    leftSide = std::vector<std::shared_ptr<Shape>>(shapes.begin(), shapes.begin() + mid);
                    rightSide = std::vector<std::shared_ptr<Shape>>(shapes.begin() + mid, shapes.end());
                }

                leftChild = std:: make_shared <BoundingVolumeNode>(
                    leftSide , (AXIS + 1) % 3, 2);
                rightChild = std:: make_shared <BoundingVolumeNode>(
                    rightSide , (AXIS + 1) % 3, 2);
            }

        }

        bool intersect(const Ray &r, float tmin , float &tmax , HitStruct &h);
    private:
        std::shared_ptr<Shape> leftChild;
        std::shared_ptr<Shape> rightChild;
        BoundingBox mainBBox;

        BoundingBox mergeBoundingBoxes(const BoundingBox &b0, const BoundingBox &b1);


};