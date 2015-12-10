#ifndef PB_GEOMETRY_H
#define PB_GEOMETRY_H

namespace pb
{

class Geometry
{

private:

    Geometry(const Geometry& rhs);
    Geometry& operator=(const Geometry& rhs);

public:

    Geometry() {};
    virtual ~Geometry() {};

    virtual void Render(const glm::mat4& vp) = 0;

};


class IInstancedGeometry : public Geometry
{

private:

    IInstancedGeometry(const IInstancedGeometry& rhs);
    IInstancedGeometry& operator=(const IInstancedGeometry& rhs);

public:

    IInstancedGeometry() {};
    virtual ~IInstancedGeometry() {};

    virtual unsigned int AddInstance() = 0;

};


}

#endif  // PB_GEOMETRY_H
