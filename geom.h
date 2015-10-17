#include <fstream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <GL/glut.h>


using namespace std;
	

struct Vertex{

	GLfloat* points;

	Vertex(float x, float y, float z){
		points = new GLfloat;
		points[0] = (x);
		points[1] = (y);
		points[2] = (z);

	}

	GLfloat * returnArray(){
		return points;
	}


};


struct VertList {
	vector<Vertex*> *listOfVertices;

	VertList(){
		listOfVertices = new vector<Vertex*>;
	}

	void addToVertex(Vertex* x){
		listOfVertices->push_back(x);
	}


	Vertex* returnVectorAt(int i){
		return listOfVertices->at(i);

	}

};

static VertList* vertexList;

struct Face{

	vector<Vertex*>* vertices;

	GLfloat* normal;
	Face(int x, int y, int z){
		vertices = new vector<Vertex*>;
		normal = new GLfloat[3];
		vertices->push_back(vertexList->returnVectorAt(x));

		vertices->push_back(vertexList->returnVectorAt(y));

		vertices->push_back(vertexList->returnVectorAt(z));

		GLfloat* a = new GLfloat[3];
		GLfloat* b = new GLfloat[3];
		GLfloat* e = new GLfloat[3];
		GLfloat* c = new GLfloat[3];
		GLfloat* d = new GLfloat[3];
		
		a = vertexList->returnVectorAt(x)->returnArray();		
		b = vertexList->returnVectorAt(y)->returnArray();
		e = vertexList->returnVectorAt(z)->returnArray();
		

		c[0] = b[0]-a[0];
		c[1] = b[1] - a[1];
		c[2] = b[2]-a[2];



		d[0] = b[0]-e[0];
		d[1] = b[1] - e[1];
		d[2] = b[2] - e[2];

		normcrossprod(c, d, normal);
	}

	void normalize(GLfloat v[3]) {    
   GLfloat d = sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); 
   if (d == 0.0) {
      return;
   }
   v[0] /= d; v[1] /= d; v[2] /= d; 
}

	void normcrossprod(GLfloat* v1, GLfloat* v2, GLfloat* out) 
	{ 

	   out[0] = v1[1]*v2[2] - v1[2]*v2[1]; 
	   out[1] = v1[2]*v2[0] - v1[0]*v2[2]; 
	   out[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
	   //normalize(out); 
	}

	GLfloat* getFacing(int i){

		return vertices->at(i)->returnArray();

	}
	GLfloat* getNormal(){

		return normal;
	}


};



class Trimesh {


	 int numOfVertices;

	
	vector<Face*> * faces;

	public:
		vector<float>* translate; 
		vector<float>* rotate;
		vector<float>* scale;

		string name;
		Trimesh(string thingName){
			vertexList = new VertList;
			faces = new vector<Face*>;
			name = thingName;
			translate = new vector<float>;
			rotate= new vector<float>;
			scale= new vector<float>;
			int i;
			for(i = 0; i < 4; i++){
				translate->push_back(0);
				scale->push_back(0);
				rotate->push_back(0);
			}

		}


	void addFace(int* x){
		Face* face = new Face(x[0], x[1], x[2]);
		faces->push_back(face);

	} 
	void setAllZero(){
			translate->clear();
			setZero(translate, 0);
			setZero(rotate, 0);
			setZero(scale, 1);


	}
	void addTranslate(float x, float y, float z){
		translate->clear();
		translate->push_back(x);
		translate->push_back(y);
		translate->push_back(z);
	}
	void addRotate(float x, float y, float z, float theta){
		rotate->clear();
		rotate->push_back(x);
		rotate->push_back(y);
		rotate->push_back(z);
		rotate->push_back(theta);
	}
	void addScale(float x, float y, float z){
		scale->clear();
		scale->push_back(x);
		scale->push_back(y);
		scale->push_back(z);
	}

	void setZero(vector<float>* vec, float num){
		int i;
		vec->clear();

		for (i = 0; i < 4; i++){
			vec->push_back(num);
		}
	}

	void addVertex(float* vertex){
		
		vertexList->addToVertex(new Vertex(vertex[0],vertex[1],vertex[2]));
		vertexList[(0)].listOfVertices->at(0);

	}

	Face* getFace(int location){
		return faces->at(location);
	}


	int sizeOfFaces(){

		return faces->size();;
	}


};