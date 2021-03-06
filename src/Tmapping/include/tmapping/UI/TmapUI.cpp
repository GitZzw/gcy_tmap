//
// Created by stumbo on 2019/12/20.
//

#include <iostream>

#include "TmapUI.h"
#include "ui_tmapUI.h"
#include "MainGView.h"
#include "ViceGView.h"

#include "tmapping/MergedExp.h"

#include "ui_dockBuildExp.h"
#include "ui_dockBuildMap.h"
#include "ui_dockSimulation.h"
#include "ui_dockRealtime.h"

#include <tmapping/NewExp.h>
#include <tmapping/GateMovement.h>
#include <tmapping/GetMaps.h>
#include <tmapping/Exp.h>
#include <tmapping/SetSurviverMapsNum.h>
#include <std_srvs/Empty.h>

#include <QKeyEvent>
#include <QImage>

#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

using namespace std;

Q_DECLARE_METATYPE(tmap::ExpDataType);
Q_DECLARE_METATYPE(tmap::ExpDataPtr);
Q_DECLARE_METATYPE(tmap::GateType);
Q_DECLARE_METATYPE(tmap::ExpPtr);

tmap::TmapUI::TmapUI(QWidget* parent) :
        QMainWindow(parent),
        uiMain(new Ui::TmapWindow),
        uiDockExpBuilder(new Ui::BuildExpDockUI),
        dockExpBuilder(new QDockWidget(this)),
        uiDockMapBuilder(new Ui::BuildMapDockUI),
        dockMapBuilder(new QDockWidget(this)),
        uiDockSimulation(new Ui::SimulationDockUI),
        dockSimulation(new QDockWidget(this)),
        uiDockRealtime(new Ui::RealTimeDockUI),
        dockRealtime(new QDockWidget(this))
{
    {   /// 添加UI组件
        uiMain->setupUi(this);
        setWindowTitle("Tmapping Viewer");

        centerLayout = new QHBoxLayout(uiMain->centralWidget);
        centerLayout->setSpacing(3);
        centerLayout->setContentsMargins(5, 5, 5, 5);
        centerLayout->setObjectName(QString::fromUtf8("centerLayout"));

        gvMain = new MainGView(uiMain->centralWidget);
        QSizePolicy p;
        p.setHorizontalPolicy(QSizePolicy::Expanding);
        p.setVerticalPolicy(QSizePolicy::Expanding);
        gvMain->setSizePolicy(p);
        gvMain->setMinimumSize(601, 401);
        centerLayout->addWidget(gvMain);
//
//        connect(gvMain, SIGNAL(SIG_RobotThroughGate(ExpPtr)),
//                this, SLOT(SLOT_ROS_ThroughGate(ExpPtr)));

        connect(gvMain, SIGNAL(SIG_RobotThroughGate(ExpPtr)),
                this, SLOT(SLOT_ROS_ThroughGate(ExpPtr)));


        smallWindowLayout = new QVBoxLayout();
        smallWindowLayout->setSpacing(3);
        smallWindowLayout->setContentsMargins(5, 0, 5, 0);

        infoView = new QTextBrowser(uiMain->centralWidget);
        infoView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        infoView->setBaseSize(221, 221);
        infoView->setText("Welcome!");
        smallWindowLayout->addWidget(infoView, 0);

        gvVice = new ViceGView(uiMain->centralWidget);
        gvVice->setFixedSize(221, 221);
        gvVice->setCursor(Qt::CrossCursor);
        gvVice->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
        smallWindowLayout->addWidget(gvVice, 0, Qt::AlignBottom);
        connect(gvMain, SIGNAL(SIG_ShowStrInInfoView(QString))
                , infoView, SLOT(setText(QString)));

        centerLayout->addLayout(smallWindowLayout);
    }

    {   /// 初始化模式切换按钮
        modeGroup = new QActionGroup(uiMain->mainToolBar);

        mode_BUILD = modeGroup->addAction("build map mode");
        mode_SIMULATION = modeGroup->addAction("simulation mode");
        mode_REALTIME = modeGroup->addAction("realtime mode");
        connect(modeGroup, SIGNAL(triggered(QAction*))
                , this, SLOT(SLOT_SwitchMode(QAction*)));

        mode_BUILD->setCheckable(true);
        mode_SIMULATION->setCheckable(true);
        mode_REALTIME->setCheckable(true);

        mode_BUILD->setChecked(true);

        uiMain->mainToolBar->addAction(mode_BUILD);
        uiMain->mainToolBar->addAction(mode_SIMULATION);
        uiMain->mainToolBar->addAction(mode_REALTIME);
        uiMain->mainToolBar->addSeparator();

        auto * dragMode = new QAction("enable Drag", uiMain->mainToolBar);
        dragMode->setCheckable(true);
        uiMain->mainToolBar->addAction(dragMode);
        uiMain->mainToolBar->addSeparator();
        connect(dragMode, SIGNAL(toggled(bool)), this, SLOT(SLOT_DragMode(bool)));

        qactConnectToROS = new QAction("Connect to ROS", uiMain->mainToolBar);
        uiMain->mainToolBar->addAction(qactConnectToROS);
        connect(qactConnectToROS, SIGNAL(triggered())
                , this, SLOT(SLOT_InitROS()));
    }

    {   /// Exp Builder Deck的初始化
        uiDockExpBuilder->setupUi(dockExpBuilder);
        addDockWidget(Qt::RightDockWidgetArea, dockExpBuilder);
        dockExpBuilder->setShown(true);

        uiDockExpBuilder->cbExpType->setItemData(0,
                QVariant::fromValue(ExpDataType::Intersection));
        uiDockExpBuilder->cbExpType->setItemData(1,
                QVariant::fromValue(ExpDataType::Room));

        QRegExp regx("[0-9]+.[0-9]+");
        uiDockExpBuilder->leExpSize->setValidator(new QRegExpValidator(regx, this));

        uiDockExpBuilder->cbGateType->setItemData(0,
                QVariant::fromValue(GateType::GateWay));
        uiDockExpBuilder->cbGateType->setItemData(1,
                QVariant::fromValue(GateType::DoorOpened));
        uiDockExpBuilder->cbGateType->setItemData(2,
                QVariant::fromValue(GateType::DoorClosed));

        connect(uiDockExpBuilder->btnBuildExp, SIGNAL(toggled(bool)),
                this, SLOT(SLOT_BuildExp(bool)));

        connect(uiDockExpBuilder->btnEditJson, SIGNAL(toggled(bool)),
                this, SLOT(SLOT_EditJsonOfBuiltExpData(bool)));

        connect(uiDockExpBuilder->cbGateType, SIGNAL(currentIndexChanged(int)),
                this, SLOT(SLOT_GateTypeChanged(int)));

        connect(uiDockExpBuilder->cbBuiltExps, SIGNAL(currentIndexChanged(int)),
                this, SLOT(SLOT_ExpDataSelected(int)));
        connect(uiDockExpBuilder->cbBuiltExps, SIGNAL(highlighted(int)),
                this, SLOT(SLOT_ExpDataSelected(int)));

        connect(uiDockExpBuilder->btnLoad, SIGNAL(clicked()),
                this, SLOT(SLOT_LoadExp()));

        connect(uiDockExpBuilder->btnSave, SIGNAL(clicked()),
                this, SLOT(SLOT_SaveExp()));

        connect(uiDockExpBuilder->btnAdd2Map, SIGNAL(clicked()),
                this, SLOT(SLOT_AddFakeNode()));

        connect(uiDockExpBuilder->leLandmark, SIGNAL(textChanged(QString)),
                gvVice, SLOT(SLOT_NextLandmarkStr(QString)));
    }

    {   /// Map Builder Dock 的初始化
        uiDockMapBuilder->setupUi(dockMapBuilder);
        addDockWidget(Qt::LeftDockWidgetArea, dockMapBuilder);
        dockExpBuilder->setShown(true);

        connect(uiDockMapBuilder->cbEnableNodesMoving, SIGNAL(toggled(bool)),
                gvMain, SLOT(SLOT_EnableMoving4FakeNodes(bool)));

        connect(uiDockMapBuilder->cbRestrictGrid, SIGNAL(toggled(bool)),
                gvMain, SLOT(SLOT_EnableGridRestriction(bool)));

        connect(uiDockMapBuilder->btnDeleteSelectedNodes, SIGNAL(clicked()),
                gvMain, SLOT(SLOT_RemoveSelectedNodes()));

        connect(uiDockMapBuilder->btnConnectGates, SIGNAL(toggled(bool)),
                this, SLOT(SLOT_DrawEdge(bool)));
        connect(uiDockMapBuilder->btnConnectGates, SIGNAL(toggled(bool)),
                uiDockMapBuilder->btnAddGate2Corridor, SLOT(setDisabled(bool)));
        connect(uiDockMapBuilder->btnConnectGates, SIGNAL(toggled(bool)),
                uiDockMapBuilder->btnEditJson, SLOT(setDisabled(bool)));

        connect(uiDockMapBuilder->btnSave, SIGNAL(clicked()),
                this, SLOT(SLOT_SaveMap()));

        connect(uiDockMapBuilder->btnLoad, SIGNAL(clicked()),
                this, SLOT(SLOT_LoadMap()));

        connect(uiDockMapBuilder->btnEditJson, SIGNAL(toggled(bool)),
                this, SLOT(SLOT_EditJsonOfNodeInFakeMap(bool)));

        connect(uiDockMapBuilder->btnAddGate2Corridor, SIGNAL(toggled(bool)),
                this, SLOT(SLOT_AddGate2Corridor(bool)));
        connect(uiDockMapBuilder->btnAddGate2Corridor, SIGNAL(toggled(bool)),
                uiDockMapBuilder->btnConnectGates, SLOT(setDisabled(bool)));
        connect(uiDockMapBuilder->btnAddGate2Corridor, SIGNAL(toggled(bool)),
                uiDockMapBuilder->btnEditJson, SLOT(setDisabled(bool)));

        connect(uiDockMapBuilder->comboMoveStrategy, SIGNAL(activated(int)),
                gvMain, SLOT(SLOT_SetMoveStrategy(int)));
    }

    {   /// Simulation Dock 的初始化
        uiDockSimulation->setupUi(dockSimulation);
        addDockWidget(Qt::LeftDockWidgetArea, dockSimulation);
        dockSimulation->setShown(false);

        QRegExp regx("[0-9]+.[0-9]+");
        uiDockSimulation->leDirError->setValidator(new QRegExpValidator(regx, this));
        uiDockSimulation->lePosError->setValidator(new QRegExpValidator(regx, this));

        connect(uiDockSimulation->btnPlaceRobot, SIGNAL(clicked()),
                this, SLOT(SLOT_PlaceRobot()));

        connect(uiDockSimulation->btnPlaceRobot, SIGNAL(toggled(bool)),
                uiDockSimulation->cbNoDetailMoving, SLOT(setDisabled(bool)));

        connect(uiDockSimulation->cbNoDetailMoving, SIGNAL(toggled(bool)),
                uiDockSimulation->cbAccidents, SLOT(setEnabled(bool)));

        connect(uiDockSimulation->cbNoDetailMoving, SIGNAL(toggled(bool)),
                uiDockSimulation->btnRandomMove, SLOT(setEnabled(bool)));
        connect(uiDockSimulation->btnRandomMove, SIGNAL(clicked()),
                this, SLOT(SLOT_RandomMove()));

        connect(uiDockSimulation->btnStartMassiveTrail, SIGNAL(clicked()),
                this, SLOT(SLOT_StartMassiveTrials()));

        connect(uiDockSimulation->btnPlaceRobot, SIGNAL(toggled(bool)),
                uiDockSimulation->btnStartMassiveTrail, SLOT(setEnabled(bool)));
        connect(uiDockSimulation->btnPlaceRobot, SIGNAL(toggled(bool)),
                uiDockSimulation->btnRandomMove, SLOT(setEnabled(bool)));

    }

    {   /// Realtime Dock 的初始化
        uiDockRealtime->setupUi(dockRealtime);
        addDockWidget(Qt::LeftDockWidgetArea, dockRealtime);
        dockRealtime->setShown(false);

        connect(uiDockRealtime->btnGetRealTimeMap, SIGNAL(clicked()),
                this, SLOT(SLOT_GetRealtimeMaps()));

        connect(uiDockRealtime->cbCandidates, SIGNAL(highlighted(int)),
                this, SLOT(SLOT_DisplayTheRealMap(int)));

        connect(uiDockRealtime->cbEnableNodesMoving, SIGNAL(toggled(bool)),
                gvMain, SLOT(SLOT_EnableMoving4RealNodes(bool)));

        connect(uiDockRealtime->btnShowPossHistory, SIGNAL(clicked()),
                this, SLOT(SLOT_ShowPossHistroy()));

        connect(uiDockRealtime->sbSurvivorMaps, SIGNAL(valueChanged(int)),
                this, SLOT(SLOT_SetMapSurvivers(int)));
    }
}

