#include "GUA_OM.h"

double Distance(Tri_Mesh::Point A, Tri_Mesh::Point B)
{
	double distance;
	distance = sqrt(((A[0] - B[0]) * (A[0] - B[0])) + ((A[1] - B[1]) * (A[1] - B[1])) + ((A[2] - B[2]) * (A[2] - B[2])));
	return distance;
}

double CalculateArea(Tri_Mesh::Point A, Tri_Mesh::Point B, Tri_Mesh::Point C)
{
	double ab, bc, ca, S, area;
	ab = Distance(A, B);
	bc = Distance(B, C);
	ca = Distance(C, A);
	S = (ab + bc + ca) / 2.0;
	area = sqrt(S * (S - ab) * (S - bc) * (S - ca));
	//std::cout << "area " << std::to_string(area) << std::endl;
	return area;
}

double cot(Tri_Mesh::Point A, Tri_Mesh::Point B, Tri_Mesh::Point C)
{
	double a, b, c;
	a = Distance(B, C);
	b = Distance(A, C);
	c = Distance(A, B);

	double cosValue = c*c - a*a - b*b + 2 * a*b;
	float degree = std::acosf(cosValue);
	
	return (1.0 / tanf(degree));
}

namespace OMT
{
	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
	}
}
/*======================================================================*/
namespace OMP
{
	Model::Model()
	{
		Mesh.request_vertex_status();
		Mesh.request_edge_status();
		Mesh.request_face_status();
	}
	Model::~Model()
	{
		Mesh.release_vertex_status();
		Mesh.release_edge_status();
		Mesh.release_face_status();
	}
	/*======================================================================*/
	bool Model::ReadFile(std::string _fileName)
	{
		bool isRead = false;
		OpenMesh::IO::Options opt;
		if (OpenMesh::IO::read_mesh(Mesh, _fileName, opt))
		{
			//read mesh from filename OK!
			isRead = true;
		}
		if (isRead)
		{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
			if (!opt.check(OpenMesh::IO::Options::VertexNormal) && Mesh.has_vertex_normals())
			{
				Mesh.update_normals();
			}
		}
		return isRead;
	}
	bool Model::SaveFile(std::string _fileName)
	{
		bool isSave = false;
		OpenMesh::IO::Options opt;
		if (OpenMesh::IO::write_mesh(Mesh, _fileName, opt))
		{
			//read mesh from filename OK!
			isSave = true;
		}
		return isSave;
	}
	/*======================================================================*/
	void Model::Render_solid()
	{
		FIter f_it;
		FVIter	fv_it;
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.5, 1.0, 0.5);
		for (f_it = Mesh.faces_begin(); f_it != Mesh.faces_end(); ++f_it)
		{
			for (fv_it = Mesh.fv_iter(f_it); fv_it; ++fv_it)
			{
				glNormal3dv(Mesh.normal(fv_it.handle()).data());
				glVertex3dv(Mesh.point(fv_it.handle()).data());
			}
		}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::Render_wireframe()
	{
		MyMesh::HalfedgeHandle _hedge;
		EIter e_it = Mesh.edges_begin();

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for (e_it = Mesh.edges_begin(); e_it != Mesh.edges_end(); ++e_it)
		{
			_hedge = Mesh.halfedge_handle(e_it.handle(), 1);

			glVertex3dv(Mesh.point(Mesh.from_vertex_handle(_hedge)).data());
			glVertex3dv(Mesh.point(Mesh.to_vertex_handle(_hedge)).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	/*======================================================================*/
	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector<sp_p>::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(p_itr->pt.data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector< sp_v >::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(Mesh.point(v_itr->vh).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_itr;
		vector< sp_f >::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr = Mesh.fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{
				glNormal3dv(Mesh.normal(fv_itr.handle()).data());
				glVertex3dv(Mesh.point(fv_itr.handle()).data());
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	/*======================================================================*/
	void Model::add_sp_p(Point   _p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}
	void Model::add_sp_v(VHandle _v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}
	void Model::add_sp_f(FHandle _f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}
	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}
	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}
	/*======================================================================*/
	VHandle Model::addVertex(Point _p)
	{
		int find_result = findVertex(_p);
		if (find_result != -1)
		{
			return Mesh.vertex_handle(find_result);
		}
		else
		{
			return Mesh.add_vertex(_p);
		}
	}
	FHandle Model::addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		vector<VHandle> face_vhandles;

		face_vhandles.clear();
		face_vhandles.push_back(_v0);
		face_vhandles.push_back(_v1);
		face_vhandles.push_back(_v2);
		face_vhandles.push_back(_v3);
		return Mesh.add_face(face_vhandles);
	}
	void Model::deleteFace(FHandle _f)
	{
		Mesh.delete_face(_f);
		Mesh.garbage_collection();
	}
	void Model::deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		/*
		v1				v0
		*<--------------*
		|				|
		|				|
		|				|
		|		f		|
		|				|
		|				|
		|				|
		* --------------*
		v2				v3
		*/

		HEHandle v0v1 = Mesh.find_halfedge(_v0, _v1);
		if (v0v1.is_valid())
		{
			FHandle fh = Mesh.face_handle(v0v1);
			if (fh.is_valid())
			{
				Mesh.delete_face(fh);
				Mesh.garbage_collection();
			}
		}
	}
	/*======================================================================*/
	bool Model::IsVertexVertex(VHandle _vj, VHandle _vi)
	{
		for (VVIter vvit = Mesh.vv_iter(_vi); vvit; ++vvit)
			if (vvit.handle() == _vj)
				return true;
		return false;
	}
	/*======================================================================*/
	int Model::quad_subdivision1(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//�o�q�O���F�ѨMindex���D
		VHandle vq, vw, vt, vr;
		vq = addVertex(Point(0, 0, 100));
		vw = addVertex(Point(1, 0, 100));
		vt = addVertex(Point(1, 1, 100));
		vr = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*�����ݭnsubdivision��face*/
		//��ltable
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while (last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*��candidate faces��subdivision*/
		/*
			v0		vd		v3
			*-------*-------*
			|		|		|
			|		|		|
			|	  ve|		|
		  va*-------*-------*vc
			|		|		|
			|		|		|
			|		|		|
			*-------*-------*
			v1		vb		v2
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va = addVertex((Mesh.point(v[0]) + Mesh.point(v[1])) / 2);
			vb = addVertex((Mesh.point(v[1]) + Mesh.point(v[2])) / 2);
			vc = addVertex((Mesh.point(v[2]) + Mesh.point(v[3])) / 2);
			vd = addVertex((Mesh.point(v[3]) + Mesh.point(v[0])) / 2);
			ve = addVertex((Mesh.point(v[0]) + Mesh.point(v[1]) + Mesh.point(v[2]) + Mesh.point(v[3])) / 4);
			addFace(vd, v[0], va, ve);
			addFace(va, v[1], vb, ve);
			addFace(vb, v[2], vc, ve);
			addFace(vc, v[3], vd, ve);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//�o��u�O���F�ѨMindex���D
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//�o�q�O���F�ѨMindex���D
		VHandle vq, vw, vt, vr;
		vq = addVertex(Point(0, 0, 100));
		vw = addVertex(Point(1, 0, 100));
		vt = addVertex(Point(1, 1, 100));
		vr = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*�����ݭnsubdivision��face*/
		//��ltable
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while (last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*��candidate faces��subdivision*/
		/*
			v0		vh		vg		v3
			*-------*-------*-------*
			|		|		|		|
			|		|		|		|
			|	  vi|	  vl|		|
		 va *-------*-------*-------*vf
			|		|		|		|
			|		|		|		|
			|	  vj|	  vk|		|
		 vb *-------*-------*-------*ve
			|		|		|		|
			|		|		|		|
			|		|		|		|
			*-------*-------*-------*
			v1		vc		vd		v2
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va = addVertex((Mesh.point(v[0]) * 2 + Mesh.point(v[1])) / 3);
			vb = addVertex((Mesh.point(v[0]) + Mesh.point(v[1]) * 2) / 3);
			vc = addVertex((Mesh.point(v[1]) * 2 + Mesh.point(v[2])) / 3);
			vd = addVertex((Mesh.point(v[1]) + Mesh.point(v[2]) * 2) / 3);
			ve = addVertex((Mesh.point(v[2]) * 2 + Mesh.point(v[3])) / 3);
			vf = addVertex((Mesh.point(v[2]) + Mesh.point(v[3]) * 2) / 3);
			vg = addVertex((Mesh.point(v[3]) * 2 + Mesh.point(v[0])) / 3);
			vh = addVertex((Mesh.point(v[3]) + Mesh.point(v[0]) * 2) / 3);

			vi = addVertex((Mesh.point(vh) * 2 + Mesh.point(vc)) / 3);
			vj = addVertex((Mesh.point(vh) + Mesh.point(vc) * 2) / 3);
			vk = addVertex((Mesh.point(vd) * 2 + Mesh.point(vg)) / 3);
			vl = addVertex((Mesh.point(vd) + Mesh.point(vg) * 2) / 3);
			addFace(v[0], va, vi, vh);
			addFace(va, vb, vj, vi);
			addFace(vb, v[1], vc, vj);
			addFace(vc, vd, vk, vj);
			addFace(vd, v[2], ve, vk);
			addFace(ve, vf, vl, vk);
			addFace(vf, v[3], vg, vl);
			addFace(vg, vh, vi, vl);
			addFace(vi, vj, vk, vl);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//�o��u�O���F�ѨMindex���D
		/*----------------------------------------------------------------------*/
		return 0;
	}
	/*======================================================================*/
	int Model::findVertex(Point _p)
	{
		for (VIter v_itr = Mesh.vertices_begin(); v_itr != Mesh.vertices_end(); ++v_itr)
			if (Mesh.point(v_itr) == _p)
				return v_itr.handle().idx();
		return -1;
	}
	/*======================================================================*/
};
/*======================================================================*/
void Tri_Mesh::Render_Solid()
{
	FIter f_it;
	FVIter	fv_it;
	//glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_LIGHTING);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(0.81, 0.74, 0.33, 0.3);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			glNormal3dv(normal(fv_it.handle()).data());
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Tri_Mesh::Render_SolidWireframe()
{
	Render_Texture();

	FIter f_it;
	FVIter	fv_it;

	//faces
	glDisable(GL_LIGHTING);
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(0, 1, 0, 1.0);
	for (int i = 0; i < selectedFaces.size(); i++)
	{
		f_it = selectedFaces[i];
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
			glVertex3dv(point(fv_it.handle()).data());
	}

	glColor4f(1.0, 0.96, 0.49, 1.0);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			//glNormal3dv(normal(fv_it.handle()));
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();

	//glDisable(GL_POLYGON_OFFSET_FILL);

	//edges
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(), 1);

		OMT::Point curVertex = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());
	}
	glEnd();

	glPopAttrib();

	//if (open)
	//{
	//	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	//	Render_Texture();
	//	open = false;
	//	return;
	//}
}

void Tri_Mesh::Render_Wireframe()
{
	//glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);

	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINES);
	for (OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(), 1);

		OMT::Point curVertex = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());
	}
	glEnd();

}

