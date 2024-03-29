#define IN_LIBEXSLT
#include "libexslt/libexslt.h"

#if defined(WIN32) && !defined (__CYGWIN__)
#include <win32config.h>
#else
#include "config.h"
#endif

#include <string.h>

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/hash.h>
#include <libxml/debugXML.h>

#include <libxslt/xsltutils.h>
#include <libxslt/variables.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>
#include <libxslt/transform.h>

#include "exslt.h"

typedef struct _exsltFuncFunctionData exsltFuncFunctionData;
struct _exsltFuncFunctionData {
    int nargs;			/* number of arguments to the function */
    xmlNodePtr content;		/* the func:fuction template content */
};

typedef struct _exsltFuncData exsltFuncData;
struct _exsltFuncData {
    xmlHashTablePtr funcs;	/* pointer to the stylesheet module data */
    xmlXPathObjectPtr result;	/* returned by func:result */
    int error;			/* did an error occur? */
};

typedef struct _exsltFuncResultPreComp exsltFuncResultPreComp;
struct _exsltFuncResultPreComp {
    xsltElemPreComp comp;
    xmlXPathCompExprPtr select;
};

static void exsltFuncFunctionFunction (xmlXPathParserContextPtr ctxt,
				       int nargs);

/**
 * exsltFuncRegisterFunc:
 * @func:  the #exsltFuncFunctionData for the function
 * @ctxt:  an XSLT transformation context
 * @URI:  the function namespace URI
 * @name: the function name
 *
 * Registers a function declared by a func:function element
 */
static void
exsltFuncRegisterFunc (exsltFuncFunctionData *data,
		       xsltTransformContextPtr ctxt,
		       const xmlChar *URI, const xmlChar *name) {
    if ((data == NULL) || (ctxt == NULL) || (URI == NULL) || (name == NULL))
	return;

    xsltGenericDebug(xsltGenericDebugContext,
		     "exsltFuncRegisterFunc: register {%s}%s\n",
		     URI, name);
    xsltRegisterExtFunction(ctxt, name, URI,
			    exsltFuncFunctionFunction);
}

/**
 * exsltFuncInit:
 * @ctxt: an XSLT transformation context
 * @URI: the namespace URI for the extension
 *
 * Initializes the EXSLT - Functions module.
 *
 * Returns the data for this transformation
 */
static exsltFuncData *
exsltFuncInit (xsltTransformContextPtr ctxt, const xmlChar *URI) {
    xmlHashTablePtr hash;
    exsltFuncData *ret;

    ret = (exsltFuncData *) xmlMalloc (sizeof(exsltFuncData));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncInit: not enough memory\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(exsltFuncData));

    ret->result = NULL;
    ret->error = 0;

    hash = (xmlHashTablePtr) xsltStyleGetExtData(ctxt->style, URI);
    xmlHashScanFull(hash, (xmlHashScannerFull) exsltFuncRegisterFunc, ctxt);

    ret->funcs = hash;

    return(ret);
}

/**
 * exsltFuncShutdown:
 * @ctxt: an XSLT transformation context
 * @URI: the namespace URI for the extension
 * @data: the module data to free up
 *
 * Shutdown the EXSLT - Functions module
 */
static void
exsltFuncShutdown (xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		   const xmlChar *URI ATTRIBUTE_UNUSED,
		   exsltFuncData *data) {
    if (data->result != NULL)
	xmlXPathFreeObject(data->result);
    xmlFree(data);
}

/**
 * exsltFuncStyleInit:
 * @style: an XSLT stylesheet
 * @URI: the namespace URI for the extension
 *
 * Allocates the styleshet data for EXSLT - Function
 *
 * Returns the allocated data
 */
static xmlHashTablePtr
exsltFuncStyleInit (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
		    const xmlChar *URI ATTRIBUTE_UNUSED) {
    return xmlHashCreate(1);
}

/**
 * exsltFuncStyleShutdown:
 * @style: an XSLT stylesheet
 * @URI: the namespace URI for the extension
 * @data: the stylesheet data to free up
 *
 * Shutdown the EXSLT - Function module
 */