tmap::TmapUI::~TmapUI()
{

}

void tmap::TmapUI::addBuiltExpData(const ExpDataPtr& expData)
{
    if (!expData) {
        cout << FILE_AND_LINE << " an invaild ExpData! (null)" << endl;
        return;
    }

    auto cbBuiltExps = uiDockExpBuilder->cbBuiltExps;
    cbBuiltExps->addItem(getExpDataLabel(expData), QVariant::fromValue(expData));
    cbBuiltExps->setCurrentIndex(cbBuiltExps->count() - 1);
}

void tmap::TmapUI::SLOT_BuildExp(bool start)
{
    if (start) {
        uiDockExpBuilder->btnBuildExp->setText("Complete Building");
        auto& cbET = uiDockExpBuilder->cbExpType;
        auto type = cbET->itemData(cbET->currentIndex()).value<ExpDataType>();
        double size = uiDockExpBuilder->leExpSize->text().toDouble();
        gvVice->beginExpBuilding(type, size);
        cout << "The Exp size is " << size << "m (the 5x5)\n";
        cout << "Left click and Drag at the circle to make a gate\n";
        cout << "Right click to make a landmark" << endl;
    } else {
        uiDockExpBuilder->btnBuildExp->setText("Build an Exp");
        auto theBuiltExp = gvVice->completeExpBuilding();
        if (theBuiltExp) {
            addBuiltExpData(theBuiltExp);
            infoView->setText("The built Exp's Json:\n");
            infoView->append(JsonHelper::JS2Str(theBuiltExp->toJS(), false).data());
        }
    }
}

