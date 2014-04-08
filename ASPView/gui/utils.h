// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__
#ifndef UTILS_H
#define UTILS_H
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <vw/Image/ImageView.h>

namespace utils{

  template<class T>
  const T * vecPtr(const std::vector<T>& X){
    if (X.size() == 0) return NULL;
    else               return &X.front();   
  }

  template<class T>
  T * vecPtr(std::vector<T>& X){
    if (X.size() == 0) return NULL;
    else               return &X.front();   
  }

  template<class T>
  std::string num2str(T num){
    std::ostringstream S;
    S << num;
    return S.str();
  }

  inline double distance(double x0, double y0, double x1, double y1){
    return sqrt( (x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0) );
  }
  
  inline int iround(double x){ return (int)round(x); }
  inline int iceil (double x){ return (int)ceil( x); }
  inline int ifloor(double x){ return (int)floor(x); }
  inline int isign (double x){
    if (x > 0) return  1;
    if (x < 0) return -1;
    return 0;
  }
  
  void expandBoxToGivenRatio(// inputs
                             double aspectRatio, 
                             // inputs/outputs
                             double & xll,  double & yll,
                             double & widx, double & widy);
  
  struct valIndex{
    double val;
    int    index;
    bool   isOutward;
    int    nextIndexInward; // Useful only when isOutward is true
  };

  void cutEdge(double x0, double y0, double x1, double y1,
               double nx, double ny, double H,
               double & cutx, double & cuty);
  
  void cutPolyLine(// inputs -- the polygonal line
                   int numVerts,
                   const double * xv, const double * yv,
                   // inputs -- the cutting window
                   double xll, double yll, double xur, double yur,
                   // outputs -- the cut polygons
                   std::vector< double> & cutX,
                   std::vector< double> & cutY,
                   std::vector< int>    & cutNumPolys);
  
  void cutPoly(// inputs -- the polygons
               int numPolys, const int * numVerts,
               const double * xv, const double * yv,
               // inputs -- the cutting window
               double xll, double yll, double xur, double yur,
               // outputs -- the cut polygons
               std::vector< double> & cutX,
               std::vector< double> & cutY,
               std::vector< int>    & cutNumPolys);

  inline bool lessThan (valIndex A, valIndex B){ return A.val < B.val; }
  
  void processPointsOnCutline(std::vector<valIndex> & ptsOnCutline);

  void cutToHalfSpace(// inputs 
                      double nx, double ny, double dotH,
                      int numV, 
                      const double * xv, const double * yv,
                      // outputs -- the cut polygons
                      std::vector<double> & cutX,
                      std::vector<double> & cutY,
                      std::vector<int>    & cutNumPolys);

  
  // A class holding a set of polygons in double precision
  class dPoly{

  public:

    dPoly(){
      reset();
    }

    void reset();
  
    bool readPoly(std::string filename,
                  bool isPointCloud = false
                  );
  
    void writePoly(std::string filename, std::string defaultColor = "yellow");
    void bdBoxCenter(double & mx, double & my) const;
  
    void appendPolygon(int numVerts,
                       const double * xv,
                       const double * yv,
                       bool isPolyClosed,
                       const std::string & color
                       );
  
    void appendPolygons(const dPoly & poly);

    void appendRectangle(double xl, double yl, double xh, double yh,
                         bool isPolyClosed,
                         const std::string & color
                         );

    void setRectangle(double xl, double yl, double xh, double yh,
                      bool isPolyClosed,
                      const std::string & color
                      );

    void clipPoly(// inputs
                  double clip_xll, double clip_yll,
                  double clip_xur, double clip_yur,
                  dPoly & clippedPoly // output
                  );
  
    const int    * get_numVerts         () const { return utils::vecPtr(m_numVerts); }
    const double * get_xv               () const { return utils::vecPtr(m_xv);       }
    const double * get_yv               () const { return utils::vecPtr(m_yv);       }
    int get_numPolys                    () const { return m_numPolys;                }
    int get_totalNumVerts               () const { return m_totalNumVerts;           }
    std::vector<char> get_isPolyClosed  () const { return m_isPolyClosed;            }
    std::vector<std::string> get_colors () const { return m_colors;                  }
  
