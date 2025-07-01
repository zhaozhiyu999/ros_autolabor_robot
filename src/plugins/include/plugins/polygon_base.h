
#ifndef XXX_POLYGON_BASE_H_
#define XXX_POLYGON_BASE_H_

namespace polygon_base
{
  class RegularPolygon
  {
    public:

    //计算周长与面积
      virtual void initialize(double side_length) = 0;
      virtual double area() = 0;
      virtual ~RegularPolygon(){}

    protected:
      RegularPolygon(){}
  };
};
#endif