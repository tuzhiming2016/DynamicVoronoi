#ifndef _DYNAMICVORONOI_H_
#define _DYNAMICVORONOI_H_


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <queue>

#include "bucketedqueue.h"

//! A DynamicVoronoi object computes and updates a distance map and Voronoi diagram.
class DynamicVoronoi {

public:

    DynamicVoronoi();
    ~DynamicVoronoi();

    //! Initialization with an empty map
    void initializeEmpty(int _sizeX, int _sizeY, bool initGridMap=true);
    //! Initialization with a given binary map (false==free, true==occupied)
    void initializeMap(int _sizeX, int _sizeY, bool** _gridMap);

    //! add an obstacle at the specified cell coordinate
    void occupyCell(int x, int y);
    //! remove an obstacle at the specified cell coordinate
    void clearCell(int x, int y);
    //! remove old dynamic obstacles and add the new ones
    void exchangeObstacles(std::vector<INTPOINT>& newObstacles);

    //! update distance map and Voronoi diagram to reflect the changes
    void update(bool updateRealDist=true);
    //! prune the Voronoi diagram
    void prune();
    //! prune the Voronoi diagram by globally revisiting all Voronoi nodes. Takes more time but gives a more sparsely pruned Voronoi graph. You need to call this after every call to udpate()
    void updateAlternativePrunedDiagram();
    //! retrieve the alternatively pruned diagram. see updateAlternativePrunedDiagram()
    int** alternativePrunedDiagram(){
        return alternativeDiagram;
    }
    //! retrieve the number of neighbors that are Voronoi nodes (4-connected)
    int getNumVoronoiNeighborsAlternative(int x, int y);
    //! returns whether the specified cell is part of the alternatively pruned diagram. See updateAlternativePrunedDiagram.
    bool isVoronoiAlternative( int x, int y );

    //! returns the obstacle distance at the specified location
    float getDistance( int x, int y );
    //! returns whether the specified cell is part of the (pruned) Voronoi graph
    bool isVoronoi( int x, int y );
    //! checks whether the specficied location is occupied
    bool isOccupied(int x, int y);
    //! write the current distance map and voronoi diagram as ppm file
    void visualize(const char* filename="result.ppm");

    //! returns the horizontal size of the workspace/map
    unsigned int getSizeX() {return sizeX;}
    //! returns the vertical size of the workspace/map
    unsigned int getSizeY() {return sizeY;}

private:                //DM和GVD的栅格用dataCell二维数组表示  gridMap_是输入的二值占据栅格地图
    struct dataCell {
        float dist;
        char voronoi;
        char queueing;
        int obstX;
        int obstY;
        bool needsRaise;
        int sqdist;
    };

    //state是voronoiKeep 的点，便是Voronoi的边上的点，组成了Voronoi图
    typedef enum {voronoiKeep=-4, freeQueued = -3, voronoiRetry=-2, voronoiPrune=-1, free=0, occupied=1} State;
    typedef enum {fwNotQueued=1, fwQueued=2, fwProcessed=3, bwQueued=4, bwProcessed=1} QueueingState;
    typedef enum {invalidObstData = SHRT_MAX/2} ObstDataState;
    //表示剪枝操作时栅格的临时状态
    typedef enum {pruned, keep, retry} markerMatchResult;



    // methods
    void setObstacle(int x, int y);
    void removeObstacle(int x, int y);
    inline void checkVoro(int x, int y, int nx, int ny, dataCell& c, dataCell& nc);
    void recheckVoro();
    void commitAndColorize(bool updateRealDist=true);
    inline void reviveVoroNeighbors(int &x, int &y);

    inline bool isOccupied(int &x, int &y, dataCell &c);
    inline markerMatchResult markerMatch(int x, int y);
    inline bool markerMatchAlternative(int x, int y);
    inline int getVoronoiPruneValence(int x, int y);

    // queues

    BucketPrioQueue<INTPOINT> open;             //保存待考察的栅格
    std::queue<INTPOINT> pruneQueue;            //保存待剪枝的栅格
    BucketPrioQueue<INTPOINT> sortedPruneQueue; //保存预处理后的待剪枝的栅格

    std::vector<INTPOINT> removeList;           //保存移除的障碍物曾占据的栅格
    std::vector<INTPOINT> addList;              //保存增加的障碍物要占据的栅格
    std::vector<INTPOINT> lastObstacles;        //保存上次添加的障碍物覆盖的栅格

    // maps
    int sizeY;
    int sizeX;
    dataCell** data;                //保存了每个栅格与最近障碍物的距离、最近障碍物的坐标、是否Voronoi点的标志
    bool** gridMap;                 //true是被占用，false是没有被占用 二值栅格地图
    bool allocatedGridMap;          //是否为gridmap分配了内存的标志位

    // parameters
    int padding;
    double doubleThreshold;         //阈值 门槛

    double sqrt2;

    //  dataCell** getData(){ return data; }
    int** alternativeDiagram;
};


#endif

