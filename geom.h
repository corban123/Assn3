#include <fstream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <GL/glut.h>


using namespace std;
	

struct Vertex{

	GLfloat points[3];

	Vertex(float x, float y, float z){

		points[0] = (x);
		points[1] = (y);
		points[2] = (z);
	}

	GLfloat * returnArray(){
		return points;
	}


};


struct VertList {
	vector<Vertex*> listOfVertices;

	

	void addToVertex(Vertex* x){
		listOfVertices.push_back(x);
	}


	Vertex* returnVectorAt(int i){
		return listOfVertices.at(i);

	}

};

static VertList* vertexList = new VertList;
struct Face{

	vector<Vertex*> vertices;

	Face(int x, int y, int z){
		


		vertices.push_back(vertexList->returnVectorAt(x));

		vertices.push_back(vertexList->returnVectorAt(y));

		vertices.push_back(vertexList->returnVectorAt(z));



	}

	GLfloat* getFacing(int i){

		return vertices[i]->returnArray();

	}


};








class Trimesh {


	int numOfVertices;

	

	vector<Face*> faces;

	public:
		string name;
		Trimesh(string thingName){
			name = thingName;


		}


	void addFace(int* x){
		Face* face = new Face(x[0], x[1], x[2]);
		faces.push_back(face);

	} 

	void addVertex(float* vertex){
		
		vertexList->addToVertex(new Vertex(vertex[0],vertex[1],vertex[2]));
	}

	Face* getFace(int location){
		return faces.at(location);
	}


	int sizeOfFaces(){

		return faces.size();;
	}


};