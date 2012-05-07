
// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------
	

// These are Qt headers:	 
#include <QApplication>
#include <QGLFormat>

// These are vecds headers:
#include "../include/vecds/main_window.h"


// This is the main code that wraps everything into an application.
int main (int argc, char *argv[])
{
  
  // Error check for OpenGl. If this does not exist throw a
  // warning/exception:
  
  // if (!QGLFormat::hasOpenGL ()) 
  //   {
  //     qWarning ("This system does not support OpenGL.");
  //     qWarning ("You should install OpenGL libraries");
  
  //     return -1;
  //   }
  // QGL::setPreferredPaintEngine (QPaintEngine::OpenGL);
  
  
  // Invoke a Qt application context:
  QApplication app (argc, argv);
  
  // Invoke the graphical context for the main window of the
  // application:
  MainWindow window;
  
  // Display that context on screen:
  window.show ();
  
  // Return value:
  return app.exec ();
}
