/*
 * libxslt.c: this modules implements the main part of the glue of the
 *           libxslt library and the Python interpreter. It provides the
 *           entry points where an automatically generated stub is either
 *           unpractical or would not match cleanly the Python model.
 *
 * If compiled with MERGED_MODULES, the entry point will be used to
 * initialize both the libxml2 and the libxslt wrappers
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */
#include <Python.h>
#include "config.h"
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "libxslt_wrap.h"
#include "libxslt-py.h"


/* #define DEBUG */
/* #define DEBUG_XPATH */
/* #define DEBUG_ERROR */
/* #define DEBUG_MEMORY */

void initlibxsltmod(void);

/************************************************************************
 *									*
 *			Per type specific glue				*
 *									*
 ************************************************************************/

PyObject *
libxslt_xsltStylesheetPtrWrap(xsltStylesheetPtr style) {
    PyObject *ret;

#ifdef DEBUG
    printf("libxslt_xsltStylesheetPtrWrap: style = %p\n", style);
#endif
    if (style == NULL) {
	Py_INCREF(Py_None);
	return(Py_None);
    }
    ret = PyCObject_FromVoidPtrAndDesc((void *) style,
	                               (char *)"xsltStylesheetPtr", NULL);
    return(ret);
}

PyObject *
libxslt_xsltTransformContextPtrWrap(xsltTransformContextPtr ctxt) {
    PyObject *ret;

#ifdef DEBUG
    printf("libxslt_xsltTransformContextPtrWrap: ctxt = %p\n", ctxt);
#endif
    if (ctxt == NULL) {
	Py_INCREF(Py_None);
	return(Py_None);
    }
    ret = PyCObject_FromVoidPtrAndDesc((void *) ctxt,
	                               (char *)"xsltTransformContextPtr", NULL);
    return(ret);
}

/************************************************************************
 *									*
 *			Extending the API				*
 *									*
 ************************************************************************/

static xmlHashTablePtr libxslt_extModuleFunctions = NULL;

static void
libxslt_xmlXPathFuncCallback(xmlXPathParserContextPtr ctxt, int nargs) {
    PyObject *list, *cur, *result;
    xmlXPathObjectPtr obj;
    xmlXPathContextPtr rctxt;
    PyObject *current_function = NULL;
    const xmlChar *name;
    const xmlChar *ns_uri;
    int i;

    if (ctxt == NULL)
	return;
    rctxt = ctxt->context;
    if (rctxt == NULL)
	return;
    name = rctxt->function;
    ns_uri = rctxt->functionURI;
#ifdef DEBUG_XPATH
    printf("libxslt_xmlXPathFuncCallback called name %s URI %s\n", name, ns_uri);
#endif

    /*
     * Find the function, it should be there it was there at lookup
     */
    current_function = xmlHashLookup2(libxslt_extModuleFunctions,
	                              name, ns_uri);
    if (current_function == NULL) {
	printf("libxslt_xmlXPathFuncCallback: internal error %s not found !\n",
	       name);
	return;
    }

    list = PyTuple_New(nargs + 1);
    PyTuple_SetItem(list, 0, libxml_xmlXPathParserContextPtrWrap(ctxt));
    for (i = 0;i < nargs;i++) {
	obj = valuePop(ctxt);
	cur = libxml_xmlXPathObjectPtrWrap(obj);
	PyTuple_SetItem(list, i + 1, cur);
    }
    result = PyEval_CallObject(current_function, list);
    Py_DECREF(list);

    obj = libxml_xmlXPathObjectPtrConvert(result);
    valuePush(ctxt, obj);
}

PyObject *
libxslt_xsltRegisterExtModuleFunction(PyObject *self ATTRIBUTE_UNUSED,
	                              PyObject *args) {
    PyObject *py_retval;
    int ret = 0;
    xmlChar *name;
    xmlChar *ns_uri;
    PyObject *pyobj_f;

    if (!PyArg_ParseTuple(args, (char *)"szO:registerXPathFunction",
		          &name, &ns_uri, &pyobj_f))
        return(NULL);

    if ((name == NULL) || (pyobj_f == NULL)) {
	py_retval = libxml_intWrap(-1);
	return(py_retval);
    }

#ifdef DEBUG_XPATH
    printf("libxslt_xsltRegisterExtModuleFunction(%s, %s) called\n",
	   name, ns_uri);
#endif

    if (libxslt_extModuleFunctions == NULL)
	libxslt_extModuleFunctions = xmlHashCreate(10);
    if (libxslt_extModuleFunctions == NULL) {
	py_retval = libxml_intWrap(-1);
	return(py_retval);
    }
    ret = xmlHashAddEntry2(libxslt_extModuleFunctions, name, ns_uri, pyobj_f);
    if (ret != 0) {
	py_retval = libxml_intWrap(-1);
	return(py_retval);
    }
    Py_XINCREF(pyobj_f);

    ret = xsltRegisterExtModuleFunction(name, ns_uri, 
	                                     libxslt_xmlXPathFuncCallback);
    py_retval = libxml_intWrap((int) ret);
    return(py_retval);
}