void Tri_Mesh::Render_Point()
{
	glPointSize(8.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (OMT::VIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		glVertex3dv(point(v_it).data());
	}
	glEnd();
}

void Tri_Mesh::Render_Texture()
{
	if (open)
		SaveMesh();

	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int num;
	std::cout << "meshes size " << std::to_string(meshes.size()) << "\n";
	for (int i = 0; i < meshes.size(); i++)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, content1Texture[meshes[i].textureID]);
		num = 0;
		glBegin(GL_TRIANGLES);
		for (FVIter fv = fv_iter(meshes[i].face); fv; ++fv)
		{
			glTexCoord2d(meshes[i].pos[num][0], meshes[i].pos[num][1]);
			glVertex3dv(point(fv.handle()).data());
			num++;
		}
		glEnd();
		glPopMatrix();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	if (open)
	{
		Clean();
		open = false;
	}

	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
}

void Tri_Mesh::Render_UV()
{
	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	uv.clear();  //�M��

	FindBoundaryVertices();
	//return;

	Boundary_num = boundaryVertices.size();
	Constrain_num = innerVertices.size();

	if (Boundary_num == 0)
	{
		std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
		return;
	}

	uv.resize(Boundary_num + Constrain_num);

	std::cout << "boundary count " << std::to_string(boundaryVertices.size()) << std::endl;
	std::cout << "inner count " << std::to_string(innerVertices.size()) << std::endl;

	int id;
	glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(0.0, 0.0, 1.0, 1.0);
	double degree;

	if (Boundary_type == 1)
		degree = 360 / boundaryVertices.size();
	else
		degree = 4.0 / boundaryVertices.size();

	int pos;

	//follow index put in the uv map
	for (int i = 0; i < boundaryVertices.size(); i++)
	{
		pos = boundaryVertices[i];
		double x, y;
		if (Boundary_type == 1)
		{
			x = 0.5 + 0.5 * cosf(i * degree * PI / 180);
			y = 0.5 + 0.5 * sinf(i * degree * PI / 180);
		}
		else
		{
			if (i * degree < 1)
			{
				x = i * degree;
				y = 0;
			}
			else if (i * degree < 2)
			{
				x = 1;
				y = i * degree - 1.0;
			}
			else if (i * degree < 3)
			{
				x = 3.0 - i * degree;
				y = 1.0;
			}
			else if (i * degree <= 4)
			{
				x = 0;
				y = 4.0 - i * degree;
			}
		}

		uv[pos].pos[0] = x;
		uv[pos].pos[1] = y;
		uv[pos].vhandle = selectedVertices[pos];

		glColor4f(0.0, 1.0, 0.0, 1.0);

		Point position = point(selectedVertices[pos]);
		std::cout << std::to_string(pos) << " xyz pos " << std::to_string(position.data()[0]) << " " << std::to_string(position.data()[1]) << " " << std::to_string(position.data()[2]) << std::endl;
		std::cout << std::to_string(pos) << " uv pos " << std::to_string(uv[pos].pos[0]) << " " << std::to_string(uv[pos].pos[1]) << std::endl;
		glVertex2f(uv[pos].pos[0], uv[pos].pos[1]);
	}
	glEnd();

	CalculateUVPosition();

	//find all edge
	std::vector<EHandle> selectedEdge;
	HalfedgeHandle he;
	FEIter fe_it;
	for (int f_it = 0; f_it < selectedFaces.size(); ++f_it)
	{
		for (fe_it = fe_iter(selectedFaces[f_it].handle()); fe_it; ++fe_it)
		{
			//find boundary edge
			if(std::find(selectedEdge.begin(), selectedEdge.end(), fe_it.handle()) == selectedEdge.end())
				selectedEdge.push_back(fe_it.handle());
		}
	}

	//�e�u
	//edges
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	for (OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		//���P�_���u�O�_�s�b
		if (std::find(selectedEdge.begin(), selectedEdge.end(), e_it.handle()) == selectedEdge.end())
			continue;

		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(), 1);
		OMT::VHandle curVertex = from_vertex_handle(_hedge);
		OMT::VHandle nexVertex = to_vertex_handle(_hedge);
		//�P�_���I�O�_�s�b
		int cur = VertexToIndex(curVertex);
		int nex = VertexToIndex(nexVertex);
		if (cur != -1 && nex != -1)
		{
			//�e�u
			glBegin(GL_LINES);
			glVertex2f(uv[cur].pos[0], uv[cur].pos[1]);
			glVertex2f(uv[nex].pos[0], uv[nex].pos[1]);
			std::cout << "cur " << std::to_string(cur) << " " << std::to_string(uv[cur].pos[0]) << " " << std::to_string(uv[cur].pos[1]) << "; nex " << std::to_string(nex) << " " << std::to_string(uv[nex].pos[0]) << " " << std::to_string(uv[nex].pos[1]) << std::endl;
			glEnd();
		}
	}
	glPopAttrib();
}

