// This file declare EGL methods for stubs or streaming

#include "glclient.h"
#include "EGL/egl.h"

EGLBoolean eglBindAPI(EGLenum api)
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglBindAPI);
	c->api = api;
	GLS_SEND_PACKET(eglBindAPI);
    
	wait_for_data("timeout:eglBindAPI");
	gls_ret_eglBindAPI_t *ret = (gls_ret_eglBindAPI_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLBoolean eglGetConfigAttrib( EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value )
{
	if (attribute == EGL_NATIVE_VISUAL_ID) {
		*value = XVisualIDFromVisual(XDefaultVisual(xDisplay, xScreenId));
		return EGL_TRUE;
	}
	
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetConfigAttrib);
	c->dpy = dpy;
	c->config = config;
	c->attribute = attribute;
	GLS_SEND_PACKET(eglGetConfigAttrib);
    
	wait_for_data("timeout:eglGetConfigAttrib");
	gls_ret_eglGetConfigAttrib_t *ret = (gls_ret_eglGetConfigAttrib_t *)glsc_global.tmp_buf.buf;
	*value = value;
	return ret->success;
}

EGLBoolean eglGetConfigs( EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config )
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetConfigs);
	c->dpy = dpy;
	c->config_size = config_size;
	GLS_SEND_PACKET(eglGetConfigs);
    
	wait_for_data("timeout:eglGetConfigs");
	gls_ret_eglGetConfigs_t *ret = (gls_ret_eglGetConfigs_t *)glsc_global.tmp_buf.buf;
	*num_config = ret->num_config;
	configs = ret->configs;
	return ret->success;
}

EGLDisplay eglGetDisplay(NativeDisplayType display)
{
/*
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetDisplay);
	c->display = display;
	GLS_SEND_PACKET(eglGetDisplay);
    
	wait_for_data("timeout:eglGetDisplay");
	gls_ret_eglGetDisplay_t *ret = (gls_ret_eglGetDisplay_t *)glsc_global.tmp_buf.buf;
	return ret->display;
*/

	// Can't getting Android EGL display from Linux Native display?
	return eglGetCurrentDisplay();
}

EGLint eglGetError( void )
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetError);
	GLS_SEND_PACKET(eglGetError);
    
	wait_for_data("timeout:eglGetError");
	gls_ret_eglGetError_t *ret = (gls_ret_eglGetError_t *)glsc_global.tmp_buf.buf;
	return ret->error;
}

EGLBoolean eglInitialize( EGLDisplay dpy, EGLint *major, EGLint *minor )
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglInitialize);
	c->dpy = dpy;
	GLS_SEND_PACKET(eglInitialize);
    
	wait_for_data("timeout:eglInitialize");
	gls_ret_eglInitialize_t *ret = (gls_ret_eglInitialize_t *)glsc_global.tmp_buf.buf;
	if (major != NULL) {
		*major = ret->major;
	} if (minor != NULL) {
		*minor = ret->minor;
	}
	return ret->success;
}

EGLBoolean eglTerminate( EGLDisplay dpy )
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglTerminate);
	c->dpy = dpy;
	GLS_SEND_PACKET(eglTerminate);
    
	wait_for_data("timeout:eglTerminate");
	gls_ret_eglTerminate_t *ret = (gls_ret_eglTerminate_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}


const char *eglQueryString( EGLDisplay dpy, EGLint name )
{
/*
    switch (name) {
		case EGL_VENDOR: return "gl-streaming wrapper";
		case EGL_VERSION: return "1.4";
		case EGL_CLIENT_APIS: return EGL_OPENGL_ES_API;
		case EGL_EXTENSIONS: return "EGL_KHR_cl_event2 EGL_KHR_config_attribs EGL_KHR_context_flush_control EGL_KHR_create_context EGL_KHR_create_context_no_error EGL_KHR_fence_sync EGL_KHR_get_all_proc_addresses EGL_KHR_gl_colorspace EGL_KHR_gl_renderbuffer_image EGL_KHR_gl_texture_2D_image EGL_KHR_gl_texture_3D_image EGL_KHR_gl_texture_cubemap_image EGL_KHR_image_base EGL_KHR_no_config_context EGL_KHR_reusable_sync EGL_KHR_surfaceless_context EGL_EXT_pixel_format_float EGL_KHR_wait_sync EGL_MESA_configless_context EGL_MESA_drm_image";
		default: return "";
	}
*/
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglQueryString);
	c->dpy = dpy;
	c->name = name;
	GLS_SEND_PACKET(eglQueryString);
    
	wait_for_data("timeout:eglQueryString");
	gls_ret_eglQueryString_t *ret = (gls_ret_eglQueryString_t *)glsc_global.tmp_buf.buf;
	return &ret->params[0];
}


EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY eglGetProcAddress( const char *procname )
{
	// Do not implement streaming
    return dlsym(dlopen(NULL, procname));
}

EGLBoolean eglChooseConfig( EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config )
{
	gls_cmd_flush();
	gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsc_global.tmp_buf.buf;
	memcpy(dat->attrib_list, attrib_list, GLS_DATA_SIZE);
	gls_cmd_send_data(0, GLS_STRING_SIZE_PLUS, glsc_global.tmp_buf.buf);
	
	GLS_SET_COMMAND_PTR(c, eglChooseConfig);
	c->dpy = dpy;
	c->config_size = config_size;
	GLS_SEND_PACKET(eglChooseConfig);
    
	wait_for_data("timeout:eglChooseConfig");
	gls_ret_eglChooseConfig_t *ret = (gls_ret_eglChooseConfig_t *)glsc_global.tmp_buf.buf;
	*num_config = ret->num_config;
	
	if (configs != NULL) {
		for (int i = 0; i < ret->num_config; i++) {
			configs[i] = (EGLConfig) ret->configs[i];
		}
	}
	
	if (ret->num_config == 0) {
		*num_config = 1;
	}
	
	return ret->success;

	// return eglGetConfigs(dpy, configs, config_size, num_config);
}

EGLSurface eglCreateWindowSurface( EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list )
{
	xWindow = window;
	
	gls_cmd_flush();
	if (attrib_list != NULL) {
		gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsc_global.tmp_buf.buf;
		memcpy(dat->attrib_list, attrib_list, GLS_DATA_SIZE);
		gls_cmd_send_data(0, GLS_STRING_SIZE_PLUS, glsc_global.tmp_buf.buf);
	}
	
	GLS_SET_COMMAND_PTR(c, eglCreateWindowSurface);
	c->dpy = dpy;
	c->config = config;
	c->window = window;
	c->attriblist_null = (attrib_list == NULL ? 1 : 0);
	printf("FINALLY ATTRIB LIST = NULL ? %i\n", c->attriblist_null);
	GLS_SEND_PACKET(eglCreateWindowSurface);
    
	wait_for_data("timeout:eglCreateWindowSurface");
	gls_ret_eglCreateWindowSurface_t *ret = (gls_ret_eglCreateWindowSurface_t *)glsc_global.tmp_buf.buf;
	return ret->surface;
}

EGLSurface eglCreatePixmapSurface( EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list )
{
	gls_cmd_flush();
	if (attrib_list != NULL) {
		gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsc_global.tmp_buf.buf;
		memcpy(dat->attrib_list, attrib_list, GLS_DATA_SIZE);
		gls_cmd_send_data(0, GLS_STRING_SIZE_PLUS, glsc_global.tmp_buf.buf);
	}
	
	GLS_SET_COMMAND_PTR(c, eglCreatePixmapSurface);
	c->dpy = dpy;
	c->config = config;
	c->pixmap = pixmap;
	c->attriblist_null = (attrib_list == NULL);
	GLS_SEND_PACKET(eglCreatePixmapSurface);
    
	wait_for_data("timeout:eglCreatePixmapSurface");
	gls_ret_eglCreatePixmapSurface_t *ret = (gls_ret_eglCreatePixmapSurface_t *)glsc_global.tmp_buf.buf;
	return ret->surface;
}