void tmap::TmapUI::SLOT_GateTypeChanged(int index)
{
    cout << "next gate's type will be "
    << uiDockExpBuilder->cbGateType->itemText(index).data() << endl;
    gvVice->setNextGateType(
            uiDockExpBuilder->cbGateType->itemData(index).value<GateType>());
}

void tmap::TmapUI::SLOT_ExpDataSelected(int index)
{
    gvVice->displayTheExpData(
            uiDockExpBuilder->cbBuiltExps->itemData(index).value<ExpDataPtr>());
}

void tmap::TmapUI::SLOT_EditJsonOfBuiltExpData(bool start)
{
    static int indexOfEditing;
    auto& exps = uiDockExpBuilder->cbBuiltExps;
    if (start) {
        cout << "Editting the Exp's Json" << endl;
        if (exps->count() == 0) {
            uiDockExpBuilder->btnEditJson->setCheckable(false);
            uiDockExpBuilder->btnEditJson->setCheckable(true);
            return;
        }
        uiDockExpBuilder->btnEditJson->setText("complete Editting");
        indexOfEditing = exps->currentIndex();
        Jsobj jExpData = exps->itemData(indexOfEditing).value<ExpDataPtr>()->toJS();
        infoView->setText(JsonHelper::JS2Str(jExpData, false).data());
        infoView->setTextColor(Qt::darkGray);
        infoView->setReadOnly(false);
    } else {
        uiDockExpBuilder->btnEditJson->setText( "Edit Json");

        Jsobj editedJs = JsonHelper::Str2JS(infoView->toPlainText().toStdString());
        ExpDataPtr madeExp = ExpData::madeFromJS(editedJs);
        if (madeExp) {
            exps->setItemData(indexOfEditing, QVariant::fromValue<ExpDataPtr>(madeExp));
            exps->setItemText(indexOfEditing, getExpDataLabel(madeExp));
            gvVice->displayTheExpData(madeExp);
        } else {
            infoView->append("\n Trans error!");
        }
        infoView->setTextColor(Qt::black);
        infoView->setReadOnly(true);
        cout << "Edit the Exp's Json complete" << endl;
    }
}

