//
// Created by stumbo on 2020/1/9.
//

#ifndef TMAPPING_INCLUDE_TMAPPING_UI_QROBOT_H
#define TMAPPING_INCLUDE_TMAPPING_UI_QROBOT_H

#include <QGraphicsEllipseItem>
#include "QNode.h"

#include <tmapping/Exp.h>

namespace tmap
{

class QRobot : public QGraphicsEllipseItem
{
    QNodePtr currentAtNode;

    SubNode atLM;

    ExpPtr currentExp;

    bool directMove;

public:
    explicit QRobot(tmap::QNodePtr at, bool directMove);

    void updatePos();

    ~QRobot() override;

    /**
     * @brief 根据输入的Pos让机器人进行移动
     * @param scenePos
     * @return 是否点到了Gate
     */
    bool try2move(QPointF scenePos);

    ExpPtr moveThroughGate(GateID gateId);

    /**
     * @brief 让机器人直接通过当前的gate进入下一个Exp
     * @return 如果成功, 返回上一个Exp的信息, 失败的话返回nullptr
     */
    ExpPtr try2ThroughGate();

    const QNodePtr& atNode() const;

    GateID enterGate() const;

};

using QRobotPtr = std::unique_ptr<QRobot>;
}


#endif //TMAPPING_INCLUDE_TMAPPING_UI_QROBOT_H