void Tri_Mesh::setRenderTextrue(bool open)
{
	this->open = open;
}

void Tri_Mesh::Clean()
{
	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	selectedFaces.clear();
	selectedVertices.clear();
	boundaryVertices.clear();
	innerVertices.clear();
	uv.clear();
}

void Tri_Mesh::FindNearFace(GLdouble * pos)
{
	FIter f_it, min_f_it = faces_begin();
	FVIter	fv_it, fv_it_2;
	Point points[3], pointMouse, vertex;
	int i;
	bool found = false;

	pointMouse[0] = pos[0], pointMouse[1] = pos[1], pointMouse[2] = pos[2];

	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		double total_area = 0, area;

		for (fv_it = fv_iter(f_it), i = 0; fv_it; ++fv_it, i++)
			points[i] = point(fv_it.handle());

		total_area += CalculateArea(pointMouse, points[0], points[1]);
		total_area += CalculateArea(pointMouse, points[0], points[2]);
		total_area += CalculateArea(pointMouse, points[1], points[2]);

		area = CalculateArea(points[0], points[1], points[2]);

		if (abs(area - total_area) < 0.000001)
		{
			min_f_it = f_it;
			found = true;
			break;
		}
	}

	//������
	ptrdiff_t face = std::find(selectedFaces.begin(), selectedFaces.end(), min_f_it) - selectedFaces.begin();
	if (face >= selectedFaces.size() && found == true)
	{
		selectedFaces.push_back(min_f_it);
		std::cout << "size: " << std::to_string(selectedFaces.size()) << "\nnearest face" << std::endl;
		for (fv_it = fv_iter(min_f_it); fv_it; ++fv_it)
		{
			std::cout << std::to_string(point(fv_it.handle())[0]) << " " << std::to_string(point(fv_it.handle())[1]) << " " << std::to_string(point(fv_it.handle())[2]) << std::endl;
		}

	}
	else if (found)
	{
		std::cout << "cancel this face\n";
		selectedFaces.erase(selectedFaces.begin() + face);
	}
}

