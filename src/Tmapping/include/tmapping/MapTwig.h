//
// Created by stumbo on 2019/11/17.
//

#ifndef TMAPPING_MAPTWIG_H
#define TMAPPING_MAPTWIG_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <map>

#include "tools/TopoParams.h"
#include "ExpCollection.h"

namespace tmap
{

/// 标明当前Twig的状态
enum class MapTwigStatus {
    /// 当前MapTwig已经停止了生长(因为MOVE2OLD发生矛盾或者MOVE2NEW发生了分歧理解),
    /// 但是可能后代分支还存在, 需要this的连接与数据, 所以this还活着
    EXPIRED,
    /// 当前MapTwig正在走向一个没有去过的位置, 有可能能产生分支
    MOVE2NEW,
    /// 当前MapTwig正在走向一个去过的位置, 不可能产生分支, 但是可能产生矛盾从而死亡
    MOVE2OLD
};

class MapTwig : public std::enable_shared_from_this<MapTwig>
{
    /// 出生在哪个Exp
    const size_t borndAt;
    /// 是哪个MapBranch生成的
    const MapTwigPtr mFather;
    /// Branch的UUID序号
    const size_t nSerial;

    /// 不包括 child's child
    std::vector<MapTwigWePtr> mChildren;
    /// 记录分歧的位置, 在何处分歧分离的(和后代的borneAt是相同的)
    size_t mDieAt = 0;
    /// 与过去经历重复的 exp
    std::vector<MergedExpPtr> mExpUsages;
    /// 当 status 为 MapTwigStatus::MOVE2OLD 的时候记录相似的Exp在哪里
    MergedExpPtr mExpOfArrivingSimilar{};
    /// 当 status 为 MapTwigStatus::MOVE2OLD 的时候记录对应相似Exp的gate是哪个
    GateID mGateOfSimilar;
    /// 当前MapBranch的状态
    MapTwigStatus status = MapTwigStatus::MOVE2NEW;
    /// 当前Branch的概率
    double mConfidence = 1.0;
    /// 与 confidence相关, 需要知道当前构型中有多少数量的TopoNode
    size_t nTopoNode;
    /// 上次计算得到的快速概率
    double mLastGlobalResult = -1.0;

#ifdef TMAPPING_CONFIG_RECORD_POSS
    std::vector<double> mPosses;

public:
    void addPoss(double poss) {
        mPosses.push_back(poss);
    }

    std::vector<double> getPossHistory(){
        std::vector<double> history;
        history.reserve(borndAt + mExpUsages.size());
        const auto& chain2origin = this->getChain2Father(0);
        for(auto it = chain2origin.rbegin() ; it != chain2origin.rend(); ++it) {
            for (const auto& poss : (**it).mPosses) {
                history.push_back(poss);
            }
        }
        return history;
    }
#endif

private: //constructor

    MapTwig(size_t bornAt, MapTwigPtr father, size_t nSerial, double confidence);

    double xConfidenceCoe(double coe);

public:

    static MapTwigPtr getAdamTwig();

    /**
     * @brief 从某个MapTwig产生一个新的后代, 自动建立父子关系, 传递概率, 设置this的死亡(但是不动status)
     * @param newSerial 新后代的Serial代号
     * @return 产生的Twig后代
     */
    MapTwigPtr bornOne(size_t newSerial);

    /**
     * @see MapTwigStatus::EXPIRED
     */
    void setExpired();

    MapTwigStatus getStatus() const;

    const MergedExpPtr& getTheArrivingSimiliarMergedExp() const;

    GateID gateOfSimilarMergedExp() const;

    /**
     * @brief 生成进行全局排序的概率分数, 在调用 resetLastGlobalConfidenceResult() 之前实际只计算一次
     * @param log_nExp 由调用者给予的log(nExp), 因为在单步内, nExp为固定数
     * @return mConfidence * exp(-nTopoNode * log(nExp))
     */
    double calGlobalPoss(double log_nExp);

    /**
     * @brief 清除上一次计算得到的全局概率, 从而能够产生新的概率(唯一方法)
     */
    void resetLastGlobalConfidenceResult();

    const std::vector<MapTwigWePtr>& getChildren() const;

    const std::vector<MergedExpPtr>& getExpUsages() const;

    bool hasChildren() const;

    double getConfidence() const;

    double getCaledGblConfidence() const;

    void nodeCountPlus();

    void setDieAt(size_t dieAt);

    /**
     * @brief 为此MapTwig添加新的融合假设, @note this->mConfidence的修正会在此函数中被调用, 无需外部调用
     * @param newMerged 新的融合假设
     */
    void addMergedExp(MergedExpPtr newMerged);

    void setTheSimilarMergedExpForNextTime(const ExpPtr& targetExp, GateID arrivingGate);

    void setMove2new();

    StructedMap makeMap(const ExpCollection& exps);

    /**
     * @brief 检查this是否从twig2check衍生而来
     * @param twig2check 要被检查是否为直系father的tiwg
     * @param nGenerations 查询代数, 默认只查询是否为father或者this
     * @return this是否由twig2check衍生而来
     */
    bool isDevelopedFrom(MapTwig* twig2check, size_t nGenerations = 1) const;

    size_t getNodeCount() const;

    /**
     * @brief 得到一个MapTwig的父子链条, front为最新,back为最老
     * @param endSerial 终点endSerial
     */
    std::vector<MapTwig*> getChain2Father(size_t endSerial);

    Jsobj structedMapHistory(const ExpCollection& exps);
};

}


#endif //TMAPPING_MAPTWIG_H