static void
deallocateCallback(void *payload, xmlChar *name ATTRIBUTE_UNUSED) {
    PyObject *function = (PyObject *) payload;

#ifdef DEBUG_XPATH
    printf("deallocateCallback(%s) called\n", name);
#endif

    Py_XDECREF(function);
}

/************************************************************************
 *									*
 *			Some customized front-ends			*
 *									*
 ************************************************************************/

PyObject *
libxslt_xsltApplyStylesheet(PyObject *self ATTRIBUTE_UNUSED, PyObject *args) {
    PyObject *py_retval;
    xmlDocPtr c_retval;
    xsltStylesheetPtr style;
    PyObject *pyobj_style;
    xmlDocPtr doc;
    PyObject *pyobj_doc;
    PyObject *pyobj_params;
    const char **params = NULL;
    int len = 0, i = 0, j;
    PyObject *name;
    PyObject *value;

    if (!PyArg_ParseTuple(args, (char *) "OOO:xsltApplyStylesheet",
		          &pyobj_style, &pyobj_doc, &pyobj_params))
        return(NULL);

    if (pyobj_params != Py_None) {
	if (PyDict_Check(pyobj_params)) {
	    len = PyDict_Size(pyobj_params);
	    if (len > 0) {
		params = (const char **) xmlMalloc((len + 1) * 2 *
			                           sizeof(char *));
		if (params == NULL) {
		    printf("libxslt_xsltApplyStylesheet: out of memory\n");
		    Py_INCREF(Py_None);
		    return(Py_None);
		}
		j = 0;
		while (PyDict_Next(pyobj_params, &i, &name, &value)) {
		    const char *tmp;
		    int size;

		    tmp = PyString_AS_STRING(name);
		    size = PyString_GET_SIZE(name);
		    params[j * 2] = (char *) xmlCharStrndup(tmp, size);
		    if (PyString_Check(value)) {
			tmp = PyString_AS_STRING(value);
			size = PyString_GET_SIZE(value);
			params[(j * 2) + 1] = (char *)
			    xmlCharStrndup(tmp, size);
		    } else {
			params[(j * 2) + 1] = NULL;
		    }
		    j = j + 1;
		}
		params[j * 2] = NULL;
		params[(j * 2) + 1] = NULL;
	    }
	} else {
	    printf("libxslt_xsltApplyStylesheet: parameters not a dict\n");
	    Py_INCREF(Py_None);
	    return(Py_None);
	}
    }
    style = (xsltStylesheetPtr) Pystylesheet_Get(pyobj_style);
    doc = (xmlDocPtr) PyxmlNode_Get(pyobj_doc);

    c_retval = xsltApplyStylesheet(style, doc, params);
    py_retval = libxml_xmlDocPtrWrap((xmlDocPtr) c_retval);
    if (params != NULL) {
	if (len > 0) {
	    for (i = 0;i < 2 * len;i++) {
		if (params[i] != NULL)
		    xmlFree((char *)params[i]);
	    }
	    xmlFree(params);
	}
    }
    return(py_retval);
}

/************************************************************************
 *									*
 *			Integrated cleanup				*
 *									*
 ************************************************************************/

PyObject *
libxslt_xsltCleanup(PyObject *self ATTRIBUTE_UNUSED,
	            PyObject *args ATTRIBUTE_UNUSED) {

    if (libxslt_extModuleFunctions != NULL) {
	xmlHashFree(libxslt_extModuleFunctions, deallocateCallback);
    }
    xsltCleanupGlobals();
    xmlCleanupParser();
    Py_INCREF(Py_None);
    return(Py_None);
}

/************************************************************************
 *									*
 *			The registration stuff				*
 *									*
 ************************************************************************/
static PyMethodDef libxsltMethods[] = {
#include "libxslt-export.c"
    { NULL, NULL, 0, NULL }
};

#ifdef MERGED_MODULES
extern void initlibxml2mod(void);
#endif

void initlibxsltmod(void) {
    static int initialized = 0;
    PyObject *m;

#ifdef MERGED_MODULES
    initlibxml2mod();
#endif

    if (initialized != 0)
	return;
    m = Py_InitModule((char *)"libxsltmod", libxsltMethods);
    initialized = 1;
    /* libxslt_xmlErrorInitialize(); */
    /*
     * Specific XSLT initializations
     */
    /* xmlInitParser(); */
    xmlInitMemory();
    /* LIBXML_TEST_VERSION */
    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    /* xmlDefaultSAXHandlerInit(); */
    xmlDefaultSAXHandler.cdataBlock = NULL;
}


