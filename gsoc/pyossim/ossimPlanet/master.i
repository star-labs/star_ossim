/*
*/

%module pyplanet

/* This tells SWIG to treat char ** as a special case */


%typemap (in) void*
{
    $1 = PyCObject_AsVoidPtr($input);
}


%typemap(out) ossim_uint8* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}
/*
%apply char **STRING_ARRAY { char* argv[] };
%apply short[] {ossim_sint16 *};
%apply int[] {ossim_sint32 *};
%apply int[] {ossim_int32 *};
%apply int[] {int *};
%apply unsigned int[] {ossim_uint32 *};
%apply unsigned short[] {ossim_uint16 *};
%apply float[] {ossim_float32 *};
%apply double[] {ossim_float64 *};
%apply unsigned char[]  { ossim_uint8 * }
%apply char* BYTE  { ossim_int8 * }


typedef char                   ossim_int8;
typedef unsigned char          ossim_uint8;
typedef signed char            ossim_sint8;

typedef short                  ossim_int16;
typedef unsigned short         ossim_uint16;
typedef signed short           ossim_sint16;

typedef int                    ossim_int32;
typedef unsigned int           ossim_uint32;
typedef signed int             ossim_sint32;

typedef float                  ossim_float32;
typedef double                 ossim_float64;


typedef long long              ossim_int64;
typedef unsigned long long     ossim_uint64;
typedef signed long long       ossim_sint64;

*/

%typemap(out) ossim_int8* {
  $result = PyString_FromString($1);
}

%typemap(out) ossim_uint8* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
  
  //$result = PyString_FromFormat("%s",$1);
}


%typemap(out) ossim_int16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_uint16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_sint16* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}



%typemap(out) ossim_int32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_uint32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

%typemap(out) ossim_sint32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}

/*



%typemap(out) ossim_float32* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}


%typemap(out) ossim_float64* {
  int len,i;
  len = 0;
  while ($1[len]) len++;
  $result = PyList_New(len);
  for (i = 0; i < len; i++) {
    PyList_SetItem($result,i, PyInt_FromLong($1[i]) );
  }
}


*/










%typemap(in) (int&, char **) 
{
        /* Check if is a list */
        if (PyList_Check($input)) 
        {
                $1 = PyList_Size($input);
        
                int i = 0;
                $2 = (char **) malloc(($1+1)*sizeof(char *));
                
                for (i = 0; i < $1; i++) 
                {
                        PyObject *o = PyList_GetItem($input,i);
                
                        if (PyString_Check(o))
                                $2[i] = PyString_AsString(PyList_GetItem($input,i));
                        
                        else 
                        {
                                PyErr_SetString(PyExc_TypeError,"list must contain strings");
                                free($2);
                                return NULL;
                        }
                }
                $2[i] = 0;
        } 
        
        else 
        {
                PyErr_SetString(PyExc_TypeError,"not a list");
                return NULL;
        }
}

/* This cleans up the char ** array we malloc'd before the function call */
%typemap(freearg) (int&, char **)
{
        free((char *) $2);
}

%rename(__set__) *::operator=;
%rename(__getitem__) *::operator[];

//%include "src/ossimPlanet.i"
%include "src/pyplanet.i"
