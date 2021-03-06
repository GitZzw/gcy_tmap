//
// Created by stumbo on 2019/11/15.
//

#ifndef TMAPPING_INTERSECTION_H
#define TMAPPING_INTERSECTION_H

#include "ExpData.h"

namespace tmap
{

class Intersection : public ExpData
{

public:
    ExpDataType type() const override { return ExpDataType::Intersection; }

    Json::Value toJS() const override;

    ExpDataPtr clone() const override;

    ExpDataPtr cloneShell() const override;
};
}


#endif //TMAPPING_INTERSECTION_H