EGLSurface eglCreatePbufferSurface( EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list )
{
	gls_cmd_flush();
	if (attrib_list != NULL) {
		gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsc_global.tmp_buf.buf;
		memcpy(dat->attrib_list, attrib_list, GLS_DATA_SIZE);
		gls_cmd_send_data(0, GLS_STRING_SIZE_PLUS, glsc_global.tmp_buf.buf);
	}
	
	GLS_SET_COMMAND_PTR(c, eglCreatePbufferSurface);
	c->dpy = dpy;
	c->config = config;
	c->attriblist_null = (attrib_list == NULL);
	GLS_SEND_PACKET(eglCreatePbufferSurface);
    
	wait_for_data("timeout:eglCreatePbufferSurface");
	gls_ret_eglCreatePbufferSurface_t *ret = (gls_ret_eglCreatePbufferSurface_t *)glsc_global.tmp_buf.buf;
	return ret->surface;
}

EGLBoolean eglDestroySurface( EGLDisplay dpy, EGLSurface surface )
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglDestroySurface);
	c->dpy = dpy;
	c->surface = surface;
	GLS_SEND_PACKET(eglDestroySurface);
    
	wait_for_data("timeout:eglDestroySurface");
	gls_ret_eglDestroySurface_t *ret = (gls_ret_eglDestroySurface_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLBoolean eglQuerySurface( EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value )
{
	// This fix size assert in `es2gears` and `es2tri`.
	if (xWindow != NULL && (attribute == EGL_WIDTH || attribute == EGL_HEIGHT)) {
		XWindowAttributes xWindowAttrs;
		
		if (!XGetWindowAttributes(xDisplay, XDefaultRootWindow(xDisplay), &xWindowAttrs)) {
			printf("Warning: XGetWindowAttributes failed!");
		} else {
			switch (attribute) {
				case EGL_WIDTH:
					*value = xWindowAttrs.width;
					return EGL_TRUE;
				case EGL_HEIGHT:
					*value = xWindowAttrs.height;
					return EGL_TRUE;
			}
		}
		
		// *value = 300;
		// return EGL_TRUE;
	}
	
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglQuerySurface);
	c->dpy = dpy;
	c->surface = surface;
	c->attribute = attribute;
	GLS_SEND_PACKET(eglQuerySurface);
    
	wait_for_data("timeout:eglQuerySurface");
	gls_ret_eglQuerySurface_t *ret = (gls_ret_eglQuerySurface_t *)glsc_global.tmp_buf.buf;
	*value = ret->value;
	return ret->success;
}

EGLBoolean eglReleaseThread(void)
{
	// Current stub
	return EGL_TRUE;
}