//�P�_�O�_�F��
bool Tri_Mesh::IsVertexVertex(VHandle _vj, VHandle _vi)
{
	for (VVIter vvit = vv_iter(_vi); vvit; ++vvit)
		if (vvit.handle() == _vj)
			return true;
	return false;
}

double Tri_Mesh::CalculateWeight(int origin, std::vector<int> neighbor, double* weights)
{
	double total_weight = 0;

	for (int i = 0; i < neighbor.size(); i++)
	{
		if (Weight_type == 0)
			weights[neighbor[i]] = 1;
		else if (Weight_type == 1)
		{
			int j = neighbor[(i + 1) % neighbor.size()];
			int e = neighbor[i % neighbor.size()];
			int k = neighbor[(i - 1) % neighbor.size()];

			double a = cot(point(selectedVertices[origin]), point(selectedVertices[e]), point(selectedVertices[k]));
			double b = cot(point(selectedVertices[origin]), point(selectedVertices[e]), point(selectedVertices[j]));

			weights[neighbor[i]] = a + b;
		}

		std::cout << std::to_string(*(weights+1)) << "\n";
		total_weight = total_weight + (*(weights + neighbor[i]));
	}
	//std::cout << "total weight " << std::to_string(total_weight) << std::endl;
	return total_weight;
}

