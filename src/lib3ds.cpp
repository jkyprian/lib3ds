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
 * $Id: lib3ds.cpp,v 1.3 2000/05/18 15:35:44 jeh Exp $
 */
#define _3DS_EXPORT
#include "lib3ds.h"
#include <config.h>
#include <cmath>

#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(__expr) if (!(__expr)) \
  cout << "***ASSERT*** " \
       << __FILE__ << ":" << __LINE__ << ": " \
       << #__expr << endl;
#ifndef __IN_WORK__
#define __IN_WORK__
#endif

const dword NULL_CHUNK            =0x0000;
const dword M3DMAGIC              =0x4D4D;    /*3DS file*/
const dword MLIBMAGIC             =0x3DAA;    /*MLI file*/
const dword CMAGIC                =0xC23D;    /*PRJ file*/
const dword M3D_VERSION           =0x0002;
const dword COLOR_F               =0x0010;
const dword COLOR_24              =0x0011;
const dword LIN_COLOR_24          =0x0012;
const dword LIN_COLOR_F           =0x0013;
const dword INT_PERCENTAGE        =0x0030;
const dword FLOAT_PERCENTAGE      =0x0031;
const dword MDATA                 =0x3D3D;
const dword MESH_VERSION          =0x3D3E;
const dword MASTER_SCALE          =0x0100;
const dword LO_SHADOW_BIAS        =0x1400;
const dword HI_SHADOW_BIAS        =0x1410;
const dword SHADOW_MAP_SIZE       =0x1420;
const dword SHADOW_SAMPLES        =0x1430;
const dword SHADOW_RANGE          =0x1440;
const dword SHADOW_FILTER         =0x1450;
const dword RAY_BIAS              =0x1460;
const dword O_CONSTS              =0x1500;
const dword AMBIENT_LIGHT         =0x2100;
const dword BIT_MAP               =0x1100;
const dword SOLID_BGND            =0x1200;
const dword V_GRADIENT            =0x1300;
const dword USE_BIT_MAP           =0x1101;
const dword USE_SOLID_BGND        =0x1201;
const dword USE_V_GRADIENT        =0x1301;
const dword FOG                   =0x2200;
const dword FOG_BGND              =0x2210;
const dword LAYER_FOG             =0x2302;
const dword DISTANCE_CUE          =0x2300;
const dword DCUE_BGND             =0x2310;
const dword USE_FOG               =0x2201;
const dword USE_LAYER_FOG         =0x2303;
const dword USE_DISTANCE_CUE      =0x2301;
const dword MAT_ENTRY             =0xAFFF;
const dword MAT_NAME              =0xA000;
const dword MAT_AMBIENT           =0xA010;
const dword MAT_DIFFUSE           =0xA020;
const dword MAT_SPECULAR          =0xA030;
const dword MAT_SHININESS         =0xA040;
const dword MAT_SHIN2PCT          =0xA041;
const dword MAT_TRANSPARENCY      =0xA050;
const dword MAT_XPFALL            =0xA052;
const dword MAT_USE_XPFALL        =0xA240;
const dword MAT_REFBLUR           =0xA053;
const dword MAT_SHADING           =0xA100;
const dword MAT_USE_REFBLUR       =0xA250;
const dword MAT_SELF_ILLUM        =0xA080;
const dword MAT_TWO_SIDE          =0xA081;
const dword MAT_DECAL             =0xA082;
const dword MAT_ADDITIVE          =0xA083;
const dword MAT_WIRE              =0xA085;
const dword MAT_FACEMAP           =0xA088;
const dword MAT_PHONGSOFT         =0xA08C;
const dword MAT_WIREABS           =0xA08E;
const dword MAT_WIRE_SIZE         =0xA087;
const dword MAT_TEX_MAP           =0xA200;
const dword MAT_SXP_TEXT_DATA     =0xA320;
const dword MAT_TEXMASK           =0xA33E;
const dword MAT_SXP_TEXTMASK_DATA =0xA32A;
const dword MAT_TEX2MAP           =0xA33A;
const dword MAT_SXP_TEXT2_DATA    =0xA321;
const dword MAT_TEX2MASK          =0xA340;
const dword MAT_SXP_TEXT2MASK_DATA=0xA32C;
const dword MAT_OPACMAP           =0xA210;
const dword MAT_SXP_OPAC_DATA     =0xA322;
const dword MAT_OPACMASK          =0xA342;
const dword MAT_SXP_OPACMASK_DATA =0xA32E;
const dword MAT_BUMPMAP           =0xA230;
const dword MAT_SXP_BUMP_DATA     =0xA324;
const dword MAT_BUMPMASK          =0xA344;
const dword MAT_SXP_BUMPMASK_DATA =0xA330;
const dword MAT_SPECMAP           =0xA204;
const dword MAT_SXP_SPEC_DATA     =0xA325;
const dword MAT_SPECMASK          =0xA348;
const dword MAT_SXP_SPECMASK_DATA =0xA332;
const dword MAT_SHINMAP           =0xA33C;
const dword MAT_SXP_SHIN_DATA     =0xA326;
const dword MAT_SHINMASK          =0xA346;
const dword MAT_SXP_SHINMASK_DATA =0xA334;
const dword MAT_SELFIMAP          =0xA33D;
const dword MAT_SXP_SELFI_DATA    =0xA328;
const dword MAT_SELFIMASK         =0xA34A;
const dword MAT_SXP_SELFIMASK_DATA=0xA336;
const dword MAT_REFLMAP           =0xA220;
const dword MAT_REFLMASK          =0xA34C;
const dword MAT_SXP_REFLMASK_DATA =0xA338;
const dword MAT_ACUBIC            =0xA310;
const dword MAT_MAPNAME           =0xA300;
const dword MAT_MAP_TILING        =0xA351;
const dword MAT_MAP_TEXBLUR       =0xA353;
const dword MAT_MAP_USCALE        =0xA354;
const dword MAT_MAP_VSCALE        =0xA356;
const dword MAT_MAP_UOFFSET       =0xA358;
const dword MAT_MAP_VOFFSET       =0xA35A;
const dword MAT_MAP_ANG           =0xA35C;
const dword MAT_MAP_COL1          =0xA360;
const dword MAT_MAP_COL2          =0xA362;
const dword MAT_MAP_RCOL          =0xA364;
const dword MAT_MAP_GCOL          =0xA366;
const dword MAT_MAP_BCOL          =0xA368;
const dword NAMED_OBJECT          =0x4000;
const dword N_DIRECT_LIGHT        =0x4600;
const dword DL_OFF                =0x4620;
const dword DL_OUTER_RANGE        =0x465A;
const dword DL_INNER_RANGE        =0x4659;
const dword DL_MULTIPLIER         =0x465B;
const dword DL_EXCLUDE            =0x4654;
const dword DL_ATTENUATE          =0x4625;
const dword DL_SPOTLIGHT          =0x4610;
const dword DL_SPOT_ROLL          =0x4656;
const dword DL_SHADOWED           =0x4630;
const dword DL_LOCAL_SHADOW2      =0x4641;
const dword DL_SEE_CONE           =0x4650;
const dword DL_SPOT_RECTANGULAR   =0x4651;
const dword DL_SPOT_ASPECT        =0x4657;
const dword DL_SPOT_PROJECTOR     =0x4653;
const dword DL_SPOT_OVERSHOOT     =0x4652;
const dword DL_RAY_BIAS           =0x4658;
const dword DL_RAYSHAD            =0x4627;
const dword N_CAMERA              =0x4700;
const dword CAM_SEE_CONE          =0x4710;
const dword CAM_RANGES            =0x4720;
const dword OBJ_HIDDEN            =0x4010;
const dword OBJ_VIS_LOFTER        =0x4011;
const dword OBJ_DOESNT_CAST       =0x4012; 
const dword OBJ_DONT_RECVSHADOW   =0x4017;
const dword OBJ_MATTE             =0x4013;
const dword OBJ_FAST              =0x4014;
const dword OBJ_PROCEDURAL        =0x4015;
const dword OBJ_FROZEN            =0x4016;
const dword N_TRI_OBJECT          =0x4100;
const dword POINT_ARRAY           =0x4110;
const dword POINT_FLAG_ARRAY      =0x4111;
const dword FACE_ARRAY            =0x4120;
const dword MSH_MAT_GROUP         =0x4130;
const dword SMOOTH_GROUP          =0x4150;
const dword MSH_BOXMAP            =0x4190;
const dword TEX_VERTS             =0x4140;
const dword MESH_MATRIX           =0x4160;
const dword MESH_COLOR            =0x4165;
const dword MESH_TEXTURE_INFO     =0x4170;
const dword KFDATA                =0xB000;
const dword KFHDR                 =0xB00A;
const dword KFSEG                 =0xB008;
const dword KFCURTIME             =0xB009;
const dword AMBIENT_NODE_TAG      =0xB001;
const dword OBJECT_NODE_TAG       =0xB002;
const dword CAMERA_NODE_TAG       =0xB003;
const dword TARGET_NODE_TAG       =0xB004;
const dword LIGHT_NODE_TAG        =0xB005;
const dword L_TARGET_NODE_TAG     =0xB006;
const dword SPOTLIGHT_NODE_TAG    =0xB007;
const dword NODE_ID               =0xB030;
const dword NODE_HDR              =0xB010;
const dword PIVOT                 =0xB013;
const dword INSTANCE_NAME         =0xB011;
const dword MORPH_SMOOTH          =0xB015;
const dword BOUNDBOX              =0xB014;
const dword POS_TRACK_TAG         =0xB020;
const dword COL_TRACK_TAG         =0xB025;
const dword ROT_TRACK_TAG         =0xB021;
const dword SCL_TRACK_TAG         =0xB022;
const dword MORPH_TRACK_TAG       =0xB026;
const dword FOV_TRACK_TAG         =0xB023;
const dword ROLL_TRACK_TAG        =0xB024;
const dword HOT_TRACK_TAG         =0xB027;
const dword FALL_TRACK_TAG        =0xB028;
const dword HIDE_TRACK_TAG        =0xB029;

#define __BIG_ENDIAN__

/*--WORD & DWORD swapping routines for little/big endian conversations--*/
#ifdef WORDS_BIGENDIAN 
#define SWAPWORD(w) w=((((w)<<8)&0xFF00)|(((w)>>8)&0xFF));
#define SWAPDWORD(d) d=((((d)>>24)&0xFF)|(((d)>>8)&0xFF00)|(((d)<<8)&0xFF0000)|(((d)<<24)&0xFF000000));
#else 
#define SWAPWORD(w)   
#define SWAPDWORD(d) 
#endif

#define PTR_CAST(_type, _x) (*((_type*)&_x))


/*!
 *
 */
C3dsShadow::C3dsShadow()
{
  loBias=0.0f;
  hiBias=0.0f;
  mapSize=512;
  filter=1.0f;
  rayBias=0.0f;
}


/*!
 *
 */