void tmap::TmapUI::SLOT_EditJsonOfNodeInFakeMap(bool start)
{
    const auto& items = gvMain->scene()->selectedItems();
    if (start) {
        cout << "Start editting the json" <<
        "\n WARNNING, the connection relationship will not be auto fixed" << endl;
        if (items.empty()) {
            uiDockMapBuilder->btnEditJson->setCheckable(false);
            uiDockMapBuilder->btnEditJson->setCheckable(true);
            return;
        }
        auto qNode = dynamic_cast<QNode*>(items.front());
        if (!qNode) {
            uiDockMapBuilder->btnEditJson->setCheckable(false);
            uiDockMapBuilder->btnEditJson->setCheckable(true);
            return;
        }
        Jsobj jExpData = qNode->expData()->toJS();
        infoView->setText(JsonHelper::JS2Str(jExpData, false).data());
        infoView->setTextColor(Qt::darkGray);
        uiDockMapBuilder->btnEditJson->setText("Complete Editting");
    } else {
        uiDockMapBuilder->btnEditJson->setText("Edit Json of Selected Node");
        Jsobj editedJs = JsonHelper::Str2JS(infoView->toPlainText().toStdString());
        ExpDataPtr madeExpData = ExpData::madeFromJS(editedJs);
        if (madeExpData) {
            auto qNode = dynamic_cast<QNode*>(items.front());
            qNode->getRelatedMergedExp()->exchangeMergedExpData(madeExpData);
            qNode->notifySizeChange();
            qNode->notifyNeighbours2Move();
        } else {
            infoView->append("\n Trans error!");
        }
        infoView->setTextColor(Qt::black);
        cout << "Json editting complete" << endl;
    }
    uiDockMapBuilder->btnAddGate2Corridor->setDisabled(start);
    uiDockMapBuilder->btnConnectGates->setDisabled(start);
    startEdittingNodes(start);
    gvMain->setDisabled(start);
    infoView->setReadOnly(!start);
}


QString tmap::TmapUI::getExpDataLabel(const tmap::ExpDataPtr& expData)
{
    QString name(expData->getName().data());
    if (name.isEmpty()) {
        static size_t indexOfAnoymousExpData = 0;
        name = "[" + QString::number(indexOfAnoymousExpData++) + "] " +
               QString::number(expData->nGates()) + "g " +
               QString{ExpData::typeStr(expData->type()).data()};
    }
    return name;
}

void tmap::TmapUI::SLOT_SaveExp()
{
    auto& exps = uiDockExpBuilder->cbBuiltExps;
    if (exps->count() == 0) {
        return;
    }
    const auto& expData2Save = exps->itemData(exps->currentIndex()).value<ExpDataPtr>();
    string fileName = uiDockExpBuilder->expName->text().toStdString();
    if (fileName.empty()) {
        fileName = expData2Save->getName();
    } else {
        expData2Save->setName(fileName);
    }
    if (JsonHelper::saveJson(expData2Save->toJS(), fileName) == 0) {
        cout << "save exp data [name:" + fileName + "] success!" << endl;
    }
}

void tmap::TmapUI::SLOT_LoadExp()
{
    auto& expNameEdit = uiDockExpBuilder->expName;
    const auto& name = expNameEdit->text();
    if (name.isEmpty()) {
        return;
    }
    Jsobj jExpData = JsonHelper::loadJson(name.toStdString());
    ExpDataPtr expRead = ExpData::madeFromJS(jExpData);
    if (expRead) {
        cout << "load exp data [name:" << name.toStdString() << "] success!" << endl;
        addBuiltExpData(expRead);
    }
}

void tmap::TmapUI::SLOT_DragMode(bool isDrag)
{
    if (isDrag) {
        cout << "Drag mode is ON" << endl;
        gvMain->setDragMode(QGraphicsView::ScrollHandDrag);
    } else {
        cout << "Drag mode is OFF" << endl;
        gvMain->setDragMode(QGraphicsView::NoDrag);
    }
}

void tmap::TmapUI::SLOT_AddFakeNode()
{
    cout << "A copy of exp is added to the map" << endl;
    auto& exps = uiDockExpBuilder->cbBuiltExps;
    if (exps->count() == 0) {
        return;
    }
    gvMain->addNode2FakeMapFromExpData(exps->itemData(exps->currentIndex()).value<ExpDataPtr>());
}

