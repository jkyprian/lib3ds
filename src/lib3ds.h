#ifndef INCLUDED_LIB3DS_H
#define INCLUDED_LIB3DS_H
/*
 * Free Animation and Modeling Project - 3ds file library
 * Copyright (C) 1996-2000 by J.E. Hoffmann <je-h@gmx.net>
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the GNU  General  Public  License  as  published by the
 * Free  Software  Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  General  Public  License
 * for more details.
 *
 * You should have received a copy of the GNU  General  Public  License along
 * with this program;  if not, write to the  Free Software Foundation,  Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: lib3ds.h,v 1.2 2000/05/18 15:35:44 jeh Exp $
 */

#if defined(_WIN32) && (!defined(_NODLLCC))
#ifdef _3DS_EXPORT
#define _CC3DS __declspec(dllexport)
#else               
#define _CC3DS __declspec(dllimport)
#endif           
#else
#define _CC3DS
#endif

#include <glibconfig.h>

typedef guint8 byte;
typedef guint16 word;
typedef guint32 dword;
typedef gint8 intb;
typedef gint16 intw;
typedef gint32 intd;

#include <cstddef>
#include <stdio.h>
#include <string>
using std::string;
#include <iosfwd>

#ifndef INCLUDED_LIST
#define INCLUDED_LIST
#include <list>
#endif
#ifndef INCLUDED_MATH3D_M3D_H
#include <math3d/m3d.h>
#endif
#ifndef INCLUDED_MATH3D_MQUAT_H
#include <math3d/mquat.h>
#endif
#ifndef INCLUDED_MATH3D_M4x4_H
#include <math3d/m4x4.h>
#endif
using namespace Math3d;

/*!
 * 3DS background shadow.
 */
class _CC3DS C3dsShadow {
  public:
    int mapSize;
    double loBias;
    double hiBias;
    double filter;
    double rayBias;
  public:
    C3dsShadow();
};

/*!
 * 3DS background bitmap.
 */
class _CC3DS C3dsBitmap {
  public:
    bool use;
    char mapName[32];
  public:
    C3dsBitmap();
};

/*!
 * 3DS solid background 
 */
class _CC3DS C3dsSolidBkgnd {
  public:
    bool use;
    double col[3];
  public:
    C3dsSolidBkgnd();
};

/*!
 * 3DS background gradient.
 */
class _CC3DS C3dsGradient {
  public:
    bool use;
    double midPct;
    double top[3];
    double middle[3];
    double bottom[3];
  public:
    C3dsGradient();
};

/*!
 * 3DS background fog.
 */
class _CC3DS C3dsFog {
  public:
    bool use;
    double col[3];
    bool fogBkgnd;
    double nearPlane;
    double nearDensity;
    double farPlane;
    double farDensity;
  public:
    C3dsFog();
};

/*!
 * 3DS background layer fog.
 */
class _CC3DS C3dsLayerFog {
  public:
   /* flags */
    static const unsigned BOTTOM_FALL_OFF =0x00000001;
    static const unsigned TOP_FALL_OFF    =0x00000002;
    static const unsigned FOG_BKGND       =0x00100000;
  public:
    bool use;
    unsigned flags;
    double col[3];
    double nearY;
    double farY;
    double density;
  public:
    C3dsLayerFog();
};

/*!
 * 3DS background distance-cue.
 */
class _CC3DS C3dsDistCue {
  public:
    bool use;
    bool cueBgnd;
    double nearPlane;
    double nearDimming;
    double farPlane;
    double farDimming;
  public:
    C3dsDistCue();
};

/*!
 * 3DS background structure
 */
class _CC3DS C3dsBkgnd {
  public:
    double ambient[3];
    C3dsShadow shadow;
    C3dsBitmap bitmap;
    C3dsSolidBkgnd solid;
    C3dsGradient gradient;
    C3dsFog fog;
    C3dsLayerFog layerFog;
    C3dsDistCue distCue;
  public:
    C3dsBkgnd();
};

/*!
 * 3DS frame settings.
 */
class _CC3DS C3dsFrames {
  public:
    int frames;
    int curframe;
    int from;
    int to;
  public:
    C3dsFrames();
};

/*!
 * 3DS texture map.
 */