static void
exsltFuncStyleShutdown (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
			const xmlChar *URI ATTRIBUTE_UNUSED,
			xmlHashTablePtr data) {
    xmlHashFree(data, (xmlHashDeallocator) xmlFree);
}

/**
 * exsltFuncNewFunctionData:
 *
 * Allocates an #exslFuncFunctionData object
 *
 * Returns the new structure
 */
static exsltFuncFunctionData *
exsltFuncNewFunctionData (void) {
    exsltFuncFunctionData *ret;

    ret = (exsltFuncFunctionData *) xmlMalloc (sizeof(exsltFuncFunctionData));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncNewFunctionData: not enough memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(exsltFuncFunctionData));

    ret->nargs = 0;
    ret->content = NULL;

    return(ret);
}

/**
 * exsltFreeFuncResultPreComp:
 * @comp:  the #exsltFuncResultPreComp to free up
 *
 * Deallocates an #exsltFuncResultPreComp
 */
static void
exsltFreeFuncResultPreComp (exsltFuncResultPreComp *comp) {
    if (comp == NULL)
	return;

    if (comp->select != NULL)
	xmlXPathFreeCompExpr (comp->select);
    xmlFree(comp);
}

/**
 * exsltFuncFunctionFunction:
 * @ctxt:  an XPath parser context
 * @nargs:  the number of arguments
 *
 * Evaluates the func:function element that defines the called function.
 */
static void
exsltFuncFunctionFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr obj, oldResult, ret;
    exsltFuncData *data;
    exsltFuncFunctionData *func;
    xmlNodePtr paramNode, oldInsert, fake;
    xsltStackElemPtr params = NULL, param;
    xsltTransformContextPtr tctxt = xsltXPathGetTransformContext(ctxt);
    int i;

    /*
     * retrieve func:function template
     */
    data = (exsltFuncData *) xsltGetExtData (tctxt,
					     EXSLT_FUNCTIONS_NAMESPACE);
    oldResult = data->result;
    data->result = NULL;

    func = (exsltFuncFunctionData*) xmlHashLookup2 (data->funcs,
						    ctxt->context->functionURI,
						    ctxt->context->function);

    /*
     * params handling
     */
    if (nargs > func->nargs) {
	xsltGenericError(xsltGenericErrorContext,
			 "{%s}%s: called with too many arguments\n",
			 ctxt->context->functionURI, ctxt->context->function);
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }
    if (func->content != NULL)
	paramNode = func->content->prev;
    else
	paramNode = NULL;
    if ((paramNode == NULL) && (func->nargs != 0)) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncFunctionFunction: nargs != 0 and "
			 "param == NULL\n");
	return;
    }
    /* defaulted params */
    for (i = func->nargs; (i > nargs) && (paramNode != NULL); i--) {
	param = xsltParseStylesheetCallerParam (tctxt, paramNode);
	param->next = params;
	params = param;
	paramNode = paramNode->prev;
    }
    /* set params */
    while ((i-- > 0) && (paramNode != NULL)) {
	obj = valuePop(ctxt);
	/* FIXME: this is a bit hackish */
	param = xsltParseStylesheetCallerParam (tctxt, paramNode);
	param->computed = 1;
	if (param->value != NULL)
	    xmlXPathFreeObject(param->value);
	param->value = obj;
	param->next = params;
	params = param;
	paramNode = paramNode->prev;
    }

    /*
     * actual processing
     */
    fake = xmlNewDocNode(tctxt->output, NULL,
			 (const xmlChar *)"fake", NULL);
    oldInsert = tctxt->insert;
    tctxt->insert = fake;
    xsltApplyOneTemplate (tctxt, xmlXPathGetContextNode(ctxt),
			  func->content, NULL, params);
    tctxt->insert = oldInsert;
    if (params != NULL)
	xsltFreeStackElemList(params);

    if (data->error != 0)
	return;

    if (data->result != NULL)
	ret = data->result;
    else
	ret = xmlXPathNewCString("");

    data->result = oldResult;

    /*
     * It is an error if the instantiation of the template results in
     * the generation of result nodes.
     */
    if (fake->children != NULL) {
#ifdef LIBXML_DEBUG_ENABLED
	xmlDebugDumpNode (stderr, fake, 1);
#endif
	xsltGenericError(xsltGenericErrorContext,
			 "{%s}%s: cannot write to result tree while "
			 "executing a function\n",
			 ctxt->context->functionURI, ctxt->context->function);
	xmlFreeNode(fake);
	return;
    }
    xmlFreeNode(fake);
    valuePush(ctxt, ret);
}