void tmap::TmapUI::SLOT_DrawEdge(bool startDraw)
{
    startEdittingNodes(startDraw);
    if (uiDockMapBuilder->cbDrawCorridor->isChecked()) {
        gvMain->SLOT_StartDrawingEdge(startDraw);
    } else {
        gvMain->SLOT_StartDirectLinking(startDraw);
    }
}

void tmap::TmapUI::SLOT_SaveMap()
{
    string fileName = uiDockMapBuilder->mapName->text().toStdString();
    gvMain->saveFakeMap(fileName);
}

void tmap::TmapUI::SLOT_LoadMap()
{
    string fileName = uiDockMapBuilder->mapName->text().toStdString();
    gvMain->loadMap(fileName);
}

void tmap::TmapUI::startEdittingNodes(bool start)
{
    uiDockMapBuilder->cbEnableNodesMoving->setDisabled(start);
    uiDockMapBuilder->cbRestrictGrid->setDisabled(start);
    uiDockMapBuilder->cbDrawCorridor->setDisabled(start);
    dockExpBuilder->setDisabled(start);
    modeGroup->setDisabled(start);

    if (start) {
        uiDockMapBuilder->cbEnableNodesMoving->setChecked(false);
        uiDockMapBuilder->cbRestrictGrid->setChecked(false);
    }
}

void tmap::TmapUI::SLOT_AddGate2Corridor(bool start)
{
    uiDockMapBuilder->btnEditJson->setDisabled(start);
    uiDockMapBuilder->btnConnectGates->setDisabled(start);
    gvMain->SLOT_AcceptAddingGates2Corridor(start);
    startEdittingNodes(start);
}

bool tmap::TmapUI::checkROS()
{
    return ros::isStarted();
}

void tmap::TmapUI::SLOT_InitROS()
{
    if (checkROS()) {
        infoView->setText("You have connected to ROS core");
        return;
    }
    int argc = 0;
    char ** argv = nullptr;
    ros::init(argc, argv, "TMappingUI", ros::init_options::AnonymousName);

    infoView->setText("connect ROS successfully!");
    qactConnectToROS->setCheckable(true);
    qactConnectToROS->setChecked(true);
    qactConnectToROS->setEnabled(false);

    ros::start();

    if (!checkROS()) {
        infoView->setText("CANT find the ROS master, plz run roscore and try again");
        return;
    }

    ros::NodeHandle n;

    RSC_newExp = n.serviceClient<tmapping::NewExp>(TMAP_STD_SERVICE_NAME_NEW_EXP);
    RSC_throughGate = n.serviceClient<tmapping::GateMovement>(TMAP_STD_SERVICE_NAME_GATE_MOVE);
    RSC_getMaps = n.serviceClient<tmapping::GetMaps>(TMAP_STD_SERVICE_NAME_GET_MAPS);
}

void tmap::TmapUI::SLOT_SwitchMode(QAction* newMode)
{
    static QAction* lastMode = mode_BUILD;

    if (newMode == lastMode) {
        return;
    }

    if (lastMode == mode_REALTIME) {
        gvMain->switch2realMode(false);
    }
    else if (newMode == mode_REALTIME) {
        gvMain->switch2realMode(true);
    }

    if (lastMode == mode_SIMULATION) {
        gvMain->switch2simMode(false);
    }
    else if (newMode == mode_SIMULATION) {
        gvMain->switch2simMode(true);
    }

    dockSimulation->setShown(false);
    dockExpBuilder->setShown(false);
    dockMapBuilder->setShown(false);
    dockRealtime->setShown(false);

    if (newMode == mode_BUILD) {
        dockMapBuilder->setShown(true);
        dockExpBuilder->setShown(true);
    }

    if (newMode == mode_SIMULATION) {
        dockSimulation->setShown(true);
    }

    if (newMode == mode_REALTIME) {
        dockRealtime->setShown(true);
    }

    lastMode = newMode;
}

void tmap::TmapUI::SLOT_PlaceRobot()
{
    if (gvMain->setRobotInFake(uiDockSimulation->cbNoDetailMoving->isChecked())) {

        if (!uiDockSimulation->cbNoDetailMoving->isChecked()) {
            uiDockSimulation->cbAccidents->setChecked(false);
        }

        uiDockSimulation->btnPlaceRobot->setCheckable(true);
        uiDockSimulation->btnPlaceRobot->setChecked(true);
        uiDockSimulation->btnPlaceRobot->setEnabled(false);
        infoView->setText("right click the gate to move robot");
    } else {
        infoView->setText("Please select a node in the map to place the robot");
    }

    if (!checkROS()) {
        infoView->append("\n\nYou didn't connect to ROS!");
    }
}