    void set_color(std::string color);

    void set_isPolyClosed(bool isPolyClosed);
  
    void appendAndShiftMarkedPolys(// Inputs
                                   std::map<int, int> & mark,
                                   double shift_x, double shift_y
                                   );
    void set_isPointCloud(bool isPointCloud){ m_isPointCloud = isPointCloud; }
    bool isPointCloud() { return m_isPointCloud;}
  
    void markPolysIntersectingBox(// Inputs
                                  double xll, double yll,
                                  double xur, double yur,
                                  // Outputs
                                  std::map<int, int> & mark
                                  ) const;
    void replaceOnePoly(int polyIndex, int numV, const double* x, const double* y);
    void bdBox(double & xll, double & yll, double & xur, double & yur) const;
  
    void bdBoxes(std::vector<double> & xll, std::vector<double> & yll,
                 std::vector<double> & xur, std::vector<double> & yur) const;
  
    void setPolygon(int numVerts,
                    const double * xv,
                    const double * yv,
                    bool isPolyClosed,
                    const std::string & color
                    );
  
    void insertVertex(int polyIndex, int vertIndex,
                      double x, double y);
    void changeVertexValue(int polyIndex, int vertIndex, double x, double y);
    void shiftEdge(int polyIndex, int vertIndex, double shift_x, double shift_y);
    void shiftOnePoly(int polyIndex, double shift_x, double shift_y);
    void shiftMarkedPolys(const std::map<int, int> & mark, double shift_x, double shift_y);

    // This viewer can read images in addition to polygons. The bounding
    // box of the image becomes the polygon.
    vw::ImageView<float> m_img;

  private:

    // If isPointCloud is true, treat each point as a set of unconnected points
    bool                     m_isPointCloud; 

    std::vector<double>      m_xv;
    std::vector<double>      m_yv; 
    std::vector<int>         m_numVerts;
    int                      m_numPolys;
    int                      m_totalNumVerts;
    std::vector<char>        m_isPolyClosed;
    std::vector<std::string> m_colors;
  
  };
  
  void bdBox(const std::vector<dPoly> & polyVec,
             // outputs
             double & xll, double & yll,
             double & xur, double & yur
             );
  
  void setUpViewBox(// inputs
                    const std::vector<dPoly> & polyVec,
                    // outputs
                    double & xll,  double & yll,
                    double & widx, double & widy
                    );


  enum closedPolyInfo{
    // If an array of points as read from file has the first vertex
    // equal to the last one, we treat it is a closed polygon (last
    // vertex joins the first vertex).  If the user wants to override
    // this behavior, the first two fields below become necessary.
    forceClosedPoly, forceNonClosedPoly, readClosedPolyInfoFromFile
  };
  
  struct polyOptions{
    // Each polygon file has these options
    bool            plotAsPoints;
    bool            isPolyFilled;
    closedPolyInfo  isPolyClosed;
    bool            useCmdLineColor;
    int             fontSize;
    int             lineWidth;
    bool            isGridOn;
    double          gridSize;
    int             gridWidth;
    bool            readPolyFromDisk;
    std::string     bgColor;
    std::string     fgColor;
    std::string     cmdLineColor;
    std::string     gridColor;
    std::string     polyFileName;
  
    polyOptions();
  
  };

  struct cmdLineOptions{
    std::vector<polyOptions> polyOptionsVec;
    int windowWidX;
    int windowWidY;
    cmdLineOptions();
  };

  std::string getDocText();
  
  void extractWindowDims(// inputs
                         int numArgs, char ** args,
                         // outputs
                         int & windowWidX, int & windowWidY
                         );

  void parseCmdOptions(//inputs
                       int argc, char** argv, std::string exeName,
                       // outputs
                       cmdLineOptions & options
                       );

  void printUsage(std::string progName);

  std::string getFilenameExtension(std::string filename);
  std::string replaceAll(std::string result, 
                         const std::string & replaceWhat, 
                         const std::string & replaceWithWhat);

}

#endif