void Tri_Mesh::CalculateUVPosition()
{
	SparseMatrix<double> A(Constrain_num, Constrain_num);
	std::vector<VectorXd> B;
	B.resize(2);

	B[0].resize(Constrain_num);
	B[1].resize(Constrain_num);

	for (int v = 0; v < Constrain_num; v++)
	{
		int vid = innerVertices[v];
		//����P���۾F�����I
		std::vector<int> neighbor;
		int id;
		for (VVIter vvit = vv_iter(selectedVertices[vid]); vvit; ++vvit)
		{
			id = VertexToIndex(vvit.handle());
			//�p�G�F�I�Oboundary
			if (std::find(boundaryVertices.begin(), boundaryVertices.end(), id) != boundaryVertices.end())
			{
				neighbor.push_back(id);
			}
			else if (std::find(innerVertices.begin(), innerVertices.end(), id) != innerVertices.end())
			{
				//�F�I�]�Oinner
				neighbor.push_back(id);
			}
		}

		//�p��������v��
		double* weights = new double[selectedVertices.size()];
		for (int i = 0; i < selectedVertices.size(); i++)
			weights[i] = 0;
		double total_wieght = CalculateWeight(vid, neighbor, weights);

		//��JA
		int innerID;
		for (int i = 0; i < Constrain_num; i++)
		{
			innerID = innerVertices[i];
			if (innerID == vid)
				A.insert(v, i) = total_wieght;
			else
				A.insert(v, i) = -weights[innerID];
		}
		//�p��B
		int neighborID;
		double sumU = 0.0, sumV = 0.0;
		for (int i = 0; i < Boundary_num; i++)
		{
			neighborID = boundaryVertices[i];
			sumU += (weights[neighborID] * uv[neighborID].pos[0]);
			sumV += (weights[neighborID] * uv[neighborID].pos[1]);
		}
		//��JB
		B[0][v] = sumU;
		B[1][v] = sumV;
		//std::cout << " sum U " << std::to_string(sumU) << std::endl;
		//std::cout << " sum V " << std::to_string(sumV) << std::endl;
	}

	//debug A
	//for (int i = 0; i < Constrain_num; i++)
	//{
	//	for (int j = 0; j < Constrain_num; j++)
	//		std::cout << std::to_string(A.data().at(0,0)) << " ";
	//	std::cout << "\n";
	//}
	//end debug


	A.makeCompressed();

	SparseQR<SparseMatrix<double>, COLAMDOrdering<int>> linearSolver;
	linearSolver.compute(A);

	std::vector<VectorXd> X;
	X.resize(2);

	X[0] = linearSolver.solve(B[0]);
	X[1] = linearSolver.solve(B[1]);

	int id;
	glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < Constrain_num; i++)
	{
		id = innerVertices[i];
		uv[id].pos[0] = X[0][i];
		uv[id].pos[1] = X[1][i];
		uv[id].vhandle = selectedVertices[id];
		std::cout << std::to_string(id) << " uv pos " << std::to_string(uv[id].pos[0]) << std::to_string(uv[id].pos[1]) << std::endl;
		glVertex2f(uv[id].pos[0], uv[id].pos[1]);
	}
	glEnd();

	//LinearSolve();
}

