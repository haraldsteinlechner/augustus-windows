class awxPlaneEquation
{
	public:
	
	awx_vector3f n;
	float d;
};

class awxRay
{
	public:
	
	awx_vector3f R0,Rd;
};


void awxCalcPlane(awx_vector3f& p1, awx_vector3f& p2, awx_vector3f& p3, awxPlaneEquation *plane){

	plane->n.x = p1.y*(p2.z-p3.z) + p2.y*(p3.z-p1.z) + p3.y*(p1.z-p2.z);
	plane->n.y = p1.z*(p2.x-p3.x) + p2.z*(p3.x-p1.x) + p3.z*(p1.x-p2.x);
	plane->n.z = p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y);
	plane->d   =-( p1.x*(p2.y*p3.z - p3.y*p2.z) +
					  p2.x*(p3.y*p1.z - p1.y*p3.z) +
					  p3.x*(p1.y*p2.z - p2.y*p1.z) );
					  
					  
}

void awxCalcPlane2(awx_vector3f& A, awx_vector3f& B, awx_vector3f& C, awxPlaneEquation *plane){

	awx_vector3f AB = B-A;
	awx_vector3f AC = C-A;
	
	plane->n = AB.crossProduct(AC);
	plane->n = plane->n.normalize();
	
	plane->d = -plane->n.scalarProduct(A);
					  
					  
}