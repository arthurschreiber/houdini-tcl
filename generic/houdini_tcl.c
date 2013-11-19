/*
 * tclsample.c --
 *
 *      This file implements a Tcl interface to the secure hashing
 *      algorithm functions in sha1.c
 *
 * Copyright (c) 1999 Scriptics Corporation.
 * Copyright (c) 2003 ActiveState Corporation.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include <tcl.h>
#include "buffer.h"
#include "houdini.h"

static int
Houdini_escape_html(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	int length;
	gh_buf buf = GH_BUF_INIT;
	const char * str;
	Tcl_Obj * input;
	int secure = 1;

	if (objc < 2 || objc > 3) {
		goto wrong_args;
	}

	if (objc == 3) {
		if (strcmp(Tcl_GetString(objv[1]), "-unsecure") != 0) {
			goto wrong_args;
		}

		input = objv[2];
		secure = 0;
	} else {
		input = objv[1];
	}

	str = Tcl_GetStringFromObj(input, &length);
	Tcl_SetObjResult(interp, input);

	if (houdini_escape_html0(&buf, (const uint8_t *)str, length, secure)) {
		Tcl_Obj * result = Tcl_NewStringObj(gh_buf_cstr(&buf), gh_buf_len(&buf));
		Tcl_SetObjResult(interp, result);
		gh_buf_free(&buf);
	}

	return TCL_OK;

wrong_args:

	Tcl_AppendResult(interp, "wrong # args: should be \"",
		Tcl_GetString(objv[0]),
		" ?-safe? string\"",
		(char *) NULL);

	return TCL_ERROR;
}

typedef int (*houdini_cb)(gh_buf *, const uint8_t *, size_t);

static int
Houdini__generic(Tcl_Interp *interp, int objc, Tcl_Obj *const objv[], houdini_cb do_escape)
{
	gh_buf buf = GH_BUF_INIT;
	const char *str;
	int length;

	if (objc != 2) {
		Tcl_AppendResult(interp, "wrong # args: should be \"",
			Tcl_GetString(objv[0]),
			" string\"",
			(char *) NULL);

		return TCL_ERROR;
	}

	str = Tcl_GetStringFromObj(objv[1], &length);
	Tcl_SetObjResult(interp, objv[1]);

	if (do_escape(&buf, (const uint8_t *)str, length)) {
		Tcl_Obj * result = Tcl_NewStringObj(gh_buf_cstr(&buf), gh_buf_len(&buf));
		Tcl_SetObjResult(interp, result);
		gh_buf_free(&buf);
	}

	return TCL_OK;
}

static int
Houdini_unescape_html(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_unescape_html);
}

static int
Houdini_escape_xml(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_escape_xml);
}

static int
Houdini_escape_uri(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_escape_uri);
}

static int
Houdini_escape_url(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_escape_url);
}

static int
Houdini_escape_href(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_escape_href);
}

static int
Houdini_unescape_uri(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_unescape_uri);
}

static int
Houdini_unescape_url(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_unescape_url);
}

static int
Houdini_escape_js(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_escape_js);
}

static int
Houdini_unescape_js(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	return Houdini__generic(interp, objc, objv, houdini_unescape_js);
}

int
Houdini_Init(Tcl_Interp *interp)
{
	if (Tcl_InitStubs(interp, "8.5", 0) == NULL) {
		return TCL_ERROR;
	}
	if (Tcl_PkgRequire(interp, "Tcl", "8.5", 0) == NULL) {
		return TCL_ERROR;
	}
	if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
		return TCL_ERROR;
	}
	if (!Tcl_CreateNamespace(interp, "Houdini", NULL, NULL)) {
		return TCL_ERROR;
	}

	Tcl_CreateObjCommand(interp, "Houdini::escape_html", (Tcl_ObjCmdProc *) Houdini_escape_html,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::unescape_html", (Tcl_ObjCmdProc *) Houdini_unescape_html,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::escape_xml", (Tcl_ObjCmdProc *) Houdini_escape_xml,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::escape_uri", (Tcl_ObjCmdProc *) Houdini_escape_uri,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::escape_url", (Tcl_ObjCmdProc *) Houdini_escape_url,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::escape_href", (Tcl_ObjCmdProc *) Houdini_escape_href,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::unescape_uri", (Tcl_ObjCmdProc *) Houdini_unescape_uri,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::unescape_url", (Tcl_ObjCmdProc *) Houdini_unescape_url,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::escape_js", (Tcl_ObjCmdProc *) Houdini_escape_js,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	Tcl_CreateObjCommand(interp, "Houdini::unescape_js", (Tcl_ObjCmdProc *) Houdini_unescape_js,
			(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

	return TCL_OK;
}
