#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>

using namespace std;

struct Atoms
{ Atoms () { n_atoms = 0;}
  int n_atoms; 
  glm::dvec3* coord;
  glm::dvec3* du;
  glm::dvec3* u;

  string* a_name;
  int* type;
  glm::dmat3* beta;
};

struct miller
{
  double fraction;
  double indices[6];
};

struct CrysCell
{
  string struct_name;
  double a, b, c;
  double alpha, beta, gamma;
  glm::dmat3 c2o;
  glm::dmat3 o2c;
};

bool read_xyz(string aname);
void extPrint(string file_name, int nvec, glm::dvec3 *vec);
void extPrintM(string file_name, int nvec, glm::dmat3 *mat);
void atoms1Print(string file_name, glm::dvec3 *vec);
void atoms2Print(string file_name, glm::dvec3 *vec1, glm::dvec3 *vec2);
bool readProgData(string);
int Love_function(const gsl_vector *x, void *par, gsl_vector *result_funct);
void singledisl(int n_a);
void multdisl(int n_a, int num_rep, double distance);
void singledisl0(int n_a, int count);
void multdisl0(int n_a, int num_rep, double distance);
glm::dvec3 mixed_u(int, glm::dvec3, double, double);
glm::dmat3 mixed_beta(int, glm::dvec3, double, double);
void processMiller(string result_text);
glm::dmat3 rotation_tensor();


string get_working_path()
{
  int MAXPATHLEN = 1024;
  char temp[MAXPATHLEN];
  return getcwd (temp, MAXPATHLEN) 
    ? 
    string(temp) 
    : 
    string("");
}

string stripBlanks(string StringToModify)
{
   if ( StringToModify.empty() ) 
     return "";

   size_t startIndex = StringToModify.find_first_not_of(" ");
   size_t endIndex = StringToModify.find_last_not_of(" ");
   string tempString = StringToModify;
   tempString = tempString.substr(startIndex, endIndex-startIndex+1);

   return tempString;
}

double read_fraction(string line) // czyta ułamek np. w zapisie wektora Burgersa, przelicza na liczbę dziesiętną
{
  line = stripBlanks(line);
  size_t i_fr = line.find('/');

  istringstream ins;
  ins.clear();
  ins.str(line);
  double result, temp;
  char c;
  if ( i_fr==string::npos ) 
    ins >> result;
  else 
    {
      ins >> result >> c >> temp;
      result /= temp;
    }
  return result;
}

void printVec (string str, glm::dvec3 vec)
{
  cout << str << " =  (" << setw(15) << vec.x << setw(15) << vec.y << setw(15) << vec.z << ")" << endl;
}

void printMat (string str, glm::dmat3 m)
{
  cout << str << "    det = " << glm::determinant(m) << endl;
  cout << " row 0 = " << setw(15) << m[0][0] << setw(15) << m[1][0] << setw(15) << m[2][0] << endl;
  cout << " row 1 = " << setw(15) << m[0][1] << setw(15) << m[1][1] << setw(15) << m[2][1] << endl;
  cout << " row 2 = " << setw(15) << m[0][2] << setw(15) << m[1][2] << setw(15) << m[2][2] << endl;
}

int identify (string s1, int size, string words[])
{
  for (int i=0; i<size; i++) 
    if ( s1.compare(words[i])==0 ) 
      return i;

  return -1;
}

vector<string> tokenize (const string& str, string del)
{
  vector<string> tokens;
  string delimiters = del;
  size_t lastPos = str.find_first_not_of(delimiters, 0);            // Skip delimiters at beginning.
  size_t pos = str.find_first_of(delimiters, lastPos);              // Find first "non-delimiter".
  tokens.clear();
  while ( pos!=string::npos || lastPos!=string::npos ) 
    {
      tokens.push_back(str.substr(lastPos, pos-lastPos));            // Found a token, add it to the vector.
      lastPos = str.find_first_not_of(delimiters, pos);              // Skip delimiters. Note the "not_of"
      pos = str.find_first_of(delimiters, lastPos);                  // Find next "non-delimiter"
    }

  return tokens;
}