static void
exsltFuncFunctionComp (xsltStylesheetPtr style, xmlNodePtr inst) {
    xmlChar *name, *prefix;
    xmlNsPtr ns;
    xmlHashTablePtr data;
    exsltFuncFunctionData *func;

    if ((style == NULL) || (inst == NULL))
	return;


    {
	xmlChar *qname;

	qname = xmlGetProp(inst, (const xmlChar *) "name");
	name = xmlSplitQName2 (qname, &prefix);
	xmlFree(qname);
    }
    if ((name == NULL) || (prefix == NULL)) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:function: not a QName\n");
	if (name != NULL)
	    xmlFree(name);
	return;
    }
    /* namespace lookup */
    ns = xmlSearchNs (inst->doc, inst, prefix);
    if (ns == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:function: undeclared prefix %s\n",
			 prefix);
	xmlFree(name);
	xmlFree(prefix);
	return;
    }
    xmlFree(prefix);

    /*
     * Create function data
     */
    func = exsltFuncNewFunctionData();
    func->content = inst->children;
    while (IS_XSLT_ELEM(func->content) &&
	   IS_XSLT_NAME(func->content, "param")) {
	func->content = func->content->next;
	func->nargs++;
    }

    xsltParseTemplateContent(style, inst);

    /*
     * Register the function data such that it can be retrieved
     * by exslFuncFunctionFunction
     */
    data = (xmlHashTablePtr) xsltStyleGetExtData (style,
						  EXSLT_FUNCTIONS_NAMESPACE);
    if (data == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncFunctionComp: no stylesheet data\n");
	xmlFree(name);
	return;
    }

    if (xmlHashAddEntry2 (data, ns->href, name, func) < 0) {
	xsltGenericError(xsltGenericErrorContext,
			 "Failed to register function {%s}%s\n",
			 ns->href, name);
    } else {
	xsltGenericDebug(xsltGenericDebugContext,
			 "exsltFuncFunctionComp: register {%s}%s\n",
			 ns->href, name);
    }
    xmlFree(name);
}

static xsltElemPreCompPtr
exsltFuncResultComp (xsltStylesheetPtr style, xmlNodePtr inst,
		     xsltTransformFunction function) {
    xmlNodePtr test;
    xmlChar *select;
    exsltFuncResultPreComp *ret;

    /*
     * "Validity" checking
     */
    /* it is an error to have any following sibling elements aside
     * from the xsl:fallback element.
     */
    for (test = inst->next; test != NULL; test = test->next) {
	if (test->type != XML_ELEMENT_NODE)
	    continue;
	if (IS_XSLT_ELEM(test) && IS_XSLT_NAME(test, "fallback"))
	    continue;
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncResultElem: only xsl:fallback is "
			 "allowed to follow func:result\n");
	return (NULL);
    }
    /* it is an error for a func:result element to not be a descendant
     * of func:function.
     * it is an error if a func:result occurs within a func:result
     * element.
     * it is an error if instanciating the content of a variable
     * binding element (i.e. xsl:variable, xsl:param) results in the 
     * instanciation of a func:result element.
     */
    for (test = inst->parent; test != NULL; test = test->parent) {
	if ((test->ns != NULL) &&
	    (xmlStrEqual(test->ns->href, EXSLT_FUNCTIONS_NAMESPACE))) {
	    if (xmlStrEqual(test->name, (const xmlChar *) "function")) {
		break;
	    }
	    if (xmlStrEqual(test->name, (const xmlChar *) "result")) {
		xsltGenericError(xsltGenericErrorContext,
				 "func:result element not allowed within"
				 " another func:result element\n");
		return (NULL);
	    }
	}
	if (IS_XSLT_ELEM(test) &&
	    (IS_XSLT_NAME(test, "variable") ||
	     IS_XSLT_NAME(test, "param"))) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result element not allowed within"
			     " a variable binding element\n");
	    return (NULL);
	}
    }

    /*
     * Precomputation
     */
    ret = (exsltFuncResultPreComp *)
	xmlMalloc (sizeof(exsltFuncResultPreComp));
    if (ret == NULL) {
	xsltPrintErrorContext(NULL, NULL, NULL);
        xsltGenericError(xsltGenericErrorContext,
                         "exsltFuncResultComp : malloc failed\n");
        return (NULL);
    }
    memset(ret, 0, sizeof(exsltFuncResultPreComp));

    xsltInitElemPreComp ((xsltElemPreCompPtr) ret, style, inst, function,
		 (xsltElemPreCompDeallocator) exsltFreeFuncResultPreComp);
    ret->select = NULL;

    /*
     * Precompute the select attribute
     */
    select = xmlGetNsProp(inst, (const xmlChar *) "select", NULL);
    if (select != NULL) {
	ret->select = xmlXPathCompile (select);
	xmlFree(select);
    }

    return ((xsltElemPreCompPtr) ret);
}