class _CC3DS C3dsTexMap {
  public:
    /* flags */
    static const unsigned DECALE       =0x0001;
    static const unsigned MIRROR       =0x0002;
    static const unsigned NEGATE       =0x0004;
    static const unsigned NO_TILE      =0x0008;
    static const unsigned SUMMED_AREA  =0x0010;
    static const unsigned ALPHA_SOURCE =0x0020;
    static const unsigned TINT         =0x0040;
    static const unsigned IGNORE_ALPHA =0x0080;
    static const unsigned RGB_TINT     =0x0100;
  public:
    char mapName[32];
    unsigned flags;
    double strength;
    double blur;
    double scale[2];
    double mapOffset[2];
    double rotAngle;
    double tintCol1[4];
    double tintCol2[4];
    double tintR[4];
    double tintG[4];
    double tintB[4];
  public:
    C3dsTexMap();
};

/*!
 * 3DS autorefection map
 */
class _CC3DS C3dsAutoReflMap {
  public:
    /* flags */
    static const unsigned USE                   =0x0001;
    static const unsigned READ_FIRST_FRAME_ONLY =0x0004;
    static const unsigned FLAT_MIRROR           =0x0008;
    enum EAntiAliasLevel {
      NONE   =0,
      LOW    =1,
      MEDIUM =2,
      HIGH   =3
    };
  public:
    int mapSize;
    unsigned flags;
    EAntiAliasLevel antiAlias;
    intd frameStep;
  public:
    C3dsAutoReflMap();
};

/*!
 * 3DS material.
 */
class _CC3DS C3dsMat {
    enum EShading {
      WIRE_FRAME =0,
      FLAT       =1, 
      GOURAUD    =2, 
      PHONG      =3, 
      METAL      =4
    }; 
  public:
    void *userData;
    char matName[32];
    double ambient[3];
    double diffuse[3];
    double specular[3];
    EShading shading;
    bool phongSoft;
    bool twoSided;
    bool faceMap;
    double shininess;
    double shinStrength;
    double transparency;
    bool useTranspFallOff;
    double transpFallOff;
    bool addTransp;
    bool useReflectBlur;
    double reflectBlur;
    int selfIllumination;
    bool wireFrame;
    bool wireAbs;
    double wireSize;
    bool mapDecal;
    C3dsTexMap tex1Map;
    C3dsTexMap tex1Mask;
    C3dsTexMap tex2Map;
    C3dsTexMap tex2Mask;
    C3dsTexMap opacMap;
    C3dsTexMap opacMask;
    C3dsTexMap bumpMap;
    C3dsTexMap bumpMask;
    C3dsTexMap specMap;
    C3dsTexMap specMask;
    C3dsTexMap shinMap;
    C3dsTexMap shinMask;
    C3dsTexMap selfiMap;
    C3dsTexMap selfiMask;
    C3dsTexMap reflMap;
    C3dsTexMap reflMask;
    C3dsAutoReflMap autoReflMap;
  public:
    C3dsMat();
};

/*!
 * STL list of C3dsMat pointers.
 */
class C3dsMatList : public list<C3dsMat*> {
};

class C3dsNode;

/*!
 * STL list of C3dsNode pointers.
 */
class C3dsNodeList : public list<C3dsNode*> {
};

/*!
 * 3DS animation node.
 */
class _CC3DS C3dsNode {
  public:
    enum EType {
      NONE,
      AMBIENT,
      LIGHT,
      SPOT_LIGHT,
      CAMERA,
      TARGET,
      INSTANCE
    };
  public:
    EType ntype; 
    void *userData;
    char objName[32];
    int nodeId;
    int parentId;
    unsigned flags1;
    unsigned flags2;
    C3dsNode *pParent;
    C3dsNodeList childL;
    M4x4 matrix;
  public:
    C3dsNode();
    virtual ~C3dsNode();
    C3dsNode* findId(intd ID);
    virtual void animate(double t);

  private:
    C3dsNode(C3dsNode& nNode);
    C3dsNode& operator=(C3dsNode& nNode);
};

/*!
 * 3DS object
 */
class _CC3DS C3dsObj {
  public:
    void *userData;
    char name[32];
    bool hidden;
    bool castShad;
    bool recvShad;
    bool matte;
    bool frozen;
  public:
    C3dsObj();
};                               

/*!
 * STL list of C3dsMat pointers.
 */
