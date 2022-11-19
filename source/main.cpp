#include "../include/mainwindow.h"

Atoms *AT;
Lattice *LATT;
Internal *INT;
Points *POINTS;
#include "../FEMApp//FEMLib/IntegrationManager.h"
#include "../FEMApp/FEMLib/IntegrationPoints.h"
#include "../FEMApp/FEMAppLib/FEMProject.h"

extern "C"
{
    int int3d_(int *l, int *lint, double *sg);
}

extern struct {
    double x;
    int a, b, c;
} abc_;

CIntegrationManager IM;

double sg8[8][4], sg27[27][4], sg64[64][4];

void CreateInfMatrices() {
    const CIntegrationPoints *ip8 = IM.GetIntegrationPoints(0,3,1);
    const CIntegrationPoints *ip27 = IM.GetIntegrationPoints(0,3,2);
    const CIntegrationPoints *ip64 = IM.GetIntegrationPoints(0,3,3);

    unsigned k;
    for ( k=0; k<ip8->GetWeights().size(); k++)
    {
        sg8[k][3] = ip8->GetWeight(k);
        sg8[k][0] = ip8->GetKsi(k)[0];
        sg8[k][1] = ip8->GetKsi(k)[1];
        sg8[k][2] = ip8->GetKsi(k)[2];
    }

    for ( k=0; k<ip27->GetWeights().size(); k++)
    {
        sg27[k][3] = ip27->GetWeight(k);
        sg27[k][0] = ip27->GetKsi(k)[0];
        sg27[k][1] = ip27->GetKsi(k)[1];
        sg27[k][2] = ip27->GetKsi(k)[2];
    }

    for ( k=0; k<ip64->GetWeights().size(); k++)
    {
        sg64[k][3] = ip64->GetWeight(k);
        sg64[k][0] = ip64->GetKsi(k)[0];
        sg64[k][1] = ip64->GetKsi(k)[1];
        sg64[k][2] = ip64->GetKsi(k)[2];
    }

}

int int3d_(int *l, int *lint, double *sg)
{
    unsigned map[27] = { 0, 2, 8, 6, 18, 20, 26, 24, 1, 5, 7, 3, 19, 23,
25, 21, 9, 11, 17, 15, 4, 22, 12, 14, 10, 16, 13 };

  unsigned k,m;
  switch (*l)
    {
    case 2 :
      *lint = 8;
      for (k=0; k<8; k++)
    for(m=0; m<4; m++)
      sg[ k*4+m] = sg8[k][m];
      break;
    case 3 :
      *lint = 27;
      for (k=0; k<27; k++)
    for(m=0; m<4; m++)
      sg[ k * 4 + m  ] = sg27[ map[ k ] ][ m ];
      break;
    case 4 :
      *lint = 64;
      for (k=0; k<64; k++)
    for(m=0; m<4; m++)
      sg[ k * 4 + m  ] = sg64[k][m];
      break;
    }
}

int main(int argc, char *argv[])
{
  //Q_INIT_RESOURCE(stylesheet);

  QApplication app(argc, argv);
  if ( ( QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5)==0 ) {
       QMessageBox::critical(0, "OpenGL features missing", "OpenGL version 1.5 or higher is required to run this program.\n",  "The program will now exit.");
       return -1;
  }

  CreateInfMatrices();

  QLocale::setDefault(QLocale::C);
  
  AT = new Atoms();
  INT = new Internal();
  LATT = new Lattice();
  POINTS = new Points();  
  
  if ( argc>1 ) INT->atName = std::string(argv[1]);

  MainWindow mainwindow;
//  mainwindow.setStyleSheet("padding: 0px;");// Display that context on screen:
  mainwindow.show ();

  return app.exec ();	
}

