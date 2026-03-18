#pragma once
#include <memory>
#include "Ray.h"
#include "BoundingBox.h"
#include "Shape.h"

class BoundingVolumeNode : public Shape {
    public:
        //build BVH
        BoundingVolumeNode(std::vector<std::shared_ptr<Shape>>& shapes, int AXIS) {
            unsigned int N = shapes.size();
            if(N == 1) {
                leftChild = shapes[0];
                rightChild = nullptr;
                mainBBox = shapes[0]->boundingBox();
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

                leftChild = std:: make_shared<BoundingVolumeNode>(
                    leftSide, (AXIS + 1) % 3);
                rightChild = std:: make_shared<BoundingVolumeNode>(
                    rightSide, (AXIS + 1) % 3);

                mainBBox = mergeBoundingBoxes(leftChild->boundingBox(), rightChild->boundingBox());
            }

        }

        bool intersect(const Ray &r, float tmin , float &tmax , HitStruct &h) override {
            //  std::cout << "hit\n";
            if (mainBBox.hit(r, tmin, tmax)) {
                HitStruct lhit, rhit;
                lhit.t = std::numeric_limits<float>::infinity();
                rhit.t = std::numeric_limits<float>::infinity();
                lhit.scene = h.scene;  // add these
                rhit.scene = h.scene;
                bool lefthit = false;
                bool righthit = false;
                
                if(leftChild) lefthit = leftChild->intersect(r, tmin, tmax, lhit);
                if(rightChild) righthit = rightChild->intersect(r, tmin, tmax, rhit);

                if(lefthit && righthit) {
                    // std::cout << "hit\n";
                    if(lhit.t < rhit.t) {
                        h = lhit;
                    } else {
                        h = rhit;
                    }
                    return true;
                } else if (lefthit) {
                    h = lhit;
                    return true;
                } else if (righthit) {
                    h = rhit;
                    return true;
                }
                return false;
            }
            return false;
        }

        const BoundingBox boundingBox() const override {
            return mainBBox;
        }

        const point3 getCenter() const override {
            return (mainBBox.llb + mainBBox.urf) / 2.0f;
        }

    private:
        std::shared_ptr<Shape> leftChild;
        std::shared_ptr<Shape> rightChild;
        BoundingBox mainBBox;

        BoundingBox mergeBoundingBoxes(const BoundingBox &b0, const BoundingBox &b1) {
            vec3  llb, urf;

            llb = vec3(
                std::min(b0.llb.x(), b1.llb.x()),
                std::min(b0.llb.y(), b1.llb.y()),
                std::min(b0.llb.z(), b1.llb.z())
            );

            urf = vec3(
                std::max(b0.urf.x(), b1.urf.x()),
                std::max(b0.urf.y(), b1.urf.y()),
                std::max(b0.urf.z(), b1.urf.z())
            );

            return BoundingBox(llb, urf);
        }
};