static void
exsltFuncResultElem (xsltTransformContextPtr ctxt,
	             xmlNodePtr node ATTRIBUTE_UNUSED, xmlNodePtr inst,
		     exsltFuncResultPreComp *comp) {
    exsltFuncData *data;
    xmlXPathObjectPtr ret;

    /* It is an error if instantiating the content of the
     * func:function element results in the instantiation of more than
     * one func:result elements.
     */
    data = (exsltFuncData *) xsltGetExtData (ctxt, EXSLT_FUNCTIONS_NAMESPACE);
    if (data == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncReturnElem: data == NULL\n");
	return;
    }
    if (data->result != NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:result already instanciated\n");
	data->error = 1;
	return;
    }
    /*
     * Processing
     */
    if (comp->select != NULL) {
	/* If the func:result element has a select attribute, then the
	 * value of the attribute must be an expression and the
	 * returned value is the object that results from evaluating
	 * the expression. In this case, the content must be empty.
	 */
	if (inst->children != NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result content must be empty if it"
			     " has a select attribute\n");
	    data->error = 1;
	    return;
	}
	ret = xmlXPathCompiledEval(comp->select, ctxt->xpathCtxt);
	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    return;
	}
    } else if (inst->children != NULL) {
	/* If the func:result element does not have a select attribute
	 * and has non-empty content (i.e. the func:result element has
	 * one or more child nodes), then the content of the
	 * func:result element specifies the value.
	 */
	xmlNodePtr container, oldInsert;

	container = xmlNewDocNode (ctxt->output, NULL,
				   (const xmlChar *) "fake", NULL);
	oldInsert = ctxt->insert;
	ctxt->insert = container;
	xsltApplyOneTemplate (ctxt, ctxt->xpathCtxt->node,
			      inst->children, NULL, NULL);
	ctxt->insert = oldInsert;

	ret = xmlXPathNewValueTree(container);
	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    data->error = 1;
	}
    } else {
	/* If the func:result element has empty content and does not
	 * have a select attribute, then the returned value is an
	 * empty string.
	 */
	ret = xmlXPathNewCString("");
    }
    data->result = ret;
}

/**
 * exsltFuncRegister:
 *
 * Registers the EXSLT - Functions module
 */
void
exsltFuncRegister (void) {
    xsltRegisterExtModuleFull (EXSLT_FUNCTIONS_NAMESPACE,
		       (xsltExtInitFunction) exsltFuncInit,
		       (xsltExtShutdownFunction) exsltFuncShutdown,
		       (xsltStyleExtInitFunction) exsltFuncStyleInit,
		       (xsltStyleExtShutdownFunction) exsltFuncStyleShutdown);

    xsltRegisterExtModuleTopLevel ((const xmlChar *) "function",
				   EXSLT_FUNCTIONS_NAMESPACE,
				   exsltFuncFunctionComp);
    xsltRegisterExtModuleElement ((const xmlChar *) "result",
			  EXSLT_FUNCTIONS_NAMESPACE,
			  (xsltPreComputeFunction)exsltFuncResultComp,
			  (xsltTransformFunction) exsltFuncResultElem);
}