void tmap::TmapUI::SLOT_ROS_ThroughGate(const ExpPtr& exp)
{
//    cout<<"exp is"<<endl;
//    cout<<JsonHelper::JS2Str(exp->toJS())<<endl;
    routeImgs.emplace_back(turnScene2Image(gvMain->getScene4FakeMap()));

    if (checkROS()) {
        tmapping::NewExp srvExp;
        ExpPtr theExp2Send;
        /// 根据机器人运动的设置, 选择拷贝的方式
        if (uiDockSimulation->cbNoDetailMoving->isChecked()) {
            if (!uiDockSimulation->cbAccidents->isChecked()) {
                /// 无脑移动方式, 直接把节点的信息丢出去
                theExp2Send = exp;
            } else {
                /// 有一定概率发生遗忘, 目前的设置是0.8的概率忘记一个点
                GateID leftGate = exp->getLeaveGate();
                GateID enterGate = exp->getEnterGate();
                if (enterGate < 0) {
                    enterGate = leftGate;
                }
                double careless = uiDockSimulation->sbCarelessPercentage->value() / 100.0;
                const auto& res = exp->expData()->buildShrinkedCopy(
                        false,
                        {
                                {SubNodeType::GATE,static_cast<uint32_t>(enterGate)},
                                {SubNodeType::GATE,static_cast<uint32_t>(leftGate)}
                        },
                        1 - careless,
                        1);
                theExp2Send.reset(new Exp(res.first, res.second[exp->getEnterGate()].index));
                theExp2Send->setLeftGate(res.second[exp->getLeaveGate()].index);
            }
        } else {
            /// 根据机器人在节点内的移动来选择观测到哪些信息
            const auto& res = exp->expData()->buildShrinkedCopy(true);
            theExp2Send.reset(new Exp(res.first, res.second[exp->getEnterGate()].index));
            theExp2Send->setLeftGate(res.second[exp->getLeaveGate()].index);
        }

        /// 添加噪声
        double posErr = 0., dirErr = 0.;
        if(uiDockSimulation->cbPosError->isChecked()) {
            posErr = uiDockSimulation->lePosError->text().toDouble();
        }
        if (uiDockSimulation->cbDirError->isChecked()) {
            dirErr = uiDockSimulation->leDirError->text().toDouble();
        }
        if (posErr != 0. || dirErr != 0.) {
            theExp2Send->expData()->addNoise(posErr, dirErr);
        }

        /// 发送数据
        auto str = JsonHelper::JS2Str(theExp2Send->toJS(), false);

        stepsMoved++;
#if TMAPPING_CONFIG_LOG_VERBOSE
        static int step = 0;
        cout << "\n=======================\nSTEP " << step++ << "\n" << str << endl;
#endif
        srvExp.request.jNewExp = str;
//        cout<<"srvExp.request.jNewExp is "<< srvExp.request.jNewExp<<endl;
        

        if (!RSC_newExp.call(srvExp)) {
            cerr << "ROS service [newExp] call failed!" << endl;
            return;
        }

        gvMain->setChampionSucceedSteps(
                JsonHelper::Str2JS(srvExp.response.jChampionStatus).asUInt64());

        tmapping::GateMovement gateMovement;
        Jsobj gateMove = theExp2Send->getLeaveGate();
        gateMovement.request.jGateMove = JsonHelper::JS2Str(gateMove);

        if (!RSC_throughGate.call(gateMovement)) {
            cerr << "ROS service [gateMove] call failed!" << endl;
            return;
        }

        sentNodes["exp"].append(theExp2Send->toJS());
        sentNodes["gateMove"].append(gateMove);
    } else {
        cout << "ROS hasn't started, the message will not be sent" << endl;
    }
}

void tmap::TmapUI::SLOT_GetRealtimeMaps()
{
    if (!checkROS()) {
        infoView->setText("Please connect to ROS first ");
        return;
    }

    tmapping::GetMaps infoBridge;
    infoBridge.request.nMapRequired = uiDockRealtime->sbMapNeeded->text().toUInt();

    if (RSC_getMaps.call(infoBridge)) {
        realtimeMaps = JsonHelper::Str2JS(infoBridge.response.jMaps);
//        cout<<realtimeMaps<<endl;
        infoView->setText("realtimeMaps received!");

#ifdef TMAPPING_CONFIG_RECORD_POSS
        mChampionPoss.clear();
        mRunnerUpPoss.clear();
        for (const auto& jPoss : realtimeMaps["championsPoss"]) {
            mChampionPoss.push_back(jPoss.asDouble());
        }
        for (const auto& jPoss : realtimeMaps["runnerUpsPoss"]) {
            mRunnerUpPoss.push_back(jPoss.asDouble());
        }
#endif

        gvMain->displayRealMap(realtimeMaps["maps"][0]);

        uiDockRealtime->cbCandidates->clear();

        for (int i = 0; i < realtimeMaps["maps"].size(); ++i) {
            uiDockRealtime->cbCandidates->addItem("Map#" + QString::number(i) + " " +
            QString::number(realtimeMaps["maps"][i]["poss"].asDouble()));
        }
    }
}

void tmap::TmapUI::SLOT_DisplayTheRealMap(int index)
{
    gvMain->displayRealMap(realtimeMaps["maps"][index]);
}

void tmap::TmapUI::SLOT_RandomMove()
{
    sentNodes.clear();
    routeImgs.clear();
    gvMain->randomMove(uiDockSimulation->sbMoveSteps->value(),
            uiDockSimulation->cbMoveUntilCover->isChecked());
}