int Tri_Mesh::VertexToIndex(VHandle vh)
{
	ptrdiff_t pos = std::find(selectedVertices.begin(), selectedVertices.end(), vh) - selectedVertices.begin();

	if (pos >= selectedVertices.size())
		return -1;
	else
		return pos;

	return pos;
}

//void Tri_Mesh::LinearSolve()
//{
//	SparseMatrix<double> A(1, 1);
//	A.insert(0, 0) = 6.0;
//
//	A.makeCompressed();
//
//	std::vector<VectorXd> B;
//	B.resize(2);
//
//	B[0].resize(1);
//	B[1].resize(1);
//
//	B[0][0] = 0.0;
//	B[1][0] = 0.0;
//
//	SparseQR<SparseMatrix<double>, COLAMDOrdering<int>> linearSolver;
//	linearSolver.compute(A);
//
//	std::vector<VectorXd> X;
//	X.resize(2);
//	X[0] = linearSolver.solve(B[0]);
//	X[1] = linearSolver.solve(B[0]);
//
//	std::cout << "test\n";
//	std::cout << std::to_string(X[0][0]) << std::endl;
//	std::cout << std::to_string(X[1][0]) << std::endl;
//}

void Tri_Mesh::SaveMesh()
{
	std::cout << __FUNCTION__ << "(" << __LINE__ << ") texture id " << std::to_string(content1Texture.size() - 1) << "\n";
	//�ˬd�O�_�O���L
	bool found;
	int num;
	for (int i = 0; i < selectedFaces.size(); i++)
	{
		found = false; num = 0;
		for (int j = 0; j < meshes.size(); j++)
		{
			if (meshes[j].face == selectedFaces[i].handle())
			{
				found = true;
				meshes[j].textureID = content1Texture.size() - 1;
				break;
			}
		}

		if (!found)
		{
			//�Ы�
			Mesh new_mesh;
			new_mesh.face = selectedFaces[i].handle();

			for (FVIter fv = fv_iter(selectedFaces[i]); fv; ++fv)
			{
				for (int v = 0; v < uv.size(); v++)
				{
					if (uv[v].vhandle == fv.handle())
					{
						new_mesh.pos[num][0] = uv[v].pos[0];
						new_mesh.pos[num][1] = uv[v].pos[1];
						num++;
						break;
					}
				}
			}

			new_mesh.textureID = content1Texture.size() - 1;
			meshes.push_back(new_mesh);
		}

	}

	//std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	//Clean();
}

