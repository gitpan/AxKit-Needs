/* dom.h
 * $Id: dom.h,v 1.13 2002/03/13 13:57:08 phish Exp $
 * Author: Christian Glahn (2001)
 * 
 * This header file provides some definitions for wrapper functions.
 * These functions hide most of libxml2 code, and should make the
 * code in the XS file more readable . 
 *
 * The Functions are sorted in four parts:
 * part 0 ..... general wrapper functions which do not belong 
 *              to any of the other parts and not specified in DOM. 
 * part A ..... wrapper functions for general nodeaccess
 * part B ..... document wrapper 
 * part C ..... element wrapper
 * 
 * I did not implement any Text, CDATASection or comment wrapper functions,
 * since it is pretty straightforeward to access these nodes. 
 */

#ifndef __LIBXML_DOM_H__
#define __LIBXML_DOM_H__

#include <libxml/tree.h>
#include <libxml/xpath.h>

/**
 * part 0:
 *
 * unsortet. 
 **/

xmlDocPtr
domCreateDocument( xmlChar* version, 
                   xmlChar *encoding );

xmlNodePtr 
domReadWellBalancedString( xmlDocPtr doc, xmlChar* string );

xmlChar*
domEncodeString( const char *encoding, const char *string );
char*
domDecodeString( const char *encoding, const xmlChar *string);

/**
 * part A:
 *
 * class Node
 **/

/* A.1 DOM specified section */

xmlChar *
domName( xmlNodePtr node );

void
domSetName( xmlNodePtr node, xmlChar* name );

xmlNodePtr
domAppendChild( xmlNodePtr self,
                xmlNodePtr newChild );
xmlNodePtr
domReplaceChild( xmlNodePtr self,
                 xmlNodePtr newChlid,
                 xmlNodePtr oldChild );
xmlNodePtr
domRemoveChild( xmlNodePtr self,
               xmlNodePtr Child );
xmlNodePtr
domInsertBefore( xmlNodePtr self, 
                 xmlNodePtr newChild,
                 xmlNodePtr refChild );

xmlNodePtr
domInsertAfter( xmlNodePtr self, 
                xmlNodePtr newChild,
                xmlNodePtr refChild );

/* A.3 extra functionality not specified in DOM L1/2*/
xmlChar*
domGetNodeValue( xmlNodePtr self );
void
domSetNodeValue( xmlNodePtr self, xmlChar* value );
void
domSetParentNode( xmlNodePtr self, 
		  xmlNodePtr newParent );
xmlNodePtr
domUnbindNode(  xmlNodePtr self );
const char*
domNodeTypeName( xmlNodePtr self );
xmlNodePtr
domReplaceNode( xmlNodePtr old, xmlNodePtr new );

xmlNodePtr
domIsNotParentOf( xmlNodePtr testNode, xmlNodePtr refNode );

/** 
 * part B:
 *
 * class Document
 **/

xmlNodePtr
domCreateCDATASection( xmlDocPtr self, xmlChar *content );
/* extra document functions */ 
xmlNodePtr
domDocumentElement( xmlDocPtr document );
xmlNodePtr
domSetDocumentElement( xmlDocPtr document, 
		       xmlNodePtr newRoot);
xmlNodePtr
domImportNode( xmlDocPtr document, xmlNodePtr node, int move );

/**
 * part C:
 *
 * class Element
 **/

xmlNodeSetPtr
domGetElementsByTagName( xmlNodePtr self, xmlChar* name );

xmlNodeSetPtr
domGetElementsByTagNameNS( xmlNodePtr self, xmlChar* nsURI, xmlChar* name );

xmlNodePtr
domSetOwnerDocument( xmlNodePtr self, xmlDocPtr doc );

xmlNsPtr
domNewNs ( xmlNodePtr elem , xmlChar *prefix, xmlChar *href );

xmlAttrPtr
domHasNsProp(xmlNodePtr node, const xmlChar *name, const xmlChar *namespace);

xmlAttrPtr
domSetAttributeNode( xmlNodePtr node , xmlAttrPtr attr );

#endif