class C3dsObjList : public list<C3dsObj*> {
};

/*!
 * 3DS tcb-spline parameters.
 */
class _CC3DS C3dsKeyHeader {
  public:
    /* flags */
    static const dword USE_TENSION    =0x0001;
    static const dword USE_CONTINUITY =0x0002;
    static const dword USE_BIAS       =0x0004;
    static const dword USE_EASE_TO    =0x0008;
    static const dword USE_EASE_FROM  =0x0010;
  public:
    int frame;
    unsigned flags;
    double tens;
    double cont;
    double bias;
    double easeTo;
    double easeFrom;
  public:
    C3dsKeyHeader();
};

class _CC3DS C3dsTrackHeader {
  public:
    /* flags */
    static const dword REPEAT    =0x0001;
    static const dword SMOOTH    =0x0002;
    static const dword LOCK_X    =0x0008;
    static const dword LOCK_Y    =0x0010;
    static const dword LOCK_Z    =0x0020;
    static const dword UNLINK_X  =0x0100;
    static const dword UNLINK_Y  =0x0200;
    static const dword UNLINK_Z  =0x0400;
  public:
    unsigned flags;
    int nkeys;
  public:
    C3dsTrackHeader();
};

/*!
 * Template for automatic track generation.
 */
template <class T> class C3dsTrack : public C3dsTrackHeader {
  public:
    T *keyL;
  public:
    C3dsTrack() : C3dsTrackHeader() 
    {
      keyL=NULL;
    }
    ~C3dsTrack() 
    {
      free();
    }
    void alloc(intd nkeys) 
    {
      keys=nkeys; 
      keyL=new T[nkeys];
    }
    void free() 
    {
      if (keyL) delete[] keyL;
    }
};

/*!
 * 3DS position key.
 */
class _CC3DS C3dsPosKey : public C3dsKeyHeader {
  public:
    M3d pos;
    M3d DS;
    M3d DD;
  public:
    C3dsPosKey() : C3dsKeyHeader() 
    {
    }
};

/*!
 * 3DS position track.
 */
class _CC3DS C3dsPosTrack : public C3dsTrack<C3dsPosKey> {
  public:
    void interpolate(M3d *nPos, double t);
};

/*!
 * 3DS rotation key.
 */
class _CC3DS C3dsRotKey : public C3dsKeyHeader {
  public:
    M3d axis;
    double angle;
    MQuat rot;
    MQuat DD;
    MQuat DS;
  public:
    C3dsRotKey() : C3dsKeyHeader() 
    {
      angle=0.0f;
    }
};

/*!
 * 3DS rotation track.
 */
class _CC3DS C3dsRotTrack : public C3dsTrack<C3dsRotKey> {
  public:
    void interpolate(MQuat *nRot, double t);
};

/*!
 * 3DS angle key.
 */
class _CC3DS C3dsAngleKey : public C3dsKeyHeader {
  public:
    double angle;
    double DD;
    double DS;
  public:
    C3dsAngleKey() : C3dsKeyHeader() {angle=DD=DS=0.0f;}
};

/*!
 * 3DS angle track.
 */
class _CC3DS C3dsAngleTrack : public C3dsTrack<C3dsAngleKey> {
  public:
    void interpolate(double *nAngle, double t);
};

/*!
 * 3DS color key.
 */
class _CC3DS C3dsColKey : public C3dsKeyHeader {
  public:
    double col[3];
    double DS[3];
    double DD[3];
  public:
    C3dsColKey() : C3dsKeyHeader() 
    {
      col[0]=col[1]=col[2]=0.0;
      DS[0]=DS[1]=DS[2]=0.0;
      DD[0]=DD[1]=DD[2]=0.0;
    }
};

/*!
 * 3DS color track.
 */
class _CC3DS C3dsColTrack : public C3dsTrack<C3dsColKey> {
  public:
    void interpolate(double nCol[3], double t);
};

/*!
 * 3DS morph key.
 */
class _CC3DS C3dsMorphKey : public C3dsKeyHeader {
  public:
    char morphName[32];
  public:
    C3dsMorphKey() : C3dsKeyHeader() {strcpy(morphName,"");}
};

/*!
 * 3DS morph track.
 */
class _CC3DS C3dsMorphTrack : public C3dsTrack<C3dsMorphKey> {
};