int toInt (string word)
{
  int i;
  istringstream ins;
  ins.clear();
  ins.str(word);
  ins >> i;
  return i;
}

double toDouble(string word)
{
  double x;
  istringstream ins;
  ins.clear();
  ins.str(word);
  ins >> x;
  return x;
}

bool internal_miller(string line2, int which, int *mil)//spradzenie poprawności danych i zamiana układu cztero na trójwsk.
{
  int numbmill;
  vector<string> fields;
  numbmill = fields.size();
  if ( line2.find_first_of(" ,")!=string::npos ) 
    {
      vector<string> fields = tokenize(line2, " ,");
      numbmill = fields.size();
      if ( numbmill<3 || numbmill>4 ) 
	{
	  cout << "ERROR - BAD NUMBER OF MILLER INDICES 1\n";
	  return false;
	}
      for (int i=0; i<numbmill; i++) mil[i] = toInt(fields.at(i));
    } 
  else 
    {
      bool minus = false;
      numbmill = 0;
      size_t i = 0;
      while ( i<line2.size() ) 
	{
	  char ch = line2.at(i++);
	  if ( ch=='-' ) 
	    {
	      minus = true;
	      continue;
	    }
	  if ( isdigit(ch) ) 
	    {
	      int n = int(ch)-int('0');
	      mil[numbmill++] = minus? -n : n; 
	      minus = false;
	    }
	  if ( ch=='.' && numbmill==2 ) mil[numbmill++] = -mil[0]-mil[1];
	}
      if ( numbmill<3 || numbmill>4 ) 
	{
	  cout << "ERROR - BAD NUMBER OF MILLER INDICES 2\n";
	  return false;}
    } //if - else
  if ( numbmill==4 ) 
    {
      if ( (mil[0]+mil[1])!=-mil[2] )
	cout << "ERROR BAD SUM - " << mil[0] << "  " << mil[1] << "  " << mil[2] << endl;
      if ( which==1 ) 
	{
	  mil[0] -= mil[2];
	  mil[1] -= mil[2];
	}
      mil[2] = mil[3];
    }

  return true;
}

miller parse_miller(string line)
{
  miller result;
  result.fraction = 0;

  int mil[4];
  int indMiller[6];
  double fract;
  string line1;
  line1 = stripBlanks(line);

  size_t i_left1 = line1.find('[');
  size_t i_right1 = line1.find(']');
  size_t i_left2 = line1.find('(');
  size_t i_right2 = line1.find(')');
  fract = (i_left1==string::npos)? 1. : read_fraction(line1.substr(0, i_left1));
  if ( i_left1>=i_right1 || i_left2>=i_right2 ) 
    {// error
      cout << "ERROR PARENTHESES\n";
      return result;
    }
  string line2 = stripBlanks(line1.substr(i_left1+1, i_right1-i_left1-1));

  if ( internal_miller(line2, 1, mil) ) 
    {
      indMiller[0] = mil[0];
      indMiller[1] = mil[1];
      indMiller[2] = mil[2];
    } 
  else 
    { 
      cout << "ERROR - BAD MILLER INDICES 1\n";
      return result;
    }

  line2 = stripBlanks(line1.substr(i_left2+1, i_right2-i_left2-1));

  if ( internal_miller(line2, 2, mil) ) 
    {
      indMiller[3] = mil[0];
      indMiller[4] = mil[1];
      indMiller[5] = mil[2];
    } 
  else 
    {
      cout << "ERROR - BAD MILLER INDICES 2\n";
      return result;
    }

  result.fraction = fract;
  result.indices[0] = double(indMiller[0]);
  result.indices[1] = double(indMiller[1]);
  result.indices[2] = double(indMiller[2]);
  result.indices[3] = double(indMiller[3]);
  result.indices[4] = double(indMiller[4]);
  result.indices[5] = double(indMiller[5]);

  return result;
}