C3dsBitmap::C3dsBitmap()
{
  use=FALSE;
  strcpy(mapName,"");
}


/*!
 *
 */
C3dsSolidBkgnd::C3dsSolidBkgnd()
{
  use=FALSE;
  col[0]=col[1]=col[2]=0.0f;
}


/*!
 *
 */
C3dsGradient::C3dsGradient() 
{
  int i;
  for (i=0; i<3; i++) {
    top[i]=0.0f;
    middle[i]=0.0f;
    bottom[i]=0.0f;
  };
  use=FALSE;
  midPct=0.0f;
}


/*!
 *
 */
C3dsFog::C3dsFog() 
{
  use=FALSE;
  col[0]=col[1]=col[2]=0.0f;
  fogBkgnd=FALSE;
  nearPlane=0.0f;
  nearDensity=0.0f;
  farPlane=10000.0f;
  farDensity=1.0f;
}


/*!
 *
 */
C3dsLayerFog::C3dsLayerFog() 
{
  use=0;
  flags=0;
  nearY=0.0f;
  farY=10000.0f;
  density=0.5f;
};


/*!
 *
 */
C3dsDistCue::C3dsDistCue() 
{
  use=FALSE;
  cueBgnd=FALSE;
  nearPlane=0.0f;
  nearDimming=0.0f;
  farPlane=10000.0f;
  farDimming=1.0f;
}


/*!
 *
 */
C3dsBkgnd::C3dsBkgnd() 
{
  ambient[0]=ambient[1]=ambient[2]=0.0f;
}


/*!
 *
 */
C3dsFrames::C3dsFrames()
{
  frames=0;
  curframe=0;
  from=0;
  to=0;
}


/*!
 *
 */
C3dsTexMap::C3dsTexMap()
{
  strcpy(mapName, "");
  flags=0;
  strength=1.0f;
  blur=1.0f;
  scale[0]=scale[1]=1.0f;
  mapOffset[0]=mapOffset[1]=0.0f;
  rotAngle=0.0f;

  tintCol1[0]=tintCol1[1]=tintCol1[2]=tintCol1[3]=0.0;
  tintCol2[0]=tintCol2[1]=tintCol2[2]=tintCol2[3]=0.0;
  tintR[0]=tintR[1]=tintR[2]=tintR[3]=0.0;
  tintG[0]=tintG[1]=tintG[2]=tintG[3]=0.0;
  tintB[0]=tintB[1]=tintB[2]=tintB[3]=0.0;
}


/*!
 *
 */
C3dsAutoReflMap::C3dsAutoReflMap()
{
  mapSize=512;
  flags=0;
  antiAlias=NONE;
  frameStep=1;
}


/*!
 *
 */
C3dsMat::C3dsMat() 
{
  userData=NULL;
  strcpy(matName,"");
  int i;
  for (i=0; i<3; i++) {
    ambient[i]=0.0f;
    diffuse[i]=0.0f;
    specular[i]=0.0f;
  }
  shading=PHONG;
  twoSided=FALSE;
  phongSoft=FALSE;
  faceMap=FALSE;
  shininess=1.0f;
  shinStrength=1.0f;
  transparency=1.0f;
  useTranspFallOff=FALSE;
  transpFallOff=1.0f;
  addTransp=FALSE;
  useReflectBlur=FALSE;
  reflectBlur=1.0f;
  wireFrame=FALSE;
  wireAbs=FALSE;
  wireSize=1.0f;
  selfIllumination=FALSE;
  mapDecal=FALSE;
}



/*!
 *
 */
C3dsNode::C3dsNode()
{
  ntype=NONE;
  userData=NULL;
  nodeId=0;
  strcpy(objName,"");
  flags1=flags2=0;
  parentId=-1;
  pParent=NULL;
}


/*!
 *
 */
C3dsNode::~C3dsNode() 
{
  {
    C3dsNode *p;
    while (!childL.empty()) {
      p=childL.front();
      childL.pop_front();
      delete p;
    }
  }
}


/*!
 *
 */
C3dsNode* C3dsNode::findId(intd ID)
{
  C3dsNodeList::iterator p;
  C3dsNode *pn;

  if (nodeId==ID) return(this);
  for (p=childL.begin(); p!=childL.end(); p++) {
    if ((pn=(*p)->findId(ID))!=NULL) return(pn);
  }
  return(NULL);
}


/*!
 *
 */
void C3dsNode::animate(double t)
{
  C3dsNodeList::iterator p;

  for (p=childL.begin(); p!=childL.end(); p++) {
    (*p)->animate(t);
  }
}


/*!
 *
 */
C3dsObj::C3dsObj()
{
  userData=0;
  strcpy(name,"");
  hidden=FALSE;
  castShad=TRUE;
  recvShad=TRUE;
  matte=FALSE;
  frozen=FALSE;
}


/*!
 *
 */
C3dsKeyHeader::C3dsKeyHeader()
{
  frame=0;
  flags=0;
  tens=cont=bias=easeTo=easeFrom=0.0f;
};


/*!
 *
 */
C3dsTrackHeader::C3dsTrackHeader() 
{ 
  flags=0; 
  nkeys=0;
}


/*!
 *
 */
void C3dsPosTrack::interpolate(M3d *nPos, double t)
{
  int i;
  double nt;
  double u;

  if (nkeys) {
    for (i=0; i<nkeys-1; i++) {
      if (t>=keyL[i].frame && t<keyL[i+1].frame) break;
    }
    if (i>=nkeys-1) {
      if (flags&C3dsTrackHeader::REPEAT) {
        nt=fmod(t,keyL[i].frame);
        for (i=0; i<nkeys-1; i++) {
          if (nt>=keyL[i].frame && nt<keyL[i+1].frame) break;
        }
        ASSERT(i<nkeys-1);
      }
      else {
        *nPos=keyL[nkeys-1].pos;
        return;
      }
    }
    else {
      nt=t;
    }
    u=nt-keyL[i].frame;
    u/=(keyL[i+1].frame-keyL[i].frame);

    nPos->cubic(
      keyL[i].pos,
      keyL[i].DD,
      keyL[i+1].DS,
      keyL[i+1].pos,
      u
    );
  }
  else {
    nPos->zero();
  }
}


/*!
 *
 */
void C3dsRotTrack::interpolate(MQuat *nRot, double t)
{
  int i;
  double u;
  double nt;
  
  if (nkeys) {
    for (i=0; i<nkeys-1; i++) {
      if (t>=keyL[i].frame && t<keyL[i+1].frame) break;
    }
    if (i>=nkeys-1) {
      if (flags&C3dsTrackHeader::REPEAT) {
        nt=fmod(t,keyL[i].frame);
        for (i=0; i<nkeys-1; i++) {
          if (nt>=keyL[i].frame && nt<keyL[i+1].frame) break;
        }
        ASSERT(i<nkeys-1);
      }
      else {
        *nRot=keyL[nkeys-1].rot;
        return;
      }
    }
    else {
      nt=t;
    }
    u=nt-keyL[i].frame;
    u/=(keyL[i+1].frame-keyL[i].frame);
    nRot->squad(
      keyL[i].rot,
      keyL[i].DD,
      keyL[i+1].DS,
      keyL[i+1].rot,
      u
    );
  }
  else {
    nRot->identity();
  }
}


static double fCubicInterpolate(double A, double Ta, double Tb, double B, double t)
{
  double a,b,c,d;   

  a=2*t*t*t - 3*t*t + 1;
  b=-2*t*t*t + 3*t*t;
  c=t*t*t - 2*t*t + t;
  d=t*t*t - t*t;
  return(a*A + b*B + c*Ta + d*Tb);
}


/*!
 *
 */
void C3dsAngleTrack::interpolate(double *nAngle, double t)
{
  int i;
  double u;
  
  if (nkeys) {
    for (i=0; i<nkeys-1; i++) {
      if (t>=keyL[i].frame && t<keyL[i+1].frame) break;
    }
    if (i<nkeys-1) {
      u=t-keyL[i].frame;
      u/=(keyL[i+1].frame-keyL[i].frame);

      *nAngle=fCubicInterpolate(
        keyL[i].angle,
        keyL[i].DD,
        keyL[i+1].DS,
        keyL[i+1].angle,
        u
      );
    }
    else {
      *nAngle=keyL[nkeys-1].angle;
    }
  }
  else {
    *nAngle=0.0;
  }
}


/*!
 *
 */
void C3dsColTrack::interpolate(double nCol[3], double t)
{
  int i;
  double nt;
  double u;

  if (nkeys) {
    for (i=0; i<nkeys-1; i++) {
      if (t>=keyL[i].frame && t<keyL[i+1].frame) break;
    }
    if (i>=nkeys-1) {
      if (flags&C3dsTrackHeader::REPEAT) {
        nt=fmod(t,keyL[i].frame);
        for (i=0; i<nkeys-1; i++) {
          if (nt>=keyL[i].frame && nt<keyL[i+1].frame) break;
        }
        ASSERT(i<nkeys-1);
      }
      else {
        nCol[0]=keyL[nkeys-1].col[0];
        nCol[1]=keyL[nkeys-1].col[1];
        nCol[2]=keyL[nkeys-1].col[2];
        return;
      }
    }
    else {
      nt=t;
    }
    u=nt-keyL[i].frame;
    u/=(keyL[i+1].frame-keyL[i].frame);

    {
      float a=2*u*u*u - 3*u*u + 1;
      float b=-2*u*u*u + 3*u*u;
      float c=u*u*u - 2*u*u + u;
      float d=u*u*u - u*u;
      int j;
      for (j=0; j<3; j++) {
        nCol[j]=
          a*keyL[i].col[j] +
          b*keyL[i+1].col[j] + 
          c*keyL[i].DD[j] + 
          d*keyL[i+1].DS[j];
      }
    }
  }
  else {
    nCol[0]=nCol[1]=nCol[2]=0.0;
  }
}


/*!
 *
 */
C3dsAmbNode::C3dsAmbNode() 
{
  ntype=AMBIENT;
  col[0]=col[1]=col[2]=0;
}


/*!
 *
 */
