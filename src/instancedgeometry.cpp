#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "core/log.h"
#include "instancedgeometry.h"

#include <list>

namespace pb
{

//void InstancedGeometry::transformAttribMgr::CreateBuffer(transformBufferState& state, Transforms& transforms)
//{
//    glGenBuffers( 1, &state.idVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
//    glBufferData( GL_ARRAY_BUFFER, EntryBufferSize(), &transforms[0][0][0], GL_DYNAMIC_DRAW );
//    glBindBuffer( GL_ARRAY_BUFFER, 0 );
//}
//
//void InstancedGeometry::transformAttribMgr::UpdateTransform(unsigned int transformId)
//{
//    unsigned int entryId = transformId / MAX_TRANSFORMS_PER_ENTRY;
//    unsigned int transformOffset = transformId % MAX_TRANSFORMS_PER_ENTRY;
//
//    transformBufferState& state = EntryState( entryId );
//    glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
//    glBufferSubData(
//        GL_ARRAY_BUFFER,
//        transformOffset * sizeof(Mat4),
//        sizeof(Mat4),
//        (void*)&mTransforms[ entryId ][ transformOffset ][ 0 ][ 0 ]);
//
//    glBindBuffer( GL_ARRAY_BUFFER, 0 );
//}

}

