

Dormir::PhysicsCore pCore;
Dormir::Program * line_draw;

GLfloat vertex_box[]={
	1.0f,1.0f,
	1.0f,-1.0f,
	-1.0f,-1.0f,
	-1.0f,1.0f,
}

GLfloat uv_box[]={
	1.0f,0.0f,
	1.0f,1.0f,
	0.0f,1.0f,
	0.0f,0.0f,
}

class Model{
public:
	Model(){
		vbo=new VertexBufferObject(vertex_box,8,2);
		uv_vbo=new VertexBufferObject(uv_box,8,2);
		draw_program=new Program("Shader/example.vert","Shader/example.frag");
		texture= new Texture2D("Shader/triangle.png");
		basic_object=NULL;
		current_instance=NULL;
	};

	void GenerateNewInstance(GLfloat sx=1,GLfloat sy=1){
		if(basic_object!=NULL){
			if(current_instance!=NULL){
				delete current_instance;
			}
				

		}
	}
	void Draw(GLfloat * world2view);

	PhysicsObject * basic_object,* current_instance;

private:
	GLfloat scale[2];
	VertexBufferObject * vbo,* uv_vbo;
	Program * draw_program;
	Texture2D * texture;
}



void DrawPolygon(Dormir::Polygon poly){
	std::vector<GLfloat> vertex;
	for(int i=0;i<poly.Vertex.size();i++){
		vertex.push_back(poly.Vertex[i].x);
		vertex.push_back(poly.Vertex[i].y);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertex.data());
	glDrawArrays( GL_LINE_LOOP,0,vertex.size()/2);
}


void RenderAllPhysics(){
	line_draw->UseProgram();
	line_draw->setUniformMat4x4("world2view",world2view);
	for(std::list<Dormir::PhysicsObject *>::iterator it=pCore.Objects.begin();it!=pCore.Objects.end();it++){
		for(std::list<Dormir::Polygon>::iterator itpoly=(*it)->Body.begin();itpoly!=(*it)->Body.end();itpoly++){
			DrawPolygon(*itpoly);
		}
	}
}

GLfloat world2view[16];

int main(){

	Dormir::initOpenGL(800,600);

	pCore.setGravity(0.0,-2.0);

	line_draw=new Dormir::Program("Shader/Line.vert","Shader/Line.frag");

	Dormir::GenerateWorld2View(world2view,0,0,0.0f,0.5f/(float)Dormir::getWidth(),0.5f/(float)Dormir::getHeight());

	while(Dormir::UpdateGraphics()){

		RenderAllPhysics();
	}


	Dormir::terminateOpenGL();

}