/*!
 * 3DS hide track.
 */
class _CC3DS C3dsHideTrack {
  public:
    intd keys;
    intd *keyL;
  public:
    C3dsHideTrack() {keys=0; keyL=NULL;}
};

/*!
 * 3DS ambient node.
 */
class _CC3DS C3dsAmbNode : public C3dsNode {
  public:
    C3dsColTrack colT;
    double col[3];
  public:
    C3dsAmbNode();
    virtual void animate(double t);

  private:
    C3dsAmbNode(C3dsAmbNode& nNode);
    C3dsAmbNode& operator=(C3dsAmbNode& nNode);
};

class C3dsLight;

/*!
 * 3DS light node.
 */
class _CC3DS C3dsLightNode : public C3dsNode {
  public:
    C3dsLight *light;
    C3dsPosTrack posT;
    M3d pos;
    C3dsColTrack colT;
    double col[3];
    C3dsAngleTrack rollT;
    double roll;
    C3dsAngleTrack hotSpotT;
    double hotSpot;
    C3dsAngleTrack fallOffT;
    double fallOff;
  public:
    C3dsLightNode();
    virtual void animate(double t);

  private:
    C3dsLightNode(C3dsLightNode& nNode);
    C3dsLightNode& operator=(C3dsLightNode& nNode);
};

/*!
 * 3DS spotlight node.
 */
class _CC3DS C3dsSpotNode : public C3dsNode {
  public:
    C3dsLight *light;
    C3dsPosTrack tgtT;
    M3d pos;
  public:
    C3dsSpotNode();
    virtual void animate(double t);

  private:
    C3dsSpotNode(C3dsSpotNode& nNode);
    C3dsSpotNode& operator=(C3dsSpotNode& nNode);
};

/*!
 * 3DS light object.
 */
class _CC3DS C3dsLight : public C3dsObj {
  public:
    C3dsLightNode *pLightNode;
    C3dsSpotNode *pSpotNode;
    bool spotLight;
    bool seeCone;
    double col[3];
    M3d pos;
    M3d tgt;
    double roll;
    bool off;
    double outerRange;
    double innerRange;
    double multiplier;
    list<const char*> excludeL;
    bool attenuation;
    bool rectangularSpot;
    bool shadowed;
    double shadBias;
    double shadFilter;
    int shadSize;
    double spotAspect;
    bool useProjector;
    char projector[32];
    int overShoot;
    bool raytraceShadows;
    double raytraceBias;
    double hotSpot;
    double fallOff;
  public:
    C3dsLight();
    virtual ~C3dsLight();

  private:
    C3dsLight(C3dsLight& nLight);
    C3dsLight& operator=(C3dsLight& nLight);
};

class C3dsLightList : public list<C3dsLight*> {
};

class C3dsCam;

/*!
 * 3DS camera-position node.
 */
class _CC3DS C3dsCamNode : public C3dsNode {
  public:
    C3dsCam *cam;
    C3dsPosTrack posT;
    M3d pos;
    C3dsAngleTrack rollT;
    double roll;
    C3dsAngleTrack fovT;
    double fov;
  public:
    C3dsCamNode();
    virtual void animate(double t);

  private:
    C3dsCamNode(C3dsCamNode& nNode);
    C3dsCamNode& operator=(C3dsCamNode& nNode);
};

/*!
 * 3DS camera-target node.
 */
class _CC3DS C3dsTgtNode : public C3dsNode {
  public:
    C3dsCam *cam;
    C3dsPosTrack tgtT;
    M3d pos;
  public:
    C3dsTgtNode();
    virtual void animate(double t);

  private:
    C3dsTgtNode(C3dsTgtNode& nNode);
    C3dsTgtNode& operator=(C3dsTgtNode& nNode);
};

/*!
 * 3DS camera object.
 */
class _CC3DS C3dsCam : public C3dsObj {
  public:
    C3dsCamNode *pCamNode;
    C3dsTgtNode *pTgtNode;
    int seeCone;
    M3d pos;
    M3d tgt;
    double roll;
    double fov;
    double nearRad;
    double farRad;
  public:
    C3dsCam();
    void cameraTransform(M4x4 *M, double *pFOV);
};

class C3dsCamList : public list<C3dsCam*> {
};

class C3dsInst;