void tmap::TmapUI::SLOT_ShowPossHistroy()
{
    infoView->setText(gvMain->currentPossHistoryStr().data());

#ifdef TMAPPING_CONFIG_RECORD_POSS
    /// 和原来的方式不统一, 但这里属于debug开发内容, 不打算仔细处理
    auto currentIndex = uiDockRealtime->cbCandidates->currentIndex();
    auto theDisplayingMap = StructedMapImpl(realtimeMaps["maps"][currentIndex]);
    const auto& possHistory = theDisplayingMap.getPossHistory();
    cout << "current VS the best err" << endl;
    for (int i = 0; i < possHistory.size(); ++i) {
        double possMain = possHistory[i];
        double possAnother = mRunnerUpPoss[i];
        if (possMain != mChampionPoss[i]) {
            possAnother = mChampionPoss[i];
        }
        cout << possMain << '\t' << possAnother << endl;
    }
#endif
}

void tmap::TmapUI::SLOT_SetMapSurvivers(int value)
{
    if (checkROS()) {
        ros::NodeHandle n;
        auto RSC_setSurvivor = n.serviceClient<tmapping::SetSurviverMapsNum>
                (TMAP_STD_SERVICE_NAME_SET_SURVIVERS);
        tmapping::SetSurviverMapsNum s;
        s.request.nMaps = value;
        if (RSC_setSurvivor.call(s)) {
            cout << "set survivor num success !" << endl;
        } else {
            cout << "service[set survivors num] call failure!" << endl;
        }
    } else {
        infoView->setText("Connect ROS First!");
    }
}

void tmap::TmapUI::SLOT_StartMassiveTrials()
{
    uiDockSimulation->cbMoveUntilCover->setChecked(true);
    uiDockSimulation->sbMoveSteps->setValue(50);
    uiDockRealtime->sbMapNeeded->setValue(1);

    ros::NodeHandle n;
    auto RSC_reset = n.serviceClient<std_srvs::Empty>
            (TMAP_STD_SERVICE_NAME_RESET);

    int nFail = 0;
    int nSuccess = 0;
    int nTotalSteps = 0;

    vector<int> successSteps;
    auto nTrials = uiDockSimulation->sbTrials->value();
    successSteps.reserve(nTrials);

    for (int i = 0; i < nTrials; ++i) {
        std_srvs::Empty e;
        RSC_reset.call(e);

        stepsMoved = 0;
        SLOT_RandomMove();

        SLOT_GetRealtimeMaps();

        if (gvMain->isTheRealTimeMapSimiliar()) {
            cout << "=";
            nSuccess++;
            successSteps.push_back(stepsMoved);
            nTotalSteps += stepsMoved;
        } else {
            cout << "[" << i << "]";
            nFail++;
#if TMAPPING_CONFIG_DEBUG_MODE
            break;
#else
            gvMain->saveRealMap("F" + to_string(i));
#endif
        }
        flush(cout);
    }

    double aveStep = nTotalSteps / (double) nSuccess;
    double stdErr = 0.0;
    for (int nStep : successSteps) {
        double dif = nStep - aveStep;
        stdErr += dif * dif;
    }
    stdErr /= nSuccess;
    stdErr = sqrt(stdErr);

    cout << "Test complete, failure: " << nFail
         << "\t success: " << nSuccess
         << "\t %: " << nSuccess / (double) nTrials << endl;
    cout << "Ave success steps taken: " << aveStep << "\tStdErr: " << stdErr << endl;
}

