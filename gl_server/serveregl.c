// This file declare EGL methods on server side

#include "glserver.h"


void glse_eglBindAPI()
{
  GLSE_SET_COMMAND_PTR(c, eglBindAPI);
  EGLBoolean success = eglBindAPI(c->api);
  
  gls_ret_eglBindAPI_t *ret = (gls_ret_eglBindAPI_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglBindAPI;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglBindAPI_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglChooseConfig()
{
  GLSE_SET_COMMAND_PTR(c, eglChooseConfig);

  gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsec_global.tmp_buf.buf;
  gls_ret_eglChooseConfig_t *ret = (gls_ret_eglChooseConfig_t *)glsec_global.tmp_buf.buf;
  
  // EGLDisplay dpy = eglGetCurrentDisplay();
  EGLConfig configs[c->config_size];
  EGLBoolean success = eglChooseConfig(c->dpy, dat->attrib_list, configs, c->config_size, &ret->num_config);
  memcpy(ret->configs, configs, ret->num_config);
  ret->success = success;
/*
  LOGD("gls info: Num Config = %i", ret->num_config);
  LOGD("gls info: ConfigSize = %i", c->config_size);
  for (int i = 0; i < ret->num_config; i++) {
	  LOGD("gls info: Config[%i]=%p", i, ret->configs[i]);
  }
*/
  // ret->configs = 

  // ret->success = EGL_TRUE;
  ret->cmd = GLSC_eglChooseConfig;
  
  glse_cmd_send_data(0,sizeof(gls_ret_eglChooseConfig_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglCreateContext()
{
  GLSE_SET_COMMAND_PTR(c, eglCreateContext);
  gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsec_global.tmp_buf.buf;
  EGLContext context = eglCreateContext(c->dpy, c->config, c->share_list, dat->attrib_list);
  
  if (context != EGL_NO_CONTEXT) {
	  glsec_global.gc->context = context;
  }
  
  gls_ret_eglCreateContext_t *ret = (gls_ret_eglCreateContext_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglCreateContext;
  ret->context = context;
  glse_cmd_send_data(0, sizeof(gls_ret_eglCreateContext_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglCreatePbufferSurface()
{
  GLSE_SET_COMMAND_PTR(c, eglCreatePbufferSurface);
  gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsec_global.tmp_buf.buf;
  
  EGLint* attrib_list;
  if (c->attriblist_null == TRUE) attrib_list = NULL;
  else attrib_list = dat->attrib_list;
  
  EGLSurface surface = eglCreatePbufferSurface(c->dpy, c->config, dat->attrib_list);
  
  if (surface != EGL_NO_SURFACE) {
	  glsec_global.gc->surface = surface;
  }
  
  gls_ret_eglCreatePbufferSurface_t *ret = (gls_ret_eglCreatePbufferSurface_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglCreatePbufferSurface;
  ret->surface = surface;
  glse_cmd_send_data(0, sizeof(gls_ret_eglCreatePbufferSurface_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglCreatePixmapSurface()
{
  GLSE_SET_COMMAND_PTR(c, eglCreatePixmapSurface);
  gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsec_global.tmp_buf.buf;
  
  EGLint* attrib_list;
  if (c->attriblist_null == TRUE) attrib_list = NULL;
  else attrib_list = dat->attrib_list;
  
  // FIXME unsupported yet!
  EGLSurface surface = eglCreatePixmapSurface(c->dpy, c->config, c->pixmap, dat->attrib_list);
  
  if (surface != EGL_NO_SURFACE) {
	  glsec_global.gc->surface = surface;
  }
  
  gls_ret_eglCreatePixmapSurface_t *ret = (gls_ret_eglCreatePixmapSurface_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglCreatePixmapSurface;
  ret->surface = surface;
  glse_cmd_send_data(0, sizeof(gls_ret_eglCreatePixmapSurface_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglCreateWindowSurface()
{
  GLSE_SET_COMMAND_PTR(c, eglCreateWindowSurface);
  gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsec_global.tmp_buf.buf;
  
  EGLint* attrib_list;
  if (c->attriblist_null == TRUE) attrib_list = NULL;
  else {
  	  attrib_list = dat->attrib_list;
	  for (int i = 0; i < 16; i++) {
		  LOGD("Attrib[%i]=%p", i, attrib_list[i]);
	  }
  }
  
  // Not using client window!!!
  EGLSurface surface = eglCreateWindowSurface(c->dpy, c->config, glsurfaceview_window, dat->attrib_list);
  
  if (surface != EGL_NO_SURFACE) {
	  glsec_global.gc->surface = surface;
  }
  
  gls_ret_eglCreateWindowSurface_t *ret = (gls_ret_eglCreateWindowSurface_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglCreateWindowSurface;
  ret->surface = surface;
  glse_cmd_send_data(0, sizeof(gls_ret_eglCreateWindowSurface_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglDestroyContext()
{
  GLSE_SET_COMMAND_PTR(c, eglDestroyContext);
  EGLBoolean success = eglDestroyContext(c->dpy, c->ctx);
  
  gls_ret_eglDestroyContext_t *ret = (gls_ret_eglDestroyContext_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglDestroyContext;
  ret->success = success;
  glse_cmd_send_data(0, sizeof(gls_ret_eglDestroyContext_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglDestroySurface()
{
  GLSE_SET_COMMAND_PTR(c, eglDestroySurface);
  EGLBoolean success = eglDestroySurface(c->dpy, c->surface);
  
  gls_ret_eglDestroySurface_t *ret = (gls_ret_eglDestroySurface_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglDestroySurface;
  ret->success = success;
  glse_cmd_send_data(0, sizeof(gls_ret_eglDestroySurface_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetConfigAttrib()
{
  GLSE_SET_COMMAND_PTR(c, eglGetConfigAttrib);
  gls_ret_eglGetConfigAttrib_t *ret = (gls_ret_eglGetConfigAttrib_t *)glsec_global.tmp_buf.buf;
  
  // EGLDisplay dpy = eglGetCurrentDisplay();
  // c->config cause EGL_BAD_CONFIG error
  EGLBoolean success = eglGetConfigAttrib(c->dpy, c->config, c->attribute, &ret->value);
  
  ret->cmd = GLSC_eglGetConfigAttrib;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglGetConfigAttrib_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetConfigs()
{
  GLSE_SET_COMMAND_PTR(c, eglGetConfigs);
  gls_ret_eglGetConfigs_t *ret = (gls_ret_eglGetConfigs_t *)glsec_global.tmp_buf.buf;
  
  // EGLDisplay dpy = eglGetCurrentDisplay();
  EGLBoolean success = eglGetConfigs(c->dpy, ret->configs, c->config_size, &ret->num_config);
  
  ret->cmd = GLSC_eglGetConfigs;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglGetConfigs_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetCurrentContext()
{
  EGLContext context = eglGetCurrentContext();
  
  if (context != EGL_NO_CONTEXT) {
	  glsec_global.gc->context = context;
  }
  
  gls_ret_eglGetCurrentContext_t *ret = (gls_ret_eglGetCurrentContext_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglGetCurrentContext;
  ret->context = context;
  glse_cmd_send_data(0, sizeof(gls_ret_eglGetCurrentContext_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetCurrentDisplay()
{
  EGLDisplay display = eglGetCurrentDisplay();

  if (display == EGL_NO_DISPLAY) {
	  display = glsec_global.gc->display;
  }

  gls_ret_eglGetCurrentDisplay_t *ret = (gls_ret_eglGetCurrentDisplay_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglGetCurrentDisplay;
  ret->display = display;
  glse_cmd_send_data(0, sizeof(gls_ret_eglGetCurrentDisplay_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetCurrentSurface()
{
  GLSE_SET_COMMAND_PTR(c, eglGetCurrentSurface);
  EGLSurface surface = eglGetCurrentSurface(c->readdraw);
  
  if (surface != EGL_NO_SURFACE && c->readdraw == EGL_DRAW) {
	  glsec_global.gc->surface = surface;
  }
  
  gls_ret_eglGetCurrentSurface_t *ret = (gls_ret_eglGetCurrentSurface_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglGetCurrentSurface;
  ret->surface = surface;
  glse_cmd_send_data(0, sizeof(gls_ret_eglGetCurrentSurface_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglGetError()
{
  GLuint error = eglGetError();
  // Should check gl error inside eglGetError() ???
  
  gls_ret_eglGetError_t *ret = (gls_ret_eglGetError_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglGetError;
  ret->error = error;
  glse_cmd_send_data(0, sizeof(gls_ret_eglGetError_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglInitialize()
{
  GLSE_SET_COMMAND_PTR(c, eglInitialize);
  gls_ret_eglInitialize_t *ret = (gls_ret_eglInitialize_t *)glsec_global.tmp_buf.buf;
  EGLBoolean success = eglInitialize(c->dpy, &ret->major, &ret->minor);
  
  ret->cmd = GLSC_eglInitialize;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglInitialize_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglMakeCurrent()
{
  GLSE_SET_COMMAND_PTR(c, eglMakeCurrent);
  EGLBoolean success = eglMakeCurrent(c->dpy, c->draw, c->read, c->ctx);
  
  gls_ret_eglMakeCurrent_t *ret = (gls_ret_eglMakeCurrent_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglMakeCurrent;
  ret->success = success;
  glse_cmd_send_data(0, sizeof(gls_ret_eglMakeCurrent_t), (char *)glsec_global.tmp_buf.buf);
}

void glse_eglQueryContext()
{
  GLSE_SET_COMMAND_PTR(c, eglQueryContext);
  gls_ret_eglQueryContext_t *ret = (gls_ret_eglQueryContext_t *)glsec_global.tmp_buf.buf;
  
  EGLBoolean success = eglQueryContext(c->dpy, c->ctx, c->attribute, &ret->value);
  
  ret->cmd = GLSC_eglQueryContext;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglQueryContext_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglQueryString()
{
  GLSE_SET_COMMAND_PTR(c, eglQueryString);
  gls_ret_eglQueryString_t *ret = (gls_ret_eglQueryString_t *)glsec_global.tmp_buf.buf;
  
  const char *params = eglQueryString(c->dpy, c->name);
  
  ret->cmd = GLSC_eglQueryString;
  // LOGD("Client asking for %i, return %s", c->name, params);
  ret->params[GLS_STRING_SIZE_PLUS - 1] = '\0';
  strncpy(ret->params, params, GLS_STRING_SIZE);
  glse_cmd_send_data(0,sizeof(gls_ret_eglQueryString_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglQuerySurface()
{
  GLSE_SET_COMMAND_PTR(c, eglQuerySurface);
  gls_ret_eglQuerySurface_t *ret = (gls_ret_eglQuerySurface_t *)glsec_global.tmp_buf.buf;
  
  EGLBoolean success = eglQuerySurface(c->dpy, c->surface, c->attribute, &ret->value);
  
  ret->cmd = GLSC_eglQuerySurface;
  ret->success = success;
  glse_cmd_send_data(0,sizeof(gls_ret_eglQuerySurface_t),(char *)glsec_global.tmp_buf.buf);
}

void glse_eglTerminate()
{
  GLSE_SET_COMMAND_PTR(c, eglTerminate);
  gls_ret_eglTerminate_t *ret = (gls_ret_eglTerminate_t *)glsec_global.tmp_buf.buf;
  ret->cmd = GLSC_eglTerminate;
  ret->success = eglTerminate(c->dpy);
  
  glse_cmd_send_data(0,sizeof(gls_ret_eglTerminate_t),(char *)glsec_global.tmp_buf.buf);
}

#define CASE_EGL_EXECUTE(FUNCNAME) case GLSC_##FUNCNAME : glse_##FUNCNAME (); break;
int egl_executeCommand(gls_command_t *c) {
	switch (c->cmd) {
		CASE_EGL_EXECUTE(eglBindAPI)
		CASE_EGL_EXECUTE(eglChooseConfig)
		CASE_EGL_EXECUTE(eglCreateContext)
		CASE_EGL_EXECUTE(eglCreateWindowSurface)
		CASE_EGL_EXECUTE(eglCreatePixmapSurface)
		CASE_EGL_EXECUTE(eglCreatePbufferSurface)
		CASE_EGL_EXECUTE(eglDestroyContext)
		CASE_EGL_EXECUTE(eglDestroySurface)
		CASE_EGL_EXECUTE(eglGetConfigAttrib)
		CASE_EGL_EXECUTE(eglGetConfigs)
		CASE_EGL_EXECUTE(eglGetCurrentContext)
		CASE_EGL_EXECUTE(eglGetCurrentDisplay)
		CASE_EGL_EXECUTE(eglGetCurrentSurface)
		CASE_EGL_EXECUTE(eglGetError)
		CASE_EGL_EXECUTE(eglInitialize)
		CASE_EGL_EXECUTE(eglMakeCurrent)
		CASE_EGL_EXECUTE(eglQueryContext)
		CASE_EGL_EXECUTE(eglQueryString)
		CASE_EGL_EXECUTE(eglQuerySurface)
		CASE_EGL_EXECUTE(eglTerminate)
		// CASE_EGL_EXECUTE(eglXXX)
/*
		case GLSC_eglXXX:
			glse_eglXXX();
			break:
*/
		default:
			return FALSE;
	}
	
	check_egl_err(c->cmd);
	return TRUE;
}
#undef CASE_EGL_EXECUTE

int egl_flushCommand(gls_command_t *c) {
	switch (c->cmd) {
		// Nothing here to flush
		default:
			return FALSE;
	}
	
	check_egl_err(c->cmd);
	return TRUE;
}