/* EGL 1.1 render-to-texture APIs */
EGLBoolean eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglSurfaceAttrib);
	c->dpy = dpy;
	c->surface = surface;
	c->attribute = attribute;
	c->value = value;
	GLS_SEND_PACKET(eglSurfaceAttrib);
    
	wait_for_data("timeout:eglSurfaceAttrib");
	gls_ret_eglSurfaceAttrib_t *ret = (gls_ret_eglSurfaceAttrib_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLBoolean eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglBindTexImage);
	c->dpy = dpy;
	c->surface = surface;
	c->buffer = buffer;
	GLS_SEND_PACKET(eglBindTexImage);
    
	wait_for_data("timeout:eglBindTexImage");
	gls_ret_eglBindTexImage_t *ret = (gls_ret_eglBindTexImage_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLBoolean eglReleaseTexImage( EGLDisplay dpy, EGLSurface surface, EGLint buffer )
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglReleaseTexImage);
	c->dpy = dpy;
	c->surface = surface;
	c->buffer = buffer;
	GLS_SEND_PACKET(eglReleaseTexImage);
    
	wait_for_data("timeout:eglReleaseTexImage");
	gls_ret_eglReleaseTexImage_t *ret = (gls_ret_eglReleaseTexImage_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

/* EGL 1.1 swap control API */
EGLBoolean eglSwapInterval( EGLDisplay dpy, EGLint interval )
{

}

EGLContext eglCreateContext( EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list )
{
    gls_cmd_flush();
	gls_data_egl_attriblist_t *dat = (gls_data_egl_attriblist_t *)glsc_global.tmp_buf.buf;
	memcpy(dat->attrib_list, attrib_list, GLS_DATA_SIZE);
	gls_cmd_send_data(0, GLS_STRING_SIZE_PLUS, glsc_global.tmp_buf.buf);
	
	GLS_SET_COMMAND_PTR(c, eglCreateContext);
	c->dpy = dpy;
	c->config = config;
	c->share_list = share_list;
	
	GLS_SEND_PACKET(eglCreateContext);
    
	wait_for_data("timeout:eglCreateContext");
	gls_ret_eglCreateContext_t *ret = (gls_ret_eglCreateContext_t *)glsc_global.tmp_buf.buf;
	return ret->context;
}

EGLBoolean eglDestroyContext( EGLDisplay dpy, EGLContext ctx )
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglDestroyContext);
	c->dpy = dpy;
	c->ctx = ctx;
	GLS_SEND_PACKET(eglDestroyContext);
    
	wait_for_data("timeout:eglDestroyContext");
	gls_ret_eglDestroyContext_t *ret = (gls_ret_eglDestroyContext_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLBoolean eglMakeCurrent( EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx )
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglMakeCurrent);
	c->dpy = dpy;
	c->draw = draw;
	c->read = read;
	c->ctx = ctx;
	GLS_SEND_PACKET(eglMakeCurrent);
    
	wait_for_data("timeout:eglMakeCurrent");
	gls_ret_eglMakeCurrent_t *ret = (gls_ret_eglMakeCurrent_t *)glsc_global.tmp_buf.buf;
	return ret->success;
}

EGLContext eglGetCurrentContext(void)
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetCurrentContext);
	GLS_SEND_PACKET(eglGetCurrentContext);
    
	wait_for_data("timeout:eglGetCurrentContext");
	gls_ret_eglGetCurrentContext_t *ret = (gls_ret_eglGetCurrentContext_t *)glsc_global.tmp_buf.buf;
	return ret->context;
}

EGLDisplay eglGetCurrentDisplay(void)
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetCurrentDisplay);
	GLS_SEND_PACKET(eglGetCurrentDisplay);
    
	wait_for_data("timeout:eglGetCurrentDisplay");
	gls_ret_eglGetCurrentDisplay_t *ret = (gls_ret_eglGetCurrentDisplay_t *)glsc_global.tmp_buf.buf;
	return ret->display;
}

EGLSurface eglGetCurrentSurface(EGLint readdraw)
{
    gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglGetCurrentSurface);
	c->readdraw = readdraw;
	GLS_SEND_PACKET(eglGetCurrentSurface);
    
	wait_for_data("timeout:eglGetCurrentSurface");
	gls_ret_eglGetCurrentSurface_t *ret = (gls_ret_eglGetCurrentSurface_t *)glsc_global.tmp_buf.buf;
	return ret->surface;
}

EGLBoolean eglQueryContext( EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value )
{
	gls_cmd_flush();
	GLS_SET_COMMAND_PTR(c, eglQueryContext);
	c->dpy = dpy;
	c->ctx = ctx;
	c->attribute = attribute;
	GLS_SEND_PACKET(eglQueryContext);
    
	wait_for_data("timeout:eglQueryContext");
	gls_ret_eglQueryContext_t *ret = (gls_ret_eglQueryContext_t *)glsc_global.tmp_buf.buf;
	*value = ret->value;
	return ret->success;
}

EGLBoolean eglWaitGL( void )
{
    return // EGL_TRUE;
	EGL_FALSE;
}

EGLBoolean eglWaitNative( EGLint engine )
{
    return EGL_FALSE;
}

EGLBoolean eglSwapBuffers( EGLDisplay dpy, EGLSurface draw )
{
    static int frame;
    return gls_cmd_flip(frame++);
}

EGLBoolean eglCopyBuffers( EGLDisplay dpy, EGLSurface surface, NativePixmapType target )
{
    return EGL_TRUE;
}

