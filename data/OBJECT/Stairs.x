xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 58;
 -80.83140;5.71302;-124.40264;,
 80.83140;5.71302;-124.40264;,
 80.83140;-1.89907;-124.40264;,
 -80.83140;-1.89907;-124.40264;,
 -80.83140;5.71302;-113.21458;,
 80.83140;5.71302;-113.21458;,
 80.83140;5.71302;-124.40264;,
 -80.83140;5.71302;-124.40264;,
 -80.83140;13.21579;-113.21458;,
 80.83140;13.21579;-113.21458;,
 -80.83140;13.21579;-102.02652;,
 80.83140;13.21579;-102.02652;,
 -80.83140;20.72450;-102.02652;,
 80.83140;20.72450;-102.02652;,
 -80.83140;20.72450;-90.83846;,
 80.83140;20.72450;-90.83846;,
 -80.83140;28.20452;-90.83846;,
 80.83140;28.20452;-90.83846;,
 -80.83140;28.20452;-79.65041;,
 80.83140;28.20452;-79.65041;,
 -80.83140;35.71723;-79.65041;,
 80.83140;35.71723;-79.65041;,
 -80.83140;35.71723;-68.46235;,
 80.83140;35.71723;-68.46235;,
 -80.83140;43.24632;-68.46235;,
 80.83140;43.24632;-68.46235;,
 -80.83140;43.24632;-57.27431;,
 80.83140;43.31152;-57.49067;,
 80.83140;88.45691;-1.55041;,
 -80.83140;88.45691;-1.55041;,
 -80.83140;88.45691;90.31685;,
 80.83140;88.45691;90.31685;,
 -80.83140;50.92359;-57.49067;,
 80.83140;50.92359;-57.49067;,
 80.83140;43.31152;-57.49067;,
 -80.83140;43.24632;-57.27431;,
 -80.83140;50.92359;-46.30259;,
 80.83140;50.92359;-46.30259;,
 80.83140;50.92359;-57.49067;,
 -80.83140;50.92359;-57.49067;,
 -80.83140;58.42639;-46.30259;,
 80.83140;58.42639;-46.30259;,
 -80.83140;58.42639;-35.11455;,
 80.83140;58.42639;-35.11455;,
 -80.83140;65.93506;-35.11455;,
 80.83140;65.93506;-35.11455;,
 -80.83140;65.93506;-23.92652;,
 80.83140;65.93506;-23.92652;,
 -80.83140;73.41514;-23.92652;,
 80.83140;73.41514;-23.92652;,
 -80.83140;73.41514;-12.73844;,
 80.83140;73.41514;-12.73844;,
 -80.83140;80.92780;-12.73844;,
 80.83140;80.92780;-12.73844;,
 -80.83140;80.92780;-1.55041;,
 80.83140;80.92780;-1.55041;,
 -80.83140;88.45691;-1.55041;,
 80.83140;88.45691;-1.55041;;
 
 25;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,5,4;,
 4;10,11,9,8;,
 4;12,13,11,10;,
 4;14,15,13,12;,
 4;16,17,15,14;,
 4;18,19,17,16;,
 4;20,21,19,18;,
 4;22,23,21,20;,
 4;24,25,23,22;,
 4;26,27,25,24;,
 3;28,29,30;,
 3;30,31,28;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,37,36;,
 4;42,43,41,40;,
 4;44,45,43,42;,
 4;46,47,45,44;,
 4;48,49,47,46;,
 4;50,51,49,48;,
 4;52,53,51,50;,
 4;54,55,53,52;,
 4;56,57,55,54;;
 
 MeshMaterialList {
  1;
  25;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.367200;0.367200;0.367200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  5;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  -0.000202;0.999996;-0.002971;,
  0.000000;1.000000;0.000000;,
  -0.000669;-0.014087;-0.999901;;
  25;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  3;1,3,1;,
  3;1,3,1;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;;
 }
 MeshTextureCoords {
  58;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.888890;,
  1.000000;0.888890;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.888890;,
  1.000000;0.888890;,
  0.000000;0.777780;,
  1.000000;0.777780;,
  0.000000;0.777780;,
  1.000000;0.777780;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.000000;0.555560;,
  1.000000;0.555560;,
  0.000000;0.555560;,
  1.000000;0.555560;,
  0.000000;0.444440;,
  1.000000;0.444440;,
  0.000000;0.444440;,
  1.000000;0.444440;,
  0.000000;0.333330;,
  1.000000;0.333330;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.888890;,
  1.000000;0.888890;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.888890;,
  1.000000;0.888890;,
  0.000000;0.777780;,
  1.000000;0.777780;,
  0.000000;0.777780;,
  1.000000;0.777780;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.000000;0.555560;,
  1.000000;0.555560;,
  0.000000;0.555560;,
  1.000000;0.555560;,
  0.000000;0.444440;,
  1.000000;0.444440;,
  0.000000;0.444440;,
  1.000000;0.444440;;
 }
}