/*!
 * 3DS bounding box.
 */
class _CC3DS C3dsBoundBox {
  public:
    double min[3];
    double max[3];
};

/*!
 * 3DS instance node.
 */
class _CC3DS C3dsInstNode : public C3dsNode {
  public:
    C3dsInst *pInst;
    char instName[32];
    double pivot[3];
    C3dsPosTrack posT;
    M3d pos;
    C3dsRotTrack rotT;
    MQuat rot;
    C3dsPosTrack sclT;
    M3d scl;
    C3dsMorphTrack morphT;
    C3dsHideTrack hideT;
    C3dsBoundBox boundBox;
    double morphSmoothAng;
  public:
    C3dsInstNode();
    virtual void animate(double t);

  private:
    C3dsInstNode(C3dsInstNode& nNode);
    C3dsInstNode& operator=(C3dsInstNode& nNode);
};

/*!
 * 3DS mesh object edge.
 */
class _CC3DS C3dsEdge {
  public:
    intd v;
    M3d normal;
};

class C3dsFace;

/*!
 * 3DS mesh object vertex.
 */
class _CC3DS C3dsVertex {
  public:
    M3d pos;
    intd faces;
    C3dsFace **faceL;
  public:
    C3dsVertex();
    virtual ~C3dsVertex();
};

/*!
 * 3DS mesh object face (triangle).
 */
class _CC3DS C3dsFace {
  public:
    C3dsMat *pMat;
    intd matIndex;
    unsigned smoothing;
    unsigned flags;
    M3d normal;
    C3dsEdge edgeL[3];
};

/*!
 * 3DS box-mapping descriptor.
 */
class _CC3DS C3dsBoxMap {
  public:
    char front[32];
    char back[32];
    char left[32];
    char right[32];
    char top[32];
    char bottom[32];
  public:
    C3dsBoxMap();
};

/*!
 * 3DS texture-coordinate mapping descriptor.
 */
class _CC3DS C3dsMapData {
  public:
    M3d pos;
    M4x4 matrix;
    double scale;
    double tile[2];
    double planarSize[2];
    double cylHeight;
  public:
    C3dsMapData();
};

/*!
 * 3DS tri-mesh object.
 */
class _CC3DS C3dsMesh : public C3dsObj {
  public:
    typedef double TTexel[2];
  public:
    M4x4 matrix;
    int objectCol;
    int vertices;
    C3dsVertex *vertexL;
    unsigned *flagL;
    TTexel *texelL;
    int faces;
    C3dsFace *faceL;
    C3dsBoxMap boxMap;
    C3dsMapData mapData;
  public:
    C3dsMesh();
    virtual ~C3dsMesh();
};

/*!
 * STL list of C3dsMesh pointers.
 */
class C3dsMeshList : public list<C3dsMesh*> {
};

/*!
 * 3DS tri-mesh object instance.
 */
class _CC3DS C3dsInst {
  public:
    void *userData;
    C3dsMesh *pMesh;
    C3dsInstNode *pInstNode;
  public:
    C3dsInst();
    C3dsInst(C3dsMesh *npMesh, C3dsInstNode *npInstNode);

    void objectTransform(M4x4* M);
};

/*!
 * STL list of C3dsInst pointers.
 */
class C3dsInstList : public list<C3dsInst*> {
};

/*!
 * 3D-Studio file data.
 */
class _CC3DS C3dsFile {
  public:
    double masterScale;
    M3d constrPlane;
    C3dsBkgnd bkgnd;  
    C3dsFrames frames;
    C3dsMatList matL;
    C3dsNodeList nodeL;
    C3dsLightList lightL;
    C3dsCamList camL;
    C3dsMeshList meshL;
    C3dsInstList instL;
  public:
    C3dsFile();
    ~C3dsFile();

    C3dsNode* findId(int ID);
    C3dsLight* findLight(const char *nObjName);
    C3dsCam* findCam(const char *nObjName);
    C3dsMesh* findMesh(const char *nObjName);

    void animate(double t);

  private:
    C3dsFile(const C3dsFile& nfile);
    const C3dsFile& operator=(const C3dsFile& nfile);
};

extern _CC3DS void load3DS(C3dsFile *pFile3ds, FILE *pBinFile);

#endif // INCLUDED_LIB3DS_H