//load texture
void Tri_Mesh::LoadTexture(char * filepath)
{
	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	GLint texture_id = TextureApp::GenTexture(filepath);
	if (texture_id != 0)
	{
		content1Texture.push_back(texture_id);
		std::cout << __FUNCTION__ << "(" << __LINE__ << ") texture size " << std::to_string(content1Texture.size()) << std::endl;
	}
}

int EdgeToIndex(std::vector<Tri_Mesh::Model::EHandle> contain, Tri_Mesh::Model::EHandle obj)
{
	ptrdiff_t pos = std::find(contain.begin(), contain.end(), obj) - contain.begin();

	if (pos >= contain.size())
		return -1;
	else
		return pos;

	return pos;
}

void Tri_Mesh::FindBoundaryVertices()
{
	std::cout << __FUNCTION__ << "(" << __LINE__ << ")\n";
	boundaryVertices.clear();
	innerVertices.clear();
	selectedVertices.clear();
	//find all edges and vertex
	std::vector<EHandle> selectedEdge;
	HalfedgeHandle he;
	VHandle first, end;
	FEIter fe_it;
	int pos;
	for (int f_it = 0; f_it < selectedFaces.size(); ++f_it)
	{
		for (fe_it = fe_iter(selectedFaces[f_it].handle()); fe_it; ++fe_it)
		{
			//find boundary edge
			pos = EdgeToIndex(selectedEdge, fe_it.handle());
			if (pos == -1)
				selectedEdge.push_back(fe_it.handle());
			else
				selectedEdge.erase(selectedEdge.begin() + pos);

			//find vertex
			he = halfedge_handle(fe_it.handle(), 0);
			first = from_vertex_handle(he), end = to_vertex_handle(he);

			if (std::find(selectedVertices.begin(), selectedVertices.end(), first) == selectedVertices.end())
				selectedVertices.push_back(first);
			if (std::find(selectedVertices.begin(), selectedVertices.end(), end) == selectedVertices.end())
				selectedVertices.push_back(end);
		}
	}

	std::cout << "boundary edge size " << std::to_string(selectedEdge.size()) << std::endl;
	std::cout << "vertices size " << std::to_string(selectedVertices.size()) << std::endl;
	/***********************/

	//find boundary and sort
	pos = 0;
	he = halfedge_handle(selectedEdge[pos], 0);
	first = from_vertex_handle(he), end = to_vertex_handle(he);
	boundaryVertices.push_back(VertexToIndex(first));
	//std::cout << "boundary index " << std::to_string(VertexToIndex(first)) << std::endl;
	boundaryVertices.push_back(VertexToIndex(end));
	//std::cout << "boundary index " << std::to_string(VertexToIndex(end)) << std::endl;

	bool found;
	int num = selectedEdge.size(), id;
	while (boundaryVertices.size() < num)
	{
		found = false;
		selectedEdge.erase(selectedEdge.begin() + pos);
		for (int i = 0; i < selectedEdge.size(); i++)
		{
			for (int type = 0; type < 2; type++)
			{
				he = halfedge_handle(selectedEdge[i], type);
				if (end == from_vertex_handle(he))
				{
					//first = end;
					end = to_vertex_handle(he);
					pos = i;
					id = VertexToIndex(end);
					if (std::find(boundaryVertices.begin(), boundaryVertices.end(), id) == boundaryVertices.end())
					{
						boundaryVertices.push_back(id);
						//num++;
						found = true;
						//std::cout << "boundary index " << std::to_string(id) << std::endl;
						//std::cout << "boundary size " << std::to_string(boundaryVertices.size()) << std::endl;
					}
					break;
				}
			}
			if (found)
				break;
		}
	}
	std::cout << "boundary size " << std::to_string(boundaryVertices.size()) << std::endl;

	//find inner
	for (int i = 0; i < selectedVertices.size(); i++)
	{
		if (std::find(boundaryVertices.begin(), boundaryVertices.end(), i) == boundaryVertices.end())
			innerVertices.push_back(i);
	}
	std::cout << "inner size " << std::to_string(innerVertices.size()) << std::endl;

#pragma region old method
	//FVIter	fv_it;
	////��������I
	//for (int f_it = 0; f_it < selectedFaces.size(); ++f_it)
	//{
	//	for (fv_it = fv_iter(selectedFaces[f_it]); fv_it; ++fv_it)
	//	{
	//		//�������I
	//		if (std::find(selectedVertices.begin(), selectedVertices.end(), fv_it.handle()) == selectedVertices.end())
	//		{
	//			selectedVertices.push_back(fv_it.handle());
	//		}
	//	}
	//}

	////�M��boundary
	//bool found;
	//for (int v = 0; v < selectedVertices.size(); v++)
	//{
	//	found = false;
	//	//���I���Ҧ��F�I
	//	for (VVIter vvit = vv_iter(selectedVertices[v]); vvit; ++vvit)
	//	{
	//		if (std::find(selectedVertices.begin(), selectedVertices.end(), vvit.handle()) == selectedVertices.end())
	//		{
	//			//���F�I���bvector�̭��A�Oboundary
	//			//std::cout << "boundary " << std::to_string(v) << std::endl;
	//			boundaryVertices.push_back(v);
	//			found = true;
	//			break;
	//		}
	//	}
	//	//�����I
	//	if (!found)
	//	{
	//		//std::cout << "inner " << std::to_string(v) << std::endl;
	//		innerVertices.push_back(v);
	//	}
	//}
#pragma endregion
}

bool ReadFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if (OpenMesh::IO::read_mesh(*_mesh, _fileName, opt))
	{
		//read mesh from filename OK!
		isRead = true;
	}
	if (isRead)
	{
		// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
		if (!opt.check(OpenMesh::IO::Options::VertexNormal) && _mesh->has_vertex_normals())
		{
			_mesh->update_normals();
		}
	}
	return isRead;
}

bool SaveFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isSave = false;
	if (OpenMesh::IO::write_mesh(*_mesh, _fileName))
	{
		isSave = true;
	}
	return isSave;
}
