bl_info = {
    "name": "DGL3 Export",
    "author": "Timur Gafarov",
    "version": (2, 0),
    "blender": (2, 6, 4),
    "location": "File > Export > DGL3 Scene (.dgl3)",
    "description": "Tools to create and export DGL3 scenes",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import os
import struct
from math import pi, radians, sqrt
import mathutils
from mathutils import Vector
#from json import JSONEncoder

import bpy
from bpy.props import StringProperty
from bpy_extras.io_utils import ExportHelper

def packVector4f(v):
    return struct.pack('<ffff', v[0], v[1], v[2], v[3])

def packVector3f(v):
    return struct.pack('<fff', v[0], v[1], v[2])

def packVector2f(v):
    return struct.pack('<ff', v[0], v[1])

def packFileHeader(name, creator, data = []):
    buf = bytearray('DGL3'.encode('ascii'))
    buf = buf + struct.pack('<i', 300)
    buf = buf + struct.pack('<i', len(name))
    buf = buf + struct.pack('<i', len(creator))
    buf = buf + struct.pack('<i', len(data))
    buf = buf + bytearray(name.encode('ascii'))
    buf = buf + bytearray(creator.encode('ascii'))
    buf = buf + bytearray(data)
    return buf
    
def packSceneHeader(numMeshes, numEntities, numLights):
    buf = struct.pack('<i', numMeshes)
    buf = buf + struct.pack('<i', numEntities)
    buf = buf + struct.pack('<i', numLights)
    return buf

def vec2neq(v1, v2):
    if (v1[0] != v2[0]) or (v1[1] != v2[1]):
        return True;
    else:
        return False;

def listEq(list1, list2):
    for i, v in enumerate(list1):
        if v != list2[i]:
            return False;
    return True;

def vertexDataIndex(vertices, vertex):
    if vertex in vertices:
        for i, v in enumerate(vertices):
            if (listEq(v, vertex)):
                return i;
    return -1;

def rvec3d(v):
    return round(v[0], 6), round(v[1], 6), round(v[2], 6)

def rvec2d(v):
    return round(v[0], 6), round(v[1], 6)

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()
    
def packMesh(meshId, mesh):
    rotX = mathutils.Matrix.Rotation(-pi/2, 4, 'X')

    buf = struct.pack('<i', meshId)
    buf = buf + struct.pack('<i', len(mesh.name))
    buf = buf + bytearray(mesh.name.encode('ascii'))
    buf = buf + struct.pack('<i', 0) #isExternal

    mesh_triangulate(mesh)
    mesh.calc_tessface()

    vertexData = []
    vertexDict = {}
    numVertices = 0
    indices = [None] * len(mesh.tessfaces)

    for facei, face in enumerate(mesh.tessfaces):

        v1 = rotX * mesh.vertices[face.vertices[0]].co
        v2 = rotX * mesh.vertices[face.vertices[1]].co
        v3 = rotX * mesh.vertices[face.vertices[2]].co

        n1 = rotX * mesh.vertices[face.vertices[0]].normal
        n2 = rotX * mesh.vertices[face.vertices[1]].normal
        n3 = rotX * mesh.vertices[face.vertices[2]].normal

        uvtex = mesh.tessface_uv_textures

        uv1_0 = [0.0, 0.0]
        uv1_1 = [0.0, 0.0]
        uv1_2 = [0.0, 0.0]
        if len(uvtex) > 0:
            uv1_0 = uvtex[0].data[facei].uv1
            uv1_1 = uvtex[0].data[facei].uv2
            uv1_2 = uvtex[0].data[facei].uv3

        vdata1 = [v1[0], v1[1], v1[2], n1[0], n1[1], n1[2], uv1_0[0], uv1_0[1]]
        vdata2 = [v2[0], v2[1], v2[2], n2[0], n2[1], n2[2], uv1_1[0], uv1_1[1]]
        vdata3 = [v3[0], v3[1], v3[2], n3[0], n3[1], n3[2], uv1_2[0], uv1_2[1]]

        key1 = rvec3d(v1), rvec3d(n1), rvec2d(uv1_0)
        key2 = rvec3d(v2), rvec3d(n2), rvec2d(uv1_1)
        key3 = rvec3d(v3), rvec3d(n3), rvec2d(uv1_2)

        if not key1 in vertexDict:
            vertexData.append(vdata1);
            i1 = len(vertexData) - 1
            vertexDict[key1] = i1
        else:
            i1 = vertexDict[key1]

        if not key2 in vertexDict:
            vertexData.append(vdata2);
            i2 = len(vertexData) - 1
            vertexDict[key2] = i2
        else:
            i2 = vertexDict[key2]

        if not key3 in vertexDict:
            vertexData.append(vdata3);
            i3 = len(vertexData) - 1
            vertexDict[key3] = i3
        else:
            i3 = vertexDict[key3]

        indices[facei] = [i1, i2, i3]

    numVertices = len(vertexData)

    vertices = []
    normals = []
    texcoords = []

    if numVertices > 0:
        vertices = [None] * numVertices
        normals = [None] * numVertices
        texcoords = [None] * numVertices

        for i, v in enumerate(vertexData):
            vertices[i] = [v[0], v[1], v[2]]
            normals[i] = [v[3], v[4], v[5]]
            texcoords[i] = [v[6], 1.0 - v[7]] # y coordinate flipped

    buf = buf + struct.pack('<i', numVertices)
    for v in vertices:
        buf = buf + packVector3f(v)
    for n in normals:
        buf = buf + packVector3f(n)
    for uv in texcoords:
        buf = buf + packVector2f(uv)

    buf = buf + struct.pack('<i', 0) #haveLightmapTexCoords

    numTriangles = len(indices)
    buf = buf + struct.pack('<i', numTriangles)
    for i in indices:
        buf = buf + struct.pack('<iii', i[0], i[1], i[2])

    buf = buf + struct.pack('<i', 0) #haveSkeletalAnimation
    buf = buf + struct.pack('<i', 0) #haveMorphTargetAnimation

    return buf

def packEntity(entityId, obj, meshId, materialName):
    rotX = mathutils.Matrix.Rotation(-pi/2, 4, 'X')
    obj_matrix = obj.matrix_world.copy()
    mat = obj_matrix #rotX * obj_matrix

    buf = struct.pack('<i', entityId)
    buf = buf + struct.pack('<i', len(obj.name))
    buf = buf + bytearray(obj.name.encode('ascii'))
    buf = buf + struct.pack('<i', 0) #isExternal
    buf = buf + struct.pack('<i', meshId)

    position = rotX * obj.location
    rot = mat.to_quaternion()
    rotation = (rot.x, rot.z, rot.y, rot.w)
    scaling = (obj.scale.x, obj.scale.z, obj.scale.y)

    buf = buf + packVector3f(position)
    buf = buf + packVector4f(rotation)
    buf = buf + packVector3f(scaling)

    buf = buf + struct.pack('<i', 1) #numCustomProperties

    propMaterial = 'material'
    materialFilename = materialName + '.mat'
    buf = buf + struct.pack('<i', len(propMaterial))
    buf = buf + bytearray(propMaterial.encode('ascii'))
    buf = buf + struct.pack('<i', 5)
    buf = buf + struct.pack('<i', len(materialFilename))
    buf = buf + bytearray(materialFilename.encode('ascii'))

    return buf

def doExport(context, filepath = ""):
    print("Exporting objects as DGL file...")
    bpy.ops.object.mode_set(mode = 'OBJECT')

    materials = {}
    maxMatId = 0

    entityId = 0

    matrix_rotX = mathutils.Matrix.Rotation(-pi/2, 4, 'X')
    
    objects = bpy.data.objects
    meshes = []
    meshesByName = {}
    
    numMeshes = 0
    numEntities = 0
    numLights = 0
    for obj in objects:
        if obj.type == 'LAMP':
            numLights = numLights + 1
            
        if obj.type == 'MESH':
            numEntities = numEntities + 1
            mesh = obj.to_mesh(context.scene, True, 'PREVIEW', calc_tessface=True)
            if not obj.data.name in meshesByName:
                print("%s: %s" % (obj.data.name, numMeshes))
                meshesByName[obj.data.name] = numMeshes
                meshes.append(mesh)
                numMeshes = numMeshes + 1

    f = open(filepath, 'wb')
    
    f.write(packFileHeader('testSceneName', 'Blender'))
    f.write(packSceneHeader(numMeshes, numEntities, numLights))
    
    for meshId, mesh in enumerate(meshes):
        f.write(packMesh(meshId, mesh))

    entityId = 0
    for obj in objects:
        if obj.type == 'MESH':
            meshId = -1
            mesh = obj.data
            print(mesh.name)
            if mesh.name in meshesByName:
                meshId = meshesByName[mesh.name]

            materialName = '__default__'
            if len(obj.data.materials) > 0:
                mat = obj.data.materials[0]
                materialName = mat.name

            f.write(packEntity(entityId, obj, meshId, materialName))

            entityId = entityId + 1

    f.close()
    return {'FINISHED'}

class ExportDGLFile(bpy.types.Operator, ExportHelper):
    bl_idname = "export_objects.dgl3"
    bl_label = "Export DGL3"
    filename_ext = ".dgl3"

    filter_glob = StringProperty(default="unknown.dgl3", options={'HIDDEN'})

    @classmethod
    def poll(cls, context):
        return True

    def execute(self, context):
        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)           
        return doExport(context, filepath)

    def invoke(self, context, event):
        wm = context.window_manager
        if True:
            wm.fileselect_add(self)
            return {'RUNNING_MODAL'}
        elif True:
            wm.invoke_search_popup(self)
            return {'RUNNING_MODAL'}
        elif False:
            return wm.invoke_props_popup(self, event)
        elif False:
            return self.execute(context)

def menu_func_export_dgl(self, context):
    self.layout.operator(ExportDGLFile.bl_idname, text = "DGL3 Scene (.dgl3)")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func_export_dgl)

def unregister():
    bpy.types.INFO_MT_file_export.remove(menu_func_export_dgl)
    bpy.utils.unregister_module(__name__)

if __name__ == "__main__":
    register()