void tmap::TmapUI::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);

    switch (event->key()) {
        case Qt::Key_J: {
            uiDockMapBuilder->btnEditJson->click();
            break;
        }
        case Qt::Key_C: {
            uiDockMapBuilder->btnConnectGates->click();
            break;
        }
        case Qt::Key_F: {
            uiDockSimulation->btnRandomMove->click();
            break;
        }
        case Qt::Key_G: {
            uiDockRealtime->btnGetRealTimeMap->click();
            break;
        }
        case Qt::Key_R: {

            if (! event->modifiers() & Qt::ControlModifier) {
                break;
            }

            SLOT_GetRealtimeMaps();

            /// 确定对应k时刻错误地图的冠军是第一名地图还是第二名地图
            auto currentIndex = uiDockRealtime->cbCandidates->currentIndex();
            auto theDisplayingMap = StructedMapImpl(realtimeMaps["maps"][currentIndex]);
            const auto& possHistory = theDisplayingMap.getPossHistory();
            /// 记录下来, 重新建图的时候进行记录
            vector<bool> isChampionAtK(possHistory.size());
            for (int i = 0; i < possHistory.size(); ++i) {
                isChampionAtK[i] = possHistory[i] == mChampionPoss[i];
            }

            /// 重置一下建图核心
            ros::NodeHandle n;
            auto RSC_reset = n.serviceClient<std_srvs::Empty>
                    (TMAP_STD_SERVICE_NAME_RESET);
            std_srvs::Empty e;
            RSC_reset.call(e);

            uiDockRealtime->sbMapNeeded->setValue(2);

            /// sentNodes里记录了上次发出去的所有移动信息, 让建图核心对相同的输入重新建图
            for (int i = 0; i < sentNodes["exp"].size(); ++i) {

                tmapping::NewExp srvExp;
                srvExp.request.jNewExp = JsonHelper::JS2Str(sentNodes["exp"][i]);
                if (!RSC_newExp.call(srvExp)) {
                    cerr << "ROS service [newExp] call failed!" << endl;
                    return;
                }

                tmapping::GateMovement gateMovement;
                gateMovement.request.jGateMove = JsonHelper::JS2Str(sentNodes["gateMove"][i]);
                if (!RSC_throughGate.call(gateMovement)) {
                    cerr << "ROS service [gateMove] call failed!" << endl;
                    return;
                }

                /// 根据实现的记录, 把错误地图冠军显示出来
                SLOT_GetRealtimeMaps();
                if (isChampionAtK[i]) {
                    SLOT_DisplayTheRealMap(1);
                } else {
                    SLOT_DisplayTheRealMap(0);
                }

                stringstream ss;
                ss << setw(4) << setfill('0') << i;
                /// 把显示的这张图保存下来
                saveImg(turnScene2Image(gvMain->getScene4RealMap()), "WRONG_" + ss.str(),
                        uiDockMapBuilder->mapName->text().toStdString());
                /// 同时保存一下上次随机运动时记录的机器人移动画面
                /// (因为当你决定保存这次结果时, 上次机器人的运动信息实际已经丢失了, 所以每次都要预存)
                saveImg(routeImgs[i], "REALMOVE_" + ss.str(),
                        uiDockMapBuilder->mapName->text().toStdString());
            }

            /// 接下来是正确地图的获取, 当被篡位时正确地图并不一定就是第二名,
            /// 但是我们可以通过地图生长树获得所有的历史形态(这一步在core完成, 我们获得的就是所有列表)
            ros::ServiceClient RSC_championHisLooker = n.serviceClient<tmapping::GetMaps>
                    (TMAP_STD_SERVICE_GET_CHAMPION_HISTORY);
            tmapping::GetMaps championHistoryMsg;
            if (RSC_championHisLooker.call(championHistoryMsg)) {
                Jsobj jMaps = JsonHelper::Str2JS(championHistoryMsg.response.jMaps);
                for (int i = 0; i < jMaps.size(); i++) {
                    gvMain->displayRealMap(jMaps[i]);
                    stringstream ss;
                    ss << setw(4) << setfill('0') << jMaps.size() - i - 1;
                    saveImg(turnScene2Image(gvMain->getScene4RealMap()),
                            "RIGHT_" + ss.str(),
                            uiDockMapBuilder->mapName->text().toStdString());
                }
            }

            break;
        }
        case Qt::Key_S: {
            if (mode_REALTIME->isChecked() && event->modifiers() & Qt::ControlModifier) {
                int id = uiDockRealtime->cbCandidates->currentIndex();
                if (id >= 0) {
                    JsonHelper::saveJson(
                            realtimeMaps["maps"][id], "result" + to_string(id), true);
                }
            }
        }
        case Qt::Key_T: {
            saveImg(turnScene2Image(gvMain->scene()), "test");
        }
        default:
            break;
    }
}

void tmap::TmapUI::saveScenePic(QGraphicsScene* scene, std::string fileName, std::string folder)
{
    const auto& outBound = scene->itemsBoundingRect();
    int maxLen = max(outBound.width(), outBound.height());
    if (maxLen < 1000) {
        maxLen = 1000;
    }
    QImage img(maxLen, maxLen, QImage::Format_RGB32);
    img.fill(Qt::white);
    QPainter painter(&img);
    QRectF targetRect(0, 0, outBound.width(), outBound.height());
    targetRect.moveCenter({maxLen / 2.0, maxLen / 2.0});
    gvMain->scene()->render(&painter, targetRect, outBound);
    painter.end();

    if (img.width() > 1000) {
        img = img.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

std::unique_ptr<QImage> tmap::TmapUI::turnScene2Image(QGraphicsScene* scene)
{
    const auto& outBound = scene->itemsBoundingRect();
    int maxLen = max(outBound.width(), outBound.height());
    if (maxLen < 1000) {
        maxLen = 1000;
    }
    unique_ptr<QImage> img(new QImage(maxLen, maxLen, QImage::Format_RGB32));
    img->fill(Qt::white);
    QPainter painter(img.get());
    QRectF targetRect(0, 0, outBound.width(), outBound.height());
    targetRect.moveCenter({maxLen / 2.0, maxLen / 2.0});
    gvMain->scene()->render(&painter, targetRect, outBound);
    painter.end();

    if (img->width() > 1000) {
        *img = img->scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return img;
}

void tmap::TmapUI::saveImg(const unique_ptr<QImage>& img, std::string fileName, std::string folder)
{
    uid_t uid;
    struct passwd* pwd;
    uid = getuid();
    pwd = getpwuid(uid);
    chdir(pwd->pw_dir);
    mkdir(tmap::TMAP_STD_FILE_SAVE_FLODER_NAME, 0b111111111);
    chdir(tmap::TMAP_STD_FILE_SAVE_FLODER_NAME);

    if (!folder.empty()) {
        struct tm * timeStructP;
        time_t timeLong;
        timeLong = time(nullptr);
        timeStructP = localtime(&timeLong);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "_%Y%m%d_%H%M", timeStructP);
        folder += tmp;
        mkdir(folder.data(), 0b111111111);
        chdir(folder.data());
    }

    fileName += ".png";

    if (img->save(fileName.data(), nullptr, 100)) {
        cout << "save img " << fileName << " success!" << endl;
    } else {
        cerr << "save img " << fileName << " FAIL!" << endl;
    }
}
