#include "dynamicvoronoi.h"

#include <iostream>
#include <fstream>
#include <string.h>
//文件输入流传的是引用 中间两个参数传的是地址 三级指针 指向某一个地图
void loadPGM( std::istream &is, int *sizeX, int *sizeY, bool ***map ) {
    std::string tag;
    is >> tag;
    //PGM 便携式灰度图像格式(portable graymap file format)图片
    //PGM格式图像格式分为两类:P2和P5类型.不管是P2还是P5类型的PGM文件,都由两部分组成,文件头部分和数据部分
    //1.PGM文件的格式类型(是P2还是P5);2.图像的宽度;3.图像的高度;4.图像灰度值可能的最大值;
    //文件头的这四部分信息都是以ASCII码形式存储的,所以可以直接在将P2或P5格式的PGM文件在记事本中打开看到文件头的信息.
    if (tag!="P5") {
        std::cerr << "Awaiting 'P5' in pgm header, found " << tag << std::endl;
        exit(-1);
    }
//返回输入流下一个字符，并把它留在输入流中，作为下一次读取的起点。返回值是整形ascll码值，可以用 char(c) 转化为字符。
//从输入流中读取n个字符并且丢弃，或者读到delim字符再停止读取。
    while (is.peek() == ' ' || is.peek()=='\n')
        is.ignore();
    while (is.peek()=='#')
        is.ignore(255, '\n');
    is >> *sizeX;
    while (is.peek()=='#')
        is.ignore(255, '\n');
    is >> *sizeY;
    while (is.peek()=='#')
        is.ignore(255, '\n');
    is >> tag;
    if (tag!="255") {
        std::cerr << "Awaiting '255' in pgm header, found " << tag << std::endl;
        exit(-1);
    }
    is.ignore(255, '\n');

    *map = new bool*[*sizeX];

    for (int x=0; x<*sizeX; x++) {
        (*map)[x] = new bool[*sizeY];
    }
    for (int y=*sizeY-1; y>=0; y--) {
        for (int x=0; x<*sizeX; x++) {
            int c = is.get();
            if ((double)c<255-255*0.2)
                (*map)[x][y] = true; // cell is occupied
            else
                (*map)[x][y] = false; // cell is free
            if (!is.good()) {
                std::cerr << "Error reading pgm map.\n";
                exit(-1);
            }
        }
    }
}


int main( int argc, char *argv[] ) {
    //argv[0] /home/tu/  || argv[1] testmap.pgm  ||  argv[2] [prune|pruneAlternative]
    if(argc<2 || argc>3 || (argc==3 && !(strcmp(argv[2],"prune")==0 || strcmp(argv[2],"pruneAlternative")==0))) {
        std::cerr<<"usage: "<<argv[0]<<" <pgm map> [prune|pruneAlternative]\n";
        exit(-1);
    }

    bool doPrune = false;
    bool doPruneAlternative = false;
    if (argc==3) doPrune = true;

    if(doPrune && strcmp(argv[2],"pruneAlternative")==0){
        doPrune = false;
        doPruneAlternative = true;
    }


    // LOAD PGM MAP AND INITIALIZE THE VORONOI
    std::ifstream is(argv[1]);
    if (!is) {
        std::cerr << "Could not open map file for reading.\n";
        exit(-1);
    }

    bool **map=NULL;
    int sizeX, sizeY;
    loadPGM( is, &sizeX, &sizeY, &map ); //文件输入流 参数传的地址(宽，高，地图)     参数传引用在函数实现中
    is.close();
    fprintf(stderr, "Map loaded (%dx%d).\n", sizeX, sizeY);

    // create the voronoi object and initialize it with the map
    DynamicVoronoi voronoi;
    voronoi.initializeMap(sizeX, sizeY, map);
    voronoi.update(); // update distance map and Voronoi diagram
    if (doPrune) voronoi.prune();  // prune the Voronoi
    if (doPruneAlternative) voronoi.updateAlternativePrunedDiagram();  // prune the Voronoi

    voronoi.visualize("initial.ppm");
    std::cerr << "Generated initial frame.\n";

    // now perform some updates with random obstacles
    char filename[20];
    int numPoints = 10 + sizeX*sizeY*0.005;
    for (int frame=1; frame<=30; frame++) {
        std::vector<IntPoint> newObstacles;
        for (int i=0; i<numPoints; i++) {
            double x = 2+rand()/(double)RAND_MAX*(sizeX-4);
            double y = 2+rand()/(double)RAND_MAX*(sizeY-4);
            newObstacles.push_back(IntPoint(x,y));
        }
        voronoi.exchangeObstacles(newObstacles); // register the new obstacles (old ones will be removed)
        voronoi.update();
        if (doPrune) voronoi.prune();
        sprintf(filename, "update_%03d.ppm", frame);
        voronoi.visualize(filename);
        std::cerr << "Performed update with random obstacles.\n";
    }

    // now remove all random obstacles again.
    // final.pgm should be very similar to initial.pgm, except for ambiguous spots
    std::vector<IntPoint> empty;
    voronoi.exchangeObstacles(empty);
    voronoi.update();
    if (doPrune) voronoi.prune();
    voronoi.visualize("final.ppm");
    std::cerr << "Done with final update (all random obstacles removed).\n";
    std::cerr << "Check initial.ppm, update_???.ppm and final.ppm.\n";
    return 0;
}