void C3dsAmbNode::animate(double t)
{
  colT.interpolate(col,t);
  matrix.identity();
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsLightNode::C3dsLightNode() 
{
  ntype=LIGHT;
  light=NULL;
}


/*!
 *
 */
void C3dsLightNode::animate(double t)
{
  posT.interpolate(&pos,t);
  colT.interpolate(col,t);
  rollT.interpolate(&roll,t);
  hotSpotT.interpolate(&hotSpot,t);
  fallOffT.interpolate(&fallOff,t);
  if (pParent) {
    matrix=pParent->matrix;
  }
  else {
    matrix.identity();
  }
  matrix.translate(pos);
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsSpotNode::C3dsSpotNode() : C3dsNode(), tgtT()
{
  ntype=SPOT_LIGHT;
  light=NULL;
}


/*!
 *
 */
void C3dsSpotNode::animate(double t)
{
  tgtT.interpolate(&pos,t);
  if (pParent) {
    matrix=pParent->matrix;
  }
  else {
    matrix.identity();
  }
  matrix.translate(pos);
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsLight::C3dsLight()
{
  pLightNode=NULL;
  pSpotNode=NULL;
  spotLight=FALSE;
  seeCone=FALSE;
  int i;
  for (i=0; i<3; i++) col[i]=0.0;
  roll=0.0;
  off=FALSE;
  outerRange=0.0f;
  innerRange=0.0f;
  multiplier=1.0f;
  attenuation=FALSE;
  hotSpot=RAD(45);
  fallOff=RAD(60);
  shadowed=FALSE;
  shadBias=0.0f;
  shadFilter=1.0f;
  shadSize=512;
  rectangularSpot=FALSE;
  useProjector=FALSE;
  overShoot=FALSE;
  strcpy(projector,"");
  raytraceShadows=FALSE;
  raytraceBias=0.0f;
}


/*!
 *
 */
C3dsLight::~C3dsLight() 
{
  list<const char*>::iterator p;
  for (p=excludeL.begin(); p!=excludeL.end(); p++) {
    delete (*p);
  }
  excludeL.clear();
}


/*!
 *
 */
C3dsCamNode::C3dsCamNode()
{
  ntype=CAMERA;
  cam=NULL;
}


/*!
 *
 */
void C3dsCamNode::animate(double t)
{
  posT.interpolate(&pos, t);
  rollT.interpolate(&roll, t);
  fovT.interpolate(&fov, t);
  if (pParent) {
    matrix=pParent->matrix;
  }
  else {
    matrix.identity();
  }
  matrix.translate(pos);
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsTgtNode::C3dsTgtNode()
{
  ntype=TARGET;
  cam=NULL;
}


/*!
 *
 */
void C3dsTgtNode::animate(double t)
{
  tgtT.interpolate(&pos,t);
  if (pParent) {
    matrix=pParent->matrix;
  }
  else {
    matrix.identity();
  }
  matrix.translate(pos);
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsCam::C3dsCam() : C3dsObj()
{    
  pCamNode=NULL;
  pTgtNode=NULL;
  seeCone=FALSE;
  roll=0.0f;
  fov=RAD(48);
  nearRad=0.0f;
  farRad=0.0f;
}


/*!
 *
 */
void C3dsCam::cameraTransform(M4x4 *M, double *pFOV) 
{    
  M3d dir;
  double az,ax;
  M3d campos;

  if (pCamNode && pTgtNode) {
    *pFOV=pCamNode->fov;
    dir.sub(pTgtNode->pos,pCamNode->pos);
    dir.normalize();
    az=atan2(dir[0],dir[1]);
    ax=-asin(dir[2]);
    M->rotateY(-pCamNode->roll);
    M->rotateX(ax);
    M->rotateZ(az);
    campos=pCamNode->pos;
    campos.neg();
    M->translate(campos);
  }
  else {
    M->identity();
    *pFOV=RAD(45);
    ASSERT(FALSE);
  }
}


/*!
 *
 */
C3dsInstNode::C3dsInstNode() 
{
  ntype=INSTANCE;
  pInst=NULL;
  strcpy(instName,"");
  morphSmoothAng=RAD(45);
}


/*!
 *
 */
void C3dsInstNode::animate(double t)
{
  M4x4 M;
  
  posT.interpolate(&pos,t);
  rotT.interpolate(&rot,t);
  sclT.interpolate(&scl,t);

  M.identity();
  M.translate(pos);
  M.rotate(rot);
  M.scale(scl);

  if (pParent) {
    matrix.mul(pParent->matrix,M);
  }
  else {
    matrix=M;
  }
  C3dsNode::animate(t);
}


/*!
 *
 */
C3dsBoxMap::C3dsBoxMap()
{
  strcpy(front, "");
  strcpy(back, "");
  strcpy(left, "");
  strcpy(right, "");
  strcpy(top, "");
  strcpy(bottom, "");
}


/*!
 *
 */
C3dsMapData::C3dsMapData()
{
  matrix.identity();
  scale=1.0f;
  tile[0]=tile[1]=1.0f;
  planarSize[0]=planarSize[1]=1.0f;
  cylHeight=1.0f;
}


/*!
 *
 */
C3dsVertex::C3dsVertex()
{
  faces=0;
  faceL=NULL;
}


/*!
 *
 */
C3dsVertex::~C3dsVertex()
{
  if (faceL) {
    delete[] faceL;
    faceL=NULL;
  }
}


/*!
 *
 */
C3dsMesh::C3dsMesh()
{
  vertices=faces=0;
  vertexL=NULL;
  flagL=NULL;
  texelL=0;
  faceL=NULL;
}


/*!
 *
 */
C3dsMesh::~C3dsMesh()
{
  if (vertexL) delete[] vertexL;
  if (flagL) delete[] flagL;
  if (texelL) delete[] texelL;
  if (faceL) delete[] faceL;
}


/*!
 *
 */
C3dsInst::C3dsInst()
{
  userData=NULL;
  pMesh=NULL;
  pInstNode=NULL;
}


/*!
 *
 */
C3dsInst::C3dsInst(C3dsMesh *npMesh, C3dsInstNode *npInstNode)
{
  userData=NULL;
  pMesh=npMesh;
  pInstNode=npInstNode;
}


/*!
 *
 */
void C3dsInst::objectTransform(M4x4 *M)
{
  if (pInstNode) {
    *M=pInstNode->matrix;
  }
  else {
    M->identity();
  }
}


/*!
 *
 */
C3dsFile::C3dsFile()
{
  masterScale=1.0f;
}


/*!
 *
 */
C3dsFile::~C3dsFile()
{
  {
    C3dsMat *p;
    while (!matL.empty()) {
      p=matL.front();
      matL.pop_front();
      delete p;
    }
  }
  {
    C3dsCam *p;
    while (!camL.empty()) {
      p=camL.front();
      camL.pop_front();
      delete p;
    }
  }
  {
    C3dsLight *p;
    while (!lightL.empty()) {
      p=lightL.front();
      lightL.pop_front();
      delete p;
    }
  }
  {
    C3dsMesh *p;
    while (!meshL.empty()) {
      p=meshL.front();
      meshL.pop_front();
      delete p;
    }
  }
  {
    C3dsNode *p;
    while (!nodeL.empty()) {
      p=nodeL.front();
      nodeL.pop_front();
      delete p;
    }
  }
  {
    C3dsInst *p;
    while (!instL.empty()) {
      p=instL.front();
      instL.pop_front();
      delete p;
    }
  }
}


/*!
 *
 */
C3dsNode* C3dsFile::findId(intd ID)
{
  C3dsNodeList::iterator p;
  C3dsNode *pn;

  for (p=nodeL.begin(); p!=nodeL.end(); p++) {
    if ((pn=(*p)->findId(ID))!=NULL) return(pn);
  }
  return(NULL);
}


/*!
 *
 */
C3dsLight* C3dsFile::findLight(const char *nObjName) 
{
  C3dsLightList::iterator p;

  for (p=lightL.begin(); p!=lightL.end(); p++) {
    if (strcmp((*p)->name,nObjName)==0) break;
  }
  return(*p);
}


/*!
 *
 */
C3dsCam* C3dsFile::findCam(const char *nObjName)
{
  C3dsCamList::iterator p;

  for (p=camL.begin(); p!=camL.end(); p++) {
    if (strcmp((*p)->name,nObjName)==0) break;
  }
  return(*p);
}


/*!
 *
 */
C3dsMesh* C3dsFile::findMesh(const char *nObjName)
{
  C3dsMeshList::iterator p;

  for (p=meshL.begin(); p!=meshL.end(); p++) {
    if (strcmp((*p)->name,nObjName)==0) break;
  }
  return(*p);
}


/*!
 *
 */
void C3dsFile::animate(double t)
{
  C3dsNodeList::iterator p;

  for (p=nodeL.begin(); p!=nodeL.end(); p++) {
    (*p)->animate(t);
  }
}


/*
########################################################################################
########################################################################################
########################################################################################
########################################################################################
########################################################################################
########################################################################################
########################################################################################
########################################################################################
########################################################################################
*/

#define _DMPDEBUG


class C3dsChunk;

typedef void (*LChunkHandler)(C3dsChunk& c);

typedef struct {
  unsigned chunk;
  unsigned parent;
  char *name;
  LChunkHandler HandProc;
} TChunkHandler;

class C3dsChunk {
  private:
    FILE *f;
    intd level;
  public:
    dword parent;
    dword chunk;
    intd pos;
    intd size;
    C3dsFile *file3ds;
    C3dsMat *pMat;
    C3dsTexMap *pMap;
    double *pPct;
    intd vGcol;
    char objname[32];
    C3dsObj *pObj;
    C3dsLight *pLight;
    C3dsCam *pCam;
    C3dsMesh *pMesh;
    C3dsNode *pNode;
    C3dsAmbNode *pNdAmb;
    C3dsTgtNode *pNdTgt;
    C3dsLightNode *pNdLight;
    C3dsSpotNode *pNdSpot;
    C3dsCamNode *pNdCam;
    C3dsInstNode *pNdInst;
  public:  
    C3dsChunk(FILE *nf, C3dsFile *nFile3ds)
    {
      f=nf;
      level=0;
      file3ds=nFile3ds;
      pMat=NULL;
      pMap=NULL;
      pPct=NULL;
      vGcol=0;
      strcpy(objname,"");
      pObj=NULL;
      pLight=NULL;
      pCam=NULL;
      pMesh=NULL;
      pNode=NULL;
      pNdAmb=NULL;
      pNdInst=NULL;
      pNdLight=NULL;
      pNdCam=NULL;
      pNdTgt=NULL;
      pNdSpot=NULL;
    }

    ~C3dsChunk()
    {
    }

  protected:
    TChunkHandler* findHandler();

  public:
    intd handle(dword nparent)
    {
      TChunkHandler *handler;

      parent=nparent;
      pos=ftell(f);
      chunk=getword();
      size=getdword();
      handler=findHandler();
      
      #ifdef _DEBUG 
      {
        char dmp[256];
        char s[256];
        int i;
        for (i=0; i<4*level; i++) dmp[i]=' ';
        dmp[i]=0;
        if (handler) {
          sprintf(s,"%s[0x%X, size=0x%4X] %s",dmp,chunk,size,handler->name);
        }
        else {
          sprintf(s,"%s[0x%4X, size=0x%4X] %s",dmp,chunk,size,"UNKNOWN");
        }
        strcat(s,"\n");
        printf(s);
      }
      #endif

      if (handler==NULL && size<6) {
        cout << "[WARNING]: Bad 3DS subchunk found in " 
             << parent << " at " << pos << endl;
        return(0);
      }
      if (handler!=NULL) {
        (*handler->HandProc)(*this);
      }
      fseek(f,pos+size,SEEK_SET);
      return(size);
    }

    void handleSubChunks(intd psize)
    {
      intd used;
      intd hused;
      
      used=psize;
      while (used<size) {
        fseek(f,pos+used,SEEK_SET);
        C3dsChunk nc(*this);
        nc.level++;
        hused=nc.handle(chunk);
        if (!hused) {
          used=size;
          break;
        }
        used+=hused;
      }
      ASSERT(used==size);
    }

    double getfloat()
    {
      unsigned d=0;
      fread(&d,1,4,f);
      SWAPDWORD(d);        
      return(PTR_CAST(float,d));
    }

    unsigned char getbyte()
    {
      return(fgetc(f));
    }

    unsigned short getword()
    {
      unsigned short d=0;
      fread(&d,1,2,f);
      SWAPWORD(d);        
      return(d);
    }

    unsigned getdword()
    {
      unsigned d=0;
      fread(&d,1,4,f);
      SWAPDWORD(d);        
      return(d);         
    }

    short getintw()
    {
      unsigned short d=0;
      fread(&d,1,2,f);
      SWAPWORD(d);        
      return((short)d);
    }

    int getintd()
    {
      unsigned d=0;
      fread(&d,1,4,f);
      SWAPDWORD(d);        
      return((int)d);         
    }

    int getstr(char *str)
    {
      char ch;
      int size=0;
  
      do {
        ch=fgetc(f);
        *str++=ch;
        size++;
      } while (ch);
      return(size-1);
    }

    void fgetRgb(double col[3])
    {
      col[0]=getfloat();
      col[1]=getfloat();
      col[2]=getfloat();
    }

  private:
    void* operator->();
};

const int CHK_HEADER_SIZE=6;
#define DECLARE_CHUNK_HANDLER(chunk,parent)\
  {chunk,parent,#chunk,HANDLE_##chunk##_##parent##},
#define IMPLEMENT_CHUNK_HANDLER(chunk,parent)\
  static void HANDLE_##chunk##_##parent##(C3dsChunk& c)


#define RED   0
#define GREEN 1
#define BLUE  2
#define ALPHA 3


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(M3DMAGIC, NULL_CHUNK)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MLIBMAGIC, NULL_CHUNK)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(CMAGIC, NULL_CHUNK)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MDATA, NULL_CHUNK)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MASTER_SCALE, MDATA)
/*==========================================================================*/
{
  c.file3ds->masterScale=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(LO_SHADOW_BIAS, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.shadow.loBias=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(HI_SHADOW_BIAS, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.shadow.hiBias=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SHADOW_MAP_SIZE, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.shadow.mapSize=c.getintw();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SHADOW_FILTER, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.shadow.filter=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(RAY_BIAS, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.shadow.rayBias=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(O_CONSTS, MDATA)
/*==========================================================================*/
{
  c.file3ds->constrPlane[0]=c.getfloat();
  c.file3ds->constrPlane[1]=c.getfloat();
  c.file3ds->constrPlane[1]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(AMBIENT_LIGHT, MDATA)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, AMBIENT_LIGHT)
/*==========================================================================*/
{
  c.file3ds->bkgnd.ambient[RED]=c.getfloat();
  c.file3ds->bkgnd.ambient[GREEN]=c.getfloat();
  c.file3ds->bkgnd.ambient[BLUE]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(BIT_MAP, MDATA)
/*==========================================================================*/
{
  c.getstr(c.file3ds->bkgnd.bitmap.mapName);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SOLID_BGND, MDATA)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, SOLID_BGND)
/*==========================================================================*/
{
  c.file3ds->bkgnd.solid.col[RED]=c.getfloat();
  c.file3ds->bkgnd.solid.col[GREEN]=c.getfloat();
  c.file3ds->bkgnd.solid.col[BLUE]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(V_GRADIENT, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.gradient.midPct=c.getfloat();
  c.vGcol=0;
  c.handleSubChunks(CHK_HEADER_SIZE+4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, V_GRADIENT)
/*==========================================================================*/
{
  switch(c.vGcol) {
    case 0:
      c.fgetRgb(c.file3ds->bkgnd.gradient.top);
      break;
    case 1:
      c.fgetRgb(c.file3ds->bkgnd.gradient.middle);
      break;
    case 2:
      c.fgetRgb(c.file3ds->bkgnd.gradient.bottom);
      break;
  }
  c.vGcol++;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_BIT_MAP, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.bitmap.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_SOLID_BGND, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.solid.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_V_GRADIENT, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.gradient.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(FOG, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.fog.nearPlane=c.getfloat();
  c.file3ds->bkgnd.fog.nearDensity=c.getfloat();
  c.file3ds->bkgnd.fog.farPlane=c.getfloat();
  c.file3ds->bkgnd.fog.farDensity=c.getfloat();
  c.handleSubChunks(CHK_HEADER_SIZE+4*4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, FOG)
/*==========================================================================*/
{
  c.file3ds->bkgnd.fog.col[RED]=c.getfloat();
  c.file3ds->bkgnd.fog.col[GREEN]=c.getfloat();
  c.file3ds->bkgnd.fog.col[BLUE]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(FOG_BGND, FOG)
/*==========================================================================*/
{
  c.file3ds->bkgnd.fog.fogBkgnd=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(LAYER_FOG, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.layerFog.nearY=c.getfloat();
  c.file3ds->bkgnd.layerFog.farY=c.getfloat();
  c.file3ds->bkgnd.layerFog.density=c.getfloat();
  c.file3ds->bkgnd.layerFog.flags=c.getdword();
  c.handleSubChunks(CHK_HEADER_SIZE+4*4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, LAYER_FOG)
/*==========================================================================*/
{
  c.file3ds->bkgnd.layerFog.col[RED]=c.getfloat();
  c.file3ds->bkgnd.layerFog.col[GREEN]=c.getfloat();
  c.file3ds->bkgnd.layerFog.col[BLUE]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DISTANCE_CUE, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.distCue.nearPlane=c.getfloat();
  c.file3ds->bkgnd.distCue.nearDimming=c.getfloat();
  c.file3ds->bkgnd.distCue.farPlane=c.getfloat();
  c.file3ds->bkgnd.distCue.farDimming=c.getfloat();
  c.handleSubChunks(CHK_HEADER_SIZE+4*4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DCUE_BGND, DISTANCE_CUE)
/*==========================================================================*/
{
  c.file3ds->bkgnd.distCue.cueBgnd=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_FOG, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.fog.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_LAYER_FOG, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.layerFog.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(USE_DISTANCE_CUE, MDATA)
/*==========================================================================*/
{
  c.file3ds->bkgnd.distCue.use=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_ENTRY, MDATA)
/*==========================================================================*/
{
  c.pMat=new C3dsMat();
  c.file3ds->matL.push_back(c.pMat);
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_NAME, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.getstr(c.pMat->matName);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(INT_PERCENTAGE, NULL_CHUNK)
/*==========================================================================*/
{
  double pct;
  
  ASSERT(c.pMat);
  pct=((float)c.getword())/100.0f;
  if (c.pMap) {
    c.pMap->strength=pct;
    return;
  }
  ASSERT(c.pPct);
  if (c.pPct) {
    *c.pPct=pct;
    return;
  }
}


/*==========================================================================*/
static void Load3dsRgbColor(C3dsChunk& c, double col[3])
/*==========================================================================*/
{
  col[0]=c.getbyte()/255.0;
  col[1]=c.getbyte()/255.0;
  col[2]=c.getbyte()/255.0;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_AMBIENT, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_24, MAT_AMBIENT)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  Load3dsRgbColor(c, c.pMat->ambient);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_DIFFUSE, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_24, MAT_DIFFUSE)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  Load3dsRgbColor(c, c.pMat->diffuse);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SPECULAR, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_24, MAT_SPECULAR)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  Load3dsRgbColor(c, c.pMat->specular);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SHININESS, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pPct=&c.pMat->shininess;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pPct=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SHIN2PCT, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pPct=&c.pMat->shinStrength;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pPct=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TRANSPARENCY, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pPct=&c.pMat->transparency;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pPct=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_XPFALL, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pPct=&c.pMat->transpFallOff;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pPct=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_USE_XPFALL, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->useTranspFallOff=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_REFBLUR, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pPct=&c.pMat->reflectBlur;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pPct=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SHADING, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->shading=(C3dsMat::EShading)c.getintw();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_USE_REFBLUR, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->useReflectBlur=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SELF_ILLUM, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->selfIllumination=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TWO_SIDE, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->twoSided=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_DECAL, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->mapDecal=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_ADDITIVE, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->addTransp=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_WIRE, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->wireFrame=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_FACEMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->faceMap=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_PHONGSOFT, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->phongSoft=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_WIREABS, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->wireAbs=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_WIRE_SIZE, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMat->wireSize=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TEX_MAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->tex1Map;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TEXMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->tex1Mask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TEX2MAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->tex2Map;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_TEX2MASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->tex2Mask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_OPACMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->opacMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_OPACMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->opacMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_BUMPMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->bumpMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_BUMPMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->bumpMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SPECMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->specMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SPECMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->specMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SHINMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->shinMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SHINMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->shinMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SELFIMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->selfiMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_SELFIMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->selfiMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_REFLMAP, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->reflMap;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_REFLMASK, MAT_ENTRY)
/*==========================================================================*/
{
  ASSERT(c.pMat);
  c.pMap=&c.pMat->reflMask;
  c.handleSubChunks(CHK_HEADER_SIZE);
  c.pMap=NULL;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_ACUBIC, MAT_ENTRY)
/*==========================================================================*/
{
  byte dummy;
  
  ASSERT(c.pMat);
  dummy=c.getbyte();
  c.pMat->autoReflMap.antiAlias=(C3dsAutoReflMap::EAntiAliasLevel)c.getbyte();
  c.pMat->autoReflMap.mapSize=c.getintd();
  c.pMat->autoReflMap.flags=c.getword();
  c.pMat->autoReflMap.frameStep=c.getintd();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAPNAME, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.getstr(c.pMap->mapName);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_TILING, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->flags=c.getword();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_TEXBLUR, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->blur=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_USCALE, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->scale[0]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_VSCALE, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->scale[1]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_UOFFSET, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->mapOffset[0]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_VOFFSET, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->mapOffset[1]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_ANG, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->rotAngle=(float)RAD(c.getfloat());
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_COL1, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->tintCol1[RED]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol1[GREEN]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol1[BLUE]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol1[ALPHA]=0.0f;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_COL2, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->tintCol2[RED]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol2[GREEN]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol2[BLUE]=((float)c.getbyte())/255.0f;
  c.pMap->tintCol2[ALPHA]=0.0f;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_RCOL, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->tintR[RED]=((float)c.getbyte())/255.0f;
  c.pMap->tintR[GREEN]=((float)c.getbyte())/255.0f;
  c.pMap->tintR[BLUE]=((float)c.getbyte())/255.0f;
  c.pMap->tintR[ALPHA]=0.0f;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_GCOL, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->tintG[RED]=((float)c.getbyte())/255.0f;
  c.pMap->tintG[GREEN]=((float)c.getbyte())/255.0f;
  c.pMap->tintG[BLUE]=((float)c.getbyte())/255.0f;
  c.pMap->tintG[ALPHA]=0.0f;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MAT_MAP_BCOL, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pMap);
  c.pMap->tintB[RED]=((float)c.getbyte())/255.0f;
  c.pMap->tintB[GREEN]=((float)c.getbyte())/255.0f;
  c.pMap->tintB[BLUE]=((float)c.getbyte())/255.0f;
  c.pMap->tintB[ALPHA]=0.0f;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(NAMED_OBJECT, MDATA)
/*==========================================================================*/
{
  intd size;
  char *p;
 
  size=c.getstr(c.objname);
  for (p=c.objname; *p; p++) {
    if (*p=='.') {
      *p=0;
      break;
    }
  }
  c.pObj=NULL;
  c.pMesh=NULL;
  c.pCam=NULL;
  c.pLight=NULL;
  c.handleSubChunks(CHK_HEADER_SIZE+size+1);
}


class C3dsFaceEntry {
  public:
    C3dsFace *pFace;
    C3dsFaceEntry *next;
  public:
    C3dsFaceEntry(C3dsFace *npFace) {pFace=npFace; next=NULL;}
};


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(N_TRI_OBJECT, NAMED_OBJECT)
/*==========================================================================*/
{
  c.pObj=c.pMesh=new C3dsMesh();
  c.file3ds->meshL.push_back(c.pMesh);
  strcpy(c.pMesh->name,c.objname);
  c.handleSubChunks(CHK_HEADER_SIZE);
  
  /** Reset transforms **/
  {
    M4x4 M;
    int j;

    M=c.pMesh->matrix;
    M.inv();
    for (j=0; j<c.pMesh->vertices; j++) {
      M.transform(c.pMesh->vertexL[j].pos);
    }
  }
  
  /** Edge generation **/
  {
    int j,i;
    C3dsFace *faceL;
    C3dsVertex *vertexL;
    C3dsFaceEntry **FEL;
    int idx;
    C3dsFaceEntry *p,*q;
    int nfaces;

    FEL=new C3dsFaceEntry*[c.pMesh->vertices];
    for (j=0; j<c.pMesh->vertices; j++) FEL[j]=NULL;
   
    faceL=c.pMesh->faceL;
    vertexL=c.pMesh->vertexL;
    for (j=0; j<c.pMesh->faces; j++) {
      faceL[j].normal.normal(
        vertexL[faceL[j].edgeL[0].v].pos,
        vertexL[faceL[j].edgeL[1].v].pos,
        vertexL[faceL[j].edgeL[2].v].pos
      );

      for (i=0; i<3; i++) {
        idx=c.pMesh->faceL[j].edgeL[i].v;
        p=new C3dsFaceEntry(&c.pMesh->faceL[j]);
        p->next=FEL[idx];
        FEL[idx]=p;
      }

    }

    for (j=0; j<c.pMesh->vertices; j++) {
      for (nfaces=0,p=FEL[j]; p; p=p->next) nfaces++;

      vertexL[j].faces=nfaces;
      if (nfaces) {
        vertexL[j].faceL=new (C3dsFace*)[nfaces];
      }
      else {
         vertexL[j].faceL=NULL;
      }

      for (p=FEL[j],i=0; p; p=q,i++) {
        vertexL[j].faceL[i]=p->pFace;
        q=p->next;
        delete p;
      }
    }
    delete FEL;

  }

  {
    intd j,i,k,l;
    intd idx;
    M3d NN,SN;
    intd uns,found;
    M3d UN[128];
    dword smoothGroup;
    C3dsFace *pp;


    for (j=0; j<c.pMesh->faces; j++) {
      smoothGroup=c.pMesh->faceL[j].smoothing;
    
      for (i=0; i<3; i++) {
        NN.zero();
        idx=c.pMesh->faceL[j].edgeL[i].v;

        uns=0;
        for (l=0; l<c.pMesh->vertexL[idx].faces; l++) {
          pp=c.pMesh->vertexL[idx].faceL[l];
          found=0;
          SN=pp->normal;
          for (k=0; k<uns; k++) {
            if ((fabs(((SN*UN[k]))-1.0)<EPSILON)) {
              found=1;
              break;
            }
          }
          if (!found) {
            if ((!smoothGroup) || (smoothGroup&pp->smoothing)) { 
              NN.add(NN,SN);
              UN[uns]=SN;
              uns++;
            }
          }
        }

        NN.normalize();
        c.pMesh->faceL[j].edgeL[i].normal=NN;
      }

    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POINT_ARRAY, N_TRI_OBJECT)
/*==========================================================================*/
{
  intd vertices;
  intd i;

  ASSERT(c.pMesh && (!c.pMesh->vertexL));
  vertices=c.getword();
  if (!c.pMesh->vertices) {
    c.pMesh->vertices=vertices;
  }
  else {
    ASSERT(c.pMesh->vertices==vertices);
  }
  c.pMesh->vertexL=new C3dsVertex[vertices];
  for (i=0; i<vertices; i++) {
    c.pMesh->vertexL[i].pos[0]=c.getfloat();
    c.pMesh->vertexL[i].pos[1]=c.getfloat();
    c.pMesh->vertexL[i].pos[2]=c.getfloat();
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POINT_FLAG_ARRAY, N_TRI_OBJECT)
/*==========================================================================*/
{
  intd vertices;
  intd i;

  ASSERT(c.pMesh && (!c.pMesh->flagL));
  vertices=c.getword();
  if (!c.pMesh->vertices) {
    c.pMesh->vertices=vertices;
  }
  else {
    ASSERT(c.pMesh->vertices==vertices);
  }
  c.pMesh->flagL=new dword[vertices];
  for (i=0; i<vertices; i++) {
    c.pMesh->flagL[i]=c.getword();
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(FACE_ARRAY, N_TRI_OBJECT)
/*==========================================================================*/
{
  intd faces;
  intd i;

  ASSERT(c.pMesh && (!c.pMesh->faceL));
  faces=c.getword();
  c.pMesh->faces=faces;
  c.pMesh->faceL=new C3dsFace[faces];
  for (i=0; i<faces; i++) {
    c.pMesh->faceL[i].matIndex=-1;
    c.pMesh->faceL[i].pMat=NULL;
    c.pMesh->faceL[i].edgeL[0].v=c.getword();
    c.pMesh->faceL[i].edgeL[1].v=c.getword();
    c.pMesh->faceL[i].edgeL[2].v=c.getword();
    c.pMesh->faceL[i].flags=c.getword();
  }

  c.handleSubChunks(CHK_HEADER_SIZE+2+8*faces);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SMOOTH_GROUP, FACE_ARRAY)
/*==========================================================================*/
{
  intd faces,i;

  faces=c.pMesh->faces;
  for (i=0; i<faces; i++) {
    c.pMesh->faceL[i].smoothing=c.getdword();
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MSH_MAT_GROUP, NULL_CHUNK)
/*==========================================================================*/
{
  char matname[64];
  intd nfaces;
  intd i,idx,matnum;
  C3dsMatList::iterator mat;
  C3dsMat *pMat;

  ASSERT(c.pMesh);
  if (c.pMesh->faces) {
    c.getstr(matname);
    
    matnum=0;
    for (mat=c.file3ds->matL.begin(); mat!=c.file3ds->matL.begin(); mat++) {
      if (strcmp(matname,(*mat)->matName)==0) break;
      matnum++;
    }
    pMat=*mat;
    if (!pMat) {
      ASSERT(false);
      return;
    }
    nfaces=c.getword();          
    for (i=0; i<nfaces; i++) {
      idx=c.getword();
      c.pMesh->faceL[idx].matIndex=matnum;
      c.pMesh->faceL[idx].pMat=pMat;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MSH_BOXMAP, FACE_ARRAY)
/*==========================================================================*/
{
  ASSERT(c.pMesh);
  c.getstr(c.pMesh->boxMap.front);
  c.getstr(c.pMesh->boxMap.back);
  c.getstr(c.pMesh->boxMap.left);
  c.getstr(c.pMesh->boxMap.right);
  c.getstr(c.pMesh->boxMap.top);
  c.getstr(c.pMesh->boxMap.bottom);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(TEX_VERTS, N_TRI_OBJECT)
/*==========================================================================*/
{
  intd vertices;
  intd i;

  ASSERT(c.pMesh && (!c.pMesh->texelL));
  vertices=c.getword();
  if (!c.pMesh->vertices) {
    c.pMesh->vertices=vertices;
  }
  else {
    ASSERT(c.pMesh->vertices==vertices);
  }
  c.pMesh->texelL=new C3dsMesh::TTexel[vertices];
  for (i=0; i<vertices; i++) {
    c.pMesh->texelL[i][0]=c.getfloat();
    c.pMesh->texelL[i][1]=c.getfloat();
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MESH_MATRIX, N_TRI_OBJECT)
/*==========================================================================*/
{
  int i,j;

  ASSERT(c.pMesh);
  c.pMesh->matrix.identity();
  for (i=0; i<4; i++) for (j=0; j<3; j++) c.pMesh->matrix[i][j]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MESH_COLOR, N_TRI_OBJECT)
/*==========================================================================*/
{
  ASSERT(c.pMesh);
  c.pMesh->objectCol=c.getbyte();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MESH_TEXTURE_INFO, N_TRI_OBJECT)
/*==========================================================================*/
{
  intd i,j;

  ASSERT(c.pMesh);
  c.pMesh->mapData.tile[0]=c.getfloat();
  c.pMesh->mapData.tile[1]=c.getfloat();
  c.pMesh->mapData.pos[0]=c.getfloat();
  c.pMesh->mapData.pos[1]=c.getfloat();
  c.pMesh->mapData.pos[2]=c.getfloat();
  c.pMesh->mapData.scale=c.getfloat();
  c.pMesh->mapData.matrix.identity();
  for (i=0; i<4; i++) 
    for (j=0; j<3; j++) c.pMesh->mapData.matrix[j][i]=c.getfloat();
  c.pMesh->mapData.planarSize[0]=c.getfloat();
  c.pMesh->mapData.planarSize[1]=c.getfloat();
  c.pMesh->mapData.cylHeight=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(N_DIRECT_LIGHT, NAMED_OBJECT)
/*==========================================================================*/
{
  c.pObj=c.pLight=new C3dsLight;
  c.file3ds->lightL.push_back(c.pLight);
  strcpy(c.pLight->name,c.objname);
  c.pLight->pos[0]=c.getfloat();
  c.pLight->pos[1]=c.getfloat();
  c.pLight->pos[2]=c.getfloat();
  c.handleSubChunks(CHK_HEADER_SIZE+12);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COLOR_F, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  if (c.pLight) {
    c.pLight->col[RED]=c.getfloat();
    c.pLight->col[GREEN]=c.getfloat();
    c.pLight->col[BLUE]=c.getfloat();
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_OFF, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->off=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_OUTER_RANGE, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->outerRange=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_INNER_RANGE, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->innerRange=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_MULTIPLIER, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->multiplier=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_EXCLUDE, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  char objname[128];

  ASSERT(c.pLight);
  c.getstr(objname);
  char *lx=new char[strlen(objname)+1];
  strcpy(lx,objname);
  c.pLight->excludeL.push_back(lx);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_ATTENUATE, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->attenuation=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOTLIGHT, N_DIRECT_LIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->tgt[0]=c.getfloat();
  c.pLight->tgt[1]=c.getfloat();
  c.pLight->tgt[2]=c.getfloat();
  c.pLight->hotSpot=c.getfloat();
  c.pLight->fallOff=c.getfloat();
  c.handleSubChunks(CHK_HEADER_SIZE+12+2*4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOT_ROLL, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->roll=(float)RAD(c.getfloat());
}   


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SHADOWED, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->shadowed=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_LOCAL_SHADOW2, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->shadBias=c.getfloat();
  c.pLight->shadFilter=c.getfloat();
  c.pLight->shadSize=c.getword();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SEE_CONE, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->seeCone=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOT_RECTANGULAR, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->rectangularSpot=TRUE;
}           


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOT_ASPECT, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->spotAspect=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOT_PROJECTOR, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->useProjector=TRUE;
  c.getstr(c.pLight->projector);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_SPOT_OVERSHOOT, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->overShoot=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_RAY_BIAS, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->raytraceBias=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(DL_RAYSHAD, DL_SPOTLIGHT)
/*==========================================================================*/
{
  ASSERT(c.pLight);
  c.pLight->raytraceShadows=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(N_CAMERA, NAMED_OBJECT)
/*==========================================================================*/
{
  c.pObj=c.pCam=new C3dsCam;
  c.file3ds->camL.push_back(c.pCam);
  strcpy(c.pCam->name,c.objname);
  c.pCam->pos[0]=c.getfloat();
  c.pCam->pos[1]=c.getfloat();
  c.pCam->pos[2]=c.getfloat();
  c.pCam->tgt[0]=c.getfloat();
  c.pCam->tgt[1]=c.getfloat();
  c.pCam->tgt[2]=c.getfloat();
  c.pCam->roll=c.getfloat();
  c.pCam->fov=2400.0f/c.getfloat();
  c.handleSubChunks(CHK_HEADER_SIZE+12*2+2*4);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(CAM_SEE_CONE, N_CAMERA)
/*==========================================================================*/
{
  ASSERT(c.pCam);
  c.pCam->seeCone=TRUE;
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(CAM_RANGES, N_CAMERA)
/*==========================================================================*/
{
  ASSERT(c.pCam);
  c.pCam->nearRad=c.getfloat();
  c.pCam->farRad=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJ_HIDDEN, NAMED_OBJECT)                  
/*==========================================================================*/
{
  //ASSERT(c.pObj);
  //if (c.pObj) {
  //  c.pObj->hidden=TRUE;
  //}
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJ_DOESNT_CAST, NAMED_OBJECT)                  
/*==========================================================================*/
{
  ASSERT(c.pObj);
  if (c.pObj) {
    c.pObj->castShad=FALSE;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJ_DONT_RECVSHADOW, NAMED_OBJECT)                  
/*==========================================================================*/
{
  ASSERT(c.pObj);
  if (c.pObj) {
    c.pObj->recvShad=FALSE;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJ_MATTE, NAMED_OBJECT)                  
/*==========================================================================*/
{
  ASSERT(c.pObj);
  if (c.pObj) {
    c.pObj->matte=TRUE;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJ_FROZEN, NAMED_OBJECT)                  
/*==========================================================================*/
{
  ASSERT(c.pObj);
  if (c.pObj) {
    c.pObj->frozen=TRUE;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(KFDATA, NULL_CHUNK)
/*==========================================================================*/
{
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(KFHDR, KFDATA)
/*==========================================================================*/
{
  intd rev;
  char name[32];

  rev=c.getintw();
  c.getstr(name);
  c.file3ds->frames.frames=c.getintd();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(KFSEG, KFDATA)
/*==========================================================================*/
{
  c.file3ds->frames.from=c.getintd();
  c.file3ds->frames.to=c.getintd();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(KFCURTIME, KFDATA)
/*==========================================================================*/
{
  c.file3ds->frames.curframe=c.getintd();
}


/*==========================================================================*/
static void load3dsKeyHeader(C3dsChunk& c, C3dsKeyHeader *H)
/*==========================================================================*/
{
  dword flags;
  H->frame=c.getintd();
  H->flags=flags=c.getword();
  if (flags&C3dsKeyHeader::USE_TENSION) {
    H->tens=c.getfloat();
  }
  if (flags&C3dsKeyHeader::USE_CONTINUITY) {
    H->cont=c.getfloat();
  }
  if (flags&C3dsKeyHeader::USE_BIAS) {
    H->bias=c.getfloat();
  }
  if (flags&C3dsKeyHeader::USE_EASE_TO) {
    H->easeTo=c.getfloat();
  }
  if (flags&C3dsKeyHeader::USE_EASE_FROM) {
    H->easeFrom=c.getfloat();
  }
}


/*=========================================================================*/
inline static void TCB(C3dsKeyHeader *p, C3dsKeyHeader *c, C3dsKeyHeader *n, 
  double& ksm, double& ksp, double& kdm, double& kdp)
/*=========================================================================*/
{
  double tm,cm,cp,bm,bp,tmcm,tmcp,cc;
  double dt,fp,fn;

  fp=fn=1.0f;
  if (p&&n) {
    dt=0.5f*(n->frame-p->frame);
    fp=((c->frame-p->frame))/dt;
    fn=((n->frame-c->frame))/dt;
    cc=fabs(c->cont);
    fp=fp+cc-cc*fp;
    fn=fn+cc-cc*fn;
  }
  cm=1.0f-c->cont;
  tm=0.5f*(1.0f-c->tens);
  cp=2.0f-cm;
  bm=1.0f-c->bias;
  bp=2.0f-bm;      
  tmcm=tm*cm;
  tmcp=tm*cp;
  ksm=tmcm*bp*fp;
  ksp=tmcp*bm*fp;
  kdm=tmcp*bp*fn;
  kdp=tmcm*bm*fn;
}


/*=========================================================================*/
static void initializePosKey(C3dsPosKey *pp, C3dsPosKey *pc, C3dsPosKey *pn)
/*=========================================================================*/
{
  int i;
  double ksm,ksp,kdm,kdp=1.0f;
  M3d p,n;

  TCB(pp,pc,pn, ksm,ksp,kdm,kdp);
  if (pp||pn) {
    if (pp) {
      p.sub(pc->pos, pp->pos); 
    }
    else {
      p.sub(pn->pos, pc->pos);
    }
    if (pn) {
      n.sub(pn->pos, pc->pos);
    }
    else {
      n.sub(pc->pos, pp->pos);
    }
    for(i=0; i<3; i++) {
      pc->DS[i]=ksm*p[i] + ksp*n[i];
      pc->DD[i]=kdm*p[i] + kdp*n[i];
    }                
  }
  else {
    pc->DS.zero();
    pc->DD.zero();
  }
}


/*==========================================================================*/
static void load3dsPosTrack(C3dsChunk& c, C3dsTrack<C3dsPosKey> *pT)
/*==========================================================================*/
{
  intd keys,i;
  C3dsPosKey *p,*n;
  
  pT->flags=c.getword();
  c.getdword();
  c.getdword();
  pT->nkeys=keys=c.getintd();
  if (keys<2) pT->flags&=~C3dsTrackHeader::SMOOTH;
  if (keys) {
    pT->keyL=new C3dsPosKey[keys];
    for (i=0; i<keys; i++) {
      load3dsKeyHeader(c,&pT->keyL[i]);
      pT->keyL[i].pos[0]=c.getfloat();
      pT->keyL[i].pos[1]=c.getfloat();
      pT->keyL[i].pos[2]=c.getfloat();
    }
  }
  if (pT->flags&C3dsTrackHeader::SMOOTH) {
    for (i=0; i<keys; i++) {
      p= (i==0)? &pT->keyL[keys-1] : &pT->keyL[i-1];
      n= (i==keys-1)? &pT->keyL[0] : &pT->keyL[i+1];
      initializePosKey(p, &pT->keyL[i], n);
    }
  } 
  else {
    for (i=0; i<keys; i++) {
      p= (i==0)? NULL : &pT->keyL[i-1];
      n= (i==keys-1)? NULL : &pT->keyL[i+1];
      initializePosKey(p, &pT->keyL[i], n);
    }
  }
}


/*=========================================================================*/
static void initializeRotKey(C3dsRotKey *pp, C3dsRotKey *pc, C3dsRotKey *pn)
/*=========================================================================*/
{
  int i;
  double ksm,ksp,kdm,kdp=1.0f;
  MQuat q,qn,qp,qa,qb;
  
  TCB(pp,pc,pn, ksm,ksp,kdm,kdp);
  if (pp) {
    if (pp->angle>TWOPI-EPSILON) {
      qp.set(pp->axis, 0.0);
      qp.ln();
    }
    else {
      q=pp->rot;
      if (q.dot(pc->rot)<0) q.neg();
      qp.lnDif(pc->rot,q);
    }
  }
  else {
    qp.zero();
  }

  if (pn) {
    if (pn->angle>TWOPI-EPSILON) {
      qn.set(pn->axis, 0.0);
      qn.ln();
    }
    else {
      q=pn->rot;
      if (q.dot(pc->rot)<0) q.neg();
      qn.lnDif(pc->rot,q);
    }
  }
  else {
    qn.zero();
  }

  for(i=0; i<4; i++) {
    qa[i]=-0.5*(kdm*qn[i]+kdp*qp[i]);
    qb[i]=-0.5*(ksm*qn[i]+ksp*qp[i]);
  }
  qa.exp();
  qb.exp();

  pc->DS.mul(pc->rot,qa);
  pc->DD.mul(pc->rot,qb);
}


/*==========================================================================*/
static void load3dsRotTrack(C3dsChunk& c, C3dsRotTrack *rT)
/*==========================================================================*/
{
  intd keys,i;
  C3dsRotKey *p,*n;
  MQuat q;
  double x,y,z;

  rT->flags=c.getword();
  c.getdword();
  c.getdword();
  rT->nkeys=keys=c.getintd();
  if (keys) {
    rT->keyL=new C3dsRotKey[keys];
    for (i=0; i<keys; i++) {
      load3dsKeyHeader(c,&rT->keyL[i]);
      rT->keyL[i].angle=c.getfloat();
      x=c.getfloat();
      y=c.getfloat();
      z=c.getfloat();
      rT->keyL[i].axis.set(x,y,z);
      q.set(rT->keyL[i].axis, rT->keyL[i].angle);

      if (i!=0) {
        rT->keyL[i].rot.mul(q, rT->keyL[i-1].rot);
      }
      else {
        rT->keyL[i].rot=q;
      }
    }
  }
  if (rT->flags&C3dsTrackHeader::SMOOTH) {
    for (i=0; i<keys; i++) {
      p= (i==0)? &rT->keyL[keys-1] : &rT->keyL[i-1];
      n= (i==keys-1)? &rT->keyL[0] : &rT->keyL[i+1];
      initializeRotKey(p, &rT->keyL[i], n);
    }
  } 
  else {
    for (i=0; i<keys; i++) {
      p= (i==0)? NULL : &rT->keyL[i-1];
      n= (i==keys-1)? NULL : &rT->keyL[i+1];
      initializeRotKey(p, &rT->keyL[i], n);
    }
  }
}


/*=========================================================================*/
static void initializeAngleKey(C3dsAngleKey *pp, C3dsAngleKey *pc, C3dsAngleKey *pn)
/*=========================================================================*/
{
  double ksm,ksp,kdm,kdp=1.0f;
  double p,n;

  TCB(pp,pc,pn, ksm,ksp,kdm,kdp);
  if (pp||pn) {
    if (pp) {
      p=pc->angle - pp->angle; 
    }
    else {
      p=pn->angle - pc->angle;
    }
    if (pn) {
      n=pn->angle - pc->angle;
    }
    else {
      n=pc->angle - pp->angle;
    }
    pc->DS=ksm*p + ksp*n;
    pc->DD=kdm*p + kdp*n;
  }
  else {
    pc->DS=pc->DD=0.0f;
  }
}


/*==========================================================================*/
static void load3dsAngleTrack(C3dsChunk& c, C3dsTrack<C3dsAngleKey> *aT)
/*==========================================================================*/
{
  intd keys,i;
  C3dsAngleKey *p,*n;
  
  aT->flags=c.getword();
  c.getdword();
  c.getdword();
  aT->nkeys=keys=c.getintd();
  if (keys) {
    aT->keyL=new C3dsAngleKey[keys];
    for (i=0; i<keys; i++) {
      load3dsKeyHeader(c,&aT->keyL[i]);
      aT->keyL[i].angle=RAD(c.getfloat());
    }
  }
  if (aT->flags&C3dsTrackHeader::SMOOTH) {
    for (i=0; i<keys; i++) {
      p= (i==0)? &aT->keyL[keys-1] : &aT->keyL[i-1];
      n= (i==keys-1)? &aT->keyL[0] : &aT->keyL[i+1];
      initializeAngleKey(p, &aT->keyL[i], n);
    }
  } 
  else {
    for (i=0; i<keys; i++) {
      p= (i==0)? NULL : &aT->keyL[i-1];
      n= (i==keys-1)? NULL : &aT->keyL[i+1];
      initializeAngleKey(p, &aT->keyL[i], n);
    }
  }
}


static void colZero(double dest[3]) 
{
  int i;
  for (i=0; i<3; i++) dest[i]=0;
}


static void colSub(double c[3], double a[3], double b[3]) 
{
  int i;
  for (i=0; i<3; i++) c[i]=a[i]-b[i];
}


/*=========================================================================*/
static void initializeColKey(C3dsColKey *pp, C3dsColKey *pc, C3dsColKey *pn)
/*=========================================================================*/
{
  int i;
  double ksm,ksp,kdm,kdp=1.0f;
  double p[3],n[3];

  TCB(pp,pc,pn, ksm,ksp,kdm,kdp);
  if (pp||pn) {
    if (pp) {
      colSub(p, pc->col, pp->col); 
    }
    else {
      colSub(p, pn->col, pc->col);
    }
    if (pn) {
      colSub(n, pn->col, pc->col);
    }
    else {
      colSub(n, pc->col, pp->col);
    }
    for(i=0; i<3; i++) {
      pc->DS[i]=ksm*p[i] + ksp*n[i];
      pc->DD[i]=kdm*p[i] + kdp*n[i];
    }                
  }
  else {
    colZero(pc->DS);
    colZero(pc->DD);
  }
}


/*==========================================================================*/
static void load3dsColTrack(C3dsChunk& c, C3dsTrack<C3dsColKey> *pT)
/*==========================================================================*/
{
  intd keys,i;
  C3dsColKey *p,*n;
  
  pT->flags=c.getword();
  c.getdword();
  c.getdword();
  pT->nkeys=keys=c.getintd();
  if (keys<2) pT->flags&=~C3dsTrackHeader::SMOOTH;
  if (keys) {
    pT->keyL=new C3dsColKey[keys];
    for (i=0; i<keys; i++) {
      load3dsKeyHeader(c,&pT->keyL[i]);
      pT->keyL[i].col[0]=c.getfloat();
      pT->keyL[i].col[1]=c.getfloat();
      pT->keyL[i].col[2]=c.getfloat();
    }
  }
  if (pT->flags&C3dsTrackHeader::SMOOTH) {
    for (i=0; i<keys; i++) {
      p= (i==0)? &pT->keyL[keys-1] : &pT->keyL[i-1];
      n= (i==keys-1)? &pT->keyL[0] : &pT->keyL[i+1];
      initializeColKey(p, &pT->keyL[i], n);
    }
  } 
  else {
    for (i=0; i<keys; i++) {
      p= (i==0)? NULL : &pT->keyL[i-1];
      n= (i==keys-1)? NULL : &pT->keyL[i+1];
      initializeColKey(p, &pT->keyL[i], n);
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(NODE_ID, NULL_CHUNK)
/*==========================================================================*/
{
  ASSERT(c.pNode);
  c.pNode->nodeId=c.getword();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(NODE_HDR, NULL_CHUNK)
/*==========================================================================*/
{
  char *p;

  ASSERT(c.pNode);
  c.getstr(c.pNode->objName);
  for (p=c.pNode->objName; *p; p++) {
    if (*p=='.') {
      *p=0;
      break;
    }
  }
  c.pNode->flags1=c.getword();
  c.pNode->flags2=c.getword();
  c.pNode->parentId=c.getword();
  if (c.pNode->parentId==65535) {
    c.pNode->parentId=-1;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(AMBIENT_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdAmb=new C3dsAmbNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COL_TRACK_TAG, AMBIENT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdAmb);
  load3dsColTrack(c,&c.pNdAmb->colT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(OBJECT_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdInst=new C3dsInstNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  if (strcmp(c.pNdInst->objName,"$$$DUMMY")!=0) {
    C3dsInst *pInst;
    C3dsMesh *pMesh;
    pMesh=c.file3ds->findMesh(c.pNdInst->objName);
    pInst=new C3dsInst(pMesh,c.pNdInst);
    c.file3ds->instL.push_back(pInst);
    c.pNdInst->pInst=pInst;
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(PIVOT, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  c.pNdInst->pivot[0]=c.getfloat();
  c.pNdInst->pivot[1]=c.getfloat();
  c.pNdInst->pivot[2]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(INSTANCE_NAME, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  c.getstr(c.pNdInst->instName);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MORPH_SMOOTH, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  c.pNdInst->morphSmoothAng=(float)RAD(c.getfloat());
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(BOUNDBOX, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  c.pNdInst->boundBox.min[0]=c.getfloat();
  c.pNdInst->boundBox.min[1]=c.getfloat();
  c.pNdInst->boundBox.min[2]=c.getfloat();
  c.pNdInst->boundBox.max[0]=c.getfloat();
  c.pNdInst->boundBox.max[1]=c.getfloat();
  c.pNdInst->boundBox.max[2]=c.getfloat();
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  load3dsPosTrack(c,&c.pNdInst->posT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(ROT_TRACK_TAG, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  load3dsRotTrack(c,&c.pNdInst->rotT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SCL_TRACK_TAG, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdInst);
  load3dsPosTrack(c,&c.pNdInst->sclT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(MORPH_TRACK_TAG, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  intd keys,i;
  
  c.pNdInst->morphT.flags=c.getword();
  c.getdword();
  c.getdword();
  c.pNdInst->morphT.nkeys=keys=c.getintd();
  if (keys) {
    c.pNdInst->morphT.keyL=new C3dsMorphKey[keys];
    for (i=0; i<keys; i++) {
      load3dsKeyHeader(c,&c.pNdInst->morphT.keyL[i]);
      c.getstr(c.pNdInst->morphT.keyL[i].morphName);
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(HIDE_TRACK_TAG, OBJECT_NODE_TAG)
/*==========================================================================*/
{
  __IN_WORK__
  c.pNdInst->hideT.keys=0;
  c.pNdInst->hideT.keyL=NULL;
  //c.pNdInst->HideT.keys=keys=c.getword();
  //c.pNdInst->HideT.KeyL=new intd[keys];
  //for (i=0; i<keys;i++) {
  //  c.pNdInst->HideT.KeyL[i]=c.getdword();
  //}
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(CAMERA_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdCam=new C3dsCamNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  {
    C3dsCam *pCam;
    pCam=c.file3ds->findCam(c.pNdCam->objName);
    ASSERT(pCam);
    if (pCam) {
      pCam->pCamNode=c.pNdCam;
      c.pNdCam->cam=pCam;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, CAMERA_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdCam);
  load3dsPosTrack(c,&c.pNdCam->posT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(FOV_TRACK_TAG, CAMERA_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdCam);
  load3dsAngleTrack(c,&c.pNdCam->fovT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(ROLL_TRACK_TAG, CAMERA_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdCam);
  load3dsAngleTrack(c,&c.pNdCam->rollT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(TARGET_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdTgt=new C3dsTgtNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  {
    C3dsCam *pCam;
    pCam=c.file3ds->findCam(c.pNdTgt->objName);
    ASSERT(pCam);
    if (pCam) {
      pCam->pTgtNode=c.pNdTgt;
      c.pNdTgt->cam=pCam;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, TARGET_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdTgt);
  load3dsPosTrack(c,&c.pNdTgt->tgtT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(LIGHT_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdLight=new C3dsLightNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  {
    C3dsLight *pLight;
    pLight=c.file3ds->findLight(c.pNdLight->objName);
    ASSERT(pLight);
    if (pLight) {
      ASSERT(pLight->pLightNode==NULL);
      pLight->pLightNode=c.pNdLight;
      c.pNdLight->light=pLight;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, LIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsPosTrack(c,&c.pNdLight->posT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COL_TRACK_TAG, LIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsColTrack(c,&c.pNdLight->colT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(SPOTLIGHT_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdLight=new C3dsLightNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  {
    C3dsLight *pLight;
    pLight=c.file3ds->findLight(c.pNdLight->objName);
    ASSERT(pLight);
    if (pLight) {
      ASSERT(pLight->pLightNode==NULL);
      pLight->pLightNode=c.pNdLight;
      c.pNdLight->light=pLight;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, SPOTLIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsPosTrack(c,&c.pNdLight->posT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(COL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsColTrack(c,&c.pNdLight->colT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(HOT_TRACK_TAG, SPOTLIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsAngleTrack(c,&c.pNdLight->hotSpotT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(FALL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsAngleTrack(c,&c.pNdLight->fallOffT);
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(ROLL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdLight);
  load3dsAngleTrack(c,&c.pNdLight->rollT);
}

                                                        
/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(L_TARGET_NODE_TAG, KFDATA)
/*==========================================================================*/
{
  c.pNode=c.pNdSpot=new C3dsSpotNode;
  c.file3ds->nodeL.push_back(c.pNode);
  c.handleSubChunks(CHK_HEADER_SIZE);
  {
    C3dsLight *pLight;
    pLight=c.file3ds->findLight(c.pNdSpot->objName);
    ASSERT(pLight);
    if (pLight) {
      pLight->pSpotNode=c.pNdSpot;
      c.pNdSpot->light=pLight;
    }
  }
}


/*==========================================================================*/
IMPLEMENT_CHUNK_HANDLER(POS_TRACK_TAG, L_TARGET_NODE_TAG)
/*==========================================================================*/
{
  ASSERT(c.pNdSpot);
  load3dsPosTrack(c,&c.pNdSpot->tgtT);
}


static TChunkHandler s_chunkHandler[]={
  DECLARE_CHUNK_HANDLER(M3DMAGIC, NULL_CHUNK)             
  DECLARE_CHUNK_HANDLER(MLIBMAGIC, NULL_CHUNK)             
  DECLARE_CHUNK_HANDLER(CMAGIC, NULL_CHUNK)             
  DECLARE_CHUNK_HANDLER(MDATA, NULL_CHUNK)

  DECLARE_CHUNK_HANDLER(MASTER_SCALE, MDATA)
  DECLARE_CHUNK_HANDLER(LO_SHADOW_BIAS, MDATA)
  DECLARE_CHUNK_HANDLER(HI_SHADOW_BIAS, MDATA)
  DECLARE_CHUNK_HANDLER(SHADOW_MAP_SIZE, MDATA)
  DECLARE_CHUNK_HANDLER(SHADOW_FILTER, MDATA)
  DECLARE_CHUNK_HANDLER(RAY_BIAS, MDATA)
  DECLARE_CHUNK_HANDLER(O_CONSTS, MDATA)
  DECLARE_CHUNK_HANDLER(AMBIENT_LIGHT, MDATA)
  DECLARE_CHUNK_HANDLER(COLOR_F, AMBIENT_LIGHT)
  DECLARE_CHUNK_HANDLER(BIT_MAP, MDATA)
  DECLARE_CHUNK_HANDLER(SOLID_BGND, MDATA)
  DECLARE_CHUNK_HANDLER(COLOR_F, SOLID_BGND)
  DECLARE_CHUNK_HANDLER(V_GRADIENT, MDATA)
  DECLARE_CHUNK_HANDLER(COLOR_F, V_GRADIENT)
  DECLARE_CHUNK_HANDLER(USE_BIT_MAP, MDATA)
  DECLARE_CHUNK_HANDLER(USE_SOLID_BGND, MDATA)
  DECLARE_CHUNK_HANDLER(USE_V_GRADIENT, MDATA)
  DECLARE_CHUNK_HANDLER(FOG, MDATA)
  DECLARE_CHUNK_HANDLER(COLOR_F, FOG)
  DECLARE_CHUNK_HANDLER(FOG_BGND, FOG)
  DECLARE_CHUNK_HANDLER(LAYER_FOG, MDATA)
  DECLARE_CHUNK_HANDLER(COLOR_F, LAYER_FOG)
  DECLARE_CHUNK_HANDLER(DISTANCE_CUE, MDATA)
  DECLARE_CHUNK_HANDLER(DCUE_BGND, DISTANCE_CUE)
  DECLARE_CHUNK_HANDLER(USE_FOG, MDATA)
  DECLARE_CHUNK_HANDLER(USE_LAYER_FOG, MDATA)
  DECLARE_CHUNK_HANDLER(USE_DISTANCE_CUE, MDATA)

  DECLARE_CHUNK_HANDLER(MAT_ENTRY, MDATA)                   
  DECLARE_CHUNK_HANDLER(INT_PERCENTAGE, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_NAME, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_AMBIENT, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(COLOR_24, MAT_AMBIENT)
  DECLARE_CHUNK_HANDLER(MAT_DIFFUSE, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(COLOR_24, MAT_DIFFUSE)
  DECLARE_CHUNK_HANDLER(MAT_SPECULAR, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(COLOR_24, MAT_SPECULAR)
  DECLARE_CHUNK_HANDLER(MAT_SHININESS, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SHIN2PCT, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TRANSPARENCY, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_XPFALL, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_USE_XPFALL, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_REFBLUR, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SHADING, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_USE_REFBLUR, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SELF_ILLUM, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TWO_SIDE, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_DECAL, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_ADDITIVE, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_WIRE, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_FACEMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_PHONGSOFT, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_WIREABS, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_WIRE_SIZE, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TEX_MAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TEXMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TEX2MAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_TEX2MASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_OPACMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_OPACMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_BUMPMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_BUMPMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SPECMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SPECMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SHINMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SHINMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SELFIMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_SELFIMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_REFLMAP, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_REFLMASK, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_ACUBIC, MAT_ENTRY)
  DECLARE_CHUNK_HANDLER(MAT_MAPNAME, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_TILING, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_TEXBLUR, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_USCALE, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_VSCALE, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_UOFFSET, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_VOFFSET, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_ANG, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_COL1, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_COL2, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_RCOL, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_GCOL, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MAT_MAP_BCOL, NULL_CHUNK)

  DECLARE_CHUNK_HANDLER(NAMED_OBJECT, MDATA)                  
  DECLARE_CHUNK_HANDLER(N_TRI_OBJECT, NAMED_OBJECT)           
  DECLARE_CHUNK_HANDLER(POINT_ARRAY, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(POINT_FLAG_ARRAY, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(FACE_ARRAY, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(SMOOTH_GROUP, FACE_ARRAY)
  DECLARE_CHUNK_HANDLER(MSH_MAT_GROUP, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(MSH_BOXMAP, FACE_ARRAY)
  DECLARE_CHUNK_HANDLER(TEX_VERTS, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(MESH_MATRIX, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(MESH_COLOR, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(MESH_TEXTURE_INFO, N_TRI_OBJECT)
  DECLARE_CHUNK_HANDLER(N_DIRECT_LIGHT, NAMED_OBJECT)         
  DECLARE_CHUNK_HANDLER(COLOR_F, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_OFF, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_OUTER_RANGE, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_INNER_RANGE, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_MULTIPLIER, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_EXCLUDE, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_ATTENUATE, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOTLIGHT, N_DIRECT_LIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOT_ROLL, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SHADOWED, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_LOCAL_SHADOW2, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SEE_CONE, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOT_RECTANGULAR, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOT_ASPECT, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOT_PROJECTOR, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_SPOT_OVERSHOOT, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_RAY_BIAS, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(DL_RAYSHAD, DL_SPOTLIGHT)
  DECLARE_CHUNK_HANDLER(N_CAMERA, NAMED_OBJECT)
  DECLARE_CHUNK_HANDLER(CAM_SEE_CONE, N_CAMERA)
  DECLARE_CHUNK_HANDLER(CAM_RANGES, N_CAMERA)
  DECLARE_CHUNK_HANDLER(OBJ_HIDDEN, NAMED_OBJECT)                  
  DECLARE_CHUNK_HANDLER(OBJ_DOESNT_CAST, NAMED_OBJECT)                  
  DECLARE_CHUNK_HANDLER(OBJ_DONT_RECVSHADOW, NAMED_OBJECT)                  
  DECLARE_CHUNK_HANDLER(OBJ_MATTE, NAMED_OBJECT)                  
  DECLARE_CHUNK_HANDLER(OBJ_FROZEN, NAMED_OBJECT)                  

  DECLARE_CHUNK_HANDLER(KFDATA, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(KFHDR, KFDATA)
  DECLARE_CHUNK_HANDLER(KFSEG, KFDATA)
  DECLARE_CHUNK_HANDLER(KFCURTIME, KFDATA)
  DECLARE_CHUNK_HANDLER(NODE_ID, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(NODE_HDR, NULL_CHUNK)
  DECLARE_CHUNK_HANDLER(AMBIENT_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(COL_TRACK_TAG, AMBIENT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(OBJECT_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(PIVOT, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(INSTANCE_NAME, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(MORPH_SMOOTH, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(BOUNDBOX, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(ROT_TRACK_TAG, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(SCL_TRACK_TAG, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(MORPH_TRACK_TAG, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(HIDE_TRACK_TAG, OBJECT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(CAMERA_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, CAMERA_NODE_TAG)
  DECLARE_CHUNK_HANDLER(FOV_TRACK_TAG, CAMERA_NODE_TAG)
  DECLARE_CHUNK_HANDLER(ROLL_TRACK_TAG, CAMERA_NODE_TAG)
  DECLARE_CHUNK_HANDLER(TARGET_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, TARGET_NODE_TAG)
  DECLARE_CHUNK_HANDLER(LIGHT_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, LIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(COL_TRACK_TAG, LIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(SPOTLIGHT_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, SPOTLIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(COL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(HOT_TRACK_TAG, SPOTLIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(FALL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(ROLL_TRACK_TAG, SPOTLIGHT_NODE_TAG)
  DECLARE_CHUNK_HANDLER(L_TARGET_NODE_TAG, KFDATA)
  DECLARE_CHUNK_HANDLER(POS_TRACK_TAG, L_TARGET_NODE_TAG)
  {NULL_CHUNK,NULL_CHUNK,NULL}
};


/*==========================================================================*/
TChunkHandler* C3dsChunk::findHandler()
/*==========================================================================*/
{
  int i;
  
  for (i=0; s_chunkHandler[i].chunk!=NULL_CHUNK; i++) {
    if ((s_chunkHandler[i].chunk==chunk) 
      && (s_chunkHandler[i].parent==NULL_CHUNK || s_chunkHandler[i].parent==parent)) {
      return(&s_chunkHandler[i]);
    }
  }
  return(NULL);
}



/*==========================================================================*/
static void buildObjectLinks(C3dsFile *pFile)
/*==========================================================================*/
{
  C3dsNodeList::iterator p,q;
  C3dsNode *pNode,*parentNode;

  p=pFile->nodeL.begin();
  while (p!=pFile->nodeL.end()) {
    q=p;
    q++;
    pNode=*p;
    if (pNode->parentId!=-1) {
      pFile->nodeL.erase(p);
      parentNode=pFile->findId(pNode->parentId);
      if (parentNode) {
        parentNode->childL.push_back(pNode);
        pNode->pParent=parentNode;
      }
      else {
        ASSERT(false);
        pFile->nodeL.push_front(pNode);
      }
    }
    p=q;
  } 
}


/*==========================================================================*/
void load3DS(C3dsFile *pFile3ds, FILE *pBinFile)
/*==========================================================================*/
{
  C3dsChunk chunk(pBinFile,pFile3ds);
  chunk.handle(NULL_CHUNK);
  buildObjectLinks(pFile3ds);
  pFile3ds->animate(0.0f);
}

