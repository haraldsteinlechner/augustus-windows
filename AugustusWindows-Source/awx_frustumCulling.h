// http://crownandcutlass.com/features/technicaldetails/camera.frustum.html

void ExtractFrustum()
{
   float   proj[16];
   float   modl[16];
   float   clip[16];
   float   t;

   /* Get the current PROJECTION matrix from OpenGL */
   //glGetFloatv( GL_PROJECTION_MATRIX, proj );

   /* Get the current MODELVIEW matrix from OpenGL */
   //glGetFloatv( GL_MODELVIEW_MATRIX, modl );
   

   for(int i=0;i<16;i++)
   {
	proj[i]=(float)project.projmatrix[i];
	modl[i]=(float)project.mvmatrix[i];
   }

   /* Combine the two matrices (multiply projection by modelview) */
   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

   clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

   clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

   clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

   /* Extract the numbers for the RIGHT plane */
   camera.frustum[0][0] = clip[ 3] - clip[ 0];
   camera.frustum[0][1] = clip[ 7] - clip[ 4];
   camera.frustum[0][2] = clip[11] - clip[ 8];
   camera.frustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt( camera.frustum[0][0] * camera.frustum[0][0] + camera.frustum[0][1] * camera.frustum[0][1] + camera.frustum[0][2] * camera.frustum[0][2] );
   camera.frustum[0][0] /= t;
   camera.frustum[0][1] /= t;
   camera.frustum[0][2] /= t;
   camera.frustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   camera.frustum[1][0] = clip[ 3] + clip[ 0];
   camera.frustum[1][1] = clip[ 7] + clip[ 4];
   camera.frustum[1][2] = clip[11] + clip[ 8];
   camera.frustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt( camera.frustum[1][0] * camera.frustum[1][0] + camera.frustum[1][1] * camera.frustum[1][1] + camera.frustum[1][2] * camera.frustum[1][2] );
   camera.frustum[1][0] /= t;
   camera.frustum[1][1] /= t;
   camera.frustum[1][2] /= t;
   camera.frustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   camera.frustum[2][0] = clip[ 3] + clip[ 1];
   camera.frustum[2][1] = clip[ 7] + clip[ 5];
   camera.frustum[2][2] = clip[11] + clip[ 9];
   camera.frustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt( camera.frustum[2][0] * camera.frustum[2][0] + camera.frustum[2][1] * camera.frustum[2][1] + camera.frustum[2][2] * camera.frustum[2][2] );
   camera.frustum[2][0] /= t;
   camera.frustum[2][1] /= t;
   camera.frustum[2][2] /= t;
   camera.frustum[2][3] /= t;

   /* Extract the TOP plane */
   camera.frustum[3][0] = clip[ 3] - clip[ 1];
   camera.frustum[3][1] = clip[ 7] - clip[ 5];
   camera.frustum[3][2] = clip[11] - clip[ 9];
   camera.frustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt( camera.frustum[3][0] * camera.frustum[3][0] + camera.frustum[3][1] * camera.frustum[3][1] + camera.frustum[3][2] * camera.frustum[3][2] );
   camera.frustum[3][0] /= t;
   camera.frustum[3][1] /= t;
   camera.frustum[3][2] /= t;
   camera.frustum[3][3] /= t;

   /* Extract the FAR plane */
   camera.frustum[4][0] = clip[ 3] - clip[ 2];
   camera.frustum[4][1] = clip[ 7] - clip[ 6];
   camera.frustum[4][2] = clip[11] - clip[10];
   camera.frustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt( camera.frustum[4][0] * camera.frustum[4][0] + camera.frustum[4][1] * camera.frustum[4][1] + camera.frustum[4][2] * camera.frustum[4][2] );
   camera.frustum[4][0] /= t;
   camera.frustum[4][1] /= t;
   camera.frustum[4][2] /= t;
   camera.frustum[4][3] /= t;

   /* Extract the NEAR plane */
   camera.frustum[5][0] = clip[ 3] + clip[ 2];
   camera.frustum[5][1] = clip[ 7] + clip[ 6];
   camera.frustum[5][2] = clip[11] + clip[10];
   camera.frustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt( camera.frustum[5][0] * camera.frustum[5][0] + camera.frustum[5][1] * camera.frustum[5][1] + camera.frustum[5][2] * camera.frustum[5][2] );
   camera.frustum[5][0] /= t;
   camera.frustum[5][1] /= t;
   camera.frustum[5][2] /= t;
   camera.frustum[5][3] /= t;
}

bool PointInFrustum( float x, float y, float z )
{
   int p;

   for( p = 0; p < 6; p++ )
      if( camera.frustum[p][0] * x + camera.frustum[p][1] * y + camera.frustum[p][2] * z + camera.frustum[p][3] <= 0 )
         return false;
   return true;
}

bool PointInFrustumWithoutY( float x, float z )
{
   int p;

   for( p = 0; p < 6; p++ )
      if( camera.frustum[p][0] * x  + camera.frustum[p][2] * z + camera.frustum[p][3] <= 0 )
         return false;
   return true;
}


bool SphereInFrustum( float x, float y, float z, float radius )
{
   int p;

   for( p = 0; p < 6; p++ )
      if( camera.frustum[p][0] * x + camera.frustum[p][1] * y + camera.frustum[p][2] * z + camera.frustum[p][3] <= -radius )
         return false;
   return true;
}

float SphereInFrustumDistance( float x, float y, float z, float radius )
{
   int p;
   float d;

   for( p = 0; p < 6; p++ )
   {
      d = camera.frustum[p][0] * x + camera.frustum[p][1] * y + camera.frustum[p][2] * z + camera.frustum[p][3];
      if( d <= -radius )
         return 0;
   }
   return d + radius;
}

bool CubeInFrustum( float x, float y, float z, float size )
{
   int p;

   for( p = 0; p < 6; p++ )
   {
      if( camera.frustum[p][0] * (x - size) + camera.frustum[p][1] * (y - size) + camera.frustum[p][2] * (z - size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x + size) + camera.frustum[p][1] * (y - size) + camera.frustum[p][2] * (z - size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x - size) + camera.frustum[p][1] * (y + size) + camera.frustum[p][2] * (z - size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x + size) + camera.frustum[p][1] * (y + size) + camera.frustum[p][2] * (z - size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x - size) + camera.frustum[p][1] * (y - size) + camera.frustum[p][2] * (z + size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x + size) + camera.frustum[p][1] * (y - size) + camera.frustum[p][2] * (z + size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x - size) + camera.frustum[p][1] * (y + size) + camera.frustum[p][2] * (z + size) + camera.frustum[p][3] > 0 )
         continue;
      if( camera.frustum[p][0] * (x + size) + camera.frustum[p][1] * (y + size) + camera.frustum[p][2] * (z + size) + camera.frustum[p][3] > 0 )
         continue;
      return false;
   }
   return true;
}