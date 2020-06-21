/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2020, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include <mrpt/graphs.h>
//#include <mrpt/graphslam/levmarq.h>
#include <mrpt/gui.h>
#include <mrpt/opengl/CGridPlaneXY.h>
#include <mrpt/opengl/CPointCloud.h>
#include <mrpt/opengl/CSetOfLines.h>
#include <mrpt/opengl/CSetOfObjects.h>
#include <mrpt/opengl/CSimpleLine.h>
#include <mrpt/opengl/graph_tools.h>
#include <mrpt/opengl/stock_objects.h>
#include <mrpt/serialization/CArchive.h>
#include <mrpt/system/CObserver.h>

using namespace mrpt;
using namespace mrpt::graphs;
using namespace mrpt::poses;
using namespace mrpt::opengl;
using namespace mrpt::system;
using namespace mrpt::math;
using namespace std;

const char* MSG_HELP_WINDOW =
	"These are the supported commands:\n"
	" - h: Toogle help view\n"
	" - i: Switch view/hide node IDs\n"
	" - +/-: Increase/reduce size of node points\n"
	" - e: Switch view/hide edges\n"
	" - c: Switch view/hide node corners\n"
	" - o/p: Increase/reduce length of node corners\n"
	" - v: Switch view/hide edge pose values\n"
	" - k/l: Inc/red. length of edge pose corners\n"
	" - s: Save 3D scene to file\n"
	" - Alt+Enter: Toogle fullscreen\n"
	" - q: Quit\n";

/**
 * display_graph
 *
 * function template for displaying a Graph (most commonly a CNetworkOfPoses
 * instance)
 */
