
// -------------------------------------------------------------------
//
// Author:
//    Jan Cholewinski 2010, 2011.
//    Toby D. Young 2012.
//
// Copyright (C) 2010, 2012 The vecds authors
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
	

                                 // These are Qt headers.
#include <QApplication>
#include <QGLFormat>

                                 // vecds headers.
#include <vecds/main_window.h>


                                 // This is the main code that wraps
                                 // everything into an application.
int main (int argc, char *argv[])
{  

                                 // Invoke a Qt application context
  QApplication app (argc, argv);

                                 // try to go ahead with the
                                 // application.
  try
    {
  
                                 // Invoke the graphical context for
                                 // the main window of the
                                 // application.
      MainWindow window;
  
                                 // Display that context on screen.
      window.show ();
    }

                                 // try to catch known errors.
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what () << std::endl
                << std::endl;

      return 1;
    }

                                 // and unknown errors.
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << std::endl;
      return 1;
    }

                                 // Return value.
  return app.exec ();

}
