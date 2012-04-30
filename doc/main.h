

/**
 * @mainpage Visual Editor of Crystal Defects (vecds)
 *
 * @section Overview
 *
 * This is the main page for class and function technical
 * documentation for the <b>vecds</b> application. This documentation
 * is mainly intended for developers of <b>vecds</b> and others with a
 * general interest in what happens inside the guts of this
 * application. 
 * 
 * For the user's documentation, see elsewhere.
 *
 * The application <b>vecds</b> is distributed under the GNU General
 * Public License version three (GPLv3) and hosted by <a
 * href="http://sourceforge.net">sourceforge</a> at <a
 * href="http://vecds.sourceforge.net">this home page</a> and is <a
 * href="http://sourceforge.net/projects/vecds/">detailed here</a>.
 *
 * @section Authors 
 *
 * <ul> <li>Jan Cholewinski: Institute of Fundamental Technological
 * Research of the Polish Academy of Sciences.
 *
 * <li><a href="http://www.ippt.gov.pl/~pdluzew">Pawel Dluzewski</a>:
 * Institute of Fundamental Technological Research of the Polish
 * Academy of Sciences. </ul>
 *
 * @section Contributors
 *
 * <ul><li><a href="http://www.ippt.gov.pl/~tyoung">Toby D. Young</a>:
 * Various bug fixes and code
 * enhancments. Documentation. Configuration issues and porting. </ul>
 *
 * @section Acknowledgments
 *
 * <ul><li>The development of <b>vecds</b> is a recipient of the
 * development project R15 01203 (2007-2010) founded by the Polish
 * Ministry of Science and Higher Education.
 *
 * <li> The <b>vecds</b> application was established under the
 * Marie-Curie Research Training Network supported by the European
 * Communitiy's Sixth Framework Programme, European Commision: PARSEM
 * MRTN-CT-2004-005583 (2005-2009).</ul>
 *
 * @section FAQ
 *
 * <ul> <li>How is <b>vecds</b> pronouced?
 *
 * The name of this application is pronounced, "vee-see-dees".
 *
 * <li> How do I install <b>vecds</b> on system x?
 *
 * The configure system for <b>vecds</b> is under development. If you
 * really need <b>vecds</b> working on your system, but it is not
 * listed here or it just does not work for you, then the best thing
 * to do is flame <a href="http://www.ippt.gov.pl/~tyoung">Toby
 * D. Young</a> and lobby him to get the damned thing working!
 *
 * In any case the procedure is mostly the same for *NIX based
 * systems. First up is to make sure that Qt >= 4.6.2 and Qwt >= 5.0.0
 * are installed either locally or on your system. Second, run the
 * configure-build script with <tt>qmake</tt> and <tt>make</tt>.
 *
 * <ul><li>Linux >= 2.6.34 <b>Standard</b>.
 *
 * Installing Qt and Qwt on your Linux box is highly dependent on
 * which distribution of choice you are using. At the moment, the
 * authors and main developers have at least one gentoo distribution,
 * so let's started with that.
 *
 * Getting and installing Qt and Qwt is done by running <tt>emerge
 * -avD qt qwt</tt>. Following that, <b>vecds</b> is built by running
 * in sequence: <tt>qmake vecds.pro</tt>, then <tt>make</tt>.
 *
 * If nothing extraordinary happens, the <b>vecds</b> excecutable can
 * be found here: <tt>bin/vecds</tt>.
 *
 * <li>Mac OS X >= 10.5 (Leopard, Snow Leopard) <b>Experimental</b>.
 *
 * The easiest way to install Qt on your Mac is to grab the correct Qt
 * version as a <tt>.dmg</tt> image from <a
 * href="http://qt.nokia.com/downloads/qt-for-open-source-cpp-development-on-mac-os-x/">qt.nokia</a>
 * and follow the built-in instructions on installation.
 *
 * Second is an installation of Qwt. For this, download the source
 * from <a href="http://qwt.sourceforge.net/">qwt.sourceforge</a> and
 * unpack that somewhere. In the Qwt directory run the following in
 * sequence: <tt>qmake -spec macx-g++</tt> to build the Qwt makefiles,
 * <tt>make</tt> and then (optionally) <tt>make install</tt>.
 *
 * Finally, after returning to your <b>vecds</b> directory, run in
 * sequence: <tt>qmake -spec macx-g++ macosx.pro</tt> to build the
 * makefiles with Mac OS X dylibs. Then, as always, <tt>make</tt> to
 * build <b>vecds</b>.
 *
 * If all went ok, the <b>vecds</b> <tt>.app</tt> application for Mac
 * OS X can be found here:
 * <tt>bin/vecds.app/Contents/MacOS/vecds</tt>.  </ul>
 *
 * @section Publications
 *
 * Here we list scientific/research based publications written with
 * the help of <b>vecds</b>. 
 *
 * <ul><li> ?
 * </ul>
 *
 * @section Presentations
 *
 * Here we list scientific/research based presenttions at seminars
 * and/or conferences that were written with the help of <b>vecds</b>.
 *
 * <ul><li> P. Dluzewski, J. Cholewinski, M. Maz... Kinga, CMM (2011).
 * </ul>
 *
 */