template <class GRAPHTYPE>
void display_graph(const GRAPHTYPE& g)
{
	// Convert into a 3D representation:
	TParametersDouble params;
	CSetOfObjects::Ptr objGraph =
		mrpt::opengl::graph_tools::graph_visualize(g, params);

	// Show in a window:
	mrpt::gui::CDisplayWindow3D win(
		"graph-slam - Graph visualization", 700, 600);

	// Set camera pointing to the bottom-center of all nodes.
	//  Since a CGridPlaneXY object is generated by graph_visualize()
	//  already centered at the bottom, use that information to avoid
	//  recomputing it again here:
	win.setCameraElevationDeg(75);
	{
		opengl::CGridPlaneXY::Ptr obj_grid =
			objGraph->CSetOfObjects::getByClass<CGridPlaneXY>();
		if (obj_grid)
		{
			float x_min, x_max, y_min, y_max;
			obj_grid->getPlaneLimits(x_min, x_max, y_min, y_max);
			const float z_min = obj_grid->getPlaneZcoord();
			win.setCameraPointingToPoint(
				0.5 * (x_min + x_max), 0.5 * (y_min + y_max), z_min);
			win.setCameraZoom(
				2.0f * std::max(10.0f, std::max(x_max - x_min, y_max - y_min)));
		}
	}

	mrpt::opengl::COpenGLScene::Ptr& scene = win.get3DSceneAndLock();
	scene->insert(objGraph);

	win.unlockAccess3DScene();
	win.repaint();

	// Create a feedback class for keystrokes:
	struct Win3D_observer : public mrpt::system::CObserver
	{
		const GRAPHTYPE& m_graph;
		CSetOfObjects::Ptr m_new_3dobj;
		TParametersDouble params;  // for "graph_visualize()"

		bool request_to_refresh_3D_view;
		bool request_to_quit;

		bool showing_help, hiding_help;
		mrpt::system::CTicTac tim_show_start, tim_show_end;

		Win3D_observer(const GRAPHTYPE& g)
			: m_graph(g),
			  request_to_refresh_3D_view(false),
			  request_to_quit(false),
			  showing_help(false),
			  hiding_help(false)
		{
			params["show_edges"] = 1;
			params["show_node_corners"] = 1;
			params["nodes_corner_scale"] = 0.7;
			params["nodes_edges_corner_scale"] = 0.4;
		}

	   protected:
		/** This virtual function will be called upon receive of any event after
		 * starting listening at any CObservable object. */
		void OnEvent(const mrptEvent& e) override
		{
			if (e.isOfType<mrpt::gui::mrptEventWindowChar>())
			{
				const auto* ev = e.getAs<mrpt::gui::mrptEventWindowChar>();
				bool rebuild_3d_obj = false;
				switch (ev->char_code)
				{
					case 'h':
					case 'H':
						std::cout << MSG_HELP_WINDOW << "\n";
						break;
					case 'q':
					case 'Q':
						request_to_quit = true;
						break;
					case 'i':
					case 'I':
						params["show_ID_labels"] =
							params["show_ID_labels"] != 0 ? 0 : 1;
						rebuild_3d_obj = true;
						break;
					case 'e':
					case 'E':
						params["show_edges"] =
							params["show_edges"] != 0 ? 0 : 1;
						rebuild_3d_obj = true;
						break;
					case 'c':
					case 'C':
						params["show_node_corners"] =
							params["show_node_corners"] != 0 ? 0 : 1;
						rebuild_3d_obj = true;
						break;
					case 's':
					case 'S':
					{
						static int cnt = 0;
						const std::string sFil =
							mrpt::format("dump_graph_%05i.3Dscene", ++cnt);
						std::cout << "Dumping scene to file: " << sFil
								  << std::endl;

						mrpt::opengl::COpenGLScene scene;
						scene.insert(m_new_3dobj);
						mrpt::io::CFileGZOutputStream f(sFil);
						mrpt::serialization::archiveFrom(f) << scene;
					}
					break;
					case 'v':
					case 'V':
						params["show_edge_rel_poses"] =
							params["show_edge_rel_poses"] != 0 ? 0 : 1;
						rebuild_3d_obj = true;
						break;
					case '+':
						params["nodes_point_size"] += 1.0;
						rebuild_3d_obj = true;
						break;
					case '-':
						params["nodes_point_size"] =
							std::max(0.0, params["nodes_point_size"] - 1.0);
						rebuild_3d_obj = true;
						break;
					case 'o':
					case 'O':
						params["nodes_corner_scale"] *= 1.2;
						rebuild_3d_obj = true;
						break;
					case 'p':
					case 'P':
						params["nodes_corner_scale"] *= 1.0 / 1.2;
						rebuild_3d_obj = true;
						break;
					case 'k':
					case 'K':
						params["nodes_edges_corner_scale"] *= 1.2;
						rebuild_3d_obj = true;
						break;
					case 'l':
					case 'L':
						params["nodes_edges_corner_scale"] *= 1.0 / 1.2;
						rebuild_3d_obj = true;
						break;
				};

				if (rebuild_3d_obj)
				{
					m_new_3dobj = mrpt::opengl::graph_tools::graph_visualize(
						m_graph, params);
					request_to_refresh_3D_view = true;
				}
			}
		}
	};
	// Create an instance of the observer:
	Win3D_observer win_feedback(g);
	// and subscribe to window events,
	win_feedback.observeBegin(win);
	// and openglviewport events:
	{
		COpenGLScene::Ptr& theScene = win.get3DSceneAndLock();
		opengl::COpenGLViewport::Ptr the_main_view =
			theScene->getViewport("main");
		win_feedback.observeBegin(*the_main_view);
		win.unlockAccess3DScene();
	}

	win.addTextMessage(0.8, 0.94, "Press 'h' for help");

	cout << "Close the window to exit.\n";
	while (win.isOpen())
	{
		if (win_feedback.request_to_refresh_3D_view)
		{
			// Replace 3D representation:
			win.get3DSceneAndLock();
			*objGraph = *win_feedback.m_new_3dobj;  // This overwrites the
			// object POINTER BY the
			// smart pointer "objGraph".
			win.unlockAccess3DScene();
			win.repaint();
			win_feedback.request_to_refresh_3D_view = false;
		}
		if (win_feedback.request_to_quit) break;
		if (win_feedback.showing_help || win_feedback.hiding_help)
		{
			win.repaint();
		}
		std::this_thread::sleep_for(10ms);
	}
}

// Explicit instantiations:
template void display_graph<CNetworkOfPoses2D>(const CNetworkOfPoses2D& g);
template void display_graph<CNetworkOfPoses3D>(const CNetworkOfPoses3D& g);
template void display_graph<CNetworkOfPoses2DInf>(
	const CNetworkOfPoses2DInf& g);
template void display_graph<CNetworkOfPoses3DInf>(
	const CNetworkOfPoses3DInf& g);